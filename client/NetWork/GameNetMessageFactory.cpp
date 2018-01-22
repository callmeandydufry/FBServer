#include "GameNetMessageFactory.h"
#include "GameNetMessageDefine.h"
#include "GameNetMessages.h"

GameNetMessageFactoryManager::GameNetMessageFactoryManager()
{
	m_Factories = NULL;

	m_Size = MESSAGE_MAX;

	m_Factories = new IGameMessageFactory*[m_Size];
	m_pMessageAllocCount = new uint32[m_Size];

	for (int32 _Idx = 0; _Idx < m_Size; _Idx++)
	{
		m_Factories[_Idx] = NULL;
		m_pMessageAllocCount[_Idx] = 0;
	}
}

GameNetMessageFactoryManager::~GameNetMessageFactoryManager()
{
	for (int32 _Idx = 0; _Idx < m_Size; _Idx++)
	{
		if (m_Factories[_Idx])
		{
			delete m_Factories[_Idx];
		}
	}

	if (m_Factories)
	{
		delete[] m_Factories;
		m_Factories = NULL;
	}
	if (m_pMessageAllocCount)
	{
		delete[] m_pMessageAllocCount;
		m_pMessageAllocCount = NULL;
	}
}

GameNetMessageFactoryManager &GameNetMessageFactoryManager::instance(void)
{
	static GameNetMessageFactoryManager	__message_factory;
	return __message_factory;
}

bool GameNetMessageFactoryManager::Init()
{
	// CG Message [12/28/2017 yz]
	AddFactory(CGHandShake::createFactory());
	AddFactory(CGRequestLogin::createFactory());
	AddFactory(CGPing::createFactory());
	AddFactory(CGRpcInvokeMsg::createFactory());

	// GCMesssage [12/28/2017 yz]
	AddFactory(GCHandShake::createFactory());
	AddFactory(GCEncryptKey::createFactory());
	AddFactory(GCPing::createFactory());
	AddFactory(GCRpcInvokeMsg::createFactory());

	return true;
}

IGameMessage* GameNetMessageFactoryManager::CreateMessage(MSG_ID packetID)
{
	if (packetID >= m_Size || m_Factories[packetID] == NULL)
	{
//		char szBuff[128] = { 0 };
// 		tsnprintf_s(szBuff, 128, "Check <packetID >= m_Size || m_Factories[%u] == NULL> fail!", packetID);
// 		KCheckEx(FALSE, szBuff);
		return NULL;
	}

	IGameMessage* pKMessage = NULL;

	//try
	{
		pKMessage = m_Factories[packetID]->createMessage();
		m_pMessageAllocCount[packetID]++;
	}
	//catch(...)
	//{
	//	pKMessage = NULL;
	//}

	return pKMessage;
}

uint32 GameNetMessageFactoryManager::GetMessageMaxSize(MSG_ID packetID)
{
	if (packetID >= m_Size || m_Factories[packetID] == NULL)
	{
// 		/*~~~~~~~~~~~~~~~~~~~~~~*/
// 		char	buff[256] = { 0 };
// 		/*~~~~~~~~~~~~~~~~~~~~~~*/
// 		sprintf(buff, "GetPacketMaxSize: PID(%d) not register, IP=%s, Port=%d", packetID, szIP, uPort);
// 		KCheckEx(FALSE, buff);
		return 0;
	}

	uint32 iRet = m_Factories[packetID]->getMessageMaxSize();
	return iRet;
}

void GameNetMessageFactoryManager::RemoveMessage(IGameMessage *pKMessage)
{
	if (pKMessage == NULL)
	{
		return;
	}

	MSG_ID packetID = pKMessage->getMsgID();
	if (packetID >= m_Size)
	{
		return;
	}

	//try
	{
		delete pKMessage;
		m_pMessageAllocCount[packetID]--;
	}
	//catch(...)
	//{
	//}

	return;
}

void GameNetMessageFactoryManager::AddFactory(IGameMessageFactory *pFactory)
{
	if (m_Factories[pFactory->getMsgID()] != NULL)
	{
		return;
	}

	m_Factories[pFactory->getMsgID()] = pFactory;
}