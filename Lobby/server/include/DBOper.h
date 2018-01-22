#ifndef __DB_OPER_H__
#define __DB_OPER_H__

#include "TypeDefine.h"
#include "BaseTool.h"
#include "DBCore.h"
#include "IntegerHashMap.h"

class	DBCore;
class DBOperMgr;

class DBOper
{
public:
	DBOper();
	~DBOper();

	//��ʼ�����ݶ���
	BOOL Init(int32 versionNum, const char* ip, uint32 port, const char* dbName, const char* user, const char* password);

	DBCore* GetInterface();

	BOOL CheckVersion();

	//��鲢�����������ݿ�
	BOOL CheckDBConnection(BOOL bForce = FALSE);

	void SetTID(const TID nTID)
	{
		m_nTID = nTID;
	}

	TID	 GetTID()
	{
		return m_nTID;
	}

private:
	DBCore *mCharDBInterface;
	CTinyTimer m_CheckDBConnectionTimer;
	TID m_nTID;
	int32 mVersionNum;
};

#endif
