#include "stdafx.h"
#include "ChatManager.h"
#include "ModuleManager.h"
#include "ClientSession.h"

ChatManager::ChatManager()
{

}

ChatManager::~ChatManager()
{

}

BOOL ChatManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	for (int32 i = 0; i < EChatChannel_End; ++i)
	{
		mArrChannelHandler[i] = NULL;
	}
	
	mArrChannelHandler[EChatChannel_Private] = &ChatManager::handlePrivateChannel;
	mArrChannelHandler[EChatChannel_Guild] = &ChatManager::handleGuildChannel;
	mArrChannelHandler[EChatChannel_World] = &ChatManager::handleWorldChannel;
	mArrChannelHandler[EChatChannel_Room] = &ChatManager::handleRoomChannel;

	return ServerModuleBase::initServerModule(moduleName, this, groupID);
}

void ChatManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void ChatManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);
}

BOOL ChatManager::rpcHandlePlayerChatMsg(tagChatMsgDealUtil& tagMsgUtil)
{
	__GUARD__

	// todo 
	/* check condition:
	1.	����
	2.	�������Ƶ��
	3.	�����ӦƵ���Ŀ������
	4.	�����д�����Ʒʱ��
		1.	�����Ʒ�ĺϷ���
		2.	����Ʒ���ж�̬���ԣ��Ƿ���Խ���̬���Ի�����ĳ���Է���������ҵĶ�̬��ȡ||�ڷ��͸�����ʱ���Ӷ�����Ϣ��������ģ�����ӻ��棿
	5.	ת���ַ�
	*/

	// handle msg
	ChannelHandler handler = mArrChannelHandler[tagMsgUtil.mChannel];
	int32 nRet = (this->*handler)(&tagMsgUtil);

	// log
	switch ((EChatResult)nRet)
	{
	case EChatResult_INVALID:
		break;
	case EChatResult_Sucess:
		break;
	case EChatResult_Fail:
		break;
	default:
		break;
	}

	return TRUE;
	__UNGUARD__
	return FALSE;
}

int32 ChatManager::handlePrivateChannel(const tagChatMsgDealUtil* pMsgUtil)
{
	__GUARD__;

	if (pMsgUtil)
	{
		if (INVALID_SNID == pMsgUtil->getReceiver()) return EChatResult_INVALID;

		if (EChatChannel_Private == pMsgUtil->getChannel()) return EChatResult_INVALID;

		g_pModuleManager->getExportCallable < IClientSessionExportCallable < ChatManager > >(
			ServerModuleUtil::getSessionModuleBySessionID(pMsgUtil->getReceiverSessionID()),
			pMsgUtil->getReceiverSessionID(),
			this
			)
			->rpcS2CSynPlayerChatData(pMsgUtil->getChatMsg().c_str(), pMsgUtil->getChatID(), pMsgUtil->getChannel(), pMsgUtil->getSender());
	}

	return EChatResult_Sucess;
	__UNGUARD__
	return EChatResult_Fail;
}

int32 ChatManager::handleGuildChannel(const tagChatMsgDealUtil* pMsgUtil)
{
	__GUARD__;

	return EChatResult_Sucess;
	__UNGUARD__;
	return EChatResult_Fail;
}

int32 ChatManager::handleWorldChannel(const tagChatMsgDealUtil* pMsgUtil)
{
	__GUARD__;

	if (pMsgUtil)
	{
		if (EChatChannel_World == pMsgUtil->getChannel()) return EChatResult_INVALID;

		g_pModuleManager->getExportCallable < IClientGateManagerExportCallable < ChatManager > >(
			ServerModuleUtil::getSessionModuleBySessionID(pMsgUtil->getSenderSessionID()),
			MODULE_DEFAULT_ID,
			this
			)
			->rpcS2CSynWorldChatData(pMsgUtil->getChatMsg().c_str(), pMsgUtil->getChatID(), pMsgUtil->getSender());
	}

	return EChatResult_Sucess;
	__UNGUARD__
	return EChatResult_Fail;
}

int32 ChatManager::handleRoomChannel(const tagChatMsgDealUtil* pMsgUtil)
{
	__GUARD__

		return EChatResult_Sucess;
	__UNGUARD__
		return EChatResult_Fail;
}
