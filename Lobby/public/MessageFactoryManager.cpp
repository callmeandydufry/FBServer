/*$T Share/MessageFactoryManager.cpp GC 1.140 10/10/07 10:06:30 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "MessageFactoryManager.h"

#if defined(__GAMESERVER__) || defined(__GAMECLIENT__)
#include "CGHandShake.h"
#include "GCHandShake.h"
#include "CGReqeustLogin.h"
#include "GCRpcInvokeMsg.h"
#include "CGRpcInvokeMsg.h"
#include "GCEncryptKey.h"
#include "CGPing.h"
#include "GCPing.h"
#endif

#if defined(__GAMESERVER__)
#include "SSHandShake.h"
#include "ModulePacketMessage.h"

#endif

using namespace Messages;

/*
 =======================================================================================================================
 =======================================================================================================================
 */

MessageFactoryManager::MessageFactoryManager()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	m_Factories = NULL;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_Size = MESSAGE_MAX;

	KCheck(m_Size > 0);

	m_Factories = new MessageFactory *[m_Size];
	KCheck(m_Factories);
	m_pMessageAllocCount = new uint32[m_Size];
	KCheck(m_pMessageAllocCount);

	for(int32 _Idx = 0; _Idx < m_Size; _Idx++)
	{
		m_Factories[_Idx] = NULL;
		m_pMessageAllocCount[_Idx] = 0;
	}

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
MessageFactoryManager::~MessageFactoryManager()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	KCheck(m_Factories != NULL);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(int32 _Idx = 0; _Idx < m_Size; _Idx++)
	{
		KSafeDelete(m_Factories[_Idx]);
	}

	KSafeDelete_ARRAY(m_Factories);
	KSafeDelete_ARRAY(m_pMessageAllocCount);

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
MessageFactoryManager &MessageFactoryManager::instance(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	static MessageFactoryManager	__packet_factory;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return __packet_factory;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL MessageFactoryManager::Init()
{
	__GUARD__


#if defined(__GAMESERVER__) || defined(__GAMECLIENT__)
	
ADD_MSG_FACTORY(CGHandShake) 
ADD_MSG_FACTORY(GCHandShake)
ADD_MSG_FACTORY(GCEncryptKey)
ADD_MSG_FACTORY(CGRequestLogin)
ADD_MSG_FACTORY(GCRpcInvokeMsg)
ADD_MSG_FACTORY(CGRpcInvokeMsg)
ADD_MSG_FACTORY(CGPing)
ADD_MSG_FACTORY(GCPing)

#endif

#if defined(__GAMESERVER__)

ADD_MSG_FACTORY(SSHandShake)
ADD_MSG_FACTORY(XXModulePacket)

#endif


	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageFactoryManager::AddFactory(MessageFactory *pFactory)
{
	__GUARD__ if(m_Factories[pFactory->GetMsgID()] != NULL)
	{
		KCheck(FALSE);
		return;
	}

	m_Factories[pFactory->GetMsgID()] = pFactory;

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
IMessage *MessageFactoryManager::CreateMessage(MSG_ID packetID)
{
	__GUARD__ if(packetID >= m_Size || m_Factories[packetID] == NULL)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~*/
		char	szBuff[128] = { 0 };
		/*~~~~~~~~~~~~~~~~~~~~~~~~*/

		tsnprintf_s(szBuff, 128, "Check <packetID >= m_Size || m_Factories[%u] == NULL> fail!", packetID);
		KCheckEx(FALSE, szBuff);
		return NULL;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	IMessage	*pKMessage = NULL;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef _LOGIN_CENTER_
	Lock();
#endif
	_NET_TRY
	{
		pKMessage = m_Factories[packetID]->CreateMessage();
		m_pMessageAllocCount[packetID]++;
	}

	_NET_CATCH
	{
		pKMessage = NULL;
	}

#ifdef _LOGIN_CENTER_
	Unlock();
#endif
	return pKMessage;

	__UNGUARD__ return NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 MessageFactoryManager::GetMessageMaxSize(MSG_ID packetID, char* szIP, int32 uPort)
{
	__GUARD__ if(packetID >= m_Size || m_Factories[packetID] == NULL)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifdef _LOGIN_CENTER_
		BOOL			bErrLog = TRUE;
		ErrLogTableIterator	it = m_ErrLogCount.find(packetID);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(it == m_ErrLogCount.end())
		{
			m_ErrLogCount.insert(ErrLogTableHashMap::value_type(packetID, 0));
		}
		else
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~*/
			uint32	nCount = it->second;
			/*~~~~~~~~~~~~~~~~~~~~~~~~*/

			if(nCount >= 3)
			{
				bErrLog = FALSE;
			}
			else
			{
				it->second = nCount + 1;
			}
		}

		if(bErrLog)
		{
#endif

			/*~~~~~~~~~~~~~~~~~~~~~~*/
			char	buff[256] = { 0 };
			/*~~~~~~~~~~~~~~~~~~~~~~*/
			sprintf(buff, "GetPacketMaxSize: PID(%d) not register, IP=%s, Port=%d", packetID, szIP, uPort);
			KCheckEx(FALSE, buff);
			return 0;
#ifdef _LOGIN_CENTER_
		}
		else
		{
			return 0xFFFFFFFF;
		}
#endif
	}

#ifdef _LOGIN_CENTER_
	Lock();
#endif

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	iRet = m_Factories[packetID]->GetMessageMaxSize();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef _LOGIN_CENTER_
	Unlock();
#endif
	return iRet;

	__UNGUARD__ return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 MessageFactoryManager::GetRegisterMaxSize(MSG_ID packetID)
{
	__GUARD__ if(packetID >= m_Size || m_Factories[packetID] == NULL)
	{
		return 0;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	iRet = m_Factories[packetID]->GetMessageMaxSize();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return iRet;
	__UNGUARD__ return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageFactoryManager::RemoveMessage(IMessage *pKMessage)
{
	__GUARD__ if(pKMessage == NULL)
	{
		KCheck(FALSE);
		return;
	}

	pKMessage->ModifyMsgIdOffset(0);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	MSG_ID	packetID = pKMessage->GetMsgID();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(packetID >= m_Size)
	{
		KCheck(FALSE);
		return;
	}

#ifdef _LOGIN_CENTER_
	Lock();
#endif
	_NET_TRY
	{
		KSafeDelete(pKMessage);
		m_pMessageAllocCount[packetID]--;
	}

	_NET_CATCH
	{
	}

#ifdef _LOGIN_CENTER_
	Unlock();
#endif
	return;

	__UNGUARD__ return;
}

void MessageFactoryManager::Stop()
{
	__GUARD__
#ifdef __MAPSERVER__
	for (int32 i = 0; i < MESSAGE_MAX; i++)
	{
		if (m_pMessageAllocCount[i] > 0)
		{
			QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "messageId=%d, msgNum=%d", i, m_pMessageAllocCount[i]);
		}
	}
#endif
	__UNGUARD__
}