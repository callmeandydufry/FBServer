#include "stdafx.h"
#include "ClientSession.h"
#include "ClientGateManager.h"
#include "ServerModuleManager.h"
#include "ClientRPCMethodInterface.h"
#include "ClientConnector.h"
#include "CGRPChandler.h"
#include "GCRpcInvokeMsg.h"
#include "ShareStruct.h"

ClientSession::ClientSession()
	: mPlayerSnid(INVALID_ID)
	, mGateManager(NULL)
	, mCurrState(ESESS_INVALID)
	, mConnector(NULL)
	, mSessionID(INVALID_ID)
{
	mClientRPCInterface = new ClientRPCMethodProxy(this);
	mServerPRCInvoker = new CGRPCHandler(this);
}

ClientSession::~ClientSession()
{
	KSafeDelete(mClientRPCInterface);
	KSafeDelete(mServerPRCInvoker);
}


void ClientSession::init(ClientGateManager* mgr)
{
	mGateManager = mgr;
	
	mSessionID = (((uint64)mgr->getGroupID()) << 32) + mgr->allocSessionID();

	registerModule(mgr);

	mCurrState = ESESS_INIT;
	mRobotFlag = FALSE;
	mSessionPlayerSyncTimer.Clear();
	mSessionPlayerSyncTimer.BeginTimer(10000, GET_TIME().TickCount());
}

void ClientSession::uninit()
{
	unregisterModule();
	clear();
}

void ClientSession::clear()
{
	mGateManager = NULL;
	mAccount.clear();
	mToken.clear();
	mPlayerSnid = INVALID_ID;
	mCurrState = ESESS_INVALID;
	mConnector = NULL;
	mConnectReservedTimer.Clear();
}

BOOL ClientSession::isNeedDestroy()
{
	if (ESESS_INVALID == mCurrState)
		return TRUE;
	return FALSE;
}

void ClientSession::needDestroy()
{
	mCurrState = ESESS_INVALID;
}

void ClientSession::initAuth(const char* account, const char* token)
{
	mAccount = account;
	mToken = token;
	mCurrState = ESESS_NEED_AUTH;
}

int64 ClientSession::getComponentId()
{
	return mSessionID;
}

void ClientSession::tickComponent(uint32 uTick)
{
	__GUARD__
	// 如果没有绑定Connector说明网络连接已经断开，这里需要计时确定Session在无连接的情况下能保留多久 [12/23/2017 yz]
	if (mConnectReservedTimer.IsSetTimer() && !mRobotFlag)
	{
		if (mConnectReservedTimer.CountingTimer(uTick)) 
		{
			if (INVALID_ID != mPlayerSnid)
			{
				// 玩家角色已经上线了，不能直接删除会话，否则上线角色得不到下线通知，会一直驻留内存 [12/23/2017 yz]
				requestPlayerOffline();
				return;
			}
			else
			{
				// todo 连接一上来就被destroy了，玩家未注册前就被销毁了，此处需重新修改
				//needDestroy();
				return;
			}
		}
	}

	// 处理从ClientConnector发过来的消息 [12/25/2017 yz]
	mServerPRCInvoker->processPacket();
	// 将需要发送客户端的消息通过Connector进行发送 [12/25/2017 yz]
	if (mConnector)
	{
		GCRpcInvokeMsg rpcMsg;
		pullClientSendStream(rpcMsg.getData());
		if (rpcMsg.getData().getDataSize() > 0)
		{
			mConnector->SendMessage(&rpcMsg);
		}
	}

	switch (mCurrState)
	{
	case ESESS_INIT:
	{

	}
	break;
	case ESESS_NEED_AUTH:
	{
		requestPlatAuth();
	}
	break;
	case ESESS_WAIT_AUTH:
	{
		// FIXME: 测试代码，忽略三方平台登录验证 [12/23/2017 yz]
		rpcNotifyPlatAuthResult(TRUE);
	}
	break;
	case ESESS_REQUEST_ONLINE:
	{

	}
	break;
	case ESESS_ONLINE:
	{
		// 心跳监测 [1/17/2018 Chief]
		if (mSessionPlayerSyncTimer.CountingTimer(uTick))
		{
			checkOnlinePlayerState();
		}
	}
	break;
	case ESESS_OnFighting:
	{

	}
	break;
	case ESESS_REQUEST_OFFLINE:
	{
		
	}
	break;
	case ESESS_OFFLINE:
	{
		needDestroy();
		return;
	}
	break;
	}
	__UNGUARD__
}

void ClientSession::bindConnector(ClientConnector* connector)
{
	__GUARD__
	mConnector = connector;
	mConnectReservedTimer.Clear();
	__UNGUARD__
}

void ClientSession::unbindConnector()
{
	__GUARD__
	mConnector = NULL;
	mConnectReservedTimer.BeginTimer(g_Config.mConfigInfo.mSessionReservedTime, GET_TIME().GetCTime());
	__UNGUARD__
}

void ClientSession::sendPacketStream(DataStream& stream)
{
	__GUARD__
	mSendClientCache.serialize(stream.getData(), stream.getDataSize());
	__UNGUARD__
}

