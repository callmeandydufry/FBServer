#ifndef __PACKET_DATA_STREAM_H__
#define __PACKET_DATA_STREAM_H__

#include "TypeDefine.h"

#include "KwNetStreamSystem.h"

class DataStream
{
public:
	DataStream()
	{
		mSize = 0;
		mReservedSize = 0;
		mData = NULL;
		mStartPos = 0;
	}

	~DataStream()
	{
		clear();
	}

	template<typename T>
	void pushValue(const T& val);

	template<typename T>
	BOOL getValue(T& val);

	void serialize(const void* data, int32 size)
	{
		if (NULL == data || size <= 0)
			return;
		int32 targetSize = mSize + size;
		reserve(targetSize);

		::memcpy((unsigned char*)mData + mSize, data, size);
		mSize += size;
	}

	BOOL unserialize(void* data, int32 size)
	{
		if (mStartPos + size <= mSize)
		{
			::memcpy(data, (unsigned char*)mData + mStartPos, size);
			mStartPos += size;
			return TRUE;
		}
		return FALSE;
	}

	BOOL isCompleted() const
	{
		if (mStartPos >= mSize)
			return TRUE;
		return FALSE;
	}

	void clear()
	{
		mSize = 0;
		mReservedSize = 0;
		if (NULL != mData)
			::free(mData);
		mStartPos = 0;
	}

#ifdef __GAMESERVER__
	void reci(RecieveStream &iStream)
	{
		clear();
		int32 size = 0;
		iStream.Reci((char *)(&size), sizeof(size));
		if (size > 0)
		{
			reserve(size);
			iStream.Reci((char*)mData, size);
			mSize = size;
		}

	}

	void send(SendStream &oStream) const
	{
		oStream.Send((char *)(&mSize), sizeof(mSize));
		if (mSize > 0)
		{
			oStream.Send((char *)(mData), mSize);
		}
	}
#else
	void reci(NetRecieveStream &iStream)
	{
		clear();
		int32 size = 0;
		iStream.Reci((char *)(&size), sizeof(size));
		if (size > 0)
		{
			reserve(size);
			iStream.Reci((char*)mData, size);
			mSize = size;
		}

	}

	void send(NetSendStream &oStream) const
	{
		oStream.Send((char *)(&mSize), sizeof(mSize));
		if (mSize > 0)
		{
			oStream.Send((char *)(mData), mSize);
		}
	}
#endif

	uint32 getSize() const
	{
		return sizeof(mSize) + mSize;
	}

	uint32 getDataSize() const
	{
		return mSize;
	}

	const void* getData() const
	{
		return mData;
	}

	DataStream& operator=(const DataStream& rhs)
	{
		clear();

		mReservedSize = rhs.mReservedSize;
		mSize = rhs.mSize;
		mStartPos = rhs.mStartPos;
		if (mReservedSize > 0)
			mData = ::malloc(mReservedSize);
		if (mSize > 0)
		{
			memcpy(mData, rhs.mData, mSize);
		}
		return *this;
	}

	void swapData(DataStream& stream)
	{
		uint32 tmpReservedSize = stream.mReservedSize;
		uint32 tmpSize = stream.mSize;
		uint32 tmpStartPos = stream.mStartPos;
		void* tmpData = stream.mData;

		stream.mReservedSize = mReservedSize;
		stream.mSize = mSize;
		stream.mStartPos = mStartPos;
		stream.mData = mData;

		mReservedSize = tmpReservedSize;
		mSize = tmpSize;
		mStartPos = tmpStartPos;
		mData = tmpData;
	}

protected:
	void reserve(uint32 targetSize)
	{
		if (targetSize > mReservedSize)
		{
			uint16 newSize = 0;
			if (targetSize > 4096)
			{
				newSize = ((targetSize / 4096) + 1) * 4096;
			}
			else
			{
				if (0 == mReservedSize)
					mReservedSize = 1;

				newSize = mReservedSize * 2;
				while (newSize < targetSize)
				{
					newSize = newSize * 2;
				}
			}
			void* newData = ::malloc(newSize);
			if (mSize > 0)
				::memcpy(newData, mData, mSize);
			if (NULL != mData)
				::free(mData);
			mData = newData;
			mReservedSize = newSize;
		}
	}

protected:
	uint32 mSize;
	uint32 mReservedSize;
	void* mData;
	uint32 mStartPos;
};

