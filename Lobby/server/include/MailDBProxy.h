#ifndef __MAIL_DB_PROXY_H__
#define __MAIL_DB_PROXY_H__

#include "DBProxyManager.h"
#include "GameMailStruct.h"
#include "PropertyMailState.h"

class DBProxyManager;

class MailDBProxy
{
public:
	MailDBProxy(DBProxyManager* pMgr) : mDBManager(pMgr) 
	{
		mDBMailData[EDBT_Base] = new DBModelBaseMailState;
		mDBMailData[EDBT_Attach] = new DBModelDBAttachmentMailState;
		mDBMailData[EDBT_Content] = new DBModelContentMailState;
		mDBMailData[EDBT_System] = new DBModelSystemMailState;
	}

	~MailDBProxy()
	{
		for (int32 i = 0; i < EDBT_END; ++i)
		{
			KSafeDelete(mDBMailData[i]);
			mDBMailData[i] = NULL;
		}
	}

	// ��õ�ǰ�ʼ����� [2/6/2018 Chief]
	int32 doFetchMailNum(EDBMailType eType, int32 nGroupID);

	// ɾ���ʼ�:������������ ��Ե�����ҵ��ʼ�&&�����ʼ�(�����ʱ�����)[2/6/2018 Chief]
	BOOL doDelMailByID(EDBMailType eType, int32 nMailID, int32 nGroupID);

	// ���ʼ�����Ϊ�Ƿ� [2/6/2018 Chief]
	BOOL doSetMailINVALID(EDBMailType eType, int32 nMailID, int32 nGroupID);

	// �����ʼ� [2/6/2018 Chief]
	BOOL doInsertBaseMailsInfo(BaseMailArchive& stMail, int32 nGroupID);
	BOOL doInsertAttachmentMailsInfo(AttachmentMailArchive& stMail, int32 nGroupID);
	BOOL doInsertContentMailsInfo(ContentMailArchive& stMail, int32 nGroupID);
	BOOL doInsertSystemMailsInfo(SystemMailArchive& stMail, int32 nGroupID);

	// ������ȡ�ʼ�����Ϣ [2/6/2018 Chief]
	BOOL doFetchBaseMailsInfo(BatchBaseMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum);
	BOOL doFetchAttachmentMailsInfo(BatchAttachmentMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum);
	BOOL doFetchContentMailsInfo(BatchContentMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum);
	BOOL doFetchSystemMailsInfo(BatchSystemMailArchive& stBatchMail, int32 nGroupID, int32 nStartID, int32 nSelectNum);

	// �����ʼ�[2/6/2018 Chief]
	BOOL doUpdateBaseMailsInfo(BaseMailArchive& stMail, int32 nGroupID);
	BOOL doUpdateAttachmentMailsInfo(AttachmentMailArchive& stMail, int32 nGroupID);
	BOOL doUpdateContentMailsInfo(ContentMailArchive& stMail, int32 nGroupID);
	BOOL doUpdateSystemMailsInfo(SystemMailArchive& stMail, int32 nGroupID);

private:
	DBProxyManager*					mDBManager;

	DBModelBase*					mDBMailData[EDBT_END];
};

#endif // __MAIL_DB_PROXY_H__