BOOL ClientSession::recvPacketStream(DataStream& stream)
{
	__GUARD__
	mRecvClientCache.swapData(stream);
	mRecvClientCache.clear();
	return TRUE;
	__UNGUARD__
	return FALSE;
}

void ClientSession::pushClientReceivedStream(DataStream& stream)
{
	__GUARD__
	mRecvClientCache.serialize(stream.getData(), stream.getDataSize());
	__UNGUARD__
}

void ClientSession::pullClientSendStream(DataStream& stream)
{
	__GUARD__
	mSendClientCache.swapData(stream);
	mSendClientCache.clear();
	__UNGUARD__
}

void ClientSession::rpcNotifyPlatAuthResult(BOOL success)
{
	__GUARD__
	if (mCurrState != ESESS_WAIT_AUTH)
	{
		// Session状态错误，立即删除 [12/23/2017 yz]
		needDestroy();
		return;
	}

	if (success)
	{
		requestPlayerOnline();
	}
	else
	{
		// 登录验证失败 [12/23/2017 yz]
		needDestroy();
	}
	__UNGUARD__
}

void ClientSession::rpcNotifyPlayerOffline(SNID_t snid)
{
	__GUARD__
		if (snid != mPlayerSnid)
		{
			// 出错了，snid与session的对应关系错误，删除会话 [12/23/2017 yz]
			needDestroy();
			return;
		}
	mCurrState = ESESS_OFFLINE;
	mPlayerSnid = INVALID_ID;
	__UNGUARD__
}


void ClientSession::requestPlatAuth()
{
	__GUARD__
	mCurrState = ESESS_WAIT_AUTH;
	__UNGUARD__
}

class __ClientSession_rpcRequestPlayerOnline_Callback : public ClientSession
{
public:
	void callbackSuccess(BOOL& result)
	{
		if (result)
		{
			// 这里还不能算作登录成功，真正的登录成功在 [12/23/2017 yz]
		}
		else
		{
			needDestroy();
		}
	}

	void callbackOvertime()
	{
		requestPlayerOnline();
	}
};

void ClientSession::requestPlayerOnline()
{
	__GUARD__

	// ClientGate与RegisterPlayerModule一一对应，所有Gate的Group对应的就是RegisterPlayerModule的Group [1/16/2018 yz]
	int32 registerPlayerGroup = ((ServerModuleBase*)mModule)->getGroupID();

	mCurrState = ESESS_REQUEST_ONLINE;
	if (mAccount.length() > 0)
	{
		g_pModuleManager->getExportCallable < IRegisterPlayerManagerExportCallable < __ClientSession_rpcRequestPlayerOnline_Callback > >(
			// todo ServerModuleUtil::getRegisterModuleByPlayerSnid(mPlayerSnid),
			ServerModuleUtil::getModuleNameByTypeAndGroup(EServerModuleType_RegisterPlayer, registerPlayerGroup),
			MODULE_DEFAULT_ID,
			(__ClientSession_rpcRequestPlayerOnline_Callback*)this
			)
			->rpcRequestPlayerOnline(mAccount.c_str(), mSessionID)
			->bindCallback(
				&__ClientSession_rpcRequestPlayerOnline_Callback::callbackSuccess,
				&__ClientSession_rpcRequestPlayerOnline_Callback::callbackOvertime,
				10000
			);
	}
	else
	{
		needDestroy();
	}
	__UNGUARD__
}

class __ClientSession_rpcRequestOffline_Callback : public ClientSession
{
public:
	void callbackSuccess(BOOL& result)
	{
		mCurrState = ESESS_OFFLINE;
	}

	void callbackOvertime()
	{
		// 失败了再次发起 [12/23/2017 yz]
		requestPlayerOffline();
	}
};

void ClientSession::requestPlayerOffline()
{
	__GUARD__
	mCurrState = ESESS_REQUEST_OFFLINE;
	if (mPlayerSnid > 0)
	{
		g_pModuleManager->getExportCallable < IOnlinePlayerManagerExportCallable < __ClientSession_rpcRequestOffline_Callback > > (
			ServerModuleUtil::getOnlineModuleByPlayerSnid(mPlayerSnid),
			MODULE_DEFAULT_ID,
			(__ClientSession_rpcRequestOffline_Callback*)this
			)
			->rpcRequestOffline(mPlayerSnid)
			->bindCallback(
				&__ClientSession_rpcRequestOffline_Callback::callbackSuccess,
				&__ClientSession_rpcRequestOffline_Callback::callbackOvertime,
				10000
			);

		int32 registerPlayerGroup = ((ServerModuleBase*)mModule)->getGroupID();
		g_pModuleManager->getExportCallable < IRegisterPlayerManagerExportCallable < ClientSession > >(
			ServerModuleUtil::getModuleNameByTypeAndGroup(EServerModuleType_RegisterPlayer, registerPlayerGroup),
			MODULE_DEFAULT_ID,
			this
			)
			->rpcUpdatePlayerOnlineState(mPlayerSnid, FALSE);
	}
	else
	{
		needDestroy();
	}
	__UNGUARD__
}

