/*$T Common/Combat/TabTemplateLoader.h GC 1.140 10/10/07 10:06:27 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


/* TabTemplateLoader.h 功能说明：技能相关数据管理器的模板 */
#ifndef _TEMPLATE_MANAGER_H_
#define _TEMPLATE_MANAGER_H_

#include "TypeDefine.h"
#include "TAB/TAB.h"

namespace	TAB
{
class	TABFile;
} using namespace	TAB;
template<typename U>
class TemplateManager
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	enum { MAXSIZE = U::MAX_ID +1, };

	TemplateManager();
	~TemplateManager();
	BOOL InitFromDB(char const *szFile);
	BOOL Init_FromDBMem(const TABFile *pDB);
	U const *const GetInstanceByID(int32 const nID) const;

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	int32 Size(void) const
	{
		return m_nSize;
	};

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
	void ClearTemplates();

private:
	BOOL	m_bReady;
	int32	m_nSize;
	U*	m_pTemplateList;
	// ID与索引的对应关系数组，增加这个的目的是加快表格索引 [yangzhen]
	int32 m_nTemplateIndexs[MAXSIZE];
	TABFile *m_pDB;
};

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<typename U>
inline TemplateManager<U>::TemplateManager() :
	m_bReady(FALSE),
	m_nSize(0),
	m_pDB(NULL),
	m_pTemplateList(NULL)
{
	for(int32 i = 0; i < MAXSIZE; ++i)
	{
		m_nTemplateIndexs[i] = -1;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<typename U>
inline TemplateManager<U>::~TemplateManager()
{
	KSafeDelete_ARRAY(m_pTemplateList);
	KSafeDelete(m_pDB);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<typename U>
inline void TemplateManager<U>::ClearTemplates()
{
	KSafeDelete_ARRAY(m_pTemplateList);
	for(int32 i = 0; i < MAXSIZE; ++i)
	{
		m_nTemplateIndexs[i] = -1;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<typename U>
inline BOOL TemplateManager<U>::Init_FromDBMem(const TABFile *pDB)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	m_nSize = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_bReady = FALSE;

	if(NULL == pDB)
	{
		return FALSE;
	}

	// 先清理之前分配过的数据 [yangzhen]
	ClearTemplates();

	m_nSize = pDB->GetRowsNum();
	m_pTemplateList = new U[m_nSize];

	/*
	 * init List ;
	 * read data
	 */
	for(int32 _Idx = 0; m_nSize > _Idx; ++_Idx)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		int32	nID = pDB->SearchField(_Idx, 0)->iValue;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(0 > nID || MAXSIZE < nID)
		{
			KCheckEx(FALSE, "[TemplateManager_T<U>::InitFromDB: nID out of range!!abort input.");
			continue;
		}

		// 建立索引 [yangzhen]
		m_nTemplateIndexs[nID] = _Idx;
		m_pTemplateList[_Idx].InitFromDB(pDB, _Idx);
	}

	m_bReady = TRUE;
	return TRUE;
	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<typename U>
inline BOOL TemplateManager<U>::InitFromDB(char const *szFile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	m_nSize = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_bReady = FALSE;

	if(NULL == szFile)
	{
		return FALSE;
	}

	if(NULL != m_pDB)
	{
		KSafeDelete(m_pDB);
	}

	m_pDB = new TABFile(0);
	if(NULL == m_pDB)
	{
		return FALSE;
	}

	if(FALSE == m_pDB->OpenFile_From_TEXT(szFile))
	{
		return FALSE;
	}

	// 先清理之前分配过的数据 [yangzhen]
	ClearTemplates();

	m_nSize = m_pDB->GetRowsNum();
	m_pTemplateList = new U[m_nSize];

	/*
	 * init List ;
	 * read data
	 */
	for(int32 _Idx = 0; m_nSize > _Idx; ++_Idx)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		int32	nID = m_pDB->SearchField(_Idx, 0)->iValue;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(0 > nID || MAXSIZE <= nID)
		{
			KCheckEx(FALSE, "[TemplateManager_T<U>::InitFromDB: nID out of range!!abort input.");
			continue;
		}

		// 建立索引 [yangzhen]
		m_nTemplateIndexs[nID] = _Idx;
		m_pTemplateList[_Idx].InitFromDB(m_pDB, _Idx);
	}

	m_bReady = TRUE;
	return TRUE;
	__UNGUARD__ return FALSE;
}

template<typename U>
inline U const *const TemplateManager<U>::GetInstanceByID(int32 const nID) const
{
	__GUARD__

	if(0 > nID || MAXSIZE <= nID)
	{
		return NULL;
	}

	if(0 == m_nSize)
	{
		return NULL;
	}

	if(FALSE == m_bReady)
	{
		return NULL;
	}

	int32 nIndex = m_nTemplateIndexs[nID];
	if(nIndex < 0)
	{
		return NULL;
	}

	return &m_pTemplateList[nIndex];
	__UNGUARD__
	return NULL;
}

#endif /* _TEMPLATE_MANAGER_H_ */
