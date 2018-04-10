#include "stdafx.h"
#include "MailDBProxy.h"
#include "DBOper.h"
#include "MailDefine.h"

//---------------------------------------------------------------------------------------------------------
// 获得当前邮件数量 [2/6/2018 Chief]
//---------------------------------------------------------------------------------------------------------
int32 MailDBProxy::doFetchMailNum(EDBMailType eType, int32 nGroupID)
{
	int32 nCount = 0;
	__GUARD__;

	if (eType < EDBT_Base || eType > EDBT_END)
		return 0;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	mDBMailData[eType]->clear();
	if (mDBMailData[eType]->doSQLSelectCount(dbCore))
	{
		if (!dbCore->Fetch())
			return nCount;

		int32 nRet;
		nCount = dbCore->Get_Property_Int(1, nRet);
	}

	return nCount;
	__UNGUARD__;
	return nCount;
}

//---------------------------------------------------------------------------------------------------------
// 删除邮件:两种情况会调用 针对单个玩家的邮件&&过期邮件(起服的时候操作)[2/6/2018 Chief]
//---------------------------------------------------------------------------------------------------------
BOOL MailDBProxy::doDelMailByID(EDBMailType eType, int32 nMailID, int32 nGroupID)
{
	__GUARD__;

	int32 nCount = 0;

	if (eType < EDBT_Base || eType > EDBT_END)
		return 0;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	mDBMailData[eType]->clear();

	switch (eType)
	{
	case EDBT_Base:
		(static_cast<DBModelBaseMailState*>(mDBMailData[eType]))->setMailID(nMailID);
		break;
	case EDBT_Attach:
		(static_cast<DBModelDBAttachmentMailState*>(mDBMailData[eType]))->setMailID(nMailID);
		break;
	case EDBT_Content:
		(static_cast<DBModelContentMailState*>(mDBMailData[eType]))->setMailID(nMailID);
		break;
	case EDBT_System:
		(static_cast<DBModelSystemMailState*>(mDBMailData[eType]))->setMailID(nMailID);
		break;
	default:
		break;
	}

	if (!mDBMailData[eType]->doSQLDelete(dbCore))
		return FALSE;

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//---------------------------------------------------------------------------------------------------------
// 将邮件设置为非法 [2/6/2018 Chief]
//---------------------------------------------------------------------------------------------------------
BOOL MailDBProxy::doSetMailINVALID(EDBMailType eType, int32 nMailID, int32 nGroupID)
{
	__GUARD__;

	if (eType < EDBT_Base || eType > EDBT_END)
		return 0;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();
	pQuery->Parse("update %s set 'valid'=0 where mailid=%llu", mDBMailData[eType]->getDBTableName(), nMailID);

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: update from table t_type?_mail failed, type=%d",
			(int32)eType
		);
		return FALSE;
	}

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//---------------------------------------------------------------------------------------------------------
// 新增邮件 [2/6/2018 Chief]
//---------------------------------------------------------------------------------------------------------
BOOL MailDBProxy::doInsertBaseMailsInfo(BaseMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelBaseMailState* pMailDBModel = static_cast<DBModelBaseMailState*>(mDBMailData[EDBT_Base]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->doSQLInsert(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}
BOOL MailDBProxy::doInsertAttachmentMailsInfo(AttachmentMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelDBAttachmentMailState* pMailDBModel = static_cast<DBModelDBAttachmentMailState*>(mDBMailData[EDBT_Attach]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->doSQLInsert(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

BOOL MailDBProxy::doInsertContentMailsInfo(ContentMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelContentMailState* pMailDBModel = static_cast<DBModelContentMailState*>(mDBMailData[EDBT_Content]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->doSQLInsert(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

BOOL MailDBProxy::doInsertSystemMailsInfo(SystemMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelSystemMailState* pMailDBModel = static_cast<DBModelSystemMailState*>(mDBMailData[EDBT_System]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->doSQLInsert(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//---------------------------------------------------------------------------------------------------------
// 批量获取邮件的信息 [2/6/2018 Chief]
//---------------------------------------------------------------------------------------------------------
BOOL MailDBProxy::doFetchBaseMailsInfo(BatchBaseMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelBaseMailState* pMailDBModel = static_cast<DBModelBaseMailState*>(mDBMailData[EDBT_Base]);
	pMailDBModel->clear();
	stBatchMail.clear();

	pMailDBModel->setMailIDRange(nStartID, nSelectNum);
	if (pMailDBModel->doSQLSelect(dbCore))
	{
		if (pMailDBModel->getAffectedRowCount() > 0)
		{
			for (int32 i = 0; i < BATCH_BaseMail_NUM && i < pMailDBModel->getAffectedRowCount(); ++i)
			{
				pMailDBModel->doSQLFetch(dbCore, &stBatchMail.mBatchBaseMailData[stBatchMail.mBatchNum].mDBState);
				stBatchMail.mBatchNum++;
			}

			return TRUE;
		}
	}

	return FALSE;
	__UNGUARD__;
	return TRUE;
}

BOOL MailDBProxy::doFetchAttachmentMailsInfo(BatchAttachmentMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelDBAttachmentMailState* pMailDBModel = static_cast<DBModelDBAttachmentMailState*>(mDBMailData[EDBT_Attach]);
	pMailDBModel->clear();
	stBatchMail.clear();

	pMailDBModel->setMailIDRange(nStartID, nSelectNum);
	if (pMailDBModel->doSQLSelect(dbCore))
	{
		if (pMailDBModel->getAffectedRowCount() > 0)
		{
			for (int32 i = 0; i < BATCH_AttachmentMail_NUM && i < pMailDBModel->getAffectedRowCount(); ++i)
			{
				pMailDBModel->doSQLFetch(dbCore, &stBatchMail.mBatchAttachmentMailData[stBatchMail.mBatchNum].mDBState);
				stBatchMail.mBatchNum++;
			}

			return TRUE;
		}
	}

	return FALSE;
	__UNGUARD__;
	return TRUE;
}

BOOL MailDBProxy::doFetchContentMailsInfo(BatchContentMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelContentMailState* pMailDBModel = static_cast<DBModelContentMailState*>(mDBMailData[EDBT_Content]);
	pMailDBModel->clear();
	stBatchMail.clear();

	pMailDBModel->setMailIDRange(nStartID, nSelectNum);
	if (pMailDBModel->doSQLSelect(dbCore))
	{
		if (pMailDBModel->getAffectedRowCount() > 0)
		{
			for (int32 i = 0; i < BATCH_ContentMail_NUM && i < pMailDBModel->getAffectedRowCount(); ++i)
			{
				pMailDBModel->doSQLFetch(dbCore, &stBatchMail.mBatchContentMailData[stBatchMail.mBatchNum].mDBState);
				stBatchMail.mBatchNum++;
			}

			return TRUE;
		}
	}

	return FALSE;
	__UNGUARD__;
	return TRUE;
}

BOOL MailDBProxy::doFetchSystemMailsInfo(BatchSystemMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelSystemMailState* pMailDBModel = static_cast<DBModelSystemMailState*>(mDBMailData[EDBT_System]);
	pMailDBModel->clear();
	stBatchMail.clear();

	pMailDBModel->setMailIDRange(nStartID, nSelectNum);
	if (pMailDBModel->doSQLSelect(dbCore))
	{
		if (pMailDBModel->getAffectedRowCount() > 0)
		{
			for (int32 i = 0; i < BATCH_SystemMail_NUM && i < pMailDBModel->getAffectedRowCount(); ++i)
			{
				pMailDBModel->doSQLFetch(dbCore, &stBatchMail.mBatchSystemMailData[stBatchMail.mBatchNum].mDBState);
				stBatchMail.mBatchNum++;
			}

			return TRUE;
		}
	}

	return FALSE;
	__UNGUARD__;
	return TRUE;
}

//---------------------------------------------------------------------------------------------------------
// 更新邮件(一般为在free调用的createfile) [2/6/2018 Chief]
//---------------------------------------------------------------------------------------------------------
BOOL MailDBProxy::doUpdateBaseMailsInfo(BaseMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelBaseMailState* pMailDBModel = static_cast<DBModelBaseMailState*>(mDBMailData[EDBT_Base]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->setMailID(stMail.mDBState.getMailID());
	pMailDBModel->doSQLUpdate(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

BOOL MailDBProxy::doUpdateAttachmentMailsInfo(AttachmentMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelDBAttachmentMailState* pMailDBModel = static_cast<DBModelDBAttachmentMailState*>(mDBMailData[EDBT_Attach]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->setMailID(stMail.mDBState.getMailID());
	pMailDBModel->doSQLUpdate(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

BOOL MailDBProxy::doUpdateContentMailsInfo(ContentMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelContentMailState* pMailDBModel = static_cast<DBModelContentMailState*>(mDBMailData[EDBT_Content]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->setMailID(stMail.mDBState.getMailID());
	pMailDBModel->doSQLUpdate(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

BOOL MailDBProxy::doUpdateSystemMailsInfo(SystemMailArchive& stMail, int32 nGroupID)
{
	__GUARD__;

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Mail, nGroupID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	DBModelSystemMailState* pMailDBModel = static_cast<DBModelSystemMailState*>(mDBMailData[EDBT_System]);
	pMailDBModel->clear();
	pMailDBModel->readFromProps(&stMail.mDBState);
	pMailDBModel->setMailID(stMail.mDBState.getMailID());
	pMailDBModel->doSQLUpdate(dbCore);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}
