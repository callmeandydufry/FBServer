#include "GameNetMessageHandler.h"
#include "GameNetSystem.h"
#include "ShareStruct.h"

GameClientRPCInvoker::GameClientRPCInvoker(GameNetSystem* netSystem)
	: ClientRPCMethodHandler(netSystem)
{

}

GameClientRPCInvoker::~GameClientRPCInvoker()
{

}

void GameClientRPCInvoker::showLog(const char* log)
{

}

void GameClientRPCInvoker::loginSucess(SNID_t snid)
{
	
}

void GameClientRPCInvoker::SynPlayerExtData(SPlayerExtData& stPlayerState)
{
	
}

void GameClientRPCInvoker::SynPlayerChatData(tagSendChatMsgData& stChat)
{
	// todo handle chat msg
}
