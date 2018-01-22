
#include "stdafx.h"
#include "ModulePacketMessage.h"
#include "ModuleManager.h"

BOOL XXModulePacket::Recieve(RecieveStream &iStream)
{
	__GUARD__
	createDataPacket()->reci(iStream);
	return TRUE;

	__UNGUARD__ return FALSE;
}

BOOL XXModulePacket::Send(SendStream &oStream) const
{
	__GUARD__;
	createDataPacket()->send(oStream);
	return TRUE;
	__UNGUARD__ return FALSE;
}

uint32 XXModulePacket::Process(Connector *pPlayer)
{
	__GUARD__ 
		
	return XXModulePacketDispatch::Process(this, pPlayer);

	__UNGUARD__ return FALSE;
}

DataPacket* XXModulePacket::createDataPacket() const
{
	__GUARD__

	if (NULL == mPacketData)
	{
		mPacketData = new DataPacket;
	}
	return mPacketData;

	__UNGUARD__ return NULL;
}

uint32 XXModulePacketDispatch::Process(XXModulePacket *pMessage, Connector *pConnector)
{
	__GUARD__

	TID	CurrentThreadID = KGetCurrentTID();

	DataPacket* packet = pMessage->exchangeDataPacket();
	g_pModuleManager->receivePacket(packet->mTargetModule, packet);

	return MP_CONTINUE;
	__UNGUARD__ return MP_ERROR;
}