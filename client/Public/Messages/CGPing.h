#ifndef __CG_PING_H__
#define __CG_PING_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"

DECLARE_NET_MESSAGE_BEGIN(CGPing, MESSAGE_CG_PING)

DECLARE_NET_MESSAGE_ATOM_VAR(int16, Index)

DECLARE_NET_MESSAGE_END(CGPing, MESSAGE_CG_PING)

#endif