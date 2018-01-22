
#include "stdafx.h"
#include "ClientConnector.h"
#include "BaseTool.h"
#include "ClientGateManager.h"
#include "CGReqeustLogin.h"

uint32 CGRequestLoginDispatch::Process(CGRequestLogin *pKMessage, Connector *pConnector)
{
	__GUARD__
	QLogSystem::QUICK_LOG
	(
		QUICK_LOG_DEBUG,
		"CGRequestLoginDispatch::Process(...) Receive new login request id:%d ...OK ",
		pConnector->GetConnectorID()
	);

	ClientConnector* clientConnector = (ClientConnector*)pConnector;
	KCheck(clientConnector);
	ClientGateManager* gateMgr = clientConnector->GetClientGateManager();
	ClientSession* session = gateMgr->createNewSession(pKMessage->GetAccount()->c_str(), pKMessage->GetToken()->c_str());
	if (NULL == session)
	{
		// ÊÍ·ÅConnectorÁ¬½Ó [12/23/2017 yz]
		return MP_ERROR;
	}

	clientConnector->bindSession(session);

	return MP_CONTINUE;

	__UNGUARD__ return MP_ERROR;
}
