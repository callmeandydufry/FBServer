#pragma once
#include "GameNetBase.h"

class GameNetMessageFactoryManager
{
public:
	GameNetMessageFactoryManager();
	virtual ~GameNetMessageFactoryManager();

	static GameNetMessageFactoryManager	&instance(void);

	bool Init();
	IGameMessage* CreateMessage(MSG_ID packetID);
	uint32 GetMessageMaxSize(MSG_ID packetID);
	void RemoveMessage(IGameMessage *pKMessage);
	
private:
	void AddFactory(IGameMessageFactory *pFactory);

private:
	IGameMessageFactory**		m_Factories;
	uint16						m_Size;

public:
	uint32	*m_pMessageAllocCount;
};