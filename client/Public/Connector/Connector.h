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

	// 初始化Connector时调用。
	// 初始化Socket,Stream，并分配Stream的内存空间。
	// 子类如需不同大小的Stream，需重载此函数。
	virtual void	InitSocket();

	// 清理操作。
	virtual void	Clear();

	// 读取数据
	virtual BOOL	Handle_Input();
	
	// 发送数据
	virtual BOOL	Handle_Output();
	
	// 处理数据
	virtual BOOL	Handle_Command();

	// 将packet放在SendStream中
	virtual BOOL	SendMessage(IMessage *pKMessage);

	// 心跳处理
	virtual BOOL	HeartBeat(uint32 uTime = 0);

	// 重置Connector的活动时间
	virtual void	ResetKick();
	
	// 标识Connector类型
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

	// 设置加密模式
	void			SetEncryptMode(uint32 mode);
	
	TID				GetOwnerThreadId() { return m_nCurrThreadId; }
	void			SetOwnerThreadId(TID nTd) { m_nCurrThreadId = nTd; }

protected:
	ConnectorManager* mConnectorMgr;
	ut_id_connector	m_ConnectorID;			// Connector连接ID
	uint32			m_Status;				// Connector状态
	BOOL			m_bDisconnect;			// 是否断开连接

	BOOL			m_IsEmpty;				// 在管理器中，标识Connector是否被使用
	ut_id			m_ManagerID;			// Connector在管理器中的ID

	uchar			m_MessageIndex;			// 发送包序列号
	MSG_ID			m_LastMessageID;			// 上一个收到包的ID

	Socket			*m_pSocket;				// Socket指针
	RecieveStream	*m_pRecieveStream;		// 接收流缓存
	SendStream		*m_pSendStream;			// 发送流缓存

	TID m_nCurrThreadId;
};
#endif
