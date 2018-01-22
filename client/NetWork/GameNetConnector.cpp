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
		// ����ȴ�NetConnector�߳̽���֮����ܼ�����Ҫ��Ȼ����delete��ʱ���߳�û�����Ļ�������ڴ��쳣 [12/26/2017 yz]
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
	// �˳�֮ǰȷ��Socket��Դ���ͷŵ� [12/26/2017 yz]
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
			// ������ն��У�Ȼ�����Ͷ��� [12/27/2017 yz]
			if (processReceiveStream() && processSendStream())
			{
				// �ӽ��ն�������ȡ��Ϣ���� [12/27/2017 yz]
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

	// ��Ϣ��buff��С��̬��չ�ȽϺã�����ȷ����໺����������Ƕ��� [4/28/2017 yz]
	mSendStream->SetStaticStream(FALSE);
	// ˵�����buff�Ǵӿͻ��˷�������˵����ݣ����͵�ʱ�����ÿ֡���������������� [12/27/2017 yz]
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
	// �̶���ͷ�ߴ�
	char header[MSG_HEADER_SIZE] = { 0 };

	// ��Ϣ������
	MSG_ID packetID = -1;
	uint32 packetuint, packetSize, packetIndex;
	//IGameMessage *pPacket = NULL;
	while (TRUE)
	{
		/* ���Զ�ȡ��Ϣͷ */
		if (FALSE == mRecieveStream->Peek(&header[0], MSG_HEADER_SIZE))
			break;

		memcpy(&packetID, &header[0], sizeof(MSG_ID));
		memcpy(&packetuint, &header[sizeof(MSG_ID)], sizeof(uint32));
		mRecieveStream->DeEncryptMsgHead(&packetID);
		mRecieveStream->DeEncryptMsgHead(&packetuint);
		packetSize = GET_MSG_LEN(packetuint);
		packetIndex = GET_MSG_INDEX(packetuint);

		// ��Ч����Ϣ
		if (packetID >= (MSG_ID)MESSAGE_MAX)
		{
			// �Ͽ�����
			closeConnect();

			// �˴�Ӧ������һ��������־����¼�������ϢID [12/27/2017 yz]

			return;
		}

		// ��������������
		uint32 uSize = mRecieveStream->Length();
		static uint32 s_uIncompleteSize = 0;

		// ��Ϣ���ߴ���ȷ
		if (packetSize <= GameNetMessageFactoryManager::instance().GetMessageMaxSize(packetID))
		{
			// ��Ϣû�н���ȫ
			if (uSize < MSG_HEADER_SIZE + packetSize)
			{
				// �����Ϣ����û���ռ���������Ϣ���Ĵ�С���жϴ�������һ֡������ٷ�����Ϣ�� [12/28/2017 yz]
				s_uIncompleteSize = uSize;
				break;
			}
			s_uIncompleteSize = 0;

			mRecieveStream->DeEncryptMessage(packetID + packetIndex, packetSize);
		}
		// ��Ϣ���ߴ����
		else
		{
			// �Ͽ�����
			closeConnect();

			return;
		}

		/* ���˱�Ǵ��ڸ�����ֵ�Ͳ��ٴ��� */
		IGameMessage *pPacket = GameNetMessageFactoryManager::instance().CreateMessage(packetID);

		if (NULL == pPacket)
		{
			// �Ͽ�����
			closeConnect();

			return;
		}

		// ������Ϣ���к�
		pPacket->setMsgIndex(packetIndex);

		uint32	nSizeBefore = mRecieveStream->Length();
		uint32	uHeadBefore = mRecieveStream->GetHead();
		uint32	uTailBefore = mRecieveStream->GetTail();
		bool ret = mRecieveStream->ReciveMessage(pPacket);
		uint32	nSizeAfter = mRecieveStream->Length();

		if (FALSE == ret)
		{
			// �Ͽ�����
			closeConnect();
			return;
		}

		if ((nSizeBefore - nSizeAfter - MSG_HEADER_SIZE) != pPacket->getMsgSize())
		{
			// �Ͽ�����
			closeConnect();
			return;
		}

		// ֻҪ�ͻ����յ���Ϣ����˵�������������ģ�����Ҫ��Ping�� [yangzhen]
		// ���´η�ping��ʱ���Ƴ� [yangzhen]
		mLastPingSendTime = FPlatformTime::Seconds();

		// ����ִ�е���Ϣ
		if (processSpecialMessage(pPacket))
		{
			// ����ִ�гɹ���ɾ��
			GameNetMessageFactoryManager::instance().RemoveMessage(pPacket);
		}
		else
		{
			// �����Ϣ������
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
	// Login���صļ���key����Ϣ�л����������߼���ֻ����FALSE.
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

	// ѹ����Ϣ��
	uint32 nSizeBefore = 0;
	uint32 nSizeAfter = 0;
	{
		nSizeBefore = mSendStream->Length();
		mSendStream->SendMessage(message, true);
		nSizeAfter = mSendStream->Length();
	}

	// ��Ϣ�ߴ�ƥ����
	if (message->getMsgSize() != nSizeAfter - nSizeBefore - MSG_HEADER_SIZE)
	{
		// ��Ϣ��С������󣬻ᵼ����Ϣ����ʧ�� [12/28/2017 yz]
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
		// ���ִ��󣬹ر����� [12/28/2017 yz]
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
		// ֻ�е����ӽ����˺�ſ�ʼ��ʱ [yangzhen]
		mLastPingSendTime = currentTime;
		mLastPingWaitTime = -1.0;
		mPingTickInterval = 0;
		return;
	}

	if (mPingSendInterval < 1.f)
	{
		// ping��Ϣ�ļ������̫С [yangzhen]
		mPingSendInterval = 1.f;
	}

	// ������֡�����ж���Ϊ�˷�ֹĳЩ�������ر𿨣�������س�����һ֡�������˼�ʮ�룬Ȼ����ping�Ļظ���û�б������������Ϊ�Ƕ��ߵ����� [yangzhen]
	mPingTickInterval++;

	// ���ϴη���ping��Ϣ��ĵȴ�ʱ����м�ʱ [yangzhen]
	if (mLastPingWaitTime >= 0.0)
	{
		mLastPingWaitTime += deltaTime;
		if (mLastPingWaitTime > mPingSendWaitTime && mPingTickInterval > 10)
		{
			// ������ping��Ϣ�ĵȴ�ʱ�䣬�ж�Ϊ���� [yangzhen]
			mLastPingWaitTime = -1.0;
			mPingTickInterval = 0;
			// ֱ�ӶϿ����ӣ�Ȼ�������ӶϿ��Ĵ������н��ж������� [yangzhen]
			closeConnect();
			return;
		}
	}
	else
	{
		double netDelay = currentTime - mLastPingSendTime;
		// mLastPingWaitTime < 0.0 ˵����ǰ����ȥ��ping��Ϣ�յ��ظ��� [yangzhen]
		if ((isForceSend || netDelay >= mPingSendInterval) && mPingTickInterval > 10)
		{
			/* ����Ping��Ϣ */
			CGPing* msgPing = (CGPing*)GameNetMessageFactoryManager::instance().CreateMessage(MESSAGE_CG_PING);
			msgPing->setMsgIndex(0);
			sendMessageToStream(msgPing);

			mLastPingSendTime = currentTime;
			// ��ʼ�ȴ�XCPing��Ϣ�ķ��� [yangzhen]
			mLastPingWaitTime = 0.01;
			mPingTickInterval = 0;
		}
	}
}

void GameNetConnector::onRecvPing(int16 pingIndex)
{
	// �յ��ϴε�ping�ظ���˵�����Կ�ʼ�´η���ping��Ϣ [yangzhen]
	mLastPingWaitTime = -1.0;
	mPingTickInterval = 0;
}

void GameNetConnector::pushReceiveMessage(IGameMessage* message)
{
	GameNetCommandMessageReceived* netCmd = new GameNetCommandMessageReceived();
	netCmd->setMessage(message);
	mNetSystem->pushNetCommand(netCmd);
}