#pragma once

#include "CoreBase.h"
#include "Containers/Queue.h"
#include "ClientServerRPCSupport.h"
#include "ServerRPCMethodInterface.h"
#include "ClientRPCMethodInterface.h"
#include "FixedString.h"
#include "Define.h"

class GameNetConnector;
class GameNetMessageHandler;
class IGameNetCommand;

class GameNetSystem 
	: public RPCPacketDataSender
	, public RPCPacketDataReceiver
{
public:
	GameNetSystem();
	virtual ~GameNetSystem();

	void initialize();
	void uninitialize();
	void tick();

	virtual void sendPacketStream(DataStream& stream);
	virtual BOOL recvPacketStream(DataStream& stream);

	ServerRPCMethodInterface* getServerRPCProxy();

	void startConnect(const char* ip, uint16 port);
	void closeConnect();
	void sendMessage(IGameMessage* message);

	GameNetConnector* getConnector() const { return mConnector; }

	void pushNetCommand(IGameNetCommand* netCommand);

	void setLoginData(const char* account, const char* token);
protected:
	void processNetCommand();
	void processNetMessage();

	void onConnectFinish();
	void onConnectClose();

protected:
	GameNetConnector* mConnector;
	
	// ���մ�NetConnector�����Ŀ������� [12/28/2017 yz]
	TNetAsyncQueue<IGameNetCommand*> mNetCommands;
	// ���յ�����Ϣ���� [12/28/2017 yz]
	TQueue<IGameMessage*> mReceiveMessages;

	// ʹ�ô˽ӿڴ������Է�������Զ�̵��� [12/25/2017 yz]
	ClientRPCMethodHandler* mClientRPCInvoker;
	// ʹ�ô˶�����Է���˵�Զ�̵��� [12/25/2017 yz]
	ServerRPCMethodInterface* mServerPRCInterface;

	DataStream mSendRpcCache;
	DataStream mRecvRpcCache;

	FixedString<MAX_ACCOUNT> mLoginAccount;
	FixedString<MAX_TOKEN_LEN> mLoginToken;
};