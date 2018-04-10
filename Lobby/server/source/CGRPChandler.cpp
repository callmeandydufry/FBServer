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

//-------------------------------------------------------------------------------------------------------------
// 邮件相关 [1/31/2018 Chief]
//-------------------------------------------------------------------------------------------------------------
// 请求当前是否有新的邮件 [1/31/2018 Chief]
void CGRPCHandler::requestNewMailList(SNID_t snid)
{
	__GUARD__;
	g_pModuleManager->getExportCallable < IRegisterPlayerManagerExportCallable < ClientSession > >(
		ServerModuleUtil::getRegisterModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestUnreadMailNums(snid)
		->bindCallback(
			&ClientSession::onRPCRequestUnreadMailNumsCallbackSucess,
			&ClientSession::onRPCRequestUnreadMailNumsCallbackOverTime,
			5000
		);
	__UNGUARD__;
}

// 根据类型请求邮件list [1/31/2018 Chief]
void CGRPCHandler::requestTypeMailList(SNID_t snid, int32 mailType)
{
	__GUARD__;

	g_pModuleManager->getExportCallable < IOnlinePlayerExportCallable < ClientSession > >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestPlayerMailList();

	__UNGUARD__;
}

// 获得邮件详情,标记当前邮件已读 [1/31/2018 Chief]
void CGRPCHandler::requestMailDetial(SNID_t snid, int32 mailID)
{
	__GUARD__;

	// 通知online邮件已阅 [2/9/2018 Chief]
	g_pModuleManager->getExportCallable < IOnlinePlayerExportCallable < ClientSession > >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestReadMail(mailID);

	// 从mailmgr处获取邮件详情 [2/9/2018 Chief]
	g_pModuleManager->getExportCallable< IMailManagerExportCallable<ClientSession> >(
		ServerModuleUtil::getMailModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestMailDetail(mailID, mClientSession->getSessionID());

	__UNGUARD__;
}

// 获取邮件附件 [1/31/2018 Chief]
void CGRPCHandler::requestGainMail(SNID_t snid, int32 mailID)
{
	__GUARD__;

	// todo 等待物品系统完成

	__UNGUARD__;
}

// 删除单个邮件 [1/31/2018 Chief]
void CGRPCHandler::requestDelMailByID(SNID_t snid, int32 mailID)
{
	__GUARD__;

	g_pModuleManager->getExportCallable < IOnlinePlayerExportCallable < ClientSession > >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestDelMail(mailID);

	__UNGUARD__;
}

// 批量删除某一类型的邮件 [1/31/2018 Chief]
void CGRPCHandler::requestDelMailByType(SNID_t snid, int32 mailType)
{
	__GUARD__;

	// todo

	__UNGUARD__;
}