void ClientSession::setSessionState(int32 nState)
{
	__GUARD__;

	if (nState >= ESESS_INVALID || nState < ESESS_INIT)
		return;

	mCurrState = (ESessionState)nState;

	__UNGUARD__;
}

void ClientSession::rpcNotifyPlayerOnline(SNID_t snid)
{
	__GUARD__
		mPlayerSnid = snid;
	mCurrState = ESESS_ONLINE;
	mClientRPCInterface->loginSucess(mPlayerSnid);
	__UNGUARD__
}

//----------------------------------------------------------------------
//玩家登录后定时检测对应Onlineplayer状态是否一致 [1/17/2018 Chief]
//----------------------------------------------------------------------
class __ClientSession_rpcSessionCheckPlayerIsOnline_Callback : public ClientSession
{
public:
	void callbackSuccess(BOOL& result)
	{
		if (!result)
		{
			needDestroy();
		}	
	}

	void callbackOvertime()
	{
		needDestroy();
	}
};

void ClientSession::checkOnlinePlayerState()
{
	__GUARD__;
	
	if (INVALID_SNID == mPlayerSnid) return;

	g_pModuleManager->getExportCallable < IOnlinePlayerExportCallable < __ClientSession_rpcSessionCheckPlayerIsOnline_Callback > >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(mPlayerSnid),
		mPlayerSnid,
		(__ClientSession_rpcSessionCheckPlayerIsOnline_Callback*)this
		)
		->rpcSessionCheckPlayerIsOnline()
		->bindCallback(
			&__ClientSession_rpcSessionCheckPlayerIsOnline_Callback::callbackSuccess,
			&__ClientSession_rpcSessionCheckPlayerIsOnline_Callback::callbackOvertime,
			5000
		);

	__UNGUARD__;
}

//----------------------------------------------------------------------
// 发送消息给客户端 start [1/17/2018 Chief]
//----------------------------------------------------------------------
void ClientSession::rpcS2CSynPlayerExtData(DBPlayerState& stState)
{
	__GUARD__;

	SPlayerExtData stExtData;
	stExtData.clear();
	stExtData.setCharacterName(stState.getCharacterName());
	stExtData.setDiamond(stState.getDiamond());
	stExtData.setExp(stState.getExp());
	stExtData.setGold(stState.getGold());
	stExtData.setLevel(stState.getLevel());

	if (mClientRPCInterface)
	{
		mClientRPCInterface->SynPlayerExtData(stExtData);
	}

	__UNGUARD__
}

void ClientSession::rpcS2CSynPlayerChatData(const char* szMsg, int32 nChatID, int32 nChatChanel, SNID_t snid)
{
	__GUARD__;

	tagSendChatMsgData tagMsgData;
	tagMsgData.clear();
	tagMsgData.setChannel(nChatChanel);
	tagMsgData.setChatID(nChatID);
	if (EChatChannel_Private == nChatChanel)
	{
		tagMsgData.setSender(snid);
	}
	tagMsgData.setChatMsg(szMsg);

	if (mClientRPCInterface)
	{
		mClientRPCInterface->SynPlayerChatData(tagMsgData);
	}

	__UNGUARD__
}


//----------------------------------------------------------------------
// 发送消息给客户端 END [1/17/2018 Chief]
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// 机器人登录 [1/15/2018 Chief]
//----------------------------------------------------------------------
void ClientSession::rpcRobotRequestLogon()
{
	__GUARD__;

	if (mCurrState >= ESESS_INVALID || mCurrState < ESESS_INIT)
		return;

	requestPlayerOnline();

	__UNGUARD__;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
BOOL ClientSession::rpcPlayerCheckSessionIsOnline()
{
	__GUARD__;

	return (ESESS_ONLINE == mCurrState);

	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
//邮件list简易数据
//----------------------------------------------------------------------
void ClientSession::rpcMgrSynMailList2Session(tagSendMailListInfo& stSendList)
{
	__GUARD__;

	if (mClientRPCInterface)
	{
		// todo 增加有效邮件个数的参数
		mClientRPCInterface->SynTypeMailList(MAX_S2CSYNMAIL_NUMS, stSendList);
	}

	__UNGUARD__;
}

//----------------------------------------------------------------------
//邮件详情
//----------------------------------------------------------------------
void ClientSession::rpcMgrSynMailDetail2Session(tagSendMailDetail& stSendMainDetail, tagSendMailAttachMent& stAttach)
{
	__GUARD__;

	if (mClientRPCInterface)
	{
		mClientRPCInterface->SynMailDetialData(stSendMainDetail, stAttach);
	}

	__UNGUARD__;
}

//----------------------------------------------------------------------
// CALLBACK [2/9/2018 Chief]
//----------------------------------------------------------------------
// rpcRequestUnreadMailNums
void ClientSession::onRPCRequestUnreadMailNumsCallbackSucess(int32& nNum)
{
	__GUARD__;

	if (mClientRPCInterface)
	{
		mClientRPCInterface->SynNewMailNums(nNum);
	}

	__UNGUARD__;
}
