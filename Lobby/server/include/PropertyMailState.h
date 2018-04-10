#ifndef _PROPERTY_MAIL_STATE_H__
#define _PROPERTY_MAIL_STATE_H__

#include "TypeDefine.h"
#include "Struct.h"
#include "StructMacro.h"
#include "ModuleDataStream.h"
//---------------------------------------------------------------------------------------------------------
// basemail [2/5/2018 Chief]
//---------------------------------------------------------------------------------------------------------
#define PROPERTYS_BaseMailState(PROPERTY)	\
	PROPERTY(int32, MailID, INVALID)\
	PROPERTY(int32, ServerID, INVALID_ID)\
	PROPERTY(int32, MailType, INVALID)\
	PROPERTY(uint32, CreateTime, 0)\
	PROPERTY(uint32, OverTime, 0)\

struct DBBaseMailState
{
	STRUCT_PROPERTY_LIST(PROPERTYS_BaseMailState);

	BOOL isOverTime(uint32 uTime)
	{
		return (uTime <= mOverTime);
	}

	BOOL isValid()
	{
		return (mMailID != INVALID && INVALID != mMailType && 0 != mOverTime);
	}

	BOOL isValid() const
	{
		return (mMailID != INVALID && INVALID != mMailType && 0 != mOverTime);
	}

	BOOL operator==(const DBBaseMailState* pData)
	{
		if (NULL == pData) return FALSE;

		return (mMailID == pData->mMailID && mMailType == pData->mMailType && mCreateTime == pData->mCreateTime && mOverTime == pData->mOverTime);
	}
};

DB_STRUCT_DATA_MODEL(DBModelBaseMailState, DBBaseMailState, PROPERTYS_BaseMailState)
{
public:
	DBModelBaseMailState()
	{
		clear();
	}

	virtual const char* getDBTableName()
	{
		return "t_base_mail";
	}

	void setMailID(int32 mailid)
	{
		mMailID = mailid;
	}

	void clear()
	{
		DBModelBase::clear();
		mMailID = INVALID;
		nBegin = 0;
		nEnd = 0;
	}

	void setMailIDRange(int32 begin, int32 nNum)
	{
		nBegin = begin;
		nEnd = nBegin + nNum;
	}
private:
	int32 mMailID;
	int32 nBegin, nEnd;

private:
	virtual String getSQLQueryConditionString()
	{
		String condition;
		condition.clear();

		// todo 

		return condition;
	}
};

//---------------------------------------------------------------------------------------------------------
// attachmentmail [2/5/2018 Chief]
//---------------------------------------------------------------------------------------------------------
#define PROPERTYS_AttachMentMailState(PROPERTY)	\
	PROPERTY(int32, MailID, INVALID_ID)\
	PROPERTY(int32, TypeMisc1, INVALID_ID)\
	PROPERTY(int32, TypeMisc2, INVALID_ID)\
	PROPERTY(int32, TypeMisc3, INVALID_ID)\
	PROPERTY(int32, Num1, INVALID_ID)\
	PROPERTY(int32, Num2, INVALID_ID)\
	PROPERTY(int32, Num3, INVALID_ID)\
	PROPERTY(int32, Valid, INVALID_ID)\

struct DBAttachmentMailState
{
	STRUCT_PROPERTY_LIST(PROPERTYS_AttachMentMailState);

	BOOL isValid()
	{
		return (INVALID != mValid);
	}

	BOOL isValid() const
	{
		return (INVALID != mValid);
	}
};

DB_STRUCT_DATA_MODEL(DBModelDBAttachmentMailState, DBAttachmentMailState, PROPERTYS_AttachMentMailState)
{
public:
	DBModelDBAttachmentMailState()
	{
		clear();
	}

	virtual const char* getDBTableName()
	{
		return "t_attachment_mail";
	}

	void setMailID(int32 mailid)
	{
		mMailID = mailid;
	}

	void clear()
	{
		DBModelBase::clear();
		mMailID = INVALID;
		nBegin = 0;
		nEnd = 0;
	}

	void setMailIDRange(int32 begin, int32 nNum)
	{
		nBegin = begin;
		nEnd = nBegin + nNum;
	}
private:
	int32 mMailID;
	int32 nBegin, nEnd;

private:
	virtual String getSQLQueryConditionString()
	{
		String condition;
		condition.clear();

		// todo 

		return condition;
	}
};

//---------------------------------------------------------------------------------------------------------
// ContentMail
//---------------------------------------------------------------------------------------------------------
#define PROPERTYS_ContentMailState(PROPERTY)	\
	PROPERTY(int32, MailID, INVALID_ID)\
	PROPERTY(FixedString<DB_TITLE_LEN>, Title, "default")\
	PROPERTY(FixedString<DB_CONTENT_LEN>, Content, "default")\
	PROPERTY(int32, Valid, INVALID_ID)\

struct DBContentMailState
{
	STRUCT_PROPERTY_LIST(PROPERTYS_ContentMailState);

	BOOL isValid()
	{
		return (INVALID != mValid);
	}

	BOOL isValid() const
	{
		return (INVALID != mValid);
	}
};

DB_STRUCT_DATA_MODEL(DBModelContentMailState, DBContentMailState, PROPERTYS_ContentMailState)
{
public:
	DBModelContentMailState()
	{
		clear();
	}

	virtual const char* getDBTableName()
	{
		return "t_content_mail";
	}

	void setMailID(int32 mailid)
	{
		mMailID = mailid;
	}

	void clear()
	{
		DBModelBase::clear();
		mMailID = INVALID;
		nBegin = 0;
		nEnd = 0;
	}

	void setMailIDRange(int32 begin, int32 nNum)
	{
		nBegin = begin;
		nEnd = nBegin + nNum;
	}
private:
	int32 mMailID;
	int32 nBegin, nEnd;

private:
	virtual String getSQLQueryConditionString()
	{
		String condition;
		condition.clear();

		// todo 

		return condition;
	}
};

//---------------------------------------------------------------------------------------------------------
// systemmail [2/5/2018 Chief]
//---------------------------------------------------------------------------------------------------------
#define PROPERTYS_SystemMailState(PROPERTY)	\
	PROPERTY(int32, MailID, INVALID_ID)\
	PROPERTY(int32, MaxLv, INVALID_ID)\
	PROPERTY(int32, MinLv, INVALID_ID)\
	PROPERTY(int32, Valid, INVALID_ID)\

struct DBSystemMailState
{
	STRUCT_PROPERTY_LIST(PROPERTYS_SystemMailState);

	BOOL isValid()
	{
		return (INVALID != mValid);
	}

	BOOL isValid() const
	{
		return (INVALID != mValid);
	}
};

DB_STRUCT_DATA_MODEL(DBModelSystemMailState, DBSystemMailState, PROPERTYS_SystemMailState)
{
public:
	DBModelSystemMailState()
	{
		clear();
	}

	virtual const char* getDBTableName()
	{
		return "t_system_mail";
	}

	void setMailID(int32 mailid)
	{
		mMailID = mailid;
	}

	void clear()
	{
		DBModelBase::clear();
		mMailID = INVALID;
		nBegin = 0;
		nEnd = 0;
	}

	void setMailIDRange(int32 begin, int32 nNum)
	{
		nBegin = begin;
		nEnd = nBegin + nNum;
	}
private:
	int32 mMailID;
	int32 nBegin, nEnd;

private:
	virtual String getSQLQueryConditionString()
	{
		String condition;
		condition.clear();

		// todo 

		return condition;
	}
};


#endif // _PROPERTY_MAIL_STATE_H__
