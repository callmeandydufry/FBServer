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
	// �ж϶Է��Ƿ�����
	BOOL bOnline = mClientSession->getClientGateManager()->checkSessionOnlineBySnid(u64ReceiverID);
	
	if (bOnline)
	{
		// ������Ϣ
		tagChatMsgDealUtil tagChatMsg;
		tagChatMsg.clear();
		tagChatMsg.setSender(mClientSession->getPlayerSnid());
		tagChatMsg.setReceiver(u64ReceiverID);
		tagChatMsg.setChannel(EChatChannel_Private);
		tagChatMsg.setChatID(0);
		//tagChatMsg.setLevel(1); // Question: ��ҪȥRegģ���ȡ��ҵĻ������ݣ��Ƿ������session���� --�����ߣ�������Ϣ����ҽ�����Ϸ��㿪����listʱ����
		tagChatMsg.setChatMsg(chatContent);
		uint64 nReceiverSessionID = mClientSession->getClientGateManager()->getSessionIDBySnid(u64ReceiverID);
		tagChatMsg.setReceiverSessionID(nReceiverSessionID);
		uint64 nSenderSessionID = mClientSession->getSessionID();
		tagChatMsg.setSenderSessionID(nSenderSessionID);

		// ���͸�chatmodule
		g_pModuleManager->getExportCallable < IChatManagerExportCallable < ClientSession > >(
			ServerModuleUtil::getOnlineModuleByPlayerSnid(mClientSession->getPlayerSnid()),
			MODULE_DEFAULT_ID,
			mClientSession
			)
			->rpcHandlePlayerChatMsg(tagChatMsg);

	}
	else
	{
		// todo ����������ң��洢���ݵ�Reg��
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
// �ʼ���� [1/31/2018 Chief]
//-------------------------------------------------------------------------------------------------------------
// ����ǰ�Ƿ����µ��ʼ� [1/31/2018 Chief]
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

// �������������ʼ�list [1/31/2018 Chief]
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

// ����ʼ�����,��ǵ�ǰ�ʼ��Ѷ� [1/31/2018 Chief]
void CGRPCHandler::requestMailDetial(SNID_t snid, int32 mailID)
{
	__GUARD__;

	// ֪ͨonline�ʼ����� [2/9/2018 Chief]
	g_pModuleManager->getExportCallable < IOnlinePlayerExportCallable < ClientSession > >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestReadMail(mailID);

	// ��mailmgr����ȡ�ʼ����� [2/9/2018 Chief]
	g_pModuleManager->getExportCallable< IMailManagerExportCallable<ClientSession> >(
		ServerModuleUtil::getMailModuleByPlayerSnid(snid),
		mClientSession->getPlayerSnid(),
		mClientSession
		)
		->rpcRequestMailDetail(mailID, mClientSession->getSessionID());

	__UNGUARD__;
}

// ��ȡ�ʼ����� [1/31/2018 Chief]
void CGRPCHandler::requestGainMail(SNID_t snid, int32 mailID)
{
	__GUARD__;

	// todo �ȴ���Ʒϵͳ���

	__UNGUARD__;
}

// ɾ�������ʼ� [1/31/2018 Chief]
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

// ����ɾ��ĳһ���͵��ʼ� [1/31/2018 Chief]
void CGRPCHandler::requestDelMailByType(SNID_t snid, int32 mailType)
{
	__GUARD__;

	// todo

	__UNGUARD__;
}