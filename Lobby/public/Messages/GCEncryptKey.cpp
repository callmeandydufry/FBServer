#include "stdafx.h"
#include "GCEncryptKey.h"

GCEncryptKey::GCEncryptKey()
{
	
}

GCEncryptKey::~GCEncryptKey()
{}

uint32 GCEncryptKey::GetMsgSize() const
{
	return m_KeyData.getSize();
}

uint32 GCEncryptKeyFactory::GetMessageMaxSize() const
{
	return sizeof(GCEncryptKey) - sizeof(IMessage);
}

IMPLEMENT_NET_MESSAGE_RECV_BEGIN(GCEncryptKey, MESSAGE_GC_ENCRYPT_KEY)
RECV_NET_MESSAGE_STRUCT_VAR(KeyData)
IMPLEMENT_NET_MESSAGE_RECV_END(GCEncryptKey, MESSAGE_GC_ENCRYPT_KEY)


IMPLEMENT_NET_MESSAGE_SEND_BEGIN(GCEncryptKey, MESSAGE_GC_ENCRYPT_KEY)
SEND_NET_MESSAGE_STRUCT_VAR(KeyData)
IMPLEMENT_NET_MESSAGE_SEND_END(GCEncryptKey, MESSAGE_GC_ENCRYPT_KEY)
