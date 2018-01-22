#include "stdafx.h"
#include "ClientConnector.h"
#include "MessageFactoryManager.h"
#include "ConnectorPool.h"
#include "BaseTool.h"
#include "Version.h"
#include "ClientSession.h"
#include "GCEncryptKey.h"

#define MAX_KHAN_INCOM_KICK_TIME				60000
#define MAX_GAMECONNECTOR_SEND_TIMES	3

ClientConnector::ClientConnector()
{
	Clear();
}

ClientConnector::~ClientConnector()
{
	//KSafeDelete(m_pPlayer);
}

void ClientConnector::InitSocket()
{
	m_pSocket = new Socket;
	KCheck(m_pSocket);

	m_pRecieveStream = new RecieveStream(m_pSocket, 16 * 1024, 96 * 1024);
	KCheck(m_pRecieveStream);

	m_pSendStream = new SendStream(m_pSocket, 100 * 1024, 1000 * 1024);
	KCheck(m_pSendStream);

	// 刚登陆后请求数据量比较大，还是动态分配好些 [4/28/2017 yz]
	m_pSendStream->SetStaticStream(FALSE);
	m_pSendStream->SetServerData(FALSE);

	m_pRecieveStream->EnableEncrpt(TRUE);
	m_pSendStream->EnableEncrpt(TRUE);

	m_pSendStream->SetMagicNum(MSGHEAD_MAGIC_NUM, MSG_MAGIC_NUM);
	m_pRecieveStream->SetMagicNum(MSGHEAD_MAGIC_NUM, MSG_MAGIC_NUM);
}

void ClientConnector::Clear()
{
	Connector::Clear();

	m_FirstIndexCheck = FALSE;
	m_ClientMessageIndex = 0;
	m_PerfMessageIndex = 0;
	m_ErrorCode = 0;
	m_PreMessageID = INVALID_KHAN_ID;
	m_PreWriteID = INVALID_KHAN_ID;
	m_OldOutPutStreamSize = 0;
	m_NowTime = 0;

	m_KickTime = 0;
	m_PlayerSNID = INVALID_KHAN_ID;

	m_LastSendTime = 0;
	m_CurrentTime = 0;
	mLeftTimeToQuit = 0;

	m_SaveTime = 0;
	m_bWaitingDisconnect = FALSE;

	m_bDebugMode = FALSE;

	m_uTotalSendSize = 0;
	m_uTotalReciSize = 0;

	m_DelayRemoveTimer.Clear();
	mGateManager = NULL;
	mSession = NULL;

	ResetKick();
}

void ClientConnector::ResetKick()
{
	__GUARD__

		m_KickTime = GET_TIME().TickCount();
	Connector::ResetKick();

	__UNGUARD__
}

BOOL ClientConnector::Handle_Input()
{
	__GUARD__

		if (!IsValid())
			return FALSE;

	if (IsDisconnect())
		return TRUE;

	_NET_TRY
	{
		uint32	ret = m_pRecieveStream->ReadFull();

	if ((int32)ret <= SOCKET_ERROR)
	{
		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"GameConnector::Handle_Input(): Error ! ret:%d SocketError:%s StreamLength=%d SNID=%llu ",
			(int32)ret,
			KLSocketError(),
			m_pRecieveStream->Length(),
			GetPlayerSNID()
		);
		return FALSE;
	}

	m_uTotalReciSize += ret;
	}
		_NET_CATCH
	{
		QLOGGER_CODEHOLDER();
	return FALSE;
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

//// 临时调试代码
class __DebugSendStream : public SendStream
{
public:
	__DebugSendStream(Socket* sock,
		uint32 BufferSize = INITWRITESIZE,
		uint32 MaxBufferSize = 100 * 1024) :SendStream(sock, BufferSize, MaxBufferSize)
	{}

	CmStream* GetCmStream() { return &m_CmStream; }
};
////

CmStream* ClientConnector::GetSendCmStream()
{
	__GUARD__
		__DebugSendStream* p = (__DebugSendStream*)m_pSendStream;
	return p->GetCmStream();
	__UNGUARD__
		return NULL;
}

BOOL ClientConnector::Handle_Output()
{
	__GUARD__

		if (!IsValid())
			return FALSE;

	if (IsDisconnect())
		return TRUE;

	_NET_TRY
	{
		int32 _Idx;
	uint32 nSendBytes[MAX_GAMECONNECTOR_SEND_TIMES] = { 0 };
	for (_Idx = 0; _Idx < MAX_GAMECONNECTOR_SEND_TIMES; _Idx++)
	{
		if (!m_pSendStream->HasDataToSend())
		{
			return TRUE;
		}

		uint32	ret = m_pSendStream->Flush();

		if ((int32)ret <= SOCKET_ERROR)
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_CONNECTOR,
				"GameConnector::Handle_Output: Error ! ret:%d SocketError:%s SNID=%llu ",
				(int32)ret,
				KLSocketError(),
				GetPlayerSNID()
			);
			return FALSE;
		}

		m_uTotalSendSize += ret;

		nSendBytes[_Idx] = ret;
	}

	if (_Idx == MAX_GAMECONNECTOR_SEND_TIMES && GetDebugMode())
	{
		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"Debug GameConnector: snid=%llu %u %u %u",
			GetPlayerSNID(),
			nSendBytes[0],
			nSendBytes[1],
			nSendBytes[2]
		);
	}
	}
		_NET_CATCH
	{
		QLOGGER_CODEHOLDER();
	return FALSE;
	}

		// 调试代码
		if (GetDebugMode())
		{
			__DebugSendStream* p = (__DebugSendStream*)m_pSendStream;
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_CONNECTOR,
				"Debug GameConnector: snid=%llu head %u, tail %u, size %u, cmsize %u",
				GetPlayerSNID(),
				p->GetHead(),
				p->GetTail(),
				p->Length(),
				p->GetCmStream()->GetSize()
			);
		}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

