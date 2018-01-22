#include "GameNetConnector.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "GameNetStream.h"
#include "GameNetMessageDefine.h"
#include "GameNetMessageFactory.h"
#include "GameNetMessages.h"
#include "GameNetSystem.h"

GameNetConnector::GameNetConnector(GameNetSystem* netSystem)
	: mNetSystem(netSystem)
	, mSocket(NULL)
	, mIsActive(false)
	, mIsHandShaked(false)
	, mSockPort(0)
	, mNetThread(NULL)
	, mRecieveStream(NULL)
	, mSendStream(NULL)
	, mPacketIndex(0)
	, mEncryptArith(0)
	, mRsaIndex(0)
	, mLoginPort(0)
	, mLastPingProcessTime(0.0)
	, mLastPingSendTime(0.0)
	, mLastPingWaitTime(-1.0)
	, mPingTickInterval(0)
	, mPingSendInterval(5.f)
	, mPingSendWaitTime(10.f)
{
	
}

GameNetConnector::~GameNetConnector()
{
	stopWork();
}

void GameNetConnector::startWork()
{
	if (mIsActive)
		return;
	mIsActive = true;

	initNetStream();

	mLastPingProcessTime = FPlatformTime::Seconds();
	mLastPingSendTime = 0.0;
	mLastPingWaitTime = -1.0;
	mPingTickInterval = 0;

	mNetThread = FRunnableThread::Create(this, TEXT("GameNetConnector"));
}

void GameNetConnector::stopWork()
{
	if (!mIsActive)
		return;

	Stop();
	if (mNetThread)
	{
		// 这里等待NetConnector线程结束之后才能继续，要不然后续delete的时候线程没结束的话会出现内存异常 [12/26/2017 yz]
		mNetThread->WaitForCompletion();
		delete mNetThread;
		mNetThread = NULL;
	}
	uninitNetStream();

	IGameMessage* message = NULL;
	while (mSendMsgs.Dequeue(message))
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
}

void GameNetConnector::startConnect(const FixedString<IP_SIZE>& ip, uint32 port)
{
	if (!mIsActive)
		return;

	closeConnect();

	mSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	if (mSocket)
	{
		FIPv4Address ipAddr;
		FIPv4Address::Parse(ip, ipAddr);
		TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ipAddr.Value, port);
		if (mSocket->Connect(addr.Get()))
		{
			mSendStream->BindSocket(mSocket);
			mRecieveStream->BindSocket(mSocket);

			onSocketConnected();
		}
		else
		{
			closeConnect();
		}
	}
}

void GameNetConnector::closeConnect()
{
	if (NULL != mSocket)
	{
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(mSocket);
		mSocket = NULL;

		mSendStream->UnbindSocket();
		mRecieveStream->UnbindSocket();

		onSocketDisconnected();
	}

	mIsHandShaked = false;
}

bool GameNetConnector::isConnected()
{
	return mSocket != NULL;
}

bool GameNetConnector::Init()
{
	return true;
}

uint32 GameNetConnector::Run()
{
	while (mIsActive)
	{
		Tick();
	}
	return 0;
}

void GameNetConnector::Stop()
{
	mIsActive = false;
}

void GameNetConnector::Exit()
{
	// 退出之前确保Socket资源被释放掉 [12/26/2017 yz]
	closeConnect();
}

FSingleThreadRunnable* GameNetConnector::GetSingleThreadInterface()
{
	return this;
}

void GameNetConnector::Tick()
{
	processCommand();

	if (NULL != mSocket)
	{
		if (mSocket->GetConnectionState() != SCS_Connected)
		{
			closeConnect();
		}
		else
		{
			// 处理接收队列，然后处理发送队列 [12/27/2017 yz]
			if (processReceiveStream() && processSendStream())
			{
				// 从接收队列中提取消息对象 [12/27/2017 yz]
				processReceiveMessage();

				if (mIsHandShaked)
				{
					processPing();
				}
			}
		}
	}
}

void GameNetConnector::pushNetCommand(IGameNetCommand* command)
{
	mNetCommands.enqueue(command);
}

