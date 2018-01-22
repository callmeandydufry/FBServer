#include "stdafx.h"
#include "CGRpcInvokeMsg.h"

BOOL CGRpcInvokeMsg::Recieve(RecieveStream &iStream)
{
	__GUARD__
		mData.reci(iStream);
	return TRUE;
	__UNGUARD__
	return FALSE;
}

BOOL CGRpcInvokeMsg::Send(SendStream &oStream) const
{
	__GUARD__
		mData.send(oStream);
	return TRUE;
	__UNGUARD__
	return FALSE;
}

uint32 CGRpcInvokeMsg::Process(Connector *pPlayer)
{
	__GUARD__

	return CGRpcInvokeMsgDispatch::Process(this, pPlayer);

	__UNGUARD__ return FALSE;
}

