#include "stdafx.h"
#include "TabStruct.h"
#include "SplitStringLevelOne.h"
#include "SplitStringLevelTwo.h"


/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 CommonStringSplit
(
	SplitStringLevelOne	*pSplitL1,
	SplitStringLevelTwo	*pSplitL2,
	const char		*pSplitString,
	int32			iMaxNum,
	int32			*pArry
)
{
	__GUARD__ 
	pSplitL1->Reset();
	pSplitL2->Reset();
	pSplitL1->Init('|', pSplitL2);
	pSplitL2->Init('~', NULL);
	pSplitL1->DoSplit(pSplitString);

	int32	iLineCount = pSplitL2->GetResultLineCount();

	KCheck(iMaxNum >= iLineCount);
	for(int32 _Idx = 0; _Idx < iLineCount; ++_Idx)
	{
		pArry[_Idx] = atoi(pSplitL2->GetResultLine(_Idx));
	}

	return iLineCount;
	__UNGUARD__ 
	return 0;
}
