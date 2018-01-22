/*$T Share/DBSystem/DataBase/ArchiveAbilityList.h GC 1.140 10/10/07 10:06:27 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef _DB_ARCHIVEINTERFACE_H_
#define _DB_ARCHIVEINTERFACE_H_

#include "TypeDefine.h"
#include "KLODBCBase.h"

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
*/
class DBCheckConnection :
	public KLODBCBase
{
public:
	DBCheckConnection(DBCore *pCore);
	void		Clear();
	virtual BOOL	DB2Memory();
	virtual BOOL	Save();
	virtual BOOL	FetchDATAS(void *pResult);
};

/*
-----------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/
class DBCheckVersion :
	public KLODBCBase
{
public:
	DBCheckVersion(DBCore *pCore);
	void		Clear();
	virtual BOOL	DB2Memory();
	virtual BOOL	Save();
	virtual BOOL	FetchDATAS(void *pResult);

public:
	BOOL	FetchDATAS(int32 dbVersion = 0);
};

#endif
