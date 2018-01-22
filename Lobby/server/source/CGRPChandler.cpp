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