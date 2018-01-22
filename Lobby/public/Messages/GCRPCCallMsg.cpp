#include "stdafx.h"
#include "GCRPCCallMsg.h"

GCRpcCallMsg::GCRpcCallMsg()
{
};

GCRpcCallMsg::~GCRpcCallMsg()
{}

uint32 GCRpcCallMsg::GetMsgSize() const
{
	return m_stDataPacket.getSize();
}

uint32 GCRpcCallMsgFactory::GetMessageMaxSize() const
{
	return sizeof(GCRpcCallMsg) - sizeof(IMessage);
}


IMPLEMENT_NET_MESSAGE_RECV_BEGIN(GCRpcCallMsg, MESSAGE_GC_RPC_CALL)
RECV_NET_MESSAGE_STRUCT_VAR(stDataPacket)
IMPLEMENT_NET_MESSAGE_RECV_END(GCRpcCallMsg, MESSAGE_GC_RPC_CALL)

IMPLEMENT_NET_MESSAGE_SEND_BEGIN(GCRpcCallMsg, MESSAGE_GC_RPC_CALL)
SEND_NET_MESSAGE_STRUCT_VAR(stDataPacket)
IMPLEMENT_NET_MESSAGE_SEND_END(GCRpcCallMsg, MESSAGE_GC_RPC_CALL)
