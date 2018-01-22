#ifndef __CG_REQUEST_LOGIN_H__
#define __CG_REQUEST_LOGIN_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "MessageDefine.h"

DECLARE_NET_MESSAGE_BEGIN(CGRequestLogin, MESSAGE_CG_REQUEST_LOGIN)
DECLARE_NET_MESSAGE_STRUCT_VAR(FixedString<MAX_ACCOUNT>, Account)
DECLARE_NET_MESSAGE_STRUCT_VAR(FixedString<MAX_TOKEN_LEN>, Token)
DECLARE_NET_MESSAGE_END(CGRequestLogin, MESSAGE_CG_REQUEST_LOGIN)

#endif
