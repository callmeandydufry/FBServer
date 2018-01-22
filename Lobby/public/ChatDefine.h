#ifndef __CHAT_DEFINE_H__
#define __CHAT_DEFINE_H__

#include "ShareStructDefine.h"
#include "FixedString.h"

/************************************************************************/
/* 聊天 server->server */
/************************************************************************/
#define STRUCT_CHATMSGDEAL_UTIL(PROPERTY)	\
	PROPERTY(SNID_t, Sender, -1)\
	PROPERTY(SNID_t, Receiver, -1)\
	PROPERTY(uint64, SenderSessionID, -1)\
	PROPERTY(uint64, ReceiverSessionID, -1)\
	PROPERTY(uint32, ChatID, 0)\
	PROPERTY(uint32, Channel, 0)\
	PROPERTY(FixedString<DB_MAX_STR_LEN>, ChatMsg, "defaultMsg")\

struct tagChatMsgDealUtil
{
	STRUCT_SHARESTRUCT_LIST(STRUCT_CHATMSGDEAL_UTIL);
};

enum EChatChannel
{
	EChatChannel_NULL		= 0,
	EChatChannel_World		= 1,							//世界
	EChatChannel_Private	= 2,							//私聊
	EChatChannel_Guild		= 3,							//公会
	EChatChannel_Room		= 4,							//队伍

	EChatChannel_End,
};

enum EChatResult
{
	EChatResult_INVALID		= -1,
	EChatResult_Sucess		= 0,
	EChatResult_Fail		= 1,

};




#endif // __CHAT_DEFINE_H__
