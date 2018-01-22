#ifndef __CLIENT_SESSION_H__
#define __CLIENT_SESSION_H__

#include "ServerModuleExportInterface.h"
#include "ClientServerRPCSupport.h"

class ClientGateManager;
class ClientConnector;
class ClientRPCMethodInterface;
class ServerRPCMethodHandler;

enum ESessionState
{
	ESESS_INIT,
	ESESS_NEED_AUTH,
	ESESS_WAIT_AUTH,
	ESESS_REQUEST_ONLINE,
	ESESS_ONLINE,
	ESESS_REQUEST_OFFLINE,
	ESESS_OFFLINE,
	ESESS_INVALID,
};

class ClientSession
	: public IClientSession
	, public RPCPacketDataSender
	, public RPCPacketDataReceiver
{
public:
	ClientSession();
	~ClientSession();

	void							init(ClientGateManager* mgr);
	void							uninit();
	void							clear();
	BOOL							isNeedDestroy();
	void							needDestroy();

	void							initAuth(const char* account, const char* token);

	virtual int64					getComponentId();
	int64							getSessionID() const { return mSessionID; }
	const char*						getAccount() const { return mAccount.c_str(); }
	const char*						getToken() const { return mToken.c_str(); }
	int32							getSessionState() const { return mCurrState; }
	SNID_t							getPlayerSnid() const { return mPlayerSnid; }

	virtual void					tickComponent(uint32 uTick);

	void							bindConnector(ClientConnector* connector);
	void							unbindConnector();
	ClientConnector*				getConnector() const { return mConnector; }

	virtual void					sendPacketStream(DataStream& stream);
	virtual BOOL					recvPacketStream(DataStream& stream);

	void							pushClientReceivedStream(DataStream& stream);
	void							pullClientSendStream(DataStream& stream);

	//----------------------------------------------------------------------
	//RPC调用接口 START
	//----------------------------------------------------------------------
	virtual void					rpcNotifyPlatAuthResult(BOOL success);

	// OnlinePlayer上线成功，OnlinePlayer已经创建后通知Session与OnlinePlayer建立关联 [12/23/2017 yz]
	virtual void					rpcNotifyPlayerOnline(SNID_t snid);

	// OnlinePlayer下线成功之后通知Session释放 [12/23/2017 yz]
	virtual void					rpcNotifyPlayerOffline(SNID_t snid);

	// 同步客户端请求的玩家数据 [1/9/2018 Chief]
	virtual void					rpcS2CSynPlayerExtData(DBPlayerState& stState);

	// 同步客户端聊天消息 [1/15/2018 Chief]
	virtual void					rpcS2CSynPlayerChatData(const char* szMsg, int32 nChatID, int32 nChatChanel, SNID_t snid);

	// 机器人登录 [1/15/2018 Chief]
	virtual void					rpcRobotRequestLogon();

	// onlineplayer/session心跳监测[1/17/2018 Chief]
	virtual BOOL					rpcPlayerCheckSessionIsOnline();
	//----------------------------------------------------------------------
	//RPC调用接口 END
	//----------------------------------------------------------------------

public:
	// 更改当前session状态 [1/17/2018 Chief]
	void							setSessionState(int32 nState);			
	BOOL							getRobotFlag()			const { return mRobotFlag; }
	void							setRobotFlag(BOOL bFlag) { mRobotFlag = bFlag; }
	ClientGateManager*				getClientGateManager()	const { return mGateManager; }
	void							requestPlayerOnline();
	// 玩家登录后定时检测对应Onlineplayer状态是否一致 [1/17/2018 Chief]
	void							checkOnlinePlayerState();
protected:
	// 发起平台登录验证 [12/23/2017 yz]
	void							requestPlatAuth();
	void							requestPlayerOffline();
protected:
	FixedString<MAX_ACCOUNT>		mAccount;
	FixedString<MAX_TOKEN_LEN>		mToken;
	SNID_t							mPlayerSnid;
	ClientGateManager*				mGateManager;
	ESessionState					mCurrState;
	int64							mSessionID;
	ClientConnector*				mConnector;

	// 无连接时会话保留多久的计时器 [12/23/2017 yz]
	CTinyTimer						mConnectReservedTimer;

	// 使用此接口发起对客户端的远程调用 [12/25/2017 yz]
	ClientRPCMethodInterface*		mClientRPCInterface;

	// 使用此对象处理来自客户端的远程调用 [12/25/2017 yz]
	ServerRPCMethodHandler*			mServerPRCInvoker;

	// 机器人标记 [1/17/2018 Chief]
	BOOL							mRobotFlag;

	// session/onlineplayer心跳检测 [1/17/2018 Chief]
	CTinyTimer						mSessionPlayerSyncTimer;

	DataStream						mSendClientCache;
	DataStream						mRecvClientCache;
};

#endif