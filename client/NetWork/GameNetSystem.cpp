
#include "GameNetSystem.h"
#include "GameNetConnector.h"
#include "GameNetCommand.h"
#include "GameNetMessageFactory.h"
#include "GameNetMessageHandler.h"
#include "GameNetMessages.h"

#include "GameClient.h"
#include "ClientPlayer.h"

GameNetSystem::GameNetSystem()
	: mConnector(NULL)
{

}

GameNetSystem::~GameNetSystem()
{
	uninitialize();
}

void GameNetSystem::initialize()
{
	mClientRPCInvoker = new GameClientRPCInvoker(this);
	// 使用此对象处理来自服务端的远程调用 [12/25/2017 yz]
	mServerPRCInterface = new ServerRPCMethodProxy(this);

	GameNetMessageFactoryManager::instance().Init();

	mConnector = new GameNetConnector(this);
	mConnector->startWork();
}

void GameNetSystem::uninitialize()
{
	if (NULL != mConnector)
	{
		mConnector->stopWork();

		delete mConnector;
		mConnector = NULL;
	}

	IGameMessage* message = NULL;
	while (mReceiveMessages.Dequeue(message))
	{
		if (NULL == message)
			continue;
		GameNetMessageFactoryManager::instance().RemoveMessage(message);
		message = NULL;
	}

	IGameNetCommand* cmd = NULL;
	while (mNetCommands.dequeue(cmd))
	{
		if (NULL == cmd)
			continue;
		delete cmd;
		cmd = NULL;
	}
	
	if (mClientRPCInvoker)
	{
		delete mClientRPCInvoker;
		mClientRPCInvoker = NULL;
	}
	if (mServerPRCInterface)
	{
		delete mServerPRCInterface;
		mServerPRCInterface = NULL;
	}
}

void GameNetSystem::tick()
{
	processNetCommand();
	processNetMessage();
}

void GameNetSystem::sendPacketStream(DataStream& stream)
{
	mSendRpcCache.serialize(stream.getData(), stream.getDataSize());
}

BOOL GameNetSystem::recvPacketStream(DataStream& stream)
{
	mRecvRpcCache.swapData(stream);
	mRecvRpcCache.clear();
	return stream.getDataSize() > 0;
}

ServerRPCMethodInterface* GameNetSystem::getServerRPCProxy()
{
	return mServerPRCInterface;
}

void GameNetSystem::startConnect(const char* ip, uint16 port)
{
	GameNetCommandStartConnect* cmd = new GameNetCommandStartConnect();
	cmd->mIP = ip;
	cmd->mPort = port;
	mConnector->pushNetCommand(cmd);
}

void GameNetSystem::closeConnect()
{
	GameNetCommandCloseConnect* cmd = new GameNetCommandCloseConnect();
	mConnector->pushNetCommand(cmd);
}

void GameNetSystem::sendMessage(IGameMessage* message)
{
	GameNetCommandSendMessage* cmd = new GameNetCommandSendMessage();
	cmd->setMessage(message);
	mConnector->pushNetCommand(cmd);
}

void GameNetSystem::pushNetCommand(IGameNetCommand* netCommand)
{
	mNetCommands.enqueue(netCommand);
}

void GameNetSystem::setLoginData(const char* account, const char* token)
{
	mLoginAccount = account;
	mLoginToken = token;
}

void GameNetSystem::processNetCommand()
{
	IGameNetCommand* netCommand = NULL;
	while (mNetCommands.dequeue(netCommand))
	{
		if (NULL == netCommand)
			break;

		switch (netCommand->getCommandID())
		{
		case EGameNetCommand_ConnectFinish:
		{
			onConnectFinish();
		}
		break;
		case EGameNetCommand_ConnectClosed:
		{
			onConnectClose();
		}
		break;
		case EGameNetCommand_MessageReceived:
		{
			GameNetCommandMessageReceived* msgRecvCmd = (GameNetCommandMessageReceived*)netCommand;
			mReceiveMessages.Enqueue(msgRecvCmd->peekMessage());
		}
		break;
		}

		delete netCommand;
		netCommand = NULL;
	}
}

void GameNetSystem::processNetMessage()
{
	IGameMessage* message = NULL;
	while (mReceiveMessages.Dequeue(message))
	{
		if (NULL == message)
			break;

		if (message->getMsgID() == MESSAGE_GC_RPC_CALL)
		{
			GCRpcInvokeMsg* rpcMsg = (GCRpcInvokeMsg*)message;
			mRecvRpcCache.serialize(rpcMsg->mData.getData(), rpcMsg->mData.getDataSize());
		}
		else
		{
			message->process(this);
		}

		GameNetMessageFactoryManager::instance().RemoveMessage(message);
		message = NULL;
	}

	// 执行服务端发过来的远程调用 [12/28/2017 yz]
	mClientRPCInvoker->processPacket();
	// 将客服端发起的远程调用发送到服务端 [12/28/2017 yz]
	if (mSendRpcCache.getDataSize() > 0)
	{
		CGRpcInvokeMsg* rpcMsg = (CGRpcInvokeMsg*)GameNetMessageFactoryManager::instance().CreateMessage(MESSAGE_CG_RPC_CALL);
		mSendRpcCache.swapData(rpcMsg->mData);
		mSendRpcCache.clear();
		sendMessage(rpcMsg);
	}
}

void GameNetSystem::onConnectFinish()
{
	CGRequestLogin* loginMsg = (CGRequestLogin*)GameNetMessageFactoryManager::instance().CreateMessage(MESSAGE_CG_REQUEST_LOGIN);
	loginMsg->mAccount = mLoginAccount;
	loginMsg->mToken = mLoginToken;
	sendMessage(loginMsg);

	gClient->getClientPlayer()->onConnectFinish();
}

void GameNetSystem::onConnectClose()
{
	gClient->getClientPlayer()->onConnectClose();
}