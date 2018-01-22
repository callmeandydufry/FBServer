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
	//RPC���ýӿ� START
	//----------------------------------------------------------------------
	virtual void					rpcNotifyPlatAuthResult(BOOL success);

	// OnlinePlayer���߳ɹ���OnlinePlayer�Ѿ�������֪ͨSession��OnlinePlayer�������� [12/23/2017 yz]
	virtual void					rpcNotifyPlayerOnline(SNID_t snid);

	// OnlinePlayer���߳ɹ�֮��֪ͨSession�ͷ� [12/23/2017 yz]
	virtual void					rpcNotifyPlayerOffline(SNID_t snid);

	// ͬ���ͻ��������������� [1/9/2018 Chief]
	virtual void					rpcS2CSynPlayerExtData(DBPlayerState& stState);

	// ͬ���ͻ���������Ϣ [1/15/2018 Chief]
	virtual void					rpcS2CSynPlayerChatData(const char* szMsg, int32 nChatID, int32 nChatChanel, SNID_t snid);

	// �����˵�¼ [1/15/2018 Chief]
	virtual void					rpcRobotRequestLogon();

	// onlineplayer/session�������[1/17/2018 Chief]
	virtual BOOL					rpcPlayerCheckSessionIsOnline();
	//----------------------------------------------------------------------
	//RPC���ýӿ� END
	//----------------------------------------------------------------------

public:
	// ���ĵ�ǰsession״̬ [1/17/2018 Chief]
	void							setSessionState(int32 nState);			
	BOOL							getRobotFlag()			const { return mRobotFlag; }
	void							setRobotFlag(BOOL bFlag) { mRobotFlag = bFlag; }
	ClientGateManager*				getClientGateManager()	const { return mGateManager; }
	void							requestPlayerOnline();
	// ��ҵ�¼��ʱ����ӦOnlineplayer״̬�Ƿ�һ�� [1/17/2018 Chief]
	void							checkOnlinePlayerState();
protected:
	// ����ƽ̨��¼��֤ [12/23/2017 yz]
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

	// ������ʱ�Ự������õļ�ʱ�� [12/23/2017 yz]
	CTinyTimer						mConnectReservedTimer;

	// ʹ�ô˽ӿڷ���Կͻ��˵�Զ�̵��� [12/25/2017 yz]
	ClientRPCMethodInterface*		mClientRPCInterface;

	// ʹ�ô˶��������Կͻ��˵�Զ�̵��� [12/25/2017 yz]
	ServerRPCMethodHandler*			mServerPRCInvoker;

	// �����˱�� [1/17/2018 Chief]
	BOOL							mRobotFlag;

	// session/onlineplayer������� [1/17/2018 Chief]
	CTinyTimer						mSessionPlayerSyncTimer;

	DataStream						mSendClientCache;
	DataStream						mRecvClientCache;
};

#endif