#ifndef __MAIL_MANAGER_H__
#define __MAIL_MANAGER_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "StringHashMap.h"
#include "CommonObjectPool.h"
#include "ServerModuleUtils.h"
#include "ServerModuleExportInterface.h"
#include "ServerModuleManager.h"
#include "GameMail.h"
#include "MailDefine.h"
#include "GameMailStruct.h"

#include <queue>


class MailManager
	: public ServerModuleBase
	, public IMailManager
{
public:
	MailManager();
	~MailManager();

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);
	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }
private:
	int32 getCurFreeMailID();
	void onRPCFetchGlobalValCallbackSucess(int32& nID);
	void onRPCFetchGlobalValCallbackOverTime(){}
	void fetchMaxMailID();
	void saveMaxMailID(int32 nID);
public:
	// ��ȡ��ǰmgr���ܹ����ȫ���ʼ����� [2/1/2018 Chief]
	void onRPCFetchAllMailNumsCallBackSuccess(tagMailsNum& nNums);
	void onRPCFetchAllMailNumsCallBackOvertime();
	void fetchAllMailNumsFromDB();

	// basemail[2/1/2018 Chief]
	void onRPCFetchBaseMailDataCallBackSuccess(BatchBaseMailArchive& stMailData);
	void onRPCFetchBaseMailDataCallBackOvertime();
	void fetchBaseMailsInfoFromDB();

	// AttachmentMail [2/5/2018 Chief]
	void onRPCFetchAttachmentMailDataCallBackSuccess(BatchAttachmentMailArchive& stMailData);
	void onRPCFetchAttachmentMailDataCallBackOvertime();
	void fetchAttachmentMailsInfoFromDB();

	// ContentMail [2/5/2018 Chief]
	void onRPCFetchContentMailDataCallBackSuccess(BatchContentMailArchive& stMailData);
	void onRPCFetchContentMailDataCallBackOvertime();
	void fetchContentMailsInfoFromDB();

	// SystemMail [2/5/2018 Chief]
	void onRPCFetchSystemMailDataCallBackSuccess(BatchSystemMailArchive& stMailData);
	void onRPCFetchSystemMailDataCallBackOvertime();
	void fetchSystemMailsInfoFromDB();

//----------------------------------------------------------------------
//RPC���ýӿ� START
//----------------------------------------------------------------------
	virtual BOOL rpcSendAttachmentMail(DBBaseMailState& stBaseMail, tagSendMailAttachMent& stAttachMent);
	virtual BOOL rpcSendContentMail(DBBaseMailState& stBaseMail, FixedString<DB_TITLE_LEN>& title, FixedString<DB_CONTENT_LEN>& content);
	virtual BOOL rpcSendSystemMail(DBBaseMailState& stBaseMail, int32 nServerID, int32 nMaxLv, int32 nMinLv);

	// �����µ��ʼ����͸�player [2/8/2018 Chief]
	virtual void rpcSynMailList(SNID_t snid);

	// ���������ʼ��ļ������� [2/9/2018 Chief]
	virtual void rpcRequestAllPlayerMailsData(SNID_t snid, tagMailConfirm& stMaillistInfo, uint64 nSessionID);

	// �����ʼ����� [2/9/2018 Chief]
	virtual void rpcRequestMailDetail(int32 nMailID, uint64 nSessionID);
//----------------------------------------------------------------------
//RPC���ýӿ� END
//----------------------------------------------------------------------

private:
	void onRPCOperatorBaseMailDataCallbackSucess(BOOL& bSucess, DataStream& userData);
	void onRPCOperatorBaseMailDataCallbackOverTime(DataStream& userData);
	BOOL handleBaseMail(DBBaseMailState* stDBState, EDBOper eType, uint64 nMailID);
	BOOL handleAttachmentMail(DBAttachmentMailState* stDBState, EDBOper eType, uint64 nMailID);
	BOOL handleContentMail(DBContentMailState* stDBState, EDBOper eType, uint64 nMailID);
	BOOL handleSystemMail(DBSystemMailState* stDBState, EDBOper eType, uint64 nMailID);

	// �����ȼ���ȡ�ʼ�list [2/9/2018 Chief]
	int32 getOnePlayerMailList(tagMailIDList& stPlayerMailArr, SNID_t snid);
public:

private:
	CommonObjectPool<DBBaseMailState>		mCommonMailPool;
	CommonObjectPool<DBAttachmentMailState>	mAttachmentMailPool;
	CommonObjectPool<DBContentMailState>	mContentMailPool;
	CommonObjectPool<DBSystemMailState>		mSystemMailPool;

	TIntegerHashMap<DBBaseMailState*>		mCommonMailIDMap;
	TIntegerHashMap<DBAttachmentMailState*>	mAttachmentMailIDMap;
	TIntegerHashMap<DBContentMailState*>	mContentMailIDMap;
	TIntegerHashMap<DBSystemMailState*>		mSystemMailIDMap;

	tagMailsNum								mAllMailNums;			// ��ǰmod���ܹ����ȫ���ʼ����� [2/1/2018 Chief]
	BOOL									mInitAllMailInfo;

	int32									mMaxMailID;
	BOOL									mIsAllMailInit;
};

#endif

class MailManagerFactory : public ServerModuleFactory
{
	MailManager*					mPtrServerModule;
public:
	MailManagerFactory() { mPtrServerModule = NULL; }
	virtual EServerModuleType			getType() const;
	virtual ServerModuleBase*			createModule(EServerModuleType type, int32 group);
	virtual void						destroyModule(ModuleBase* module);
};

// tips: ���͸���������ʼ�,�����ڵĻ��Ʋ�����reg�洢��,���ʼ�����δ����

/*
tips: �������л�ȡ,ȱ��,������������ʱ���ܻ����-->�ַ����
select top m * //��ȡ��ǰm��
from tablename
where id not in( //���˵�ǰn��
select top n id
from tablename
)
*/