void GameNetConnector::initNetStream()
{
	mRecieveStream = new NetRecieveStream(16 * 1024, 96 * 1024);
	mSendStream = new NetSendStream(100 * 1024, 1000 * 1024);

	// 消息的buff大小动态扩展比较好，不能确定最多缓存的数据量是多少 [4/28/2017 yz]
	mSendStream->SetStaticStream(FALSE);
	// 说明这个buff是从客户端发往服务端的数据，发送的时候会有每帧发送数据量的限制 [12/27/2017 yz]
	mSendStream->SetServerData(FALSE);

	mRecieveStream->EnableEncrpt(TRUE);
	mSendStream->EnableEncrpt(TRUE);

	mSendStream->SetMagicNum(MSGHEAD_MAGIC_NUM, MSG_MAGIC_NUM);
	mRecieveStream->SetMagicNum(MSGHEAD_MAGIC_NUM, MSG_MAGIC_NUM);
}

void GameNetConnector::uninitNetStream()
{
	if (mRecieveStream)
	{
		delete mRecieveStream;
		mRecieveStream = NULL;
	}

	if (mSendStream)
	{
		delete mSendStream;
		mSendStream = NULL;
	}
}

void GameNetConnector::clearNetStream()
{
	if (mRecieveStream)
	{
		mRecieveStream->Clear();
	}

	if (mSendStream)
	{
		mSendStream->Clear();
	}
}

void GameNetConnector::processCommand()
{
	IGameNetCommand* command = NULL;
	while (mNetCommands.dequeue(command))
	{
		if (NULL == command)
			break;

		switch (command->getCommandID())
		{
		case EGameNetCommand_StartConnect:
		{
			GameNetCommandStartConnect* startConnectCmd = (GameNetCommandStartConnect*)command;
			mLoginIP = startConnectCmd->mIP;
			mLoginPort = startConnectCmd->mPort;
			startConnect(startConnectCmd->mIP.c_str(), startConnectCmd->mPort);
		}
		break;
		case EGameNetCommand_CloseConnect:
		{
			GameNetCommandCloseConnect* closeConnectCmd = (GameNetCommandCloseConnect*)command;
			closeConnect();
		}
		break;
		case EGameNetCommand_SendMessage:
		{
			GameNetCommandSendMessage* sendMessageCmd = (GameNetCommandSendMessage*)command;
			IGameMessage* message = sendMessageCmd->peekMessage();
			mSendMsgs.Enqueue(message);
		}
		break;
		}

		delete command;
		command = NULL;
	}
}

bool GameNetConnector::processReceiveStream()
{
	uint32 pendingSize;
	if (mSocket && mSocket->HasPendingData(pendingSize))
	{
		int32 ret = mRecieveStream->ReadFull();
		if (ret <= NET_SOCKET_ERROR)
		{
			closeConnect();
			return false;
		}
	}
	return true;
}

bool GameNetConnector::processSendStream()
{
	IGameMessage* message = NULL;
	while (mSendMsgs.Dequeue(message))
	{
		if (NULL == message)
			break;
		sendMessageToStream(message);
		message = NULL;
	}

	int32 ret = 0;
	{
		ret = (int32)mSendStream->Flush();
	}
	if (ret <= NET_SOCKET_ERROR)
	{
		closeConnect();
		return false;
	}

	return true;
}

