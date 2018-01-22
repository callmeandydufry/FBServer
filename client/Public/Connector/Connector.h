#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "TypeDefine.h"
#include "KwNetSocket.h"
#include "KwNetMessage.h"

#define MAX_CACHE_SIZE		1024
#define EXE_COUNT_PER_TICK	1024

enum MSG_FLAG { PF_NONE = 0, PF_REMOVE };

class ConnectorManager;

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
struct ASYNC_MSG
{
	IMessage		*m_pMessage;
	ut_id_connector	m_PlayerID;
	uint32			m_Flag;

	ASYNC_MSG()
	{
		m_pMessage	= NULL;
		m_PlayerID	= INVALID_KHAN_ID;
		m_Flag		= PF_NONE;
	};
	~ASYNC_MSG()
	{
		KSafeDelete(m_pMessage);
		m_PlayerID	= INVALID_KHAN_ID;
		m_Flag		= PF_NONE;
	};
};


/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
class	Connector
{
public:
	Connector();
	virtual ~Connector();

	// ��ʼ��Connectorʱ���á�
	// ��ʼ��Socket,Stream��������Stream���ڴ�ռ䡣
	// �������費ͬ��С��Stream�������ش˺�����
	virtual void	InitSocket();

	// ���������
	virtual void	Clear();

	// ��ȡ����
	virtual BOOL	Handle_Input();
	
	// ��������
	virtual BOOL	Handle_Output();
	
	// ��������
	virtual BOOL	Handle_Command();

	// ��packet����SendStream��
	virtual BOOL	SendMessage(IMessage *pKMessage);

	// ��������
	virtual BOOL	HeartBeat(uint32 uTime = 0);

	// ����Connector�Ļʱ��
	virtual void	ResetKick();
	
	// ��ʶConnector����
	virtual BOOL	IsGameConnector() = 0;
	virtual BOOL	IsServerConnector() = 0;

public:
	BOOL			IsValid();

	Socket*			GetSocket() const				{ return m_pSocket; }
	void			SetSocket(Socket* pSocket);

	ut_id_connector	GetConnectorID() const			{ return m_ConnectorID; }
	void			SetConnectorID(ut_id_connector id){ m_ConnectorID = id; }

	ConnectorManager* GetConnectorMgr() const { return mConnectorMgr; }
	void			SetConnectorMgr(ConnectorManager* connMgr) { mConnectorMgr = connMgr; }

	ut_id			GetManagerID() const 			{ return m_ManagerID; }
	void			SetManagerID(ut_id id)			{ m_ManagerID = id; }

	BOOL			IsEmpty() const					{ return m_IsEmpty; }
	void			SetEmpty(BOOL b = TRUE)			{ m_IsEmpty = b; }

	uint32			GetStatus() const				{ return m_Status; }
	void			SetStatus(uint32 status)		{ m_Status = status; }

	BOOL			IsDisconnect()					{ return m_bDisconnect; }
	void			SetDisconnect(BOOL b)			{ m_bDisconnect = b; }

	RecieveStream*	GetRecieveStream()				{ return m_pRecieveStream; }
	SendStream*		GetSendStream()					{ return m_pSendStream; }

	// ���ü���ģʽ
	void			SetEncryptMode(uint32 mode);
	
	TID				GetOwnerThreadId() { return m_nCurrThreadId; }
	void			SetOwnerThreadId(TID nTd) { m_nCurrThreadId = nTd; }

protected:
	ConnectorManager* mConnectorMgr;
	ut_id_connector	m_ConnectorID;			// Connector����ID
	uint32			m_Status;				// Connector״̬
	BOOL			m_bDisconnect;			// �Ƿ�Ͽ�����

	BOOL			m_IsEmpty;				// �ڹ������У���ʶConnector�Ƿ�ʹ��
	ut_id			m_ManagerID;			// Connector�ڹ������е�ID

	uchar			m_MessageIndex;			// ���Ͱ����к�
	MSG_ID			m_LastMessageID;			// ��һ���յ�����ID

	Socket			*m_pSocket;				// Socketָ��
	RecieveStream	*m_pRecieveStream;		// ����������
	SendStream		*m_pSendStream;			// ����������

	TID m_nCurrThreadId;
};
#endif
