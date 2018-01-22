#include "stdafx.h"
#include "CGRPChandler.h"
#include "ServerModuleExportInterface.h"
#include "ModuleInlineMacros.h"
#include "OnlinePlayerManager.h"
#include "ClientSession.h"
#include "ClientGateManager.h"
#include "ChatDefine.h"

void CGRPCHandler::sendChatMsg2OtherPlayer(const char* chatContent,SNID_t u64ReceiverID)
{
	// 判断对方是否在线
	BOOL bOnline = mClientSession->getClientGateManager()->checkSessionOnlineBySnid(u64ReceiverID);
	
	if (bOnline)
	{
		// 构造消息
		tagChatMsgDealUtil tagChatMsg;
		tagChatMsg.clear();
		tagChatMsg.setSender(mClientSession->getPlayerSnid());
		tagChatMsg.setReceiver(u64ReceiverID);
		tagChatMsg.setChannel(EChatChannel_Private);
		tagChatMsg.setChatID(0);
		//tagChatMsg.setLevel(1); // Question: 需要去Reg模块获取玩家的基本数据？是否可以在session缓存 --》或者，基础信息在玩家进入游戏后点开好友list时请求
		tagChatMsg.setChatMsg(chatContent);
		uint64 nReceiverSessionID = mClientSession->getClientGateManager()->getSessionIDBySnid(u64ReceiverID);
		tagChatMsg.setReceiverSessionID(nReceiverSessionID);
		uint64 nSenderSessionID = mClientSession->getSessionID();
		tagChatMsg.setSenderSessionID(nSenderSessionID);

		// 发送给chatmodule
		g_pModuleManager->getExportCallable < IChatManagerExportCallable < ClientSession > >(
			ServerModuleUtil::getOnlineModuleByPlayerSnid(mClientSession->getPlayerSnid()),
			MODULE_DEFAULT_ID,
			mClientSession
			)
			->rpcHandlePlayerChatMsg(tagChatMsg);

	}
	else
	{
		// todo 对于下线玩家，存储数据到Reg？
	}
}

void CGRPCHandler::requestPlayerData(SNID_t snid)
{
	g_pModuleManager->getExportCallable < IOnlinePlayerExportCallable < ClientSession > >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestPlayerDetialData(mClientSession->getSessionID());
}