void GameNetConnector::processReceiveMessage()
{
	// 固定的头尺寸
	char header[MSG_HEADER_SIZE] = { 0 };

	// 消息包数据
	MSG_ID packetID = -1;
	uint32 packetuint, packetSize, packetIndex;
	//IGameMessage *pPacket = NULL;
	while (TRUE)
	{
		/* 尝试读取消息头 */
		if (FALSE == mRecieveStream->Peek(&header[0], MSG_HEADER_SIZE))
			break;

		memcpy(&packetID, &header[0], sizeof(MSG_ID));
		memcpy(&packetuint, &header[sizeof(MSG_ID)], sizeof(uint32));
		mRecieveStream->DeEncryptMsgHead(&packetID);
		mRecieveStream->DeEncryptMsgHead(&packetuint);
		packetSize = GET_MSG_LEN(packetuint);
		packetIndex = GET_MSG_INDEX(packetuint);

		// 无效的消息
		if (packetID >= (MSG_ID)MESSAGE_MAX)
		{
			// 断开连接
			closeConnect();

			// 此处应该生成一条调试日志，记录出错的消息ID [12/27/2017 yz]

			return;
		}

		// 输入数据流长度
		uint32 uSize = mRecieveStream->Length();
		static uint32 s_uIncompleteSize = 0;

		// 消息包尺寸正确
		if (packetSize <= GameNetMessageFactoryManager::instance().GetMessageMaxSize(packetID))
		{
			// 消息没有接收全
			if (uSize < MSG_HEADER_SIZE + packetSize)
			{
				// 如果消息数据没有收集到整个消息包的大小则中断处理，等下一帧凑齐后再分析消息包 [12/28/2017 yz]
				s_uIncompleteSize = uSize;
				break;
			}
			s_uIncompleteSize = 0;

			mRecieveStream->DeEncryptMessage(packetID + packetIndex, packetSize);
		}
		// 消息包尺寸错误
		else
		{
			// 断开连接
			closeConnect();

			return;
		}

		/* 当此标记大于给定的值就不再处理 */
		IGameMessage *pPacket = GameNetMessageFactoryManager::instance().CreateMessage(packetID);

		if (NULL == pPacket)
		{
			// 断开连接
			closeConnect();

			return;
		}

		// 设置消息序列号
		pPacket->setMsgIndex(packetIndex);

		uint32	nSizeBefore = mRecieveStream->Length();
		uint32	uHeadBefore = mRecieveStream->GetHead();
		uint32	uTailBefore = mRecieveStream->GetTail();
		bool ret = mRecieveStream->ReciveMessage(pPacket);
		uint32	nSizeAfter = mRecieveStream->Length();

		if (FALSE == ret)
		{
			// 断开连接
			closeConnect();
			return;
		}

		if ((nSizeBefore - nSizeAfter - MSG_HEADER_SIZE) != pPacket->getMsgSize())
		{
			// 断开连接
			closeConnect();
			return;
		}

		// 只要客户端收到消息包就说明连接是正常的，不需要发Ping包 [yangzhen]
		// 将下次发ping的时间推迟 [yangzhen]
		mLastPingSendTime = FPlatformTime::Seconds();

		// 立即执行的消息
		if (processSpecialMessage(pPacket))
		{
			// 立即执行成功，删除
			GameNetMessageFactoryManager::instance().RemoveMessage(pPacket);
		}
		else
		{
			// 添加消息到缓冲
			pushReceiveMessage(pPacket);
		}
	}
	mRecieveStream->Clear();
}

bool GameNetConnector::processSpecialMessage(IGameMessage *message)
{
	if (NULL == message)
		return FALSE;

	switch (message->getMsgID())
	{
	// Login返回的加密key。消息中还有其它的逻辑，只返回FALSE.
	case MESSAGE_GC_HANDSHAKE:
	{
		GCHandShake* pMSg = (GCHandShake*)message;
		onSocketHandShake(pMSg);
	}
	return FALSE;

	case MESSAGE_GC_ENCRYPT_KEY:
	{
		GCEncryptKey* pMsg = (GCEncryptKey*)message;
		setEncryptKey(pMsg->mKeyData.c_str());
	}
	return TRUE;

	case MESSAGE_GC_PING:
	{
		GCPing* pMsg = (GCPing*)message;
		onRecvPing(pMsg->mIndex);
	}
	return TRUE;

	default:
		break;
	}
	return FALSE;
}

void GameNetConnector::sendMessageToStream(IGameMessage *message)
{
	if (FALSE == isConnected())
		return;

	if (NULL == message)
		return;

	message->setMsgIndex(mPacketIndex++);

	// 压入消息流
	uint32 nSizeBefore = 0;
	uint32 nSizeAfter = 0;
	{
		nSizeBefore = mSendStream->Length();
		mSendStream->SendMessage(message, true);
		nSizeAfter = mSendStream->Length();
	}

	// 消息尺寸匹配检查
	if (message->getMsgSize() != nSizeAfter - nSizeBefore - MSG_HEADER_SIZE)
	{
		// 消息大小计算错误，会导致消息接收失败 [12/28/2017 yz]
		closeConnect();
	}
	else
	{
	}

	GameNetMessageFactoryManager::instance().RemoveMessage(message);
}

