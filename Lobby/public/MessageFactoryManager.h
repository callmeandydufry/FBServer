/*$T Share/MessageFactoryManager.h GC 1.140 10/10/07 10:06:30 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __MESSAGEFACTORYMANAGER_H__
#define __MESSAGEFACTORYMANAGER_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#ifdef _LOGIN_CENTER_
#include "IntegerHashMap.h"
#endif
class	MessageFactoryManager
{
	MessageFactoryManager();
	~				MessageFactoryManager();

	MessageFactoryManager(const MessageFactoryManager &);
	MessageFactoryManager &operator	=(const MessageFactoryManager &);

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	static MessageFactoryManager	&instance(void);

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
	void	AddFactory(MessageFactory *pFactory);

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	BOOL		Init();
	IMessage	*CreateMessage(MSG_ID packetID);
	uint32		GetMessageMaxSize(MSG_ID packetID, char* szIP = NULL, int32 uPort = -1);
	uint32		GetRegisterMaxSize(MSG_ID packetID);
	void		RemoveMessage(IMessage *pKMessage);
	void		Stop();

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void Lock()
	{
		m_Lock.Lock();
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void Unlock()
	{
		m_Lock.Unlock();
	};

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
	MessageFactory					**m_Factories;
	uint16						m_Size;
	FastLock					m_Lock;

#ifdef _LOGIN_CENTER_
#ifdef __WINDOWS__
	typedef stdext::hash_map<MSG_ID, uint32>	ErrLogTableHashMap;
#else
	typedef __gnu_cxx::hash_map<MSG_ID, uint32>	ErrLogTableHashMap;
#endif
	typedef ErrLogTableHashMap::iterator		ErrLogTableIterator;
	ErrLogTableHashMap m_ErrLogCount;
#endif

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	uint32	*m_pMessageAllocCount;
};

#define INIT_MESSAGE_FACTORY()			MessageFactoryManager::instance().Init()
#define CREATE_MESSAGE_OBJ(pkt_class, pkt_id)	(pkt_class *) MessageFactoryManager::instance().CreateMessage(pkt_id)
#define DESTROY_MESSAGE_OBJ(pkt_obj)		MessageFactoryManager::instance().RemoveMessage(pkt_obj)
#define GETMESSAGE_MAX_SIZE(pkt_id, szIP, nPort)		MessageFactoryManager::instance().GetMessageMaxSize(pkt_id, szIP, nPort)
#define GETMESSAGE_REG_MAX_SIZE(pkt_id)		MessageFactoryManager::instance().GetRegisterMaxSize(pkt_id)
#define GETMESSAGE_ALLOC_COUNT(pkt_id)		MessageFactoryManager::instance().m_pMessageAllocCount[pkt_id]
#endif
