#ifndef __CG_HAND_SHAKE_H__
#define __CG_HAND_SHAKE_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "MessageDefine.h"

DECLARE_NET_MESSAGE_BEGIN(CGHandShake, MESSAGE_CG_HANDSHAKE)
DECLARE_NET_MESSAGE_ATOM_VAR(int32, MagicNum)
DECLARE_NET_MESSAGE_ATOM_VAR(int8, QuickMark)		// 定义快速注册的标志
DECLARE_NET_MESSAGE_ATOM_VAR(uint32, CheckVer)
BOOL IsLegal();
DECLARE_NET_MESSAGE_END(CGHandShake, MESSAGE_CG_HANDSHAKE)

#endif