void GameNetConnector::onSocketConnected()
{
	CGHandShake* handShakeMsg = (CGHandShake*)GameNetMessageFactoryManager::instance().CreateMessage(MESSAGE_CG_HANDSHAKE);
	sendMessageToStream(handShakeMsg);
}

void GameNetConnector::onSocketDisconnected()
{
	GameNetCommandConnectClosed* netCmd = new GameNetCommandConnectClosed();
	mNetSystem->pushNetCommand(netCmd);
}

void GameNetConnector::onSocketHandShake(GCHandShake *message)
{
	if (message->mResult == EHAND_SHAKE_SUCCESS)
	{
		setEncryptArith(message->mEncryptArith);
		setRsaIndex(message->mRsaIndex);
		mLoginIP = message->mLoginIP;
		mLoginPort = message->mLoginPort;

		mIsHandShaked = true;

		GameNetCommandConnectFinish* netCmd = new GameNetCommandConnectFinish();
		mNetSystem->pushNetCommand(netCmd);
	}
	else
	{
		// 握手错误，关闭连接 [12/28/2017 yz]
		closeConnect();
	}
}

void GameNetConnector::setEncryptKey(const char* pKey)
{
	mRecieveStream->GetEncrptor()->SetPdata((const code_char*)pKey, STREAM_MAGIC_NUM);
	mSendStream->GetEncrptor()->SetPdata((const code_char*)pKey, STREAM_MAGIC_NUM);
}

void GameNetConnector::processPing(bool isForceSend)
{
	double currentTime = FPlatformTime::Seconds();
	double deltaTime = currentTime - mLastPingProcessTime;
	mLastPingProcessTime = currentTime;
	if (deltaTime > 0.1)
	{
		deltaTime = 0.1;
	}

	if (!isConnected())
	{
		// 只有当连接建立了后才开始计时 [yangzhen]
		mLastPingSendTime = currentTime;
		mLastPingWaitTime = -1.0;
		mPingTickInterval = 0;
		return;
	}

	if (mPingSendInterval < 1.f)
	{
		// ping消息的间隔不能太小 [yangzhen]
		mPingSendInterval = 1.f;
	}

	// 这里用帧数做判断是为了防止某些机器上特别卡，比如加载场景，一帧就消耗了几十秒，然后导致ping的回复包没有被处理而被误认为是断线的问题 [yangzhen]
	mPingTickInterval++;

	// 对上次发出ping消息后的等待时间进行计时 [yangzhen]
	if (mLastPingWaitTime >= 0.0)
	{
		mLastPingWaitTime += deltaTime;
		if (mLastPingWaitTime > mPingSendWaitTime && mPingTickInterval > 10)
		{
			// 超过了ping消息的等待时间，判断为断线 [yangzhen]
			mLastPingWaitTime = -1.0;
			mPingTickInterval = 0;
			// 直接断开连接，然后在连接断开的处理函数中进行断线重连 [yangzhen]
			closeConnect();
			return;
		}
	}
	else
	{
		double netDelay = currentTime - mLastPingSendTime;
		// mLastPingWaitTime < 0.0 说明以前发出去的ping消息收到回复了 [yangzhen]
		if ((isForceSend || netDelay >= mPingSendInterval) && mPingTickInterval > 10)
		{
			/* 发送Ping消息 */
			CGPing* msgPing = (CGPing*)GameNetMessageFactoryManager::instance().CreateMessage(MESSAGE_CG_PING);
			msgPing->setMsgIndex(0);
			sendMessageToStream(msgPing);

			mLastPingSendTime = currentTime;
			// 开始等待XCPing消息的返回 [yangzhen]
			mLastPingWaitTime = 0.01;
			mPingTickInterval = 0;
		}
	}
}

void GameNetConnector::onRecvPing(int16 pingIndex)
{
	// 收到上次的ping回复，说明可以开始下次发送ping消息 [yangzhen]
	mLastPingWaitTime = -1.0;
	mPingTickInterval = 0;
}

void GameNetConnector::pushReceiveMessage(IGameMessage* message)
{
	GameNetCommandMessageReceived* netCmd = new GameNetCommandMessageReceived();
	netCmd->setMessage(message);
	mNetSystem->pushNetCommand(netCmd);
}