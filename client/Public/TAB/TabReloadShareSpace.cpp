/*$T GLServer/TabReloadNameSpaceMisc.cpp GC */


/*$6
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
命名空间中的函数实现
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include "stdafx.h"
#include "TabReload.h"
#include "SplitStringLevelOne.h"
#include "SplitStringLevelTwo.h"
#include "TAB.h"

using namespace TAB;
// 
// namespace TabReloadDefine
// {
// 	/*
// 	=======================================================================================================================
// 	=======================================================================================================================
// 	*/
// 	void ReadTiemrDoingData
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		int32			*pVal,
// 		int32			MaxCnt
// 		)
// 	{
// 		pSplitL1->Init('|', pSplitL2);
// 		pSplitL2->Init('~', NULL);
// 		pSplitL1->DoSplit(pSplitString);
// 
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 		int32	iLineCount = pSplitL2->GetResultLineCount();
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 
// 		KCheck(MaxCnt >= iLineCount);
// 		if(MaxCnt < iLineCount)
// 		{
// 			return;
// 		}
// 
// 		for(int32 id = 0; id < iLineCount; ++id)
// 		{
// 			if(atoi(pSplitL2->GetResultLine(0)) == 0)
// 			{
// 				for(int32 _Idx = 0; _Idx < MaxCnt; _Idx++)
// 				{
// 					pVal[id] = 1;
// 					return;
// 				}
// 			}
// 			else
// 				pVal[atoi(pSplitL2->GetResultLine(id))] = atoi(pSplitL2->GetResultLine(id));
// 		}
// 	}
// 
// 	/*
// 	=======================================================================================================================
// 	=======================================================================================================================
// 	*/
// 	void ReadTiemrDoingData_MapList
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		ut_id_map			*pVal,
// 		int32			MaxCnt
// 		)
// 	{
// 		pSplitL1->Init('|', pSplitL2);
// 		pSplitL2->Init('~', NULL);
// 		pSplitL1->DoSplit(pSplitString);
// 
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 		int32	iLineCount = pSplitL2->GetResultLineCount();
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 
// 		KCheck(MaxCnt >= iLineCount);
// 		if(MaxCnt < iLineCount)
// 		{
// 			return;
// 		}
// 
// 		/*~~~~~~~~~~~~~~~~~~*/
// 		int32	nCurCount = 0;
// 		/*~~~~~~~~~~~~~~~~~~*/
// 
// 		for(int32 id = 0; id < iLineCount; ++id)
// 		{
// 			if(atoi(pSplitL2->GetResultLine(id)) != -1)
// 			{
// 				if(nCurCount < TIMER_DOING_MAP_LIST) pVal[nCurCount++] = atoi(pSplitL2->GetResultLine(id));
// 			}
// 		}
// 	}
// 
// 	/*
// 	=======================================================================================================================
// 	=======================================================================================================================
// 	*/
// 	void ReadTiemrDoingReserveEX
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		int32			*pVal,
// 		int32			MaxCnt
// 		)
// 	{
// 		pSplitL1->Init('|', pSplitL2);
// 		pSplitL2->Init('~', NULL);
// 		pSplitL1->DoSplit(pSplitString);
// 
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 		int32	iLineCount = pSplitL2->GetResultLineCount();
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 
// 		KCheck(MaxCnt >= iLineCount);
// 		if(MaxCnt < iLineCount)
// 		{
// 			return;
// 		}
// 
// 		for(int32 id = 0; id < iLineCount; ++id)
// 		{
// 			pVal[id] = atoi(pSplitL2->GetResultLine(id));
// 		}
// 	}
// 
// 	/*
// 	=======================================================================================================================
// 	=======================================================================================================================
// 	*/
// 	void ReadTiemrDoingMapInfo
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		int32			*pVal1,
// 		int32			*pVal2,
// 		int32			MaxCnt
// 		)
// 	{
// 		pSplitL1->Init('|', pSplitL2);
// 		pSplitL2->Init('~', NULL);
// 		pSplitL1->DoSplit(pSplitString);
// 
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 		int32	iLineCount = pSplitL2->GetResultLineCount();
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 
// 		KCheck(MaxCnt >= iLineCount);
// 		if(MaxCnt < iLineCount)
// 		{
// 			return;
// 		}
// 		{
// 			*pVal1 = atoi(pSplitL2->GetResultLine(0));
// 			*pVal2 = atoi(pSplitL2->GetResultLine(1));
// 		}
// 	}
// 
// 	/*
// 	=======================================================================================================================
// 	=======================================================================================================================
// 	*/
// 	void ReadSplitStringToTable(SplitStringLevelOne *pSplitL1, const char *pSplitString, int32 *pArry)
// 	{
// 		pSplitL1->Reset();
// 		pSplitL1->Init('|', NULL);
// 		pSplitL1->DoSplit(pSplitString);
// 
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 		int32	iLineCount = pSplitL1->GetResultLineCount();
// 		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// 
// 		KCheck(MAX_GUILDSKILL_LEVEL >= iLineCount && "天赋修行级别超过最大限度");
// 		if(MAX_GUILDSKILL_LEVEL < iLineCount)
// 		{
// 			iLineCount = MAX_GUILDSKILL_LEVEL;
// 		}
// 
// 		for(int32 _Idx = 0; _Idx < iLineCount; ++_Idx)
// 		{
// 			pArry[_Idx] = atoi(pSplitL1->GetResultLine(_Idx));
// 		}
// 	}
// 
// }