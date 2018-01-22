#ifndef __DYNA_STRING_H__
#define __DYNA_STRING_H__

#include "SharedTypes.h"
#include "PacketDataStream.h"

#ifdef __GAMESERVER__
#include "KwNetStreamSystem.h"
#else
#include "GameNetStream.h"
#endif

NAMESPACE_LOBBY_BEGIN

#define MAX_DYNASTRING_LEN 65535
class DynaString
{
public:
	DynaString()
	{
		clear();
	}

	DynaString(const char* str)
	{
		if (NULL != str)
		{
			setStr(str);
		}
		else
		{
			clear();
		}
	}

	~DynaString()
	{
		clear();
	}

	void clear()
	{
		mLength = 0;
		if (mContent)
		{
			delete mContent;
			mContent = NULL;
		}
	}

	void setStr(const char* val)
	{
		clear();
		mLength = (uint16)strnlen(val, MAX_DYNASTRING_LEN - 1);
		mContent = new char[mLength + 1];
		memcpy(mContent, val, mLength);
		mContent[mLength] = 0;
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

	uint16 length() const
	{
		return mLength;
	}

	const char* c_str() const
	{
		return mContent;
	}

	const char* data() const
	{
		return mContent;
	}

	char* data()
	{
		return mContent;
	}

	BOOL serialize(DataStream &oStream) const
	{
		oStream.pushValue(mLength);
		oStream.serialize(mContent, mLength);
		return TRUE;
	}

	BOOL unserialize(DataStream &iStream)
	{
		clear();
		iStream.getValue(mLength);
		if (mLength > 0)
		{
			mContent = new char[mLength + 1];
			iStream.unserialize(mContent, mLength);
			mContent[mLength] = 0;
		}

		return TRUE;
	}

	DynaString& operator += (const DynaString& rhs)
	{
		if (rhs.length() == 0)
			return *this;

		if (mContent)
		{
			mContent = (char*)::realloc(mContent, mLength + rhs.length() + 1);
			memcpy(mContent + mLength, (const void*)rhs.data(), rhs.length());
			mLength += rhs.length();
			mContent[mLength] = 0;
		}
		else
		{
			mLength = rhs.length();
			mContent = new char[mLength + 1];
			memcpy(mContent, rhs.data(), mLength);
			mContent[mLength] = 0;
		}

		return *this;
	}

	DynaString& operator = (const DynaString& rhs)
	{
		if (&rhs == this)
			return *this;
		if (rhs.length() == mLength)
		{
			if (mLength != 0)
			{
				memcpy(mContent, rhs.data(), mLength);
				mContent[mLength] = 0;
			}
		}
		else
		{
			clear();
			mLength = rhs.length();
			mContent = new char[mLength + 1];
			memcpy(mContent, rhs.data(), mLength);
			mContent[mLength] = 0;
		}

		return *this;
	}

	DynaString& operator = (const char* src)
	{
		setStr(src);

		return *this;
	}

	bool operator == (const DynaString& rhs) const
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

	bool operator == (const char* src)
	{
		if (strncmp(mContent, src, mLength) == 0)
			return true;
		return false;
	}

	explicit operator const char*() const
	{
		return mContent;
	}

protected:
	uint16 mLength;
	char* mContent;
};

NAMESPACE_LOBBY_END

#endif