#ifdef __LINUX__
extern void* read_bytess(char*, char*, int32);
#endif

BOOL ClientConnector::Handle_Command()
{
	__GUARD__

		BOOL		ret;
	char		header[MSG_HEADER_SIZE];
	MSG_ID		packetID;
	uint32		packetuint, packetSize, packetIndex;
	IMessage	*pKMessage = NULL;

	m_ErrorCode = 0;

	// 	KCheck(m_pPlayer);
	// 
	// 	if(1 == m_pPlayer->GetMapIsNullInNotifyUser())
	// 	{
	// 		QLogSystem::CONNECTOR_LOG
	// 			(
	// 				"GameConnector::Handle_Command(): Error ! MapIsNULLInNotifyUser，SNID=%llu", 
	// 				m_pPlayer->GetSNID()
	// 			);
	// 		return FALSE;
	// 	}

	if (IsDisconnect())
		return TRUE;

	_NET_TRY
	{
		for (int32 _Idx = 0; _Idx < EXE_COUNT_PER_TICK; _Idx++)
		{
			if (!m_pRecieveStream->Peek(&header[0], MSG_HEADER_SIZE))
			{
				break;
			}

			memcpy(&packetID, &header[0], sizeof(MSG_ID));
			m_pRecieveStream->DeEncryptMsgHead(&packetID);

			// 			if (m_pPlayer != NULL)
			// 				packetID = packetID - m_pPlayer->GetMsgIdOffset();

			memcpy(&packetuint, &header[sizeof(MSG_ID)], sizeof(uint32));
			m_pRecieveStream->DeEncryptMsgHead(&packetuint);

#ifdef __LINUX__
			m_pRecieveStream->Encode(&header[0],&header[0],sizeof(MSG_ID) + sizeof(uint32));
			read_bytess(&header[0],GetSocket()->m_IP,GetSocket()->getSOCKET());
#endif	
			packetSize = GET_MSG_LEN(packetuint);
			packetIndex = GET_MSG_INDEX(packetuint);

			if (IsLegalMessageID(packetID) == FALSE)
			{
				m_ErrorCode = 1;
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! invalid packetID %u! snid=%llu,ip=%s",
					packetID,
					GetPlayerSNID(),
					GetSocket()->m_IP
				);
				return FALSE;
			}

			_NET_TRY
			{
				if (m_pRecieveStream->Length() < MSG_HEADER_SIZE + packetSize)
				{
					break;
				}

			uint32	maxPacketSize = GETMESSAGE_MAX_SIZE(packetID, GetSocket()->m_IP, GetSocket()->m_Port);
			if (packetSize > maxPacketSize)
			{
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! 消息大小异常,PacketID=%u,PrePacketID=%u,Size=%d,MaxSize=%d SNID=%llu ",
					packetID,
					m_PreMessageID,
					packetSize,
					maxPacketSize,
					GetPlayerSNID()
				);
			}

			m_pRecieveStream->DeEncryptMessage(packetID + packetIndex,packetSize);

			IMessage* pKMessage = CREATE_MESSAGE_OBJ(IMessage, packetID);
			if (pKMessage == NULL)
			{
				m_ErrorCode = 3;
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! CreateMessage Fail,packetID=%u",
					packetID
				);
				return FALSE;
			}

			pKMessage->ModifyMsgIdOffset(0);
			pKMessage->SetComIndex(packetIndex);

			uint32	nSizeBefore = m_pRecieveStream->Length();
			ret = m_pRecieveStream->ReciveMessage(pKMessage);
			uint32	nSizeAfter = m_pRecieveStream->Length();
			if (ret == FALSE)
			{
				m_ClientMessageIndex++;
				m_PerfMessageIndex++;
				DESTROY_MESSAGE_OBJ(pKMessage);
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! RecieveMessage Error!packetID=%u",
					packetID
				);
				m_ErrorCode = 4;
				return FALSE;
			}

			if ((nSizeBefore - nSizeAfter - 6) != pKMessage->GetMsgSize())
			{
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! packetID=%u,(Read%d,Should%d),PID=%d,SNID=llu",
					pKMessage->GetMsgID(),
					nSizeBefore - nSizeAfter - 6,
					pKMessage->GetMsgSize(),
					m_PreMessageID,
					GetPlayerSNID()
				);
			}

			BOOL	bNeedRemove = TRUE;
			BOOL	bException = FALSE;
			_NET_TRY
			{
				ResetKick();

			uint32	uret;

			if (TestMessageIndex(packetIndex))
			{
				m_ClientMessageIndex++;
				m_PerfMessageIndex++;
				_NET_TRY
				{
					if (FilterMessage(pKMessage) == FALSE)
					{
						uret = pKMessage->Process(this);
					}
					else
					{
						uret = MP_CONTINUE;
					}
				m_PreMessageID = packetID;
				}
					_NET_CATCH
				{
					QLOGGER_MESSAGECODEHOLDER(packetID);
				uret = MP_ERROR;
				}
			}
			else
			{
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! PIndex Wrong! SNID=%llu IP=%s",
					GetPlayerSNID(),
					m_pSocket->m_IP
				);
				uret = MP_ERROR;
			}

			if (uret == MP_ERROR)
			{
				if (pKMessage)
					DESTROY_MESSAGE_OBJ(pKMessage);
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! uret=MP_ERROR, packetID=%u",
					packetID
				);
				m_ErrorCode = 6;
				return FALSE;
			}
			else if (uret == MP_BREAK)
			{
				if (pKMessage) DESTROY_MESSAGE_OBJ(pKMessage);
				break;
			}
			else if (uret == MP_CONTINUE)
			{
			}
			else if (uret == MP_NOTREMOVE)
			{
				bNeedRemove = FALSE;
			}
			else if (uret == MP_NOTREMOVE_ERROR)
			{
				m_ErrorCode = 7;
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"GameConnector::Handle_Command(): Error ! uret=MP_NOTREMOVE_ERROR, packetID=%u",
					packetID
				);
				return FALSE;
			}
			else
			{
			}
			}
				_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			bException = TRUE;
			}

				if (pKMessage && bNeedRemove)
					DESTROY_MESSAGE_OBJ(pKMessage);

			if (bException)
			{
				m_ErrorCode = 8;
				QLogSystem::QUICK_LOG(QUICK_LOG_CONNECTOR, "GameConnector::Handle_Command(): Exception!");
				return FALSE;
			}
			}
				_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
			m_ErrorCode = 9;
			return FALSE;
			}
		}
	}
		_NET_CATCH
	{
		QLOGGER_CODEHOLDER();
	m_ErrorCode = 10;
	return FALSE;
	}

	return TRUE;
	__UNGUARD__
		m_ErrorCode = 11;
	return FALSE;
}

