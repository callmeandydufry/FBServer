#include "stdafx.h"
#include "CGHandShake.h"

CGHandShake::CGHandShake()
{
	m_MagicNum = 0;

	m_QuickMark = 0;
	m_CheckVer = GET_CURRENT_VERSION();
};

CGHandShake::~CGHandShake()
{}

BOOL CGHandShake::IsLegal()
{
	if (m_MagicNum == 20100801)
		return TRUE;
	return FALSE;
}

uint32 CGHandShake::GetMsgSize() const
{
	return sizeof(int32) + sizeof(int8) + sizeof(m_CheckVer);
}

uint32 CGHandShakeFactory::GetMessageMaxSize() const
{
	return sizeof(int32) + sizeof(int8) + sizeof(uint32);
}


IMPLEMENT_NET_MESSAGE_RECV_BEGIN(CGHandShake, MESSAGE_CG_HANDSHAKE)
RECV_NET_MESSAGE_ATOM_VAR(MagicNum)
RECV_NET_MESSAGE_ATOM_VAR(QuickMark)
RECV_NET_MESSAGE_ATOM_VAR(CheckVer)
IMPLEMENT_NET_MESSAGE_RECV_END(CGHandShake, MESSAGE_CG_HANDSHAKE)

IMPLEMENT_NET_MESSAGE_SEND_BEGIN(CGHandShake, MESSAGE_CG_HANDSHAKE)
SEND_NET_MESSAGE_ATOM_VAR(MagicNum)
SEND_NET_MESSAGE_ATOM_VAR(QuickMark)
SEND_NET_MESSAGE_ATOM_VAR(CheckVer)
IMPLEMENT_NET_MESSAGE_SEND_END(CGHandShake, MESSAGE_CG_HANDSHAKE)

