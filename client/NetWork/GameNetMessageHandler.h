#pragma once
#include "CoreBase.h"
#include "GameNetBase.h"
#include "ClientRPCMethodInterface.h"

class GameClientRPCInvoker : public ClientRPCMethodHandler
{
public:
	GameClientRPCInvoker(GameNetSystem* netSystem);
	virtual ~GameClientRPCInvoker();

	virtual void showLog(const char* log) override;

	virtual void loginSucess(SNID_t snid) override;

	virtual void SynPlayerExtData(SPlayerExtData& stPlayerState) override;

	virtual void SynPlayerChatData(tagSendChatMsgData& stChat) override;
};