template<typename T>
inline void _DataStream_pushValue(DataStream& stream, const T& val)
{
	val.serialize(stream);
}

typedef char* CharPtr;
typedef const char* ConstCharPtr;

template<> inline void _DataStream_pushValue(DataStream& stream, const char& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const unsigned char& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const int16& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const uint16& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const int32& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const uint32& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const int64& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const uint64& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const float& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const double& val) { stream.serialize(&val, sizeof(val)); }
template<> inline void _DataStream_pushValue(DataStream& stream, const CharPtr& val)
{
	if (NULL == val)
	{
		_DataStream_pushValue<uint32>(stream, 0);
		return;
	}

	uint32 size = (uint32)strlen(val);
	_DataStream_pushValue(stream, size);
	if (size > 0)
	{
		stream.serialize(val, size);
	}
}

template<> inline void _DataStream_pushValue(DataStream& stream, const ConstCharPtr& val)
{
	if (NULL == val)
	{
		_DataStream_pushValue<uint32>(stream, 0);
		return;
	}

	uint32 size = (uint32)strlen(val);
	_DataStream_pushValue(stream, size);
	if (size > 0)
	{
		stream.serialize(val, size);
	}
}

#ifdef __GAMESERVER__
template<> inline void _DataStream_pushValue(DataStream& stream, const std::string& val)
{
	if (val.empty())
	{
		_DataStream_pushValue<uint32>(stream, 0);
		return;
	}

	uint32 size = (uint32)val.length();
	_DataStream_pushValue(stream, size);
	if (size > 0)
	{
		stream.serialize(val.c_str(), size);
	}
}
#endif

template<typename T>
inline BOOL _DataStream_getValue(DataStream& stream, T& val)
{
	return val.unserialize(stream);
}

template<> inline BOOL _DataStream_getValue(DataStream& stream, char& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, unsigned char& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, int16& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, uint16& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, int32& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, uint32& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, int64& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, uint64& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, float& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, double& val) { return stream.unserialize(&val, sizeof(val)); }
template<> inline BOOL _DataStream_getValue(DataStream& stream, CharPtr& val)
{
	if (NULL == val)
		return FALSE;

	uint32 size = 0;
	if (!_DataStream_getValue(stream, size))
		return FALSE;
	if (size > 0)
	{
		if (!stream.unserialize(val, size))
			return FALSE;
		val[size] = 0;
	}
	return TRUE;
}

template<> inline BOOL _DataStream_getValue(DataStream& stream, ConstCharPtr& val)
{
	if (NULL == val)
		return FALSE;

	uint32 size = 0;
	if (!_DataStream_getValue(stream, size))
		return FALSE;
	if (size > 0)
	{
		CharPtr valPtr = const_cast<CharPtr>(val);
		if (!stream.unserialize(valPtr, size))
			return FALSE;
		valPtr[size] = 0;
	}
	return TRUE;
}

#ifdef __GAMESERVER__
template<> inline BOOL _DataStream_getValue(DataStream& stream, std::string& val)
{
	uint32 size = 0;
	if (!_DataStream_getValue(stream, size))
		return FALSE;
	if (size > 0)
	{
		char* str = new char[size + 1];
		if (!stream.unserialize(str, size))
		{
			delete[] str;
			return FALSE;
		}
		str[size] = 0;
		val = str;
		delete[] str;
	}
	return TRUE;
}
#endif

template<typename T>
void DataStream::pushValue(const T& val)
{
	_DataStream_pushValue(*this, val);
}

template<typename T>
BOOL DataStream::getValue(T& val)
{
	return _DataStream_getValue(*this, val);
}

NAMESPACE_LOBBY_END

#endif
