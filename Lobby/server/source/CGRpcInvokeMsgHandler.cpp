
#include "stdafx.h"
#include "ClientConnector.h"
#include "BaseTool.h"
#include "CGRpcInvokeMsg.h"
#include "ClientSession.h"

uint32 CGRpcInvokeMsgDispatch::Process(CGRpcInvokeMsg *pKMessage, Connector *pConnector)
{
	__GUARD__

	ClientConnector* clientConnector = (ClientConnector*)pConnector;
	KCheck(clientConnector);
	ClientSession* session = clientConnector->getSession();
	if (NULL == session)
	{
		// 如果Session还没有绑定，说明这个消息发送的时机过早，直接丢弃 [12/23/2017 yz]
		return MP_CONTINUE;
	}

	// 将收到的消息缓存起来，在Session tick的时候会处理 [12/25/2017 yz]
	session->pushClientReceivedStream(pKMessage->getData());

	return MP_CONTINUE;

	__UNGUARD__ return MP_ERROR;
}
