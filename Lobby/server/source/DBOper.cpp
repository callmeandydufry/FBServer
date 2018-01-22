
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

	//端口
	uint32 Port = port;

	//数据库名称
	char Database[DATABASE_STR_LEN];
	strncpy(Database, dbName, DATABASE_STR_LEN);
	Database[DATABASE_STR_LEN - 1] = '\0';

	//连接的用户名
	char User[DB_USE_STR_LEN];
	strncpy(User, user, DB_USE_STR_LEN);
	User[DB_USE_STR_LEN - 1] = '\0';

	//连接的密码
	char Password[DB_PASSWORD_STR_LEN];
	strncpy(Password, password, DB_PASSWORD_STR_LEN);
	Password[DB_PASSWORD_STR_LEN - 1] = '\0';

	//空指针例行检查
	KCheck(mCharDBInterface);

	//连接到数据库
	mCharDBInterface->Connect(Database, User, Password);

	//检查连接结果
	if(!mCharDBInterface->IsConnected())
	{
		QLogSystem::QUICK_LOG( QUICK_LOG_DBDEBUG, 
			"ArchiveThread:mCharDBInterface->Connect()... Get Errors: %s ",
			mCharDBInterface->GetErrorMsg() );
	}

	//返回连接结果
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
		//如果没有连接，尝试连接
		BOOL bRet = mCharDBInterface->Connect();
		if(!bRet)
		{
			//连接失败
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
			//连接成功
			QLogSystem::QUICK_LOG
				(
				SLOW_LOG_GL_DBCENTER,
				"DBOper::CheckDBConnection,Reconnect database,TID=%d",
				GetTID());
		}
	}

	//设置DB连接检查的定时器，以定时检查连接状态
	if(!m_CheckDBConnectionTimer.IsSetTimer())
	{
		m_CheckDBConnectionTimer.BeginTimer(1800 * 1000, GET_TIME().TickCount());
	}

	//检查是否可以触发DB检查
	if(m_CheckDBConnectionTimer.CountingTimer(GET_TIME().TickCount()) || bForce)
	{
		if (bForce)
		{
			m_CheckDBConnectionTimer.BeginTimer(1800 * 1000, GET_TIME().TickCount());
		}
		//检查的原理，就是根据当前数据库连接，获取一个数据，如果能够成功获取，表示连接有效
		//如果获失败，则表示当前数据库连接已经失效
		DBCheckConnection checkDBConnection(mCharDBInterface);
		if(checkDBConnection.DB2Memory())
		{
			//如果获取成功，记录日志
			checkDBConnection.FetchDATAS(NULL);
			QLogSystem::QUICK_LOG
				(
				SLOW_LOG_GL_DBCENTER,
				">>>> DBOper::CheckDBConnection,check database connection,TID=%d >>>>", GetTID() );
		}
		else
		{
			//如果没有连接，尝试连接
			BOOL bRet = mCharDBInterface->Connect();
			if(!bRet)
			{
				//连接失败
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
				//连接成功
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

