
#include "stdafx.h"
#include "ClientConnector.h"
#include "BaseTool.h"
#include "ConnectorManager.h"

#include "GCHandShake.h"
uint32 GCHandShakeDispatch::Process(GCHandShake *pKMessage, Connector *pConnector)
{
	__GUARD__
	return MP_CONTINUE;
	__UNGUARD__ return MP_ERROR;
}

#include "GCRpcInvokeMsg.h"
uint32 GCRpcInvokeMsgDispatch::Process(GCRpcInvokeMsg *pKMessage, Connector *pConnector)
{
	__GUARD__
	return MP_CONTINUE;
	__UNGUARD__ return MP_ERROR;
}

#include "GCEncryptKey.h"
uint32 GCEncryptKeyDispatch::Process(GCEncryptKey *pKMessage, Connector *pConnector)
{
	__GUARD__
		return MP_CONTINUE;
	__UNGUARD__ return MP_ERROR;
}

#include "GCPing.h"
uint32 GCPingDispatch::Process(GCPing *pKMessage, Connector *pConnector)
{
	__GUARD__
		return MP_CONTINUE;
	__UNGUARD__ return MP_ERROR;
}