BOOL ClientConnector::HeartBeat(uint32 uTime, int32 nFlag)
{
	__GUARD__

		if (IsDisconnect())
		{
			mLeftTimeToQuit -= ABS(uTime - m_CurrentTime);
			if (mLeftTimeToQuit < 0)
			{
				QLogSystem::QUICK_LOG
				(
					QUICK_LOG_CONNECTOR,
					"GameConnector::HeartBeat(): Error ! SNID=%llu m_i_LeftTimeToQuit=%d",
					m_PlayerSNID,
					mLeftTimeToQuit
				);
				return FALSE;
			}
		}

	if (nFlag)
	{
		if (ReachTime(m_KickTime + MAX_KHAN_INCOM_KICK_TIME, uTime))
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_CONNECTOR,
				"GameConnector::HeartBeat(): Error ! Incoming Kick SNID=%llu",
				m_PlayerSNID
			);
			return FALSE;
		}
	}
	else
	{
		if (ReachTime(m_KickTime + g_Config.mConfigInfo.m_KickUserTime, uTime))
		{
			QLogSystem::QUICK_LOG
			(
				QUICK_LOG_CONNECTOR,
				"GameConnector::HeartBeat(): Error ! Kick SNID=%llu KickTime=%u",
				m_PlayerSNID,
				m_KickTime
			);
			return FALSE;
		}
	}

	// 到达延迟删除时间，清理连接 [yangzhen]
	if (CkeckDelayRemove(uTime))
	{
		QLogSystem::QUICK_LOG
		(
			QUICK_LOG_CONNECTOR,
			"GameConnector::HeartBeat(): CheckDelayRemove == TRUE, remove connector now ! SNID=%llu",
			m_PlayerSNID
		);
		return FALSE;
	}

	return Connector::HeartBeat(uTime);

	__UNGUARD__
		return FALSE;
}

