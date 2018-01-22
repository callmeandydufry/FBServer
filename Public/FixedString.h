
#ifndef __FIXEDLENGTHSTRING_H__
#define __FIXEDLENGTHSTRING_H__

#include "SharedTypes.h"
#include "PacketDataStream.h"

#ifdef __GAMESERVER__
#include "KwNetStreamSystem.h"
#else
#include "GameNetStream.h"
#endif

NAMESPACE_LOBBY_BEGIN

template<int TMAXLEN, typename TSIZE_TYPE = unsigned short>
class FixedString
{
public:
	FixedString()
	{
		clear();
	}

	FixedString(const char* str)
	{
		if (NULL != str)
		{
			TSIZE_TYPE len = (TSIZE_TYPE)strnlen(str, TMAXLEN - 1);
			memcpy(mContent, str, len);
			mContent[len] = 0;
			mLength = len;
		}
		else
		{
			clear();
		}
	}

#ifdef __GAMESERVER__
	FixedString(const std::string& str)
	{
		if (!str.empty())
		{
			TSIZE_TYPE len = (TSIZE_TYPE)strnlen(str.c_str(), TMAXLEN - 1);
			memcpy(mContent, str.c_str(), len);
			mContent[len] = 0;
			mLength = len;
		}
		else
		{
			clear();
		}
	}
#endif

	void clear()
	{
		mLength = 0;
		memset(mContent, 0, sizeof(mContent));
	}

#ifdef __GAMESERVER__
	BOOL reci(RecieveStream &iStream)
	{
		iStream.Reci((char*)&mLength, sizeof(mLength));
		if (mLength > 0)
		{
			iStream.Reci((char*)mContent, mLength);
		}

		return TRUE;
	}

	BOOL send(SendStream &oStream) const
	{
		oStream.Send((char*)&mLength, sizeof(mLength));
		if (mLength > 0)
		{
			oStream.Send((char*)mContent, mLength);
		}
		return TRUE;
	}
#else
	BOOL reci(NetRecieveStream &iStream)
	{
		iStream.Reci((char*)&mLength, sizeof(mLength));
		if (mLength > 0)
		{
			iStream.Reci((char*)mContent, mLength);
		}

		return TRUE;
	}

	BOOL send(NetSendStream &oStream) const
	{
		oStream.Send((char*)&mLength, sizeof(mLength));
		if (mLength > 0)
		{
			oStream.Send((char*)mContent, mLength);
		}
		return TRUE;
	}
#endif

	uint32 getSize() const
	{
		return sizeof(mLength) + mLength;
	}

	TSIZE_TYPE length() const
	{
		return mLength;
	}

	const char* c_str() const
	{
		return mContent;
	}

	char* data()
	{
		return mContent;
	}

	void reCalcLength()
	{
		mLength = (TSIZE_TYPE)strnlen(mContent, TMAXLEN);
	}

	int maxlen() const
	{
		return TMAXLEN;
	}

	BOOL serialize(DataStream &oStream) const
	{
		oStream.pushValue(mLength);
		oStream.serialize(mContent, mLength);
		return TRUE;
	}

	BOOL unserialize(DataStream &iStream)
	{
		iStream.getValue(mLength);
		iStream.unserialize(mContent, mLength);
		return TRUE;
	}

	template<int OTHERLEN, typename OTHERSIZETYPE>
	FixedString& operator += (const FixedString<OTHERLEN, OTHERSIZETYPE>& rhs)
	{
		TSIZE_TYPE newLen = mLength + (TSIZE_TYPE)rhs.length();
		if (newLen > TMAXLEN - 1)
			newLen = TMAXLEN - 1;
		
		if (newLen - mLength > 0)
		{
			memcpy(mContent + mLength, rhs.c_str(), newLen - mLength);
			mLength = newLen;
		}

		return *this;
	}

	template<int OTHERLEN, typename OTHERSIZETYPE>
	FixedString& operator = (const FixedString<OTHERLEN, OTHERSIZETYPE>& rhs)
	{
		TSIZE_TYPE len = (TSIZE_TYPE)rhs.length();
		if (len > TMAXLEN - 1)
			len = TMAXLEN - 1;
		memcpy(mContent, rhs.c_str(), len);
		mContent[len] = 0;
		mLength = len;

		return *this;
	}

	FixedString& operator = (const char* src)
	{
		TSIZE_TYPE len = (TSIZE_TYPE)strnlen(src, TMAXLEN - 1);
		memcpy(mContent, src, len);
		mContent[len] = 0;
		mLength = len;

		return *this;
	}

	template<int OTHERLEN, typename OTHERSIZETYPE>
	bool operator == (const FixedString<OTHERLEN, OTHERSIZETYPE>& rhs) const
	{
		if (rhs.length() != mLength)
			return false;

		const char* dest = rhs.c_str();
		for (int i = 0; i < mLength; ++i)
		{
			if (dest[i] != mContent[i])
				return false;
		}
		return true;
	}

	template<int OTHERLEN, typename OTHERSIZETYPE>
	bool operator != (const FixedString<OTHERLEN, OTHERSIZETYPE>& rhs) const
	{
		if (rhs.length() != mLength)
			return true;

		const char* dest = rhs.c_str();
		for (int i = 0; i < mLength; ++i)
		{
			if (dest[i] != mContent[i])
				return true;
		}
		return false;
	}

	bool operator == (const char* src)
	{
		if (strncmp(mContent, src, mLength) == 0)
			return true;
		return false;
	}

	bool operator != (const char* src)
	{
		if (strncmp(mContent, src, mLength) != 0)
			return true;
		return false;
	}

#ifdef __GAMESERVER__
	explicit operator std::string() const
	{
		std::string ret = mContent;
		return ret;
	}
#endif

	explicit operator const char*() const
	{
		return mContent;
	}

protected:
	TSIZE_TYPE mLength;
	char mContent[TMAXLEN];
};

NAMESPACE_LOBBY_END
#endif
