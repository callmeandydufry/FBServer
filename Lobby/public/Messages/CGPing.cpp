
#include "stdafx.h"
#include "CGPing.h"

CGPing::CGPing()
{
}

CGPing::~CGPing()
{
}

IMPLEMENT_NET_MESSAGE_RECV_BEGIN(CGPing, MESSAGE_CG_PING)
RECV_NET_MESSAGE_ATOM_VAR(Index)
IMPLEMENT_NET_MESSAGE_RECV_END(CGPing, MESSAGE_CG_PING)

IMPLEMENT_NET_MESSAGE_SEND_BEGIN(CGPing, MESSAGE_CG_PING)
SEND_NET_MESSAGE_ATOM_VAR(Index)
IMPLEMENT_NET_MESSAGE_SEND_END(CGPing, MESSAGE_CG_PING)

uint32 CGPing::GetMsgSize() const
{
	return sizeof(m_Index);
}

uint32 CGPingFactory::GetMessageMaxSize() const
{
	return sizeof(int16);
}
