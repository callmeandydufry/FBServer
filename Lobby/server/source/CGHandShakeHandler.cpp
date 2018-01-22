
#include "stdafx.h"
#include "CGHandShake.h"
#include "ClientConnector.h"
#include "BaseTool.h"
#include "ConnectorManager.h"
#include "IncomingConnectorManager.h"
#include "SessionConnectorManager.h"
#include "ClientGateManager.h"
#include "GCHandShake.h"

BOOL CheckVersion(uint32 uCheckVer, SNID_t uSnid)
{
	uint32 iVersionClient = uCheckVer;
	iVersionClient = iVersionClient & STRICT_VERSION_CHECK_MASK;
	uint32 iVersionServer = CURRENT_STRICT_VERSION;

	if (iVersionClient != iVersionServer)
	{
		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_LOGIN,
			"ERROR: CheckVersion CodeVersion check failed SNID=%llu, ClientVer=%d, ServerVer=%d",
			uSnid,
			uCheckVer,
			GET_CURRENT_VERSION()
		);
		return FALSE;
	}

	// 检测客户端与share下资源的版本，这个版本也需要与服务器同步 [yangzhen]
	uint32 uSharedVer = VERSION_GET_3RD_SECTION(uCheckVer);
	// 偶数版，需要客户端和客户端强同步 [yangzhen]
	if ((uSharedVer % 2) == 0)
	{
		if (uSharedVer != VERSION_GET_3RD_SECTION(GET_CURRENT_VERSION()))
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_LOGIN,
				"ERROR: CheckVersion Client and server shared resource version check failed SNID=%llu, ClientServerVer=%d, ServerServerVer=%d",
				uSnid,
				uSharedVer,
				VERSION_GET_3RD_SECTION(GET_CURRENT_VERSION())
			);
			return FALSE;
		}
	}
	// 如果是奇数则要求客户端版本一定要大于服务端，因为纯客户端资源更新也会升级版本号 [yangzhen]
	else if (uSharedVer < VERSION_GET_3RD_SECTION(GET_CURRENT_VERSION()))
	{
		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_LOGIN,
			"ERROR: CheckVersion Client and server shared resource version check failed SNID=%llu, ClientServerVer=%d, ServerServerVer=%d",
			uSnid,
			uSharedVer,
			VERSION_GET_3RD_SECTION(GET_CURRENT_VERSION())
		);
		return FALSE;
	}

	// 客户端发过来的服务端版本号一定要小于服务端本身的版本号，否则就说明版本发错了 [yangzhen]
	if (VERSION_GET_4TH_SECTION(uCheckVer) > VERSION_GET_4TH_SECTION(GET_CURRENT_VERSION()))
	{
		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_LOGIN,
			"ERROR: CheckVersion ServerVersion Check FALSE SNID=%llu, ClientServerVer=%d, ServerServerVer=%d",
			uSnid,
			VERSION_GET_4TH_SECTION(uCheckVer),
			VERSION_GET_4TH_SECTION(GET_CURRENT_VERSION())
		);
		return FALSE;
	}

	return TRUE;
}


uint32 CGHandShakeDispatch::Process(CGHandShake *pKMessage, Connector *pConnector)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ QLogSystem::QUICK_LOG
	(
		QUICK_LOG_DEBUG,
		"CGHandShakeDispatch::Process(...) Receive new connector id:%d ...OK ",
		pConnector->GetConnectorID()
	);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (!pKMessage->IsLegal())
	{
		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_LOGIN,
			"ERROR: CGHandShakeDispatch::Process Client MagicNum check failed! IP=%s",
			pConnector->GetSocket()->m_IP
		);
		return MP_ERROR;
	}

	if (pConnector->GetConnectorMgr() != NULL 
		&& strncmp(pConnector->GetConnectorMgr()->GetManagerName(), "IncomingConnectorManager", MAX_ACCOUNT) == 0)
	{

		IncomingConnectorManager* incomingMgr = (IncomingConnectorManager*)pConnector->GetConnectorMgr();

		if (!CheckVersion(pKMessage->GetCheckVer(), 0xFFFFFFFF))
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_LOGIN,
				"ERROR: CGHandShakeDispatch::Process Client version check failed! IP=%s",
				pConnector->GetSocket()->m_IP
			);

			GCHandShake	Msg;
			Msg.SetResult(EHAND_SHAKE_VERSION_FAILED);
			Msg.SetUserKey(0);
			Msg.SetEncryptArith(0);
			Msg.SetRsaIndex(0);

			pConnector->SendMessage(&Msg);

			return MP_CONTINUE;
		}

		((ClientConnector*)pConnector)->SetClientVersion(pKMessage->GetCheckVer());
		((ClientConnector*)pConnector)->SetClientGateManager(incomingMgr->getGateManager());
		
		//从Incoming管理器转移到Gate管理器
		incomingMgr->DelConnector(pConnector);

		// 设置状态为进入Login线程 [3/19/2015 yangzhen]
		//pGameConnetor->SetStatus(PS_KHAN_LOGIN_ENTERING_LOGIN);

		SessionConnectorManager* sessConnMgr = incomingMgr->getGateManager()->getSessionConnectorManager();
		sessConnMgr->SendMessage(pKMessage, pConnector->GetConnectorID());

		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_LOGIN,
			"CGHandShakeDispatch::Process(...)...IncomingThread,IP=%s",
			pConnector->GetSocket()->m_IP
		); 

		return MP_NOTREMOVE;
	}
	else
	{
		ClientGateManager* gateMgr = ((ClientConnector*)pConnector)->GetClientGateManager();
		SessionConnectorManager* sessionConnMgr = gateMgr->getSessionConnectorManager();
		sessionConnMgr->AddConnector(pConnector);

// 		if (pConnector->GetStatus() != PS_KHAN_LOGIN_ENTERING_LOGIN)
// 		{
// 			// 连接状态不对，直接抛弃消息 [3/19/2015 yangzhen]
// 			return MP_ERROR;
// 		}

		GCHandShake	Msg;
		Msg.SetResult(EHAND_SHAKE_SUCCESS);
		Msg.SetUserKey(0);
		Msg.SetEncryptArith(0);
		Msg.SetRsaIndex(0);
		Msg.SetLoginIP(pConnector->GetSocket()->m_IP);
		Msg.SetLoginPort(pConnector->GetSocket()->m_Port);
		pConnector->SendMessage(&Msg);

		// 握手结束后开启消息数据加密 [12/28/2017 yz]
		((ClientConnector*)pConnector)->enableEncryptKey();
	}

	return MP_CONTINUE;

	__UNGUARD__ return MP_ERROR;
}