BOOL ClientConnector::SendMessage(IMessage *pKMessage)
{
	__GUARD__

		if (IsDisconnect())
			return TRUE;

	if (m_pSendStream != NULL)
	{
		pKMessage->ModifyMsgIdOffset(0);

		pKMessage->SetComIndex(m_MessageIndex++);

		// 		if (m_pPlayer != NULL)
		// 		{
		// 			pKMessage->ModifyMsgIdOffset(m_pPlayer->GetMsgIdOffset());
		// 			m_pPlayer->IncreaseMsgCount(1);
		// 		}

		uint32	nSizeBefore = m_pSendStream->Length();
		BOOL	ret = m_pSendStream->SendMessage(pKMessage, true);
		KCheck(ret);
		uint32	nSizeAfter = m_pSendStream->Length();
		if (pKMessage->GetMsgSize() != nSizeAfter - nSizeBefore - 6)
		{
			QLogSystem::QUICK_LOG
			(
				SLOW_LOG_MSGERROR,
				"GameConnector::SendMessage() Error ! Wrong MSG Size! ID=%d(Write%d,Should%d),prePid=%d,SNID=%llu",
				pKMessage->GetMsgID() - pKMessage->GetMsgIdOffset(),
				nSizeAfter - nSizeBefore - 6,
				pKMessage->GetMsgSize(),
				m_PreWriteID,
				GetPlayerSNID()
			);
		}

		m_PreWriteID = pKMessage->GetMsgID() - pKMessage->GetMsgIdOffset();
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

void ClientConnector::Init()
{
	SetDisconnect(FALSE);
	ResetKick();
	//SetDirty(TRUE);
}

void ClientConnector::Disconnect()
{
	mLeftTimeToQuit = g_Config.mConfigInfo.mDisconnectTime;

	SetDisconnect(TRUE);

	_NET_TRY
	{
		m_pSocket->close();
	}
		_NET_CATCH
	{
		QLOGGER_CODEHOLDER();
	}
}

BOOL ClientConnector::FreeMySelf()
{
	__GUARD__

		// 解除会话绑定之后Session才可以被回收，否则Session会被耗尽 [12/23/2017 yz]
		unbindSession();

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL ClientConnector::SendMessageDirectly(IMessage *pKMessage)
{
	__GUARD__

		BOOL bRet = SendMessage(pKMessage);

	if (bRet)
	{
		int32 cnt = 0;
		while (m_pSendStream->HasDataToSend())
		{
			if (cnt++ > 3)
			{
				bRet = FALSE;
				break;
			}
			Handle_Output();
		}
	}

	return bRet;

	__UNGUARD__
		return FALSE;
}

BOOL ClientConnector::TestMessageIndex(uchar bIndex)
{
	if (bIndex == m_ClientMessageIndex)
		return TRUE;
	else
	{
		if (m_FirstIndexCheck == FALSE)
		{
			m_ClientMessageIndex = bIndex;
			m_FirstIndexCheck = TRUE;
			return TRUE;
		}
		return FALSE;
	}
}

BOOL ClientConnector::IsLegalMessageID(const MSG_ID packetID)
{
	__GUARD__

		if (packetID >= (MSG_ID)Messages::MESSAGE_MAX)
		{
			return FALSE;
		}

	if (IsServerConnector() == TRUE)
	{
		return TRUE;
	}

	if (packetID > (MSG_ID)MESSAGE_CG_BEGIN && packetID < (MSG_ID)MESSAGE_CG_END)
	{
		return TRUE;
	}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

void ClientConnector::EnableCompress(BOOL bEnable, TID threadID)
{
	m_pSendStream->EnableCompress(bEnable, threadID);
}

void ClientConnector::SetDefaultEncrptorKey()
{
	char buf[Encrptor::ED_DATA_LENGTH + 1] = { 0 };
	uint32 version = STREAM_ENCRPTOR_KEY;

	char* pBuf = buf;
	int32 _Idx = 0;
	while (TRUE)
	{
		if (_Idx + sizeof(uint32) > Encrptor::ED_DATA_LENGTH)
			break;

		memcpy(pBuf, &version, sizeof(uint32));
		pBuf += sizeof(uint32);
		_Idx += sizeof(uint32);
	}

	SetEncrptorKey(buf);
}

void ClientConnector::SetEncrptorKey(char* pKey)
{
	m_pSendStream->GetEncrptor()->SetPdata(pKey, STREAM_MAGIC_NUM);
	m_pRecieveStream->GetEncrptor()->SetPdata(pKey, STREAM_MAGIC_NUM);
}

BOOL ClientConnector::FilterMessage(IMessage* pKMessage)
{
	//if( g_Config.m_MultiGroupConfig.m_nGroupCount == 0 )
	//	return FALSE;

	// Ping消息可能随时发送，所以任何线程都需要响应Ping消息 [4/27/2017 yz]
	if (pKMessage->GetMsgID() == Messages::MESSAGE_CG_PING)
		return FALSE;

	return FALSE;
}

void ClientConnector::EnterDelayRemove(uint32 uDelayTime)
{
	__GUARD__
		if (!m_DelayRemoveTimer.IsSetTimer())
		{
			m_DelayRemoveTimer.BeginTimer(uDelayTime, GET_TIME().TickCount());
		}
	__UNGUARD__
}

void ClientConnector::CancelDelayRemove()
{
	__GUARD__
		if (m_DelayRemoveTimer.IsSetTimer())
		{
			m_DelayRemoveTimer.Clear();
		}
	__UNGUARD__
}

BOOL ClientConnector::CkeckDelayRemove(uint32 uTime)
{
	__GUARD__
		if (m_DelayRemoveTimer.IsSetTimer())
		{
			return m_DelayRemoveTimer.CountingTimer(uTime);
		}
	return FALSE;
	__UNGUARD__
		return FALSE;
}

void ClientConnector::bindSession(ClientSession* session)
{
	__GUARD__
	unbindSession();
	mSession = session;
	if (mSession)
	{
		mSession->bindConnector(this);
	}
	__UNGUARD__
}

void ClientConnector::unbindSession()
{
	__GUARD__
	if (mSession)
	{
		mSession->unbindConnector();
		mSession = NULL;
	}
	__UNGUARD__
}

BOOL ClientConnector::enableEncryptKey()
{
	__GUARD__

	if (g_Config.mConfigInfo.mEnableRandEncryptKey == 0)
		return TRUE;

	BOOL bRet = FALSE;

	FixedString<128> bufStr;

	char buf[Encrptor::ED_DATA_LENGTH + 1] = { 0 };
	srand(GET_TIME().GetCTime());
	for (int32 _Idx = 0; _Idx < Encrptor::ED_DATA_LENGTH; _Idx++)
	{
		buf[_Idx] = rand() % 256;
	}
	bufStr = buf;
	
	// 将生成的加密数组发送到客户端 [12/28/2017 yz]
	GCEncryptKey Msg;
	Msg.SetKeyData(&bufStr);
	bRet = SendMessageDirectly(&Msg);

	// 将生成的加密数组设置到服务端连接，保持与客户端一致 [12/28/2017 yz]
	SetEncrptorKey(buf);

	return bRet;

	__UNGUARD__
		return FALSE;
}