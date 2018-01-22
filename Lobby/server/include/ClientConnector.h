#ifndef __CLIENT_CONNECTOR_H__
#define __CLIENT_CONNECTOR_H__

#include "Connector.h"
#include "ConnectorPool.h"

class ClientGateManager;
class ClientSession;

class ClientConnector : public Connector
{
public:
	ClientConnector();
	virtual ~ClientConnector();

	virtual void	InitSocket();
	virtual void	Clear();
	virtual void	ResetKick();

	virtual BOOL	Handle_Input();
	virtual BOOL	Handle_Output();
	virtual BOOL	Handle_Command();

	virtual BOOL	SendMessage(IMessage *pKMessage);

	virtual BOOL	IsGameConnector() { return TRUE; }
	virtual BOOL	IsServerConnector() { return FALSE; }


public:
	BOOL			HeartBeat(uint32 uTime = 0, int32 nFlag = 0);

	void			Init();
	BOOL			FreeMySelf();

	BOOL			SendMessageDirectly(IMessage *pKMessage);
	BOOL			TestMessageIndex(uchar bIndex);
	BOOL			IsLegalMessageID(const MSG_ID packetID);
	BOOL			FilterMessage(IMessage* pKMessage);


	SNID_t			GetPlayerSNID() const { return m_PlayerSNID; }
	void			SetPlayerSNID(SNID_t snid) { m_PlayerSNID = snid; }

	void			Disconnect();

	int32			GetErrorCode() const { return m_ErrorCode; }

	BOOL			IsWaitingDisconnect() const { return m_bWaitingDisconnect; }
	void			SetWaitingDisconnect(BOOL b) { m_bWaitingDisconnect = b; }

	int32			GetLeftTimeToQuit() const { return mLeftTimeToQuit; }
	void			SetLeftTimeToQuit(int32 val) { mLeftTimeToQuit = val; }

	void			SetDebugMode(BOOL benable) { m_bDebugMode = benable; }
	BOOL			GetDebugMode() { return m_bDebugMode; }

	void			EnableCompress(BOOL bEnable, TID threadID = 0);

	void			SetDefaultEncrptorKey();
	void			SetEncrptorKey(char* pKey);

	CmStream*		GetSendCmStream();

	uint32			GetTotalSendSize() { return m_uTotalSendSize; }
	uint32			GetTotalReciSize() { return m_uTotalReciSize; }


	// 有时候当发生逻辑错误的时候需要先给客户端会给消息包再删除连接，否则消息不能达到客户端 [yangzhen]
	void			EnterDelayRemove(uint32 uDelayTime = 100);
	void			CancelDelayRemove();
	BOOL			CkeckDelayRemove(uint32 uTime);

	// 登陆时记录客户端的版本号 [xingzhishan]
	void			SetClientVersion(uint32 uVersion) { m_uClientVersion = uVersion; }
	uint32			GetClientVersion() const { return m_uClientVersion; }

	void			SetClientGateManager(ClientGateManager* gateMgr) { mGateManager = gateMgr; }
	ClientGateManager* GetClientGateManager() const { return mGateManager; }

	void			bindSession(ClientSession* session);
	void			unbindSession();
	ClientSession*	getSession() const { return mSession; }

	BOOL			enableEncryptKey();
public:

protected:
	SNID_t			m_PlayerSNID;

	BOOL			m_FirstIndexCheck;
	uchar			m_ClientMessageIndex;
	uint32			m_PerfMessageIndex;
	int32			m_ErrorCode;
	int32			m_PreMessageID;
	int32			m_PreWriteID;
	int32			m_OldOutPutStreamSize;

	uint32			m_NowTime;
	uint32			m_KickTime;
	uint32			m_LastSendTime;
	uint32			m_CurrentTime;
	int32			mLeftTimeToQuit;
	int32			m_SaveTime;

	BOOL			m_bWaitingDisconnect;

	BOOL			m_bDebugMode;
	uint32			m_uTotalSendSize;
	uint32			m_uTotalReciSize;

	// 延迟删除连接计时器 [yangzhen]
	CTinyTimer		m_DelayRemoveTimer;

	//客户端版本号 [xingzhishan]
	uint32			m_uClientVersion;

	ClientGateManager* mGateManager;
	ClientSession* mSession;
};

#endif
