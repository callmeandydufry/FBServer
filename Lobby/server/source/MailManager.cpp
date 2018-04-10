#include "stdafx.h"
#include "MailManager.h"
#include "FixedString.h"

MailManager::MailManager()
{

}

MailManager::~MailManager()
{

}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

BOOL MailManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	ServerModuleBase::initServerModule(moduleName, this, groupID);

	mCommonMailIDMap.InitTable(MAX_MAIL_POOL_NUMS);		// 4M 1000000
	mAttachmentMailIDMap.InitTable(MAX_MAIL_POOL_NUMS); // 4M 1000000
	mContentMailIDMap.InitTable(MAX_MAIL_POOL_NUMS);	// 4M 1000000
	mSystemMailIDMap.InitTable(MAX_MAIL_POOL_NUMS);		// 4M 1000000

	mCommonMailPool.initPool(MAX_MAIL_POOL_NUMS);
	mAttachmentMailPool.initPool(MAX_MAIL_POOL_NUMS);
	mContentMailPool.initPool(MAX_MAIL_POOL_NUMS);
	mSystemMailPool.initPool(MAX_MAIL_POOL_NUMS);

	mMaxMailID = 1;

	return TRUE;
}

void MailManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void MailManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);

	if (!mIsAllMailInit)
	{
		fetchMaxMailID();
		fetchAllMailNumsFromDB();
	}

	// 清除过期邮件 [2/7/2018 Chief]
	TDynaArray<uint64> destroyList;
	TIntegerHashMap<DBBaseMailState*>::IDTableIterator it = mCommonMailIDMap.Begin();
	while (it != mCommonMailIDMap.End())
	{
		if (it->second->getOverTime() < uTick)
		{
			destroyList.AddItem(it->second->getMailID());
		}

		it++;
	}

	for (int32 i = 0; i < destroyList.GetSize(); i++)
	{
		handleBaseMail(NULL, EDBO_Delete, destroyList[i]);
		handleAttachmentMail(NULL, EDBO_Delete, destroyList[i]);
		handleContentMail(NULL, EDBO_Delete, destroyList[i]);
		handleSystemMail(NULL, EDBO_Delete, destroyList[i]);

		// todo 通知mgr对应的Reg模块,清空所有在线邮件 [2/22/2018 Chief]
	}
}

void MailManager::onRPCFetchGlobalValCallbackSucess(int32& nID)
{
	mMaxMailID = nID;
	// ID复用[2/8/2018 Chief]
	if (mMaxMailID >= MAX_MAIL_USE_ID)
	{
		mMaxMailID = 1;
		saveMaxMailID(mMaxMailID);
	}
}

void MailManager::fetchMaxMailID()
{
	int32 nGroup = getGroupID();
	g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
		ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcFetchGlobalVal(nGroup, (int32)EPF_MailID)
		->bindCallback(
			&MailManager::onRPCFetchGlobalValCallbackSucess,
			&MailManager::onRPCFetchGlobalValCallbackOverTime,
			10000
		);
}

void MailManager::saveMaxMailID(int32 nID)
{
	int32 nGroup = getGroupID();
	EProFlag eType = EPF_MailID;
	g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
		ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcSaveGlobalVal(nGroup, (int32)EPF_MailID, nID);
}

int32 MailManager::getCurFreeMailID()
{
	if (++mMaxMailID >= MAX_MAIL_USE_ID)
	{
		mMaxMailID = 1;
		saveMaxMailID(mMaxMailID);
	}
	return mMaxMailID;
}

//----------------------------------------------------------------------
// 通过groupid获取邮件数量 [2/5/2018 Chief]
//----------------------------------------------------------------------
void MailManager::onRPCFetchAllMailNumsCallBackSuccess(tagMailsNum& nNums)
{
	// todo 改为获取所有表中的邮件个数
	mAllMailNums = nNums;

	fetchBaseMailsInfoFromDB();
	fetchAttachmentMailsInfoFromDB();
	fetchContentMailsInfoFromDB();
	fetchSystemMailsInfoFromDB();
}

void MailManager::onRPCFetchAllMailNumsCallBackOvertime()
{

}

