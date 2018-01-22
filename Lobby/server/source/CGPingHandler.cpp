
#include "stdafx.h"
#include "ClientConnector.h"
#include "BaseTool.h"
#include "CGPing.h"
#include "GCPing.h"

using namespace Messages;

uint32 CGPingDispatch::Process(CGPing *pKMessage, Connector *pConnector)
{
	__GUARD__

	GCPing msg;
	msg.SetIndex(pKMessage->GetIndex());
	pConnector->SendMessage(&msg);

	return MP_CONTINUE;

	__UNGUARD__ return MP_ERROR;
}
