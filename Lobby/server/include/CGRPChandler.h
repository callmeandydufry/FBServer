#ifndef __CG_RPC_HANDLER_H__
#define __CG_RPC_HANDLER_H__

#include "ServerRPCMethodInterface.h"
#include "ClientSession.h"

class CGRPCHandler : public ServerRPCMethodHandler
{
public:
	CGRPCHandler(ClientSession* dataReceiver)
		: mClientSession(dataReceiver),
		ServerRPCMethodHandler(dataReceiver)
	{}

	virtual void sendChatMsg2OtherPlayer(const char* chatContent, SNID_t u64ReceiverID);
	virtual void requestPlayerData(SNID_t snid);

private:
	ClientSession*			mClientSession;
};

#endif