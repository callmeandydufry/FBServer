#pragma once

#include "CoreBase.h"
#include "GameNetBase.h"
#include "GameNetCommand.h"
#include "TypeDefine.h"
#include "GameNetSystem.h"
#include "FixedString.h"
#include "BaseTool.h"


class GCHandShake;

class GameNetConnector 
	:public KThread
{
public:
	GameNetConnector(GameNetSystem* netSystem);
	~GameNetConnector();

	void startWork();
	void stopWork();

	void startConnect(const FixedString<IP_SIZE>& ip, uint32 port);
	void closeConnect();
	bool isConnected();

	//----------------------------------------------------------
	// 来自FRunnable的接口 [12/26/2017 yz]
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
	virtual class FSingleThreadRunnable* GetSingleThreadInterface();
	//----------------------------------------------------------

	//----------------------------------------------------------
	// 来自FSingleThreadRunnable的接口 [12/26/2017 yz]
	virtual void Tick();
	//----------------------------------------------------------

	void setPingInterval(float interval) { mPingSendInterval = interval; }
	void setPingWaitTime(float waitTime) { mPingSendWaitTime = waitTime; }

	void pushNetCommand(IGameNetCommand* command);

protected:
	void initNetStream();
	void uninitNetStream();
	void clearNetStream();

	void processCommand();
	bool processReceiveStream();
	bool processSendStream();
	void processReceiveMessage();
	bool processSpecialMessage(IGameMessage *message);
	void sendMessageToStream(IGameMessage *message);

	void onSocketConnected();
	void onSocketDisconnected();
	void onSocketHandShake(GCHandShake *message);

	// 设置数据加密协议 [12/28/2017 yz]
	void setEncryptKey(const char* pKey);

	void processPing(bool isForceSend = false);
	void onRecvPing(int16 pingIndex);

	void setEncryptArith(uint32 encryptArith) { mEncryptArith = encryptArith; }
	uint32 getEncryptArith(void) { return mEncryptArith; }
	void setRsaIndex(uint32 index) { mRsaIndex = index; }
	uint32 getRsaIndex(void) { return mRsaIndex; }

	void pushReceiveMessage(IGameMessage* message);
protected:
	GameNetSystem* mNetSystem;

	FSocket* mSocket;
	bool mIsActive;

	// 与服务端的连接是否握手成功 [12/28/2017 yz]
	bool mIsHandShaked;

	// NetConnector正式运行于新创建的线程中，用来保存NetConnector的执行线程 [12/26/2017 yz]
	KThread* mNetThread;

	FixedString<IP_SIZE> mSockIP;
	uint32 mSockPort;

	TNetAsyncQueue<IGameNetCommand*> mNetCommands;
	TQueue<IGameMessage*> mSendMsgs;

	NetRecieveStream* mRecieveStream;
	NetSendStream* mSendStream;

	uint8 mPacketIndex;

	// 发送的时间记录
	double mLastPingProcessTime;
	double mLastPingSendTime;
	double mLastPingWaitTime;
	int32 mPingTickInterval;
	float mPingSendInterval;
	float mPingSendWaitTime;

	FixedString<IP_SIZE> mLoginIP;
	uint32 mLoginPort;
	/* 校验索引 */
	uint32 mEncryptArith;
	uint32 mRsaIndex;
};