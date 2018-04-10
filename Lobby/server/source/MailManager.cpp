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

	// ��������ʼ� [2/7/2018 Chief]
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

		// todo ֪ͨmgr��Ӧ��Regģ��,������������ʼ� [2/22/2018 Chief]
	}
}

void MailManager::onRPCFetchGlobalValCallbackSucess(int32& nID)
{
	mMaxMailID = nID;
	// ID����[2/8/2018 Chief]
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
// ͨ��groupid��ȡ�ʼ����� [2/5/2018 Chief]
//----------------------------------------------------------------------
void MailManager::onRPCFetchAllMailNumsCallBackSuccess(tagMailsNum& nNums)
{
	// todo ��Ϊ��ȡ���б��е��ʼ�����
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
// �����λ�ȡ�ʼ������� [2/5/2018 Chief]
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
// �����λ�ȡ�ʼ������� [2/5/2018 Chief]
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
// �����λ�ȡ�ʼ������� [2/5/2018 Chief]
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
// �����λ�ȡ�ʼ������� [2/5/2018 Chief]
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
//�����ʼ�
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

		// �ʼ�����,����������� [2/7/2018 Chief]
		stArchive.mDBState.markDirtyFlags();
		mCommonMailIDMap.Remove(nMailID);
		// ������DB����ɾ������,ֻ�ǽ�����Ϊ���� [2/7/2018 Chief]
		eType = EDBO_Update;
	}
	else
	{
		if (!stDBState->isValid())
			return FALSE;

		if (NULL != pOldMail)
		{
			eType = EDBO_Update;
			// ����ͬһ���ʼ�,�򲻽��д��� [2/7/2018 Chief]
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

	// ֪ͨ��ӦServer������RegPlayer�����µ�MailID [2/22/2018 Chief]

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
//����
//----------------------------------------------------------------------
BOOL MailManager::handleAttachmentMail(DBAttachmentMailState* stDBState, EDBOper eType, uint64 nMailID)
{
	__GUARD__;
	
	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
//�ı��ʼ�
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

		// todo ��ʱת��Ϊ����,�Ժ���Ϊɾ�� [2/22/2018 Chief]
		eType = EDBO_Update;
	}
	else if (EDBO_Insert == eType)
	{
		if (!stDBState->isValid())
			return FALSE;

		if (NULL != pOldMail)
		{
			eType = EDBO_Update;
			// todo ֮�������޸� [2/22/2018 Chief]
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
//ϵͳ�ʼ�
//----------------------------------------------------------------------
BOOL MailManager::handleSystemMail(DBSystemMailState* stDBState, EDBOper eType, uint64 nMailID)
{
	__GUARD__;


	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//----------------------------------------------------------------------
// �����ȼ���ȡ�ʼ�list [2/9/2018 Chief]
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

	// ϵͳ�ʼ�
	int32 nSysNums = mSystemMailIDMap.GetCount();
	TIntegerHashMap<DBSystemMailState*>::IDTableIterator itSysBegin = mSystemMailIDMap.Begin();
	for (int32 i = 0; nSysNums > 0, nCurMailNum < MAX_MAIL_SIZE, itSysBegin != mSystemMailIDMap.End(); ++i)
	{
		nMailID = itSysBegin->second->getMailID();

		pMail = mCommonMailIDMap.Get(nMailID);
		if (NULL == pMail) continue;

		nMailServerID = pMail->getServerID();  // -1 ʱΪȫ���ʼ�
		if ((INVALID == nMailServerID || nServerID == nMailServerID) /* && lv etc.*/)
		{
			// todo �����ж�,���ʼ���״̬��������ֶ�ɾ��,��Ҫ�޸�online���͹���������,������,ָ�벻Ϊ������и�״̬�ж�

			stPlayerMailArr.mMailData[nCurMailNum].mMailID = nMailID;
			nSysNums--;
			nCurMailNum++;
			itSysBegin++;
		}
	}

	// todo ����(�ֶ�������&&�����������,��δ����)

	// �ı�
	int32 nContentNums = mContentMailIDMap.GetCount();
	TIntegerHashMap<DBContentMailState*>::IDTableIterator itConBegin = mContentMailIDMap.Begin();
	for (int32 i = 0; nContentNums > 0, nCurMailNum < MAX_MAIL_SIZE, itConBegin != mContentMailIDMap.End(); ++i)
	{
		nMailID = itConBegin->second->getMailID();

		pMail = mCommonMailIDMap.Get(nMailID);
		if (NULL == pMail) continue;

		nMailServerID = pMail->getServerID();  // -1 ʱΪȫ���ʼ�
		if ((INVALID == nMailServerID || nServerID == nMailServerID) /* && lv etc.*/)
		{
			stPlayerMailArr.mMailData[nCurMailNum].mMailID = nMailID;
			nContentNums--;
			nCurMailNum++;
			itConBegin++;
		}
	}

	// todo ��Ը��˶Ը��˵��ʼ�(ϵͳ��δ����)

	return nCurMailNum;
	__UNGUARD__;
	return 0;
}

//----------------------------------------------------------------------
//RPC���ýӿ� START
//----------------------------------------------------------------------
// ���͸���
BOOL MailManager::rpcSendAttachmentMail(DBBaseMailState& stBaseMail, tagSendMailAttachMent& stAttachMent)
{
	__GUARD__;
	// ���������ʼ�
	stBaseMail.setMailID(getCurFreeMailID());
	if (!handleBaseMail(&stBaseMail, EDBO_Insert, stBaseMail.getMailID())) return FALSE;

	// ���Ӹ���
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

// �����ı��ʼ�
BOOL MailManager::rpcSendContentMail(DBBaseMailState& stBaseMail, FixedString<DB_TITLE_LEN>& title, FixedString<DB_CONTENT_LEN>& content)
{
	__GUARD__;
	// ���������ʼ�
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

// ����ϵͳ�ʼ�
BOOL MailManager::rpcSendSystemMail(DBBaseMailState& stBaseMail, int32 nServerID, int32 nMaxLv, int32 nMinLv)
{
	__GUARD__;
	// ���������ʼ�
	stBaseMail.setMailID(getCurFreeMailID());
	if (!handleBaseMail(&stBaseMail, EDBO_Insert, stBaseMail.getMailID())) return FALSE;


	
	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// �����µ��ʼ����͸�player [2/8/2018 Chief]
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

// �������������ʼ��������� [2/9/2018 Chief]
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
	// �����ʼ���ȡ����ʱ�������� && �����ʼ��洢�Ķ�ȡ���[2/9/2018 Chief]
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

	// ����һ��������Ϣ, id,��������,�Ƿ�δ��,���������������������ʼ�ʱ���� [2/9/2018 Chief]
	g_pModuleManager->getExportCallable < IClientSessionExportCallable < MailManager > >(
		ServerModuleUtil::getSessionModuleBySessionID(nSessionID),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcMgrSynMailList2Session(stSendList);

	__UNGUARD__;
}

// �����ʼ�����,��ʱ�������Ƿ��Ѷ� [2/9/2018 Chief]
void MailManager::rpcRequestMailDetail(int32 nMailID, uint64 nSessionID)
{
	__GUARD__;

	// �����ʼ�����,����ʱ�������������listʱ���͹� [2/9/2018 Chief]
	DBBaseMailState* pMail = mCommonMailIDMap.Get(nMailID);
	if (NULL == pMail) return;
	tagSendMailDetail stSendMainDetail;
	stSendMainDetail.setMailType(pMail->getMailType());

	// �����ʼ���ȡ�������� [2/9/2018 Chief]
	DBContentMailState* pConMail = mContentMailIDMap.Get(nMailID);
	if (NULL != pConMail)
	{
		stSendMainDetail.setTitle(pConMail->getTitle());
		stSendMainDetail.setContent(pConMail->getContent());
	}
	
	tagSendMailAttachMent stAttach;
	// todo add attach
	stAttach.clear();
	
	// �����ʼ����鷢�� [2/9/2018 Chief]
	g_pModuleManager->getExportCallable < IClientSessionExportCallable < MailManager > >(
		ServerModuleUtil::getSessionModuleBySessionID(nSessionID),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcMgrSynMailDetail2Session(stSendMainDetail, stAttach);

	__UNGUARD__;
}
//----------------------------------------------------------------------
//RPC���ýӿ� END
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