#ifndef __CONNECTOR_MANAGER_H__
#define __CONNECTOR_MANAGER_H__

#include "TypeDefine.h"
#include "ConnectorPool.h"
#include "KwNetAcceptor.h"


class ConnectorManager
{
public:
	ConnectorManager();
	virtual ~ConnectorManager();

	// ��ʼ������
	// ָ��ConnectorID������������������ռ�
	// ����������������ʼ����������Ҫ���ش˺�����
	virtual BOOL	Init(uint32 uMaxCount,const char* ip=NULL,uint16 port=0, uint32 uBackLog = 5);
	
	// ��ʼ��ConnectorPool����
	virtual BOOL	InitPoolData(ConnectorPool* pool = NULL) = 0;

	// ��������
	virtual BOOL	HeartBeat(uint32 uTime = 0);

	// ����Connector��Socket
	virtual BOOL	AddConnectorSocket(SOCKET fd,ut_id_connector pid) { return TRUE; }
	
	// ɾ��Connector��Socket
	virtual BOOL	DelConnectorSocket(SOCKET fd, ut_id_connector pid) { return TRUE; }

	// ����Connector
	virtual BOOL	AddConnector(Connector *pConnector);

	// ɾ��Connector(������������SOCKET)
	virtual BOOL	DelConnector(Connector *pConnector);
	
	// ����ɾ��Connector(������������SOCKET��POOL����ɾ��Cache�����е���ذ�)
	virtual BOOL	RemoveConnector(Connector *pConnector);
	
	// ɾ���������е�����Connector
	void			RemoveAllConnector();

	// ȡ��Connectorָ��
	Connector*		GetConnector(ut_id_connector pid);

	// ��������Ƿ���������ӹ������� [yangzhen]
	Connector*		FindConnectorInManager(ut_id_connector pid);

	// ͨ��Manager�е�������ȡ [yangzhen]
	Connector*		GetConnectorByIndex(int32 nIndex);

	// 2010-9-25 by rj �Կ����ʽȡ������
	Connector		*GetConnectorForGroupMode(ut_id_connector pid);

	// ȡ�õ�ǰ��������Connector����
	uint32			GetConnectorCount();

	// ����Ϣ�����������Ϣ
	virtual BOOL	SendMessage(IMessage *pKMessage, ut_id_connector ServerID, uint32 Flag = PF_NONE);
	
	// ������Ϣ�����е���Ϣ
	BOOL			Handle_CacheCommands();
	
	// ����Ϣ������ȡ����Ϣ
	BOOL			RecvMessage(IMessage * &pKMessage, ut_id_connector &PlayerID, uint32 &Flag);
	
	// ɾ����Ϣ�����е���Ϣ
	BOOL			MoveMessage(ut_id_connector PlayerID);

	// ��չ��Ϣ���пռ�
	BOOL			ResizeCache();

	ut_id_connector*	GetConnectorIDs() { return m_aConnecorID; }

	//������Ӳ��������������ɷ���������߳�ִ�� 
	virtual BOOL		CheckAndReconnectToServer();

	//ɾ��m_aConnecorID ����Ч������ [xingzhishan]
	BOOL DelInvalidConnectorFromMgr(uint32 Index);

	// ��ȡ���ConnectorManager�����ƣ�����д��־ [yangzhen]
	virtual const char* GetManagerName();

protected:
	// ��Connector��ӵ���������
	virtual BOOL	AddConnectorToMgr(Connector *pConnector);
	
	// �ӹ�������ɾ��Connector
	virtual BOOL	DelConnectorFromMgr(Connector *pConnector);

	// �����µ�����
	virtual Connector*	RecvNewConnector();


public:
	TID				m_ThreadID;

protected:
	BOOL			m_bServerMode;				// ������ģʽ��TRUEΪ������ģʽ��FALSEΪ�ͻ���ģʽ
	Acceptor*		m_pServerSocket;			// ������Acceptorָ��
	SOCKET			m_SocketID;					// ������SocketID
	uint32			m_uFDSize;					// ʵ�ʵ�������������

	ut_id_connector*	m_aConnecorID;				// ConnectorID����
	uint32			m_uMaxConnectorCount;		// Connector�������
	uint32			m_uConnectorCount;			// Connector��ǰ����
	ConnectorPool*	m_pConnectorPool;			// ConnectorPoolָ��

	FastLock		m_Lock;
	ASYNC_MSG	*m_MessageQue;				// ��Ϣ����
	uint32			m_QueSize;					// ��Ϣ���д�С
	uint32			m_Head;						// ��Ϣ����Head���
	uint32			m_Tail;						// ��Ϣ����Tail���
};

#endif
