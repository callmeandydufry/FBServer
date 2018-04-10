#ifndef __MAIL_DEFINE_H__
#define __MAIL_DEFINE_H__

// 因为有组合的可能，所以该type起不到严格区分的作用
enum EMailType
{
	EMT_INVALID					= -1,
	//----------------------------------------------------------------------
	// 附件类
	//----------------------------------------------------------------------
	EMT_ATTACHMENT				= 0,							

	EMT_Gift_FriendGiven,										// 好友赠送奖励
	EMT_ArenaAward,												// 竞技场奖励
	EMT_ATTACHMENT_END			= EMT_ATTACHMENT + 10000,		// 附件类
	
	//----------------------------------------------------------------------
	// 系统类(由客户端配置显示固定内容)
	//----------------------------------------------------------------------
	EMT_System					= EMT_ATTACHMENT_END + 1,		

	EMT_Invite_FromFriend,										// 好友邀请
	EMTSys_END					= EMT_System + 10000,

	//----------------------------------------------------------------------
	// 文本类(如公告等不定文本)
	//----------------------------------------------------------------------
	EMT_CONTENT					= EMTSys_END + 1,

	EMT_NOTICE,													// 系统公告
	EMT_CONTENT_END				= EMT_NOTICE + 10000,

	EMT_END,
};

enum EMailStatus
{
	EMS_Read					= 0x01,		// 已读未读
	EMS_GainAttachMents			= 0x02,		// 是否获取了奖励
	EMS_DEL						= 0x04,		// 玩家手动删除邮件
	EMS_OtherS					= 0x08,
};

#define MAX_MAIL_SEND_LV		(200)
#define MIN_MAIL_SEND_LV		(0)

// 附件个数
#define MAX_ATTACHMENT_NUMS		(3)
// 最大邮件数量
#define MAX_MAIL_NUMS			(50)
// 每次s2c同步时发送的邮件数量
#define MAX_S2CSYNMAIL_NUMS		(50)  

enum EDBMailType
{
	EDBT_Base = 0,
	EDBT_Attach = 1,
	EDBT_Content = 2,
	EDBT_System = 3,

	EDBT_END,
};

enum EDBOper
{
	EDBO_Insert,
	EDBO_Update,
	EDBO_Delete,
};

#define BATCH_BaseMail_NUM		(1000)
#define BATCH_AttachmentMail_NUM (1000)
#define BATCH_ContentMail_NUM	(1000)
#define BATCH_SystemMail_NUM	(1000)

#define STRUCT_MAIL_NUMS(PROPERTY)	\
	PROPERTY(int32, BaseMailNum, -1)\
	PROPERTY(int32, AttachmentMailNum, -1)\
	PROPERTY(int32, ContentMailNum, -1)\
	PROPERTY(int32, SystemMailNum, -1)\

struct tagMailsNum
{
	STRUCT_SHARESTRUCT_LIST(STRUCT_MAIL_NUMS);

	void operator=(tagMailsNum& stNums)
	{
		mBaseMailNum		= stNums.mBaseMailNum;
		mAttachmentMailNum	= stNums.mAttachmentMailNum;
		mContentMailNum		= stNums.mContentMailNum;
		mSystemMailNum		= stNums.mSystemMailNum;
	}
};

struct tagPlayerMailBase
{
	int32 mMailID;
	BYTE mMailStatus;

	void Clear()
	{
		mMailID = INVALID;
		mMailStatus = 0;
	}

	void serializeJson(JsonSerializer* serializer)
	{
		JSON_ATTRIBUTE("ID", mMailID);
		JSON_ATTRIBUTE("U", mMailStatus);
	}

	void setMailStatus(EMailStatus eType)
	{
		mMailStatus |= eType;
	}

	BOOL getMailStatus(EMailStatus eType)
	{
		return (mMailStatus &= eType);
	}

	JSON_SERIALIZE_METHODS();
};

// 最大邮件个数 [2/8/2018 Chief]
#define MAX_MAIL_SIZE			(50)
struct tagMailIDList
{
	tagPlayerMailBase mMailData[MAX_MAIL_SIZE];
	void Clear()
	{
		for (int32 i = 0; i < MAX_MAIL_SIZE; ++i)
		{
			mMailData[i].Clear();
		}
	}
	void serializeJson(JsonSerializer* serializer)
	{
		JSON_ATTRIBUTE_ARRAY("MailData", mMailData);
	}

	int32 getValidMailNums()
	{
		int32 nNum = 0;
		for (int32 i = 0; i < MAX_MAIL_SIZE; ++i)
		{
			if (mMailData[i].mMailID != INVALID)
				nNum++;
		}
		return nNum;
	}

	int32 findMailByID(int32 nID)
	{
		for (int32 i = 0; i < MAX_MAIL_SIZE; ++i)
		{
			if (mMailData[i].mMailID == nID)
				return i;
		}
		return 0;
	}

	void operator=(tagMailIDList& stList)
	{
		for (int32 i = 0; i < MAX_MAIL_SIZE; ++i)
		{
			mMailData[i].mMailID = stList.mMailData[i].mMailID;
			mMailData[i].mMailStatus = stList.mMailData[i].mMailStatus;
		}
	}

	JSON_SERIALIZE_METHODS();
};

// 邮件持续时间,初始定为3天 [2/8/2018 Chief]
#define MAX_MAIL_USE_TIME		(3600 * 12 * 3)



#endif // __MAIL_DEFINE_H__