void MailManager::fetchAllMailNumsFromDB()
{
	__GUARD__;
	int32 nGropuID = getGroupID();
	g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
		ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcRequestAllMailsNum(nGropuID)
		->bindCallback(
			&MailManager::onRPCFetchAllMailNumsCallBackSuccess,
			&MailManager::onRPCFetchAllMailNumsCallBackOvertime,
			10000
		);
	return ;
	__UNGUARD__;
	return ;
}

//----------------------------------------------------------------------
// 分批次获取邮件的数据 [2/5/2018 Chief]
//----------------------------------------------------------------------
void MailManager::onRPCFetchBaseMailDataCallBackSuccess(BatchBaseMailArchive& stMailData)
{
	for (int32 i = 0; i < stMailData.mBatchNum; ++i)
	{
		DBBaseMailState* pBaseMail = mCommonMailPool.allocObject();
		if (NULL == pBaseMail)
		{
			QLogSystem::QUICK_LOG(QUICK_LOG_MAILMGR, "MailManager onRPCFetchAttachmentMailDataCallBackSuccess error : the mCommonMailPool is not enough!");
			KCheck(0);
			return;
		}

		int32 nMailID = stMailData.mBatchBaseMailData[i].mDBState.getMailID();
		if (nMailID > mMaxMailID) mMaxMailID = nMailID;

		mCommonMailIDMap.Add(nMailID, pBaseMail);
	}
}

void MailManager::onRPCFetchBaseMailDataCallBackOvertime()
{

}

void MailManager::fetchBaseMailsInfoFromDB()
{
	__GUARD__;
	int32 nStartID = 0;
	int32 nSelectNum = BATCH_BaseMail_NUM;
	for (int32 i = 0; i <= mAllMailNums.getBaseMailNum() / BATCH_BaseMail_NUM; ++i)
	{
		nStartID = i * BATCH_BaseMail_NUM;
		if (i == mAllMailNums.getBaseMailNum() / BATCH_BaseMail_NUM)
		{
			nSelectNum = mAllMailNums.getBaseMailNum() % BATCH_BaseMail_NUM;
		}
		g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
			ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
			MODULE_DEFAULT_ID,
			this
			)
			->rpcFetchBaseMailData(nStartID + 1, nSelectNum, getGroupID())
			->bindCallback(
				&MailManager::onRPCFetchBaseMailDataCallBackSuccess,
				&MailManager::onRPCFetchBaseMailDataCallBackOvertime,
				10000
			);
	}
	return;
	__UNGUARD__;
	return;
}

//----------------------------------------------------------------------
// 分批次获取邮件的数据 [2/5/2018 Chief]
//----------------------------------------------------------------------
void MailManager::onRPCFetchAttachmentMailDataCallBackSuccess(BatchAttachmentMailArchive& stMailData)
{
	for (int32 i = 0; i < stMailData.mBatchNum; ++i)
	{
		DBBaseMailState* pBaseMail = mCommonMailPool.allocObject();
		if (NULL == pBaseMail)
		{
			return ;
		}

		mCommonMailIDMap.Add(stMailData.mBatchAttachmentMailData[i].mDBState.getMailID(), pBaseMail);
	}

}

void MailManager::onRPCFetchAttachmentMailDataCallBackOvertime()
{

}

void MailManager::fetchAttachmentMailsInfoFromDB()
{
	__GUARD__;
	int32 nStartID = 0;
	int32 nSelectNum = BATCH_AttachmentMail_NUM;
	for (int32 i = 0; i <= mAllMailNums.getAttachmentMailNum() / BATCH_AttachmentMail_NUM; ++i)
	{
		nStartID = i * BATCH_AttachmentMail_NUM;
		if (i == mAllMailNums.getBaseMailNum() / BATCH_AttachmentMail_NUM)
		{
			nSelectNum = mAllMailNums.getBaseMailNum() % BATCH_AttachmentMail_NUM;
		}
		g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
			ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
			MODULE_DEFAULT_ID,
			this
			)
			->rpcFetchAttachmentMailData(nStartID + 1, nSelectNum, getGroupID())
			->bindCallback(
				&MailManager::onRPCFetchAttachmentMailDataCallBackSuccess,
				&MailManager::onRPCFetchAttachmentMailDataCallBackOvertime,
				10000
			);
	}
	return;
	__UNGUARD__;
	return;
}

//----------------------------------------------------------------------
// 分批次获取邮件的数据 [2/5/2018 Chief]
//----------------------------------------------------------------------
void MailManager::onRPCFetchContentMailDataCallBackSuccess(BatchContentMailArchive& stMailData)
{
	for (int32 i = 0; i < stMailData.mBatchNum; ++i)
	{
		DBAttachmentMailState* pMail = mAttachmentMailPool.allocObject();
		if (NULL == pMail) return;
		mAttachmentMailIDMap.Add(stMailData.mBatchContentMailData[i].mDBState.getMailID(), pMail);
	}

}

void MailManager::onRPCFetchContentMailDataCallBackOvertime()
{

}

void MailManager::fetchContentMailsInfoFromDB()
{
	__GUARD__;
	int32 nStartID = 0;
	int32 nSelectNum = BATCH_ContentMail_NUM;
	for (int32 i = 0; i <= mAllMailNums.getContentMailNum() / BATCH_ContentMail_NUM; ++i)
	{
		nStartID = i * BATCH_ContentMail_NUM;
		if (i == mAllMailNums.getBaseMailNum() / BATCH_ContentMail_NUM)
		{
			nSelectNum = mAllMailNums.getBaseMailNum() % BATCH_ContentMail_NUM;
		}
		g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
			ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
			MODULE_DEFAULT_ID,
			this
			)
			->rpcFetchContentMailData(nStartID + 1, nSelectNum, getGroupID())
			->bindCallback(
				&MailManager::onRPCFetchContentMailDataCallBackSuccess,
				&MailManager::onRPCFetchContentMailDataCallBackOvertime,
				10000
			);
	}
	return;
	__UNGUARD__;
	return;
}

//----------------------------------------------------------------------
// 分批次获取邮件的数据 [2/5/2018 Chief]
//----------------------------------------------------------------------
void MailManager::onRPCFetchSystemMailDataCallBackSuccess(BatchSystemMailArchive& stMailData)
{
	for (int32 i = 0; i < stMailData.mBatchNum; ++i)
	{
		DBAttachmentMailState* pMail = mAttachmentMailPool.allocObject();
		if (NULL == pMail) return;
		mAttachmentMailIDMap.Add(stMailData.mBatchSystemMailData[i].mDBState.getMailID(), pMail);
	}

}

void MailManager::onRPCFetchSystemMailDataCallBackOvertime()
{

}

void MailManager::fetchSystemMailsInfoFromDB()
{
	__GUARD__;
	int32 nStartID = 0;
	int32 nSelectNum = BATCH_SystemMail_NUM;
	for (int32 i = 0; i <= mAllMailNums.getSystemMailNum() / BATCH_SystemMail_NUM; ++i)
	{
		nStartID = i * BATCH_SystemMail_NUM;
		if (i == mAllMailNums.getBaseMailNum() / BATCH_SystemMail_NUM)
		{
			nSelectNum = mAllMailNums.getBaseMailNum() % BATCH_SystemMail_NUM;
		}
		g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
			ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
			MODULE_DEFAULT_ID,
			this
			)
			->rpcFetchSystemMailData(nStartID + 1, nSelectNum, getGroupID())
			->bindCallback(
				&MailManager::onRPCFetchSystemMailDataCallBackSuccess,
				&MailManager::onRPCFetchSystemMailDataCallBackOvertime,
				10000
			);
	}
	return;
	__UNGUARD__;
	return;
}

//----------------------------------------------------------------------
//基础邮件
//----------------------------------------------------------------------
void MailManager::onRPCOperatorBaseMailDataCallbackSucess(BOOL& bSucess, DataStream& userData)
{

}

void MailManager::onRPCOperatorBaseMailDataCallbackOverTime(DataStream& userData)
{

}

