
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

	// ���ͻ�����share����Դ�İ汾������汾Ҳ��Ҫ�������ͬ�� [yangzhen]
	uint32 uSharedVer = VERSION_GET_3RD_SECTION(uCheckVer);
	// ż���棬��Ҫ�ͻ��˺Ϳͻ���ǿͬ�� [yangzhen]
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
	// �����������Ҫ��ͻ��˰汾һ��Ҫ���ڷ���ˣ���Ϊ���ͻ�����Դ����Ҳ�������汾�� [yangzhen]
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

	// �ͻ��˷������ķ���˰汾��һ��ҪС�ڷ���˱���İ汾�ţ������˵���汾������ [yangzhen]
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
		
		//��Incoming������ת�Ƶ�Gate������
		incomingMgr->DelConnector(pConnector);

		// ����״̬Ϊ����Login�߳� [3/19/2015 yangzhen]
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
// 			// ����״̬���ԣ�ֱ��������Ϣ [3/19/2015 yangzhen]
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

		// ���ֽ���������Ϣ���ݼ��� [12/28/2017 yz]
		((ClientConnector*)pConnector)->enableEncryptKey();
	}

	return MP_CONTINUE;

	__UNGUARD__ return MP_ERROR;
}
