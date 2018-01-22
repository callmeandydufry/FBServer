#ifndef __GC_RPC_CALLMSG_H__
#define __GC_RPC_CALLMSG_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "MessageDefine.h"
#include "ModulePacket.h"

DECLARE_NET_MESSAGE_BEGIN(GCRpcCallMsg, MESSAGE_GC_RPC_CALL)
DECLARE_NET_MESSAGE_STRUCT_VAR(DataPacket, stDataPacket)
DECLARE_NET_MESSAGE_END(GCRpcCallMsg, MESSAGE_GC_RPC_CALL)

#endif  // __GC_RPC_CALLMSG_H__