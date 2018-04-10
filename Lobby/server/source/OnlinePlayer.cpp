#include "stdafx.h"
#include "OnlinePlayer.h"
#include "OnlinePlayerManager.h"
#include "ModuleManager.h"
#include "GameMailStruct.h"

OnlinePlayer::OnlinePlayer()
{
	mSnid = INVALID_ID;
	mOnlineState = EOnlineState_Uninited;
	mSessionID = INVALID_ID;
}

OnlinePlayer::~OnlinePlayer()
{

}

void OnlinePlayer::init(OnlinePlayerManager* mgr, const char* account, SNID_t snid)
{
	__GUARD__

		mSnid = snid;
	mPlayerArchive.mDBState.setSnid(snid);
	mPlayerArchive.mDBState.setAccountName(account);
	mOnlineState = EOnlineState_Created;

	registerModule(mgr);

	mPlayerSessionSyncTimer.Clear();
	mPlayerSessionSyncTimer.BeginTimer(10000, GET_TIME().TickCount());

	__UNGUARD__
}

void OnlinePlayer::uninit()
{
	__GUARD__
	unregisterModule();
	clear();
	mOnlineState = EOnlineState_Uninited;
	__UNGUARD__
}

void OnlinePlayer::clear()
{
	__GUARD__
	mSnid = INVALID_ID;
	mSessionID = INVALID_ID;
	mPlayerArchive.clear();
	__UNGUARD__
}

void OnlinePlayer::bindSessionID(int64 sessionID)
{
	__GUARD__
		mSessionID = sessionID;
	__UNGUARD__
}

void OnlinePlayer::unbindSession()
{
	__GUARD__
		mSessionID = INVALID_ID;
	__UNGUARD__
}

int64 OnlinePlayer::getComponentId()
{
	return mSnid;
}

void OnlinePlayer::tickComponent(uint32 uTick)
{
	__GUARD__

	switch (mOnlineState)
	{
	case EOnlineState_Created:
		break;

	case EOnlineState_WaitDBData:
		break;

	case EOnlineState_DBInited:
		{
			// 拿到数据之后就可以上线 [12/16/2017 yz]
			enterOnlineState();
		}
		break;

	case EOnlineState_EnterOnline:
		{
			// 已经准备上线了，卡到这个状态说明到ReigsterPlayer的远程调用失败了，再尝试一次 [12/16/2017 yz]
		}
		break;

	case EOnlineState_ConnectSession:
		{

		}
		break;

	case EOnlineState_Online:
		{
			// 定时心跳检测 [2/8/2018 Chief]
			if (mPlayerSessionSyncTimer.CountingTimer(uTick))
			{
				checkClientSessionState();

				// 定期请求邮件list [2/8/2018 Chief]
				trySynMailList();
			}
			
		}
		break;

	case EOnlineState_RequestLeaveOnline:
		// todo 跳过存储直接清空
		mOnlineState = EOnlineState_Invalid;
		break;

	case EOnlineState_SavingBeforeLeave:
		break;

	case EOnlineState_SavedAndOffline:
		break;

	case EOnlineState_Invalid:
		break;
	}

	__UNGUARD__
}

OnlinePlayerManager* OnlinePlayer::getPlayerManager()
{ 
	return static_cast<OnlinePlayerManager*>(mModule); 
}

class __OnlinePlayer_rpcFetchOnlinePlayerArchive_Callback : public OnlinePlayer
{
public:
	void callbackSuccess(PlayerArchive& dbArchive)
	{
		receiveDBData(&dbArchive);

		getPlayerManager()->setPlayerOnline(this, dbArchive.mDBState.getSnid(), mSessionID);
	}

	void callbackOvertime()
	{
		mOnlineState = EOnlineState_Invalid;
	}
};

void OnlinePlayer::requestDBData()
{
	__GUARD__

	mOnlineState = EOnlineState_WaitDBData;

	SNID_t snid = mPlayerArchive.mDBState.getSnid();
	g_pModuleManager->getExportCallable< IDBProxyManagerExportCallable<__OnlinePlayer_rpcFetchOnlinePlayerArchive_Callback> >(
		ServerModuleUtil::getDBProxyModuleByPlayerSnid(snid),
		MODULE_DEFAULT_ID,
		(__OnlinePlayer_rpcFetchOnlinePlayerArchive_Callback*)this
		)
		->rpcFetchOnlinePlayerArchive(snid)
		->bindCallback(
			&__OnlinePlayer_rpcFetchOnlinePlayerArchive_Callback::callbackSuccess,
			&__OnlinePlayer_rpcFetchOnlinePlayerArchive_Callback::callbackOvertime,
			10000
		);

	__UNGUARD__
}

