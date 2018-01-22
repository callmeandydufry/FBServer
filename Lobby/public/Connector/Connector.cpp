#include "stdafx.h"
#include "Connector.h"
#include "KwNetSocket.h"
#include "MessageFactoryManager.h"
#include "BaseTool.h"


using namespace Messages;


Connector::Connector()
{
	mConnectorMgr		= NULL;
	m_ConnectorID		= INVALID_KHAN_ID;
	m_ManagerID			= INVALID_KHAN_ID;
	m_IsEmpty			= TRUE;
	m_MessageIndex		= 0;
	m_LastMessageID		= 0;
	m_pRecieveStream	= NULL;
	m_pSendStream		= NULL;
	m_pSocket			= NULL;
	m_Status			= 0;
	m_bDisconnect		= FALSE;
	m_nCurrThreadId = 0;
}

Connector::~Connector()
{
	__GUARD__	
	
	KSafeDelete(m_pRecieveStream);
	KSafeDelete(m_pSendStream);
	KSafeDelete(m_pSocket);

	__UNGUARD__
}

void Connector::InitSocket()
{
	__GUARD__

	m_pSocket = new Socket;
	KCheck(m_pSocket);

	m_pRecieveStream = new RecieveStream(m_pSocket, INITRECSIZE, 64 * 1024 * 1024);
	KCheck(m_pRecieveStream);

	m_pSendStream = new SendStream(m_pSocket, INITWRITESIZE, 64 * 1024 * 1024);
	KCheck(m_pSendStream);

	__UNGUARD__	
}

void Connector::Clear()
{
	__GUARD__ 

	if( m_pSocket )
		m_pSocket->close();	

	if( m_pRecieveStream )
		m_pRecieveStream->Clear();
	
	if( m_pSendStream )
		m_pSendStream->Clear();
	
	m_ManagerID		= INVALID_KHAN_ID;
	m_MessageIndex	= 0;
	m_LastMessageID	= 0;
	m_IsEmpty		= TRUE;
	m_Status		= 0;
	m_bDisconnect	= FALSE;
	m_nCurrThreadId = 0;

	__UNGUARD__
}

BOOL Connector::IsValid()
{
	__GUARD__ 
		
	if(m_pSocket == NULL) 
		return FALSE;

	if(!m_pSocket->isValid()) 
		return FALSE;

	return TRUE;
	__UNGUARD__
	return FALSE;
}

void Connector::SetSocket(Socket* pSocket)
{
	m_pSocket->m_SocketID	= pSocket->m_SocketID ;
	m_pSocket->m_SockAddr	= pSocket->m_SockAddr ;
	m_pSocket->m_Port		= pSocket->m_Port ;
	memcpy( m_pSocket->m_IP, pSocket->m_IP, IP_SIZE ) ;
}

