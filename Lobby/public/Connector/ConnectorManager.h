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

	// 初始化函数
	// 指定ConnectorID数组的最大数量，分配空间
	// 子类如需做其它初始化操作，需要重载此函数。
	virtual BOOL	Init(uint32 uMaxCount,const char* ip=NULL,uint16 port=0, uint32 uBackLog = 5);
	
	// 初始化ConnectorPool数据
	virtual BOOL	InitPoolData(ConnectorPool* pool = NULL) = 0;

	// 心跳处理
	virtual BOOL	HeartBeat(uint32 uTime = 0);

	// 增加Connector的Socket
	virtual BOOL	AddConnectorSocket(SOCKET fd,ut_id_connector pid) { return TRUE; }
	
	// 删除Connector的Socket
	virtual BOOL	DelConnectorSocket(SOCKET fd, ut_id_connector pid) { return TRUE; }

	// 增加Connector
	virtual BOOL	AddConnector(Connector *pConnector);

	// 删除Connector(包括管理器，SOCKET)
	virtual BOOL	DelConnector(Connector *pConnector);
	
	// 彻底删除Connector(包括管理器，SOCKET，POOL，并删除Cache队列中的相关包)
	virtual BOOL	RemoveConnector(Connector *pConnector);
	
	// 删除管理器中的所有Connector
	void			RemoveAllConnector();

	// 取得Connector指针
	Connector*		GetConnector(ut_id_connector pid);

	// 检测连接是否在这个连接管理器中 [yangzhen]
	Connector*		FindConnectorInManager(ut_id_connector pid);

	// 通过Manager中的索引获取 [yangzhen]
	Connector*		GetConnectorByIndex(int32 nIndex);

	// 2010-9-25 by rj 以跨服方式取得连接
	Connector		*GetConnectorForGroupMode(ut_id_connector pid);

	// 取得当前管理器中Connector数量
	uint32			GetConnectorCount();

	// 向消息队列中添加消息
	virtual BOOL	SendMessage(IMessage *pKMessage, ut_id_connector ServerID, uint32 Flag = PF_NONE);
	
	// 处理消息队列中的消息
	BOOL			Handle_CacheCommands();
	
	// 从消息队列中取出消息
	BOOL			RecvMessage(IMessage * &pKMessage, ut_id_connector &PlayerID, uint32 &Flag);
	
	// 删除消息队列中的消息
	BOOL			MoveMessage(ut_id_connector PlayerID);

	// 扩展消息队列空间
	BOOL			ResizeCache();

	ut_id_connector*	GetConnectorIDs() { return m_aConnecorID; }

	//检查连接并重连服务器，由服务端连接线程执行 
	virtual BOOL		CheckAndReconnectToServer();

	//删除m_aConnecorID 中无效的连接 [xingzhishan]
	BOOL DelInvalidConnectorFromMgr(uint32 Index);

	// 获取这个ConnectorManager的名称，用于写日志 [yangzhen]
	virtual const char* GetManagerName();

protected:
	// 将Connector添加到管理器中
	virtual BOOL	AddConnectorToMgr(Connector *pConnector);
	
	// 从管理器中删除Connector
	virtual BOOL	DelConnectorFromMgr(Connector *pConnector);

	// 接受新的连接
	virtual Connector*	RecvNewConnector();


public:
	TID				m_ThreadID;

protected:
	BOOL			m_bServerMode;				// 管理器模式，TRUE为服务器模式，FALSE为客户端模式
	Acceptor*		m_pServerSocket;			// 服务器Acceptor指针
	SOCKET			m_SocketID;					// 服务器SocketID
	uint32			m_uFDSize;					// 实际的网络连接数量

	ut_id_connector*	m_aConnecorID;				// ConnectorID数组
	uint32			m_uMaxConnectorCount;		// Connector最大数量
	uint32			m_uConnectorCount;			// Connector当前数量
	ConnectorPool*	m_pConnectorPool;			// ConnectorPool指针

	FastLock		m_Lock;
	ASYNC_MSG	*m_MessageQue;				// 消息队列
	uint32			m_QueSize;					// 消息队列大小
	uint32			m_Head;						// 消息队列Head标记
	uint32			m_Tail;						// 消息队列Tail标记
};

#endif
