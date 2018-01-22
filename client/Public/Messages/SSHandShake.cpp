/*$T Common/Messages/SSHandShake.cpp GC 1.140 10/10/07 10:06:39 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "SSHandShake.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */

BOOL SSHandShake::Recieve(RecieveStream &iStream)
{
	__GUARD__ iStream.Reci((char *) (&m_MapID), sizeof(ID_t));
	iStream.Reci((char *) (&m_MainVersion), sizeof(ID_t));
	iStream.Reci((char *) (&m_SubVersion), sizeof(ID_t));
	iStream.Reci((char *) (&m_InnerVersion), sizeof(ID_t));
	iStream.Reci((char *) (&m_Revision), sizeof(ID_t));

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL SSHandShake::Send(SendStream &oStream) const
{
	__GUARD__ oStream.Send((char *) (&m_MapID), sizeof(ID_t));
	oStream.Send((char *) (&m_MainVersion), sizeof(ID_t));
	oStream.Send((char *) (&m_SubVersion), sizeof(ID_t));
	oStream.Send((char *) (&m_InnerVersion), sizeof(ID_t));
	oStream.Send((char *) (&m_Revision), sizeof(ID_t));

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 SSHandShake::Process(Connector *pConnector)
{
	__GUARD__ return SSHandShakeDispatch::Process(this, pConnector);

	__UNGUARD__ return FALSE;
}