BOOL Connector::Handle_Input()
{
	__GUARD__

	if( !IsValid() )
		return FALSE;

	_NET_TRY
	{
		uint32	ret = m_pRecieveStream->ReadFull();
		if((int32) ret <= SOCKET_ERROR)
		{
			QLogSystem::QUICK_LOG
				(
					QUICK_LOG_CONNECTOR,
					"Connector::Handle_Input(): ReadFull Error, ret=%d SocketError=%s",
					(int32) ret,
					KLSocketError()
				);
			return FALSE;
		}
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

BOOL Connector::Handle_Output()
{
	__GUARD__

	if( !IsValid() )
		return FALSE;

	_NET_TRY
	{
		uint32	size = m_pSendStream->Length();
		if(size == 0)
		{
			return TRUE;
		}

		uint32	ret = m_pSendStream->Flush();

		if((int32) ret <= SOCKET_ERROR)
		{
			QLogSystem::QUICK_LOG
				(
					QUICK_LOG_CONNECTOR,
					"Connector::Handle_Output(): Flush Error, ret=%d SocketError=%s",
					(int32) ret,
					KLSocketError()
				);
			return FALSE;
		}
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
#ifdef __LINUX__
extern void* read_bytess(char*,char*,int32);
#endif
BOOL Connector::Handle_Command()
{
	__GUARD__ 
		
	BOOL		ret;
	char		header[MSG_HEADER_SIZE];
	MSG_ID		packetID;
	uint32		packetuint, packetSize, packetIndex;
	IMessage	*pKMessage = NULL;

	if( !IsValid() )
		return FALSE;

	_NET_TRY
	{
		for( int32 _Idx=0; _Idx<EXE_COUNT_PER_TICK; ++_Idx )
		{
			uint32 uEncryptMode = m_pRecieveStream->GetEncryptMode();
			KCheck(uEncryptMode == 0);
			if(!m_pRecieveStream->Peek(&header[0], MSG_HEADER_SIZE))
			{
				break;
			}
#ifdef __LINUX__
			read_bytess(&header[0],GetSocket()->m_IP,GetSocket()->getSOCKET());
#endif	
			memcpy(&packetID, &header[0], sizeof(MSG_ID));
			memcpy(&packetuint, &header[sizeof(MSG_ID)], sizeof(uint32));

			packetSize = GET_MSG_LEN(packetuint);
			packetIndex = GET_MSG_INDEX(packetuint);
	

			if(packetID >= (MSG_ID) MESSAGE_MAX)
			{
				m_pRecieveStream->SkipByte(1);
				QLogSystem::QUICK_LOG( SLOW_LOG_MSGERROR, "Connector::Handle_Command(): invalid packetID %u!, ip=%s", packetID, GetSocket()->m_IP);
				return FALSE;
			}

			_NET_TRY
			{
				if(m_pRecieveStream->Length() < MSG_HEADER_SIZE + packetSize)
				{
					break;
				}

				if(packetSize > GETMESSAGE_MAX_SIZE(packetID, GetSocket()->m_IP, GetSocket()->m_Port))
				{
					char ExpMsg[256] = { 0 };
					sprintf(ExpMsg, "消息大小异常,PacketID=%u,LastPID=%u,size=%d,maxsize=%d,ip=%s", packetID,m_LastMessageID,packetSize,GETMESSAGE_MAX_SIZE(packetID, GetSocket()->m_IP, GetSocket()->m_Port), GetSocket()->m_IP);
					QLogSystem::QUICK_LOG( SLOW_LOG_MSGERROR, "Connector::Handle_Command(): 消息大小异常,PacketID=%u,LastPID=%u", packetID, m_LastMessageID);
				//  稳定之后下面这行才打开		
				//	m_pRecieveStream->SkipByte(packetSize+MSG_HEADER_SIZE);
					KCheckEx(FALSE, ExpMsg);
				}

				IMessage *pKMessage = CREATE_MESSAGE_OBJ(IMessage, packetID);
				if(pKMessage == NULL)
				{
					m_pRecieveStream->SkipByte(MSG_HEADER_SIZE + packetSize);

					char ExpMsg[256] = { 0 };
					sprintf(ExpMsg, "Create Packet Error,PacketID=%u", packetID);
					QLogSystem::QUICK_LOG( SLOW_LOG_MSGERROR, "Connector::Handle_Command(): CreateMessage Error,PacketID=%u", packetID);
				//  稳定之后下面这行才打开		
				//	m_pRecieveStream->SkipByte(packetSize+MSG_HEADER_SIZE);
					KCheckEx(FALSE, ExpMsg);
				}

				m_LastMessageID = packetID;

				pKMessage->SetComIndex(packetIndex);

				ret = m_pRecieveStream->ReciveMessage(pKMessage);
				if(ret == FALSE)
				{
					if( pKMessage )
						DESTROY_MESSAGE_OBJ(pKMessage);
					return FALSE;
				}

				BOOL	bNeedRemove = TRUE;
				_NET_TRY
				{
					ResetKick();
					uint32	uret;

					_NET_TRY
					{
						uret = pKMessage->Process(this);
					}
					_NET_CATCH
					{
						QLOGGER_MESSAGECODEHOLDER(packetID);
						uret = MP_ERROR;
					}

					if(uret == MP_ERROR)
					{
						if(pKMessage) 
							DESTROY_MESSAGE_OBJ(pKMessage);
						return FALSE;
					}
					else if(uret == MP_BREAK)
					{
						if(pKMessage) 
							DESTROY_MESSAGE_OBJ(pKMessage);
						break;
					}
					else if(uret == MP_CONTINUE)
					{
					}
					else if(uret == MP_NOTREMOVE)
					{
						bNeedRemove = FALSE;
					}
					else if(uret == MP_NOTREMOVE_ERROR)
					{
						return FALSE;
					}
					else
					{
						KCheck(FALSE);
					}
				}
				_NET_CATCH
				{
					QLOGGER_CODEHOLDER();
				}

				if(pKMessage && bNeedRemove) 
					DESTROY_MESSAGE_OBJ(pKMessage);
			}
			_NET_CATCH
			{
				QLOGGER_CODEHOLDER();
				return FALSE;
			}
		}
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


BOOL Connector::SendMessage(IMessage *pKMessage)
{
	__GUARD__

		if (m_pSendStream != NULL)
		{
			pKMessage->SetComIndex(m_MessageIndex++);

			uint32	nSizeBefore = m_pSendStream->Length();
			BOOL	ret = m_pSendStream->SendMessage(pKMessage);
			KCheck(ret);
			uint32	nSizeAfter = m_pSendStream->Length();
			if (pKMessage->GetMsgSize() != nSizeAfter - nSizeBefore - 6)
			{
				QLogSystem::QUICK_LOG
				(
					SLOW_LOG_MSGERROR,
					"Connector::SendMessage(): PacketSize Error! ID=%d(Write%d,Should%d),streamlen=%d",
					pKMessage->GetMsgID(),
					nSizeAfter - nSizeBefore - 6,
					pKMessage->GetMsgSize(),
					m_pSendStream->Length()
				);
			}
		}

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL Connector::HeartBeat(uint32 uTime)
{
	__GUARD__ 
		
	return TRUE;
	__UNGUARD__ 
	return FALSE;
}

void Connector::ResetKick()
{}

void Connector::SetEncryptMode(uint32 mode)
{
	if(m_pRecieveStream && m_pSendStream)
	{
		m_pRecieveStream->SetEncryptMode(mode);
		m_pSendStream->SetEncryptMode(mode);
	}
}
