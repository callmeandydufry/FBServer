#include "stdafx.h"
#include "ModuleConnector.h"
#include "KwNetSocket.h"
#include "MessageFactoryManager.h"
#include "BaseTool.h"
using namespace Messages;

ModuleConnector::ModuleConnector()
{
}

ModuleConnector::~ModuleConnector()
{
}

BOOL ModuleConnector::Handle_Command()
{
	__GUARD__

		BOOL		ret;
	char		header[MSG_HEADER_SIZE];
	MSG_ID		packetID;
	uint32		packetuint, packetSize, packetIndex;

	IMessage	*pKMessage = NULL;

	if (!IsValid())
		return FALSE;

	_NET_TRY
	{
		for (int32 _Idx = 0; _Idx < EXE_COUNT_PER_TICK; ++_Idx)
		{

			uint32 uEncryptMode = m_pRecieveStream->GetEncryptMode();
			KCheck(uEncryptMode == ENCPYPT_TYPE_PLATFORMSERVER);
			if (!m_pRecieveStream->Peek(&header[0], MSG_HEADER_SIZE))
			{
				break;
			}
#ifdef __LINUX__
			read_bytess(&header[0], GetSocket()->m_IP, GetSocket()->getSOCKET());
#endif	
			memcpy(&packetID, &header[0], sizeof(MSG_ID));
			memcpy(&packetuint, &header[sizeof(MSG_ID)], sizeof(uint32));

			packetSize = BILL_GET_MSG_LEN(packetuint);
			packetIndex = BILL_GET_MSG_INDEX(packetuint);

			if (packetID >= (MSG_ID)MESSAGE_MAX)
			{
				m_pRecieveStream->SkipByte(1);
				QLogSystem::QUICK_LOG(SLOW_LOG_MSGERROR, "PlatConnector::Handle_Command(): invalid packetID %u!, ip=%s", packetID, GetSocket()->m_IP);
				return FALSE;
			}

			_NET_TRY
			{
				if (m_pRecieveStream->Length() < MSG_HEADER_SIZE + packetSize)
				{
					break;
				}

				if (packetSize > GETMESSAGE_MAX_SIZE(packetID, GetSocket()->m_IP, GetSocket()->m_Port))
				{
					char ExpMsg[256] = { 0 };
					sprintf(ExpMsg, "消息大小异常,PacketID=%u,LastPID=%u,size=%d,maxsize=%d,ip=%s", packetID, m_LastMessageID, packetSize, GETMESSAGE_MAX_SIZE(packetID, GetSocket()->m_IP, GetSocket()->m_Port), GetSocket()->m_IP);
					QLogSystem::QUICK_LOG(SLOW_LOG_MSGERROR, "PlatConnector::Handle_Command(): 消息大小异常,PacketID=%u,LastPID=%u", packetID, m_LastMessageID);
					//  稳定之后下面这行才打开		
					//	m_pRecieveStream->SkipByte(packetSize+MSG_HEADER_SIZE);
					KCheckEx(FALSE, ExpMsg);
				}

				IMessage *pKMessage = CREATE_MESSAGE_OBJ(IMessage, packetID);
				if (pKMessage == NULL)
				{
					m_pRecieveStream->SkipByte(MSG_HEADER_SIZE + packetSize);

					char ExpMsg[256] = { 0 };
					sprintf(ExpMsg, "Create Packet Error,PacketID=%u", packetID);
					QLogSystem::QUICK_LOG(SLOW_LOG_MSGERROR, "PlatConnector::Handle_Command(): CreateMessage Error,PacketID=%u", packetID);
					//  稳定之后下面这行才打开		
					//	m_pRecieveStream->SkipByte(packetSize+MSG_HEADER_SIZE);
					KCheckEx(FALSE, ExpMsg);
				}

				m_LastMessageID = packetID;

				pKMessage->SetComIndex(packetIndex);

				ret = m_pRecieveStream->ReciveMessage(pKMessage);
				if (ret == FALSE)
				{
					if (pKMessage)
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

						if (uret == MP_ERROR)
						{
							if (pKMessage)
								DESTROY_MESSAGE_OBJ(pKMessage);
							return FALSE;
						}
						else if (uret == MP_BREAK)
						{
							if (pKMessage)
								DESTROY_MESSAGE_OBJ(pKMessage);
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

					if (pKMessage && bNeedRemove)
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