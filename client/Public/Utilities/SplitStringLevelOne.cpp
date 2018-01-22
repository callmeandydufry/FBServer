/*$T Share/Utilities/SplitStringLevelOne.cpp GC 1.140 10/10/07 10:06:57 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "SplitStringLevelOne.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */

BOOL SplitStringLevelOne::DoSplit(const char *strSplit)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	KCheck(strSplit != NULL && m_SplitChar != 0 && "SplitStringLevelOne::DoSplit");
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	uint32 nStrlen = (uint32)strlen(strSplit);
	KCheck(nStrlen <= MAX_INPUT_STRING_LENGTH && "SplitStringLevelOne::DoSplit");
	if(nStrlen == 0)
		return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	size_t	iPos = strcspn(strSplit, m_SplitChar);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	// 说明在开头的位置找到了分隔符，跳过这个无效的分隔符 [yangzhen]
	if(iPos == 0)
	{
		strSplit += iPos + 1;
		iPos = strcspn(strSplit, m_SplitChar);
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32	iIndex = m_iLevelOneLineCount;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	while(iPos)
	{
		KCheck(iIndex < MAX_LEVEL_ONE_LENGTH && "SplitStringLevelOne::DoSplit");
		KCheck(iPos < MAX_SPLIT_STRING_LENGTH && "SplitStringLevelOne::DoSplit");

		if(iIndex >= MAX_LEVEL_ONE_LENGTH || iPos >= MAX_SPLIT_STRING_LENGTH) return FALSE;

		strncpy(m_strSplitLevelOne[iIndex], strSplit, iPos);
		++iIndex;

		if(*(strSplit + iPos) == 0) break;

		strSplit += iPos + 1;

		iPos = strcspn(strSplit, m_SplitChar);
	}

	m_iLevelOneLineCount = iIndex;
	DoSubSplit();

	return TRUE;

	__UNGUARD__ return FALSE;
}