BOOL MailManager::handleBaseMail(DBBaseMailState* stDBState, EDBOper eType, uint64 nMailID)
{
	__GUARD__;

	BaseMailArchive stArchive;
	stArchive.clear();

	DBBaseMailState* pOldMail = mCommonMailIDMap.Get(nMailID);
	if (eType == EDBO_Delete)
	{
		if (NULL == pOldMail || !pOldMail->isValid())
			return TRUE;

		// 邮件存在,清空所有数据 [2/7/2018 Chief]
		stArchive.mDBState.markDirtyFlags();
		mCommonMailIDMap.Remove(nMailID);
		// 并不在DB进行删除操作,只是将其标记为过期 [2/7/2018 Chief]
		eType = EDBO_Update;
	}
	else
	{
		if (!stDBState->isValid())
			return FALSE;

		if (NULL != pOldMail)
		{
			eType = EDBO_Update;
			// 若是同一封邮件,则不进行处理 [2/7/2018 Chief]
			if (pOldMail == stDBState)
				return TRUE;
		}

		DBBaseMailState* pMail = mCommonMailPool.allocObject();
		if (NULL == pMail) return FALSE;
		pMail->copyProperty(stDBState);
		mCommonMailIDMap.Add(pMail->mMailID, pMail);

		stArchive.mDBState.copyProperty(stDBState);
	}

	DataStream userData;
	userData.pushValue((int32)eType);

	g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
		ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcOperatorBaseMailData(stArchive, getGroupID(), eType)
		->bindCallback(
			&MailManager::onRPCOperatorBaseMailDataCallbackSucess,
			&MailManager::onRPCOperatorBaseMailDataCallbackOverTime,
			userData,
			10000
		);

	// 通知对应Server中所有RegPlayer增加新的MailID [2/22/2018 Chief]

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
//附件
//----------------------------------------------------------------------
BOOL MailManager::handleAttachmentMail(DBAttachmentMailState* stDBState, EDBOper eType, uint64 nMailID)
{
	__GUARD__;
	
	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
//文本邮件
//----------------------------------------------------------------------
BOOL MailManager::handleContentMail(DBContentMailState* stDBState, EDBOper eType, uint64 nMailID)
{
	__GUARD__;

	ContentMailArchive stArchive;
	stArchive.clear();

	DBContentMailState* pOldMail = mContentMailIDMap.Get(nMailID);
	if (eType == EDBO_Delete)
	{
		if (NULL == pOldMail || !pOldMail->isValid()) return TRUE;
		mContentMailIDMap.Remove(nMailID);
		stArchive.mDBState.markDirtyFlags();

		// todo 暂时转换为更新,以后会改为删除 [2/22/2018 Chief]
		eType = EDBO_Update;
	}
	else if (EDBO_Insert == eType)
	{
		if (!stDBState->isValid())
			return FALSE;

		if (NULL != pOldMail)
		{
			eType = EDBO_Update;
			// todo 之后会进行修改 [2/22/2018 Chief]
			if (pOldMail == stDBState)
				return TRUE;
		}

		DBContentMailState* pMail = mContentMailPool.allocObject();
		if (NULL == pMail) return FALSE;
		pMail->copyProperty(stDBState);
		mContentMailIDMap.Add(pMail->mMailID, pMail);

		stArchive.mDBState.copyProperty(stDBState);
	}

	g_pModuleManager->getExportCallable < IDBProxyManagerExportCallable < MailManager > >(
		ServerModuleUtil::getDBProxyModuleForMailMgrByGroup(getGroupID()),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcOperatorContentMailData(stArchive, getGroupID(), eType);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
//系统邮件
//----------------------------------------------------------------------
BOOL MailManager::handleSystemMail(DBSystemMailState* stDBState, EDBOper eType, uint64 nMailID)
{
	__GUARD__;


	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
// 按优先级获取邮件list [2/9/2018 Chief]
//----------------------------------------------------------------------
int32 MailManager::getOnePlayerMailList(tagMailIDList& stPlayerMailArr, SNID_t snid)
{
	__GUARD__;

	stPlayerMailArr.Clear();
	int32 nServerID = SnidUtil::getPlayerServerID(snid);
	int32 nCurMailNum = 0;
	int32 nMailID = 0;
	int32 nMailServerID = 0;
	DBBaseMailState* pMail = NULL;

	// 系统邮件
	int32 nSysNums = mSystemMailIDMap.GetCount();
	TIntegerHashMap<DBSystemMailState*>::IDTableIterator itSysBegin = mSystemMailIDMap.Begin();
	for (int32 i = 0; nSysNums > 0, nCurMailNum < MAX_MAIL_SIZE, itSysBegin != mSystemMailIDMap.End(); ++i)
	{
		nMailID = itSysBegin->second->getMailID();

		pMail = mCommonMailIDMap.Get(nMailID);
		if (NULL == pMail) continue;

		nMailServerID = pMail->getServerID();  // -1 时为全服邮件
		if ((INVALID == nMailServerID || nServerID == nMailServerID) /* && lv etc.*/)
		{
			// todo 增加判断,该邮件的状态不是玩家手动删除,需要修改online发送过来的内容,不缓存,指针不为空则进行该状态判断

			stPlayerMailArr.mMailData[nCurMailNum].mMailID = nMailID;
			nSysNums--;
			nCurMailNum++;
			itSysBegin++;
		}
	}

	// todo 附件(分定服奖励&&个人随机奖励,暂未开放)

	// 文本
	int32 nContentNums = mContentMailIDMap.GetCount();
	TIntegerHashMap<DBContentMailState*>::IDTableIterator itConBegin = mContentMailIDMap.Begin();
	for (int32 i = 0; nContentNums > 0, nCurMailNum < MAX_MAIL_SIZE, itConBegin != mContentMailIDMap.End(); ++i)
	{
		nMailID = itConBegin->second->getMailID();

		pMail = mCommonMailIDMap.Get(nMailID);
		if (NULL == pMail) continue;

		nMailServerID = pMail->getServerID();  // -1 时为全服邮件
		if ((INVALID == nMailServerID || nServerID == nMailServerID) /* && lv etc.*/)
		{
			stPlayerMailArr.mMailData[nCurMailNum].mMailID = nMailID;
			nContentNums--;
			nCurMailNum++;
			itConBegin++;
		}
	}

	// todo 针对个人对个人的邮件(系统暂未开放)

	return nCurMailNum;
	__UNGUARD__;
	return 0;
}

//----------------------------------------------------------------------
//RPC调用接口 START
//----------------------------------------------------------------------
// 发送附件
BOOL MailManager::rpcSendAttachmentMail(DBBaseMailState& stBaseMail, tagSendMailAttachMent& stAttachMent)
{
	__GUARD__;
	// 新增基础邮件
	stBaseMail.setMailID(getCurFreeMailID());
	if (!handleBaseMail(&stBaseMail, EDBO_Insert, stBaseMail.getMailID())) return FALSE;

	// 增加附件
	DBAttachmentMailState stAttachmentMail;
	stAttachmentMail.setMailID(stBaseMail.mMailID);
	if (!stAttachMent.isValid())
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_MAILMGR, "MailManager rpcSendAttachmentMail error : the stAttachMent is invalid!");
		return FALSE;
	}

	stAttachmentMail.setTypeMisc1(stAttachMent.getTypeMisc(0));
	stAttachmentMail.setTypeMisc2(stAttachMent.getTypeMisc(1));
	stAttachmentMail.setTypeMisc3(stAttachMent.getTypeMisc(2));
	stAttachmentMail.setNum1(stAttachMent.getNum(0));
	stAttachmentMail.setNum2(stAttachMent.getNum(1));
	stAttachmentMail.setNum3(stAttachMent.getNum(2));

	handleAttachmentMail(&stAttachmentMail, EDBO_Insert, stAttachmentMail.getMailID());

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// 发送文本邮件
BOOL MailManager::rpcSendContentMail(DBBaseMailState& stBaseMail, FixedString<DB_TITLE_LEN>& title, FixedString<DB_CONTENT_LEN>& content)
{
	__GUARD__;
	// 新增基础邮件
	stBaseMail.setMailID(getCurFreeMailID());
	if (!handleBaseMail(&stBaseMail, EDBO_Insert, stBaseMail.getMailID())) return FALSE;

	DBContentMailState stExtraMail;
	stExtraMail.setMailID(stBaseMail.mMailID);
	if (!stExtraMail.isValid())
	{
		QLogSystem::QUICK_LOG(QUICK_LOG_MAILMGR, "MailManager rpcSendContentMail error : the ContentMail is invalid!");
		return FALSE;
	}

	stExtraMail.setTitle(title);
	stExtraMail.setContent(content);

	handleContentMail(&stExtraMail, EDBO_Insert, stBaseMail.getMailID());

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// 发送系统邮件
BOOL MailManager::rpcSendSystemMail(DBBaseMailState& stBaseMail, int32 nServerID, int32 nMaxLv, int32 nMinLv)
{
	__GUARD__;
	// 新增基础邮件
	stBaseMail.setMailID(getCurFreeMailID());
	if (!handleBaseMail(&stBaseMail, EDBO_Insert, stBaseMail.getMailID())) return FALSE;


	
	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// 将最新的邮件发送给player [2/8/2018 Chief]
void MailManager::rpcSynMailList(SNID_t snid)
{
	__GUARD__;

	tagMailIDList stPlayerMailArr;
	stPlayerMailArr.Clear();

	getOnePlayerMailList(stPlayerMailArr, snid);

	String strMailList;
	stPlayerMailArr.serialize(strMailList);
	tagMailConfirm stListInfo;
	stListInfo.clear();
	stListInfo.setMailIDList(strMailList);

	g_pModuleManager->getExportCallable < IOnlinePlayerExportCallable < MailManager > >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(snid),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcMgrSynMailList2Player(stListInfo);

	__UNGUARD__;
}

// 请求该玩家所有邮件简易数据 [2/9/2018 Chief]
void MailManager::rpcRequestAllPlayerMailsData(SNID_t snid, tagMailConfirm& stMaillistInfo, uint64 nSessionID)
{
	__GUARD__;

	tagMailIDList stPlayerMailArr;
	stPlayerMailArr.Clear();
	int32 nCurMailNum = getOnePlayerMailList(stPlayerMailArr, snid);

	tagMailIDList stOldMailArr;
	stOldMailArr.Clear();
	stOldMailArr.unserialize(stMaillistInfo.getMailIDList().c_str());

	tagSendMailListInfo stSendList;
	stSendList.clear();
	// 基本邮件获取过期时间与类型 && 附加邮件存储的读取标记[2/9/2018 Chief]
	int32 nMailID = 0;
	BYTE nStatus = 0;
	int32 nFindIndex = 0;
	DBBaseMailState* pMail = NULL;
	for (int32 i = 0; i < nCurMailNum; ++i)
	{
		// id
		nMailID = stPlayerMailArr.mMailData[i].mMailID;
		stSendList.setMailID(nMailID, i);
		// type
		pMail = mCommonMailIDMap.Get(nMailID);
		if (NULL == pMail) continue;
		stSendList.setType(pMail->getMailType(), i);
		stSendList.setOverTime(pMail->getOverTime(), i);
		// status
		nFindIndex = stOldMailArr.findMailByID(nMailID);
		if (nFindIndex)
		{
			nStatus = stOldMailArr.mMailData[nFindIndex].mMailStatus;
		}
		stSendList.setStatus(nStatus, i);
	}

	// 发送一波简易消息, id,所属类型,是否未读,其他详情在玩家请求具体邮件时发送 [2/9/2018 Chief]
	g_pModuleManager->getExportCallable < IClientSessionExportCallable < MailManager > >(
		ServerModuleUtil::getSessionModuleBySessionID(nSessionID),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcMgrSynMailList2Session(stSendList);

	__UNGUARD__;
}

// 请求邮件详情,此时不关心是否已读 [2/9/2018 Chief]
void MailManager::rpcRequestMailDetail(int32 nMailID, uint64 nSessionID)
{
	__GUARD__;

	// 基本邮件类型,基础时间等已在请求总list时发送过 [2/9/2018 Chief]
	DBBaseMailState* pMail = mCommonMailIDMap.Get(nMailID);
	if (NULL == pMail) return;
	tagSendMailDetail stSendMainDetail;
	stSendMainDetail.setMailType(pMail->getMailType());

	// 其他邮件获取其他内容 [2/9/2018 Chief]
	DBContentMailState* pConMail = mContentMailIDMap.Get(nMailID);
	if (NULL != pConMail)
	{
		stSendMainDetail.setTitle(pConMail->getTitle());
		stSendMainDetail.setContent(pConMail->getContent());
	}
	
	tagSendMailAttachMent stAttach;
	// todo add attach
	stAttach.clear();
	
	// 单个邮件详情发送 [2/9/2018 Chief]
	g_pModuleManager->getExportCallable < IClientSessionExportCallable < MailManager > >(
		ServerModuleUtil::getSessionModuleBySessionID(nSessionID),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcMgrSynMailDetail2Session(stSendMainDetail, stAttach);

	__UNGUARD__;
}
//----------------------------------------------------------------------
//RPC调用接口 END
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// factory
//----------------------------------------------------------------------

EServerModuleType MailManagerFactory::getType() const
{
	return EServerModuleType_Mail;
}

ServerModuleBase* MailManagerFactory::createModule(EServerModuleType type, int32 group)
{
	if (NULL != mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}

	mPtrServerModule = new MailManager;
	return mPtrServerModule;
}

void MailManagerFactory::destroyModule(ModuleBase* module)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}
}