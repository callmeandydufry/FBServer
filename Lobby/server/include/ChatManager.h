#ifndef __CHAT_MANAGER_H__
#define __CHAT_MANAGER_H__

#include "ChatDefine.h"
#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "ServerModuleExportInterface.h"

class ChatManager
	: public ServerModuleBase
	, public IChatManager
{
	typedef int32 (ChatManager::*ChannelHandler)(const tagChatMsgDealUtil* pMsgUtil);

public:
	ChatManager();
	~ChatManager();

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);

	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }

public:

	//---------------------------------------------------------------------------------------
	// RPC远程接口定义
	//---------------------------------------------------------------------------------------
	// 聊天接口 [1/12/2018 Chief]
	virtual BOOL rpcHandlePlayerChatMsg(tagChatMsgDealUtil& tagMsgUtil);
	//---------------------------------------------------------------------------------------
	// RPC远程接口定义
	//---------------------------------------------------------------------------------------

private:
	int32				handlePrivateChannel(const tagChatMsgDealUtil* pMsgUtil);
	int32				handleGuildChannel(const tagChatMsgDealUtil* pMsgUtil);
	int32				handleWorldChannel(const tagChatMsgDealUtil* pMsgUtil);
	int32				handleRoomChannel(const tagChatMsgDealUtil* pMsgUtil);
protected:
	ChannelHandler		mArrChannelHandler[EChatChannel_End];

};


#endif // __CHAT_MANAGER_H__
