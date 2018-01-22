
#include "stdafx.h"
#include "DBOper.h"

#include "DBCore.h"
#include "Setting.h"
#include "ArchiveInterface.h"
#include "ArchiveHolder.h"
#include "MKManager.h"

DBOperMgr* g_pDBOperMgr = NULL;
/*
 =======================================================================================================================
 =======================================================================================================================
 */
DBOper::DBOper()
{	
	mCharDBInterface = new DBCore;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
DBOper::~DBOper()
{
	KSafeDelete(mCharDBInterface);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBOper::Init(int32 versionNum, const char* ip, uint32 port, const char* dbName, const char* user, const char* password)
{
	__GUARD__;
	
	 m_nTID = GetOrigineThreadID();
	 mVersionNum = versionNum;

	//IP
	char Host[HOST_STR_LEN];
	strncpy(Host, ip, HOST_STR_LEN);
	Host[HOST_STR_LEN - 1] = '\0';

	//�˿�
	uint32 Port = port;

	//���ݿ�����
	char Database[DATABASE_STR_LEN];
	strncpy(Database, dbName, DATABASE_STR_LEN);
	Database[DATABASE_STR_LEN - 1] = '\0';

	//���ӵ��û���
	char User[DB_USE_STR_LEN];
	strncpy(User, user, DB_USE_STR_LEN);
	User[DB_USE_STR_LEN - 1] = '\0';

	//���ӵ�����
	char Password[DB_PASSWORD_STR_LEN];
	strncpy(Password, password, DB_PASSWORD_STR_LEN);
	Password[DB_PASSWORD_STR_LEN - 1] = '\0';

	//��ָ�����м��
	KCheck(mCharDBInterface);

	//���ӵ����ݿ�
	mCharDBInterface->Connect(Database, User, Password);

	//������ӽ��
	if(!mCharDBInterface->IsConnected())
	{
		QLogSystem::QUICK_LOG( QUICK_LOG_DBDEBUG, 
			"ArchiveThread:mCharDBInterface->Connect()... Get Errors: %s ",
			mCharDBInterface->GetErrorMsg() );
	}

	//�������ӽ��
	return mCharDBInterface->IsConnected();
	__UNGUARD__;
	return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
DBCore *DBOper::GetInterface()
{
	return mCharDBInterface;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL DBOper::CheckVersion()
{
	__GUARD__

	KCheck(mCharDBInterface);
	DBCheckVersion	checkVersion(mCharDBInterface);

	BOOL bRet = FALSE;
	bRet = checkVersion.DB2Memory();

	if (bRet)
	{
		bRet = checkVersion.FetchDATAS(mVersionNum);
	}

	if (!bRet)
	{
		KCheckEx(FALSE, "DBVersion is error");
	}

	return TRUE;

	__UNGUARD__
		return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBOper::CheckDBConnection(BOOL bForce)
{
	__GUARD__

	KCheck(mCharDBInterface);
	if(mCharDBInterface->IsConnected() == FALSE)
	{
		//���û�����ӣ���������
		BOOL bRet = mCharDBInterface->Connect();
		if(!bRet)
		{
			//����ʧ��
			QLogSystem::QUICK_LOG
				(
				SLOW_LOG_GL_DBCENTER,
				"DBOper::CheckDBConnection,mCharDBInterface->Connect() Get Errors: %s,TID = %d,ErrorCode =%d",
				mCharDBInterface->GetErrorMsg(),
				GetTID(),
				mCharDBInterface->GetError());
		}
		else
		{
			//���ӳɹ�
			QLogSystem::QUICK_LOG
				(
				SLOW_LOG_GL_DBCENTER,
				"DBOper::CheckDBConnection,Reconnect database,TID=%d",
				GetTID());
		}
	}

	//����DB���Ӽ��Ķ�ʱ�����Զ�ʱ�������״̬
	if(!m_CheckDBConnectionTimer.IsSetTimer())
	{
		m_CheckDBConnectionTimer.BeginTimer(1800 * 1000, GET_TIME().TickCount());
	}

	//����Ƿ���Դ���DB���
	if(m_CheckDBConnectionTimer.CountingTimer(GET_TIME().TickCount()) || bForce)
	{
		if (bForce)
		{
			m_CheckDBConnectionTimer.BeginTimer(1800 * 1000, GET_TIME().TickCount());
		}
		//����ԭ�����Ǹ��ݵ�ǰ���ݿ����ӣ���ȡһ�����ݣ�����ܹ��ɹ���ȡ����ʾ������Ч
		//�����ʧ�ܣ����ʾ��ǰ���ݿ������Ѿ�ʧЧ
		DBCheckConnection checkDBConnection(mCharDBInterface);
		if(checkDBConnection.DB2Memory())
		{
			//�����ȡ�ɹ�����¼��־
			checkDBConnection.FetchDATAS(NULL);
			QLogSystem::QUICK_LOG
				(
				SLOW_LOG_GL_DBCENTER,
				">>>> DBOper::CheckDBConnection,check database connection,TID=%d >>>>", GetTID() );
		}
		else
		{
			//���û�����ӣ���������
			BOOL bRet = mCharDBInterface->Connect();
			if(!bRet)
			{
				//����ʧ��
				QLogSystem::QUICK_LOG
					(
					SLOW_LOG_GL_DBCENTER,
					"DBOper::CheckDBConnection,Reconnect MySql Get Errors: %s,TID = %d,ErrorCode =%d",
					mCharDBInterface->GetErrorMsg(),
					GetTID(),
					mCharDBInterface->GetError());
			}
			else
			{
				//���ӳɹ�
				QLogSystem::QUICK_LOG
					(
					SLOW_LOG_GL_DBCENTER,
					"DBOper::CheckDBConnection,Reconnect MySql Success database,TID=%d",
					GetTID());
			}
		}
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}

