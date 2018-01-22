
#include "stdafx.h"
#include "GCPing.h"

GCPing::GCPing()
{
}

GCPing::~GCPing()
{
}

IMPLEMENT_NET_MESSAGE_RECV_BEGIN(GCPing, MESSAGE_GC_PING)
RECV_NET_MESSAGE_ATOM_VAR(Index)
IMPLEMENT_NET_MESSAGE_RECV_END(GCPing, MESSAGE_GC_PING)

IMPLEMENT_NET_MESSAGE_SEND_BEGIN(GCPing, MESSAGE_GC_PING)
SEND_NET_MESSAGE_ATOM_VAR(Index)
IMPLEMENT_NET_MESSAGE_SEND_END(GCPing, MESSAGE_GC_PING)

uint32 GCPing::GetMsgSize() const
{
	return sizeof(m_Index);
}

uint32 GCPingFactory::GetMessageMaxSize() const
{
	return sizeof(int16);
}
