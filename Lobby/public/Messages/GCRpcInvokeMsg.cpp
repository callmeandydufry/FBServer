#include "stdafx.h"
#include "GCRpcInvokeMsg.h"

BOOL GCRpcInvokeMsg::Recieve(RecieveStream &iStream)
{
	__GUARD__
		mData.reci(iStream);
	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL GCRpcInvokeMsg::Send(SendStream &oStream) const
{
	__GUARD__
		mData.send(oStream);
	return TRUE;
	__UNGUARD__
		return FALSE;
}

uint32 GCRpcInvokeMsg::Process(Connector *pPlayer)
{
	__GUARD__

		return GCRpcInvokeMsgDispatch::Process(this, pPlayer);

	__UNGUARD__ return FALSE;
}

