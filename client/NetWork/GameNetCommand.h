#pragma once

#include "GameNetBase.h"
#include "FixedString.h"

enum EGameNetCommand
{
	EGameNetCommand_Unknown,
	EGameNetCommand_StartConnect,
	EGameNetCommand_ConnectFinish,
	EGameNetCommand_CloseConnect,
	EGameNetCommand_ConnectClosed,
	EGameNetCommand_SendMessage,
	EGameNetCommand_MessageReceived,
};

// ��Ϸ�߳��������̼߳䴫�ݿ��ƵĿ������� [12/28/2017 yz]
class IGameNetCommand
{
public:
	IGameNetCommand() {}
	virtual ~IGameNetCommand() {}

	virtual EGameNetCommand getCommandID() const = 0;
};

class GameNetCommandStartConnect : public IGameNetCommand
{
public:
	GameNetCommandStartConnect()
	{
		mPort = 0;
	}

	virtual EGameNetCommand getCommandID() const
	{
		return EGameNetCommand_StartConnect;
	}

	FixedString<NET_IP_SIZE> mIP;
	uint16 mPort;
};

class GameNetCommandConnectFinish : public IGameNetCommand
{
public:
	virtual EGameNetCommand getCommandID() const
	{
		return EGameNetCommand_ConnectFinish;
	}
};

class GameNetCommandCloseConnect : public IGameNetCommand
{
public:
	virtual EGameNetCommand getCommandID() const
	{
		return EGameNetCommand_CloseConnect;
	}
};

class GameNetCommandConnectClosed : public IGameNetCommand
{
public:
	virtual EGameNetCommand getCommandID() const
	{
		return EGameNetCommand_ConnectClosed;
	}
};

class GameNetCommandSendMessage : public IGameNetCommand
{
public:
	virtual ~GameNetCommandSendMessage()
	{
		if (mMessage)
		{
			delete mMessage;
			mMessage = NULL;
		}
	}

	virtual EGameNetCommand getCommandID() const
	{
		return EGameNetCommand_SendMessage;
	}

	void setMessage(IGameMessage* msg)
	{
		if (mMessage)
		{
			delete mMessage;
			mMessage = NULL;
		}
		mMessage = msg;
	}

	IGameMessage* peekMessage()
	{
		IGameMessage* msg = mMessage;
		mMessage = NULL;
		return msg;
	}

protected:
	IGameMessage* mMessage;
};

class GameNetCommandMessageReceived : public IGameNetCommand
{
public:
	virtual ~GameNetCommandMessageReceived()
	{
		if (mMessage)
		{
			delete mMessage;
			mMessage = NULL;
		}
	}

	virtual EGameNetCommand getCommandID() const
	{
		return EGameNetCommand_MessageReceived;
	}

	void setMessage(IGameMessage* msg)
	{
		if (mMessage)
		{
			delete mMessage;
			mMessage = NULL;
		}
		mMessage = msg;
	}

	IGameMessage* peekMessage()
	{
		IGameMessage* msg = mMessage;
		mMessage = NULL;
		return msg;
	}

protected:
	IGameMessage* mMessage;
};