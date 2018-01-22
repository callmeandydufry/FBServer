#include "stdafx.h"
#include "GCHandShake.h"
using namespace Messages;


BOOL GCHandShake::Recieve(RecieveStream &iStream)
{
	__GUARD__ 
	iStream.Reci((char *)&Result, sizeof(Result));
	iStream.Reci((char *)&LoginPort, sizeof(LoginPort));
	iStream.Reci((char *)&LoginIP, sizeof(LoginIP));
	iStream.Reci((char *)&Key, sizeof(Key));
	iStream.Reci((char *)&EncryptArith, sizeof(EncryptArith));
	iStream.Reci((char *)&RsaIndex, sizeof(RsaIndex));

	return TRUE;

	__UNGUARD__ return FALSE;
}

BOOL GCHandShake::Send(SendStream &oStream) const
{
	__GUARD__ oStream.Send((char *)&Result, sizeof(Result));
	oStream.Send((char *)&LoginPort, sizeof(LoginPort));
	oStream.Send((char *)&LoginIP, sizeof(LoginIP));
	oStream.Send((char *)&Key, sizeof(Key));
	oStream.Send((char *)&EncryptArith, sizeof(EncryptArith));
	oStream.Send((char *)&RsaIndex, sizeof(RsaIndex));

	return TRUE;

	__UNGUARD__ return FALSE;
}

uint32 GCHandShake::Process(Connector *pPlayer)
{
	__GUARD__ return GCHandShakeDispatch::Process(this, pPlayer);

	__UNGUARD__ return FALSE;
}
