#ifndef __GC_PING_H__
#define __GC_PING_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"

DECLARE_NET_MESSAGE_BEGIN(GCPing, MESSAGE_GC_PING)

DECLARE_NET_MESSAGE_ATOM_VAR(int16, Index)

DECLARE_NET_MESSAGE_END(GCPing, MESSAGE_GC_PING)

#endif