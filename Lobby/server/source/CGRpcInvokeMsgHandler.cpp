
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
		// ���Session��û�а󶨣�˵�������Ϣ���͵�ʱ�����磬ֱ�Ӷ��� [12/23/2017 yz]
		return MP_CONTINUE;
	}

	// ���յ�����Ϣ������������Session tick��ʱ��ᴦ�� [12/25/2017 yz]
	session->pushClientReceivedStream(pKMessage->getData());

	return MP_CONTINUE;

	__UNGUARD__ return MP_ERROR;
}
