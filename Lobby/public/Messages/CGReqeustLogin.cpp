#include "stdafx.h"
#include "CGReqeustLogin.h"

CGRequestLogin::CGRequestLogin()
{
};

CGRequestLogin::~CGRequestLogin()
{}

uint32 CGRequestLogin::GetMsgSize() const
{
	return m_Account.getSize() + m_Token.getSize();
}

uint32 CGRequestLoginFactory::GetMessageMaxSize() const
{
	return sizeof(CGRequestLogin) - sizeof(IMessage);
}


IMPLEMENT_NET_MESSAGE_RECV_BEGIN(CGRequestLogin, MESSAGE_CG_REQUEST_LOGIN)
RECV_NET_MESSAGE_STRUCT_VAR(Account)
RECV_NET_MESSAGE_STRUCT_VAR(Token)
IMPLEMENT_NET_MESSAGE_RECV_END(CGRequestLogin, MESSAGE_CG_REQUEST_LOGIN)

IMPLEMENT_NET_MESSAGE_SEND_BEGIN(CGRequestLogin, MESSAGE_CG_REQUEST_LOGIN)
SEND_NET_MESSAGE_STRUCT_VAR(Account)
SEND_NET_MESSAGE_STRUCT_VAR(Token)
IMPLEMENT_NET_MESSAGE_SEND_END(CGRequestLogin, MESSAGE_CG_REQUEST_LOGIN)
