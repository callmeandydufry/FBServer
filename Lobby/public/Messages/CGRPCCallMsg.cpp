#include "stdafx.h"
#include "CGRPCCallMsg.h"

CGRpcCallMsg::CGRpcCallMsg()
{
};

CGRpcCallMsg::~CGRpcCallMsg()
{}

uint32 CGRpcCallMsg::GetMsgSize() const
{
	return m_stDataPacket.getSize();
}

uint32 CGRpcCallMsgFactory::GetMessageMaxSize() const
{
	return sizeof(CGRpcCallMsg) - sizeof(IMessage);
}


IMPLEMENT_NET_MESSAGE_RECV_BEGIN(CGRpcCallMsg, MESSAGE_CG_RPC_CALL)
RECV_NET_MESSAGE_STRUCT_VAR(stDataPacket)
IMPLEMENT_NET_MESSAGE_RECV_END(CGRpcCallMsg, MESSAGE_CG_RPC_CALL)

IMPLEMENT_NET_MESSAGE_SEND_BEGIN(CGRpcCallMsg, MESSAGE_CG_RPC_CALL)
SEND_NET_MESSAGE_STRUCT_VAR(stDataPacket)
IMPLEMENT_NET_MESSAGE_SEND_END(CGRpcCallMsg, MESSAGE_CG_RPC_CALL)
