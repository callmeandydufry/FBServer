#ifndef __GC_ENCRYPT_KEY_H__
#define __GC_ENCRYPT_KEY_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "MessageDefine.h"
#include "FixedString.h"

DECLARE_NET_MESSAGE_BEGIN(GCEncryptKey, MESSAGE_GC_ENCRYPT_KEY)

DECLARE_NET_MESSAGE_STRUCT_VAR(FixedString<128>, KeyData)

DECLARE_NET_MESSAGE_END(GCEncryptKey, MESSAGE_GC_ENCRYPT_KEY)

#endif