// 同步邮件,用来确定玩家拥有多少新的邮件 [2/8/2018 Chief]
void OnlinePlayer::trySynMailList()
{
	__GUARD__;

	g_pModuleManager->getExportCallable< IMailManagerExportCallable<OnlinePlayer> >(
		ServerModuleUtil::getMailModuleByPlayerSnid(mSnid),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcSynMailList(mSnid);

	__UNGUARD__
}

void OnlinePlayer::receiveDBData(PlayerArchive* dbArchive)
{
	__GUARD__
	mPlayerArchive = *dbArchive;

	mOnlineState = EOnlineState_DBInited;
	__UNGUARD__
}

class __OnlinePlayer_rpcUpdatePlayerOnlineState_Callback : public OnlinePlayer
{
public:
	void callbackSuccess(BOOL& success)
	{
		connectClientSession();
	}

	void callbackOvertime()
	{

	}
};

void OnlinePlayer::enterOnlineState()
{
	__GUARD__
	mOnlineState = EOnlineState_EnterOnline;

	// 通知玩家注册模块，玩家已经正式上线了 [12/16/2017 yz]
	SNID_t snid = mPlayerArchive.mDBState.getSnid();
	g_pModuleManager->getExportCallable< IRegisterPlayerManagerExportCallable<__OnlinePlayer_rpcUpdatePlayerOnlineState_Callback> >(
		//ServerModuleUtil::getRegisterModuleByPlayerSnid(snid),
		ServerModuleUtil::getModuleNameByTypeAndGroup(EServerModuleType_RegisterPlayer, 0),
		MODULE_DEFAULT_ID,
		(__OnlinePlayer_rpcUpdatePlayerOnlineState_Callback*)this
		)
		->rpcUpdatePlayerOnlineState(snid, TRUE)
		->bindCallback(
			&__OnlinePlayer_rpcUpdatePlayerOnlineState_Callback::callbackSuccess,
			&__OnlinePlayer_rpcUpdatePlayerOnlineState_Callback::callbackOvertime,
			2000
		);

	// test code 玩家上线后创建文本邮件 [2/22/2018 Chief]
	DBBaseMailState stBaseMail;
	stBaseMail.clear();
	stBaseMail.setMailType(EMT_NOTICE);
	uint32 nTimer = GET_TIME().GetCTime();
	stBaseMail.setCreateTime(nTimer);
	stBaseMail.setCreateTime(nTimer + 3600 * 24 * 1);

	FixedString<DB_TITLE_LEN> title("Player online");

	FixedString<DB_CONTENT_LEN> content;
	String strCon;
	strCon = StringUtil::printf("player '%s' is online now", mPlayerArchive.mDBState.getAccountName().c_str());
	content = strCon;

	g_pModuleManager->getExportCallable< IMailManagerExportCallable<OnlinePlayer> >(
		ServerModuleUtil::getMailModuleByPlayerSnid(snid),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcSendContentMail(stBaseMail, title, content);

	__UNGUARD__
}

void OnlinePlayer::connectClientSession()
{
	__GUARD__
		mOnlineState = EOnlineState_ConnectSession;
	if (INVALID_ID != mSessionID)
	{
		// 通知Session对象玩家角色已经完成了上线的整个流程 [12/23/2017 yz]
		// 通知玩家注册模块，玩家已经正式上线了 [12/16/2017 yz]
		SNID_t snid = mPlayerArchive.mDBState.getSnid();
		g_pModuleManager->getExportCallable< IClientSessionExportCallable<OnlinePlayer> >(
			ServerModuleUtil::getSessionModuleBySessionID(mSessionID),
			mSessionID,
			this
			)
			->rpcNotifyPlayerOnline(snid);

		mOnlineState = EOnlineState_Online;
	}
	else
	{
		mOnlineState = EOnlineState_Invalid;
	}
	__UNGUARD__
}

void OnlinePlayer::finishOnlineState()
{
	__GUARD__
	mOnlineState = EOnlineState_Online;
	__UNGUARD__
}

void OnlinePlayer::leaveOnlineState()
{
	__GUARD__
	mOnlineState = EOnlineState_RequestLeaveOnline;
	__UNGUARD__
}

void OnlinePlayer::dealInvalidState()
{
	__GUARD__
		getPlayerManager()->removeInvalidPlayer(this);
	__UNGUARD__
}

DBPlayerState* OnlinePlayer::getPlayerState()
{
	return &mPlayerArchive.mDBState;
}

SNID_t OnlinePlayer::getSnid()
{
	return mPlayerArchive.mDBState.getSnid();
}

const char* OnlinePlayer::getAccount()
{
	return mPlayerArchive.mDBState.getAccountName().c_str();
}

//----------------------------------------------------------------------
// rpc START [1/8/2018 Chief]
//----------------------------------------------------------------------
void OnlinePlayer::rpcRequestPlayerDetialData(uint64 nSessionID)
{
	if (mSessionID != nSessionID)
	{
		KCheck(0 && "rpcRequestPlayerDetialData ERROR!: SessionID ERROR!");
		bindSessionID(nSessionID);
	}
	requestPlayerDetialData();
}

BOOL OnlinePlayer::rpcSessionCheckPlayerIsOnline()
{
	return (EOnlineState_Online == mOnlineState);
}

//----------------------------------------------------------------------
// clientsession检测 [1/8/2018 Chief]
//----------------------------------------------------------------------
class __OnlinePlayer_rpcPlayerCheckSessionIsOnline_Callback : public OnlinePlayer
{
public:
	void callbackSuccess(BOOL& result)
	{
		if (!result)
		{
			leaveOnlineState();
		}
	}

	void callbackOvertime()
	{
		leaveOnlineState();
	}
};

void OnlinePlayer::checkClientSessionState()
{
	__GUARD__;

	if (INVALID_ID == mSessionID) return;

	g_pModuleManager->getExportCallable < IClientSessionExportCallable < __OnlinePlayer_rpcPlayerCheckSessionIsOnline_Callback > >(
		ServerModuleUtil::getSessionModuleBySessionID(mSessionID),
		mSessionID,
		(__OnlinePlayer_rpcPlayerCheckSessionIsOnline_Callback*)this
		)
		->rpcPlayerCheckSessionIsOnline()
		->bindCallback(
			&__OnlinePlayer_rpcPlayerCheckSessionIsOnline_Callback::callbackSuccess,
			&__OnlinePlayer_rpcPlayerCheckSessionIsOnline_Callback::callbackOvertime,
			5000
		);

	__UNGUARD__;
}

//----------------------------------------------------------------------
// client 请求数据 [1/8/2018 Chief]
//----------------------------------------------------------------------
void OnlinePlayer::requestPlayerDetialData()
{
	SNID_t snid = mPlayerArchive.mDBState.getSnid();
	g_pModuleManager->getExportCallable< IClientSessionExportCallable<OnlinePlayer> >(
		ServerModuleUtil::getSessionModuleBySessionID(mSessionID),
		mSessionID,
		this
		)
		->rpcS2CSynPlayerExtData(mPlayerArchive.mDBState);

}

//----------------------------------------------------------------------
// 邮件 [1/29/2018 Chief]
//----------------------------------------------------------------------
void OnlinePlayer::rpcRequestPlayerMailList()
{
	__GUARD__;

	// 将邮件状态和list发送给mgr [2/9/2018 Chief]
	String strMailListInfo;
	mPlayerMailArr.serialize(strMailListInfo);

	tagMailConfirm stMaillistInfo;
	stMaillistInfo.clear();
	stMaillistInfo.setMailIDList(strMailListInfo);

	// rpcsend(stMaillistInfo, mSessionID)
	g_pModuleManager->getExportCallable< IMailManagerExportCallable<OnlinePlayer> >(
		ServerModuleUtil::getMailModuleByPlayerSnid(mSnid),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcRequestAllPlayerMailsData(mSnid, stMaillistInfo, mSessionID);

	__UNGUARD__;
}

//----------------------------------------------------------------------
// 同步maillist信息 [1/29/2018 Chief]
//----------------------------------------------------------------------
void OnlinePlayer::rpcMgrSynMailList2Player(tagMailConfirm& stListInfo)
{
	__GUARD__;

	tagMailIDList stMailList;
	stMailList.Clear();
	stMailList.unserialize(stListInfo.getMailIDList().c_str());

	int32 nSize = stMailList.getValidMailNums();
	mNewMailsNum = 0;
	// 只关心未读邮件 [2/9/2018 Chief]
	for (int32 i = 0; i < nSize; ++i)
	{
		BYTE bOldStatus = 0;
		int32 nFindIndex = mPlayerMailArr.findMailByID(stMailList.mMailData[i].mMailID);
		if (nFindIndex)
		{
			bOldStatus = mPlayerMailArr.mMailData[nFindIndex].mMailStatus;
			if (bOldStatus) mNewMailsNum++;
		}
		else
		{
			mNewMailsNum++;
		}

		stMailList.mMailData[i].mMailStatus = bOldStatus;
	}

	// 同步邮件list [2/9/2018 Chief]
	mPlayerMailArr = stMailList;

	// todo 存储到DB [2/9/2018 Chief]
	

	__UNGUARD__;
}

//----------------------------------------------------------------------
// 阅读邮件 [2/9/2018 Chief]
//----------------------------------------------------------------------
void OnlinePlayer::rpcRequestReadMail(int32 nMailID)
{
	__GUARD__;

	int32 nPos = mPlayerMailArr.findMailByID(nMailID);
	if (nPos)
	{
		mPlayerMailArr.mMailData[nPos].setMailStatus(EMS_Read);
	}
	// todo save to db [2/9/2018 Chief]

	__UNGUARD__;
}

//----------------------------------------------------------------------
// 删除邮件 [2/9/2018 Chief]
//----------------------------------------------------------------------
void OnlinePlayer::rpcRequestDelMail(int32 nMailID)
{
	__GUARD__;

	int32 nPos = mPlayerMailArr.findMailByID(nMailID);
	if (nPos)
	{
		mPlayerMailArr.mMailData[nPos].setMailStatus(EMS_DEL);
	}
	// todo save to db [2/9/2018 Chief]

	__UNGUARD__;
}
