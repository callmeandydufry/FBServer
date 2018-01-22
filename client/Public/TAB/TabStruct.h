#ifndef __TAB_STRUCT_H__
#define __TAB_STRUCT_H__

#include "TypeDefine.h"

// 表格模板类，目前只有TabDefine_Share用到
template <class TYPE,int32 MAXSIZE>
class TabTemplate
{
public:
	TabTemplate()
	{
		memset( m_TableExt, 0, sizeof(m_TableExt) );
		m_Count = 0;
	}

	~TabTemplate() {}

	int32	GetSize()
	{
		return m_Count;
	}

	void	SetSize(int32 nSize)
	{
		m_Count = nSize;
	}

	int32	GetMaxSize()
	{
		return MAXSIZE;
	}

	TYPE*	GetData()
	{
		return m_TableExt;
	}

	TYPE*	GetData(int32 nIndex)
	{
		KCheck( nIndex>=0 && nIndex<m_Count );
		return &(m_TableExt[nIndex]);
	}

public:
	TYPE	m_TableExt[MAXSIZE];
	int32	m_Count;
};


/*
 =======================================================================================================================
 =======================================================================================================================
 */
class SplitStringLevelOne;
class SplitStringLevelTwo;
int32	CommonStringSplit
	(
		SplitStringLevelOne	*pSplitL1,
		SplitStringLevelTwo	*pSplitL2,
		const char		*pSplitString,
		int32 iMaxNum,
		int32 *pArry
	);

template<class T>
int32 CompareTemplateTB(const void *pArg1, const void *pArg2)
{
	__GUARD__;
	int32	nIndexID1, nIndexID2;

	nIndexID1 = *(int*)(const T *) pArg1;
	nIndexID2 = *(int*)(const T *) pArg2;

	if(nIndexID1 > nIndexID2)
		return 1;
	else if(nIndexID1 < nIndexID2)
		return -1;
	else
		return 0;
	__UNGUARD__ return 0;
}

// 表读取二叉查找模板 [xingzhishan]
template<class T>
T* GetItemBsearch(T *pTab, int32 nCount ,int32 nIndex)
{
	__GUARD__;
	T tb;
	(*(int*)&tb) = nIndex;

	T *pResult = (T *) bsearch
		(
		&tb,
		pTab,
		nCount,
		sizeof(T),
		(int32(*) (const void *, const void *))CompareTemplateTB<T>
		);

	if(pResult)
		return pResult;
	else
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "GetItemBsearch ItemSerial:%d  Get Errors", nIndex);
	}
	__UNGUARD__ return 0;
}

#endif