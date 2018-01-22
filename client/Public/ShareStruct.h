#ifndef __SHARE_STRUCT_H__
#define __SHARE_STRUCT_H__

#include "ShareStructDefine.h"
#include "Define.h"

//--------------------------------------------------------------------------------------------------
// 玩家登录成功后请求的数据 [1/11/2018 Chief]
//--------------------------------------------------------------------------------------------------
#define STRUCT_PLAYERDETIALDATA(PROPERTY)	\
	PROPERTY(FixedString<MAX_CHARACTER_NAME>, CharacterName, " ")\
	PROPERTY(int32, Gold, 0)\
	PROPERTY(int32, Diamond, 0)\
	PROPERTY(int32, Level, 0)\
	PROPERTY(uint64, Exp, 0)\

struct SPlayerExtData
{
	STRUCT_SHARESTRUCT_LIST(STRUCT_PLAYERDETIALDATA);
};

//--------------------------------------------------------------------------------------------------
// 聊天数据 [1/15/2018 Chief]
//--------------------------------------------------------------------------------------------------

#define STRUCT_SERVER2CLIENTCHATMSG_DATA(PROPERTY)	\
	PROPERTY(SNID_t, Sender, -1)\
	PROPERTY(uint32, ChatID, 0)\
	PROPERTY(uint32, Channel, 0)\
	PROPERTY(FixedString<DB_MAX_STR_LEN>, ChatMsg, "defaultMsg")\

struct tagSendChatMsgData
{
	STRUCT_SHARESTRUCT_LIST(STRUCT_SERVER2CLIENTCHATMSG_DATA);
};

//--------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------


#endif // __SHARE_STRUCT_H__