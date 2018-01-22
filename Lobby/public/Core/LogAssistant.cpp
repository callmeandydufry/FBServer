/*$T MapServer/Server/Base/LogAssistant.cpp GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void FillServerIPOfItemLog(ItemLogRecordInfo *pLogParam)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ int32 ServerID = g_pServerConnectorMgr->GetCurServerID();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(ServerID < 0 || ServerID >= g_Config.m_ServerInfo.m_ServerCount)
	{
		KCheck(FALSE);
	}

	strncpy(pLogParam->IP, g_Config.m_ServerInfo.m_pServer[ServerID].m_IP[0], sizeof(char) * IP_SIZE);
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FillItemLogCommonInf(ItemLogRecordInfo *pLogParam, Player *pPlayer)
{
	__GUARD__ 
	if (pPlayer->GetMap())
	{
		pLogParam->MapID = pPlayer->GetMap()->MapID();
	}
	else
	{
		pLogParam->MapID = INVALID_ID;
	}
	pLogParam->XPos = pPlayer->GetGLPos()->m_fX;
	pLogParam->ZPos = pPlayer->GetGLPos()->m_fZ;
	pLogParam->CharSNID = pPlayer->GetSNID();
	strncpy(pLogParam->Name, pPlayer->GetName(), sizeof(char) * MAX_KHAN_CHARACTER_NAME - 1);
	FillServerIPOfItemLog(pLogParam);
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
//void FillServerIPOfEquipLog(EquipLogRecordInfo *pLogParam)
//{
//	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//	__GUARD__ int32 ServerID = g_pServerConnectorMgr->GetCurServerID();
//	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//
//	if(ServerID < 0 || ServerID >= g_Config.m_ServerInfo.m_ServerCount)
//	{
//		KCheck(FALSE);
//	}
//
//	strncpy(pLogParam->IP, g_Config.m_ServerInfo.m_pServer[ServerID].m_IP[0], sizeof(char) * IP_SIZE);
//	__UNGUARD__
//}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
//void FillEquipLogCommonInf(EquipLogRecordInfo *pLogParam, Player *pPlayer, Item *pEquip)
//{
//	__GUARD__ pLogParam->CharSNID = pPlayer->GetSNID();
//	strncpy(pLogParam->Name, pPlayer->GetName(), sizeof(char) * MAX_KHAN_CHARACTER_NAME - 1);
//	pLogParam->MapID = pPlayer->GetMap()->MapID();
//	FillServerIPOfEquipLog(pLogParam);
//	pLogParam->ItemSnid = pEquip->GetSNID();
//	pLogParam->ItemType = pEquip->Get_Item_TableIndex();
//	pLogParam->Level = 0; 
//
//	__UNGUARD__
//}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FillMoneyLogCommonInf(CurrencyLogRecordInfo *pLogParam, Player *pPlayer)
{
	pLogParam->MapID = NULL != pPlayer->GetMap() ? pPlayer->GetMap()->MapID() : INVALID_ID;
	pLogParam->XPos = pPlayer->GetGLPos()->m_fX;
	pLogParam->ZPos = pPlayer->GetGLPos()->m_fZ;
	pLogParam->CharSNID = pPlayer->GetSNID();
	strncpy(pLogParam->Name, pPlayer->GetName(), sizeof(char) * MAX_KHAN_CHARACTER_NAME - 1);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
//void FillStallLogCommonInf(TradeLogRecordInfo *pLogParam, Player *pBuyer, Player *pSeller, Item *pItem, int32 nCount, uchar MoneyType)
//{
//	KCheck(pBuyer && pSeller && pItem);
//	pLogParam->m_anTradeParam[0].m_nCharSnid = pBuyer->GetSNID();
//	pLogParam->m_anTradeParam[1].m_nCharSnid = pSeller->GetSNID();
//	tsnprintf_s(pLogParam->m_anTradeParam[0].m_szCharName, MAX_KHAN_CHARACTER_NAME, "%s", pBuyer->GetName());
//	tsnprintf_s(pLogParam->m_anTradeParam[1].m_szCharName, MAX_KHAN_CHARACTER_NAME, "%s", pSeller->GetName());
//	pLogParam->m_anTradeParam[0].m_nMapId = pBuyer->GetMap()->MapID();
//	pLogParam->m_anTradeParam[1].m_nMapId = pSeller->GetMap()->MapID();
//
//	if( MoneyType == 0 )
//	{
//		pLogParam->m_anTradeParam[0].m_nTradeType = TradeLogRecordInfo::TradeParam::eStallSiller;
//		pLogParam->m_anTradeParam[1].m_nTradeType = TradeLogRecordInfo::TradeParam::eStallSiller;
//		pLogParam->m_anTradeParam[0].m_nPreGold = pBuyer->GetSiller();
//		pLogParam->m_anTradeParam[1].m_nPreGold = pSeller->GetSiller();
//	}
//	else if( MoneyType == 1 )
//	{
//		pLogParam->m_anTradeParam[0].m_nTradeType = TradeLogRecordInfo::TradeParam::eStallGold;
//		pLogParam->m_anTradeParam[1].m_nTradeType = TradeLogRecordInfo::TradeParam::eStallGold;
//		pLogParam->m_anTradeParam[0].m_nPreGold = pBuyer->GetGold();
//		pLogParam->m_anTradeParam[1].m_nPreGold = pSeller->GetGold();	
//	}
//
//	pLogParam->m_anTradeParam[0].m_anItem[0].m_nItemId = pItem->Get_Item_TableIndex();
//	pLogParam->m_anTradeParam[0].m_anItem[0].m_nItemSnid = pItem->GetSNID();
//	pLogParam->m_anTradeParam[0].m_anItem[0].m_nCount = nCount;
//	pLogParam->m_anTradeParam[1].m_anItem[0].m_nItemId = pItem->Get_Item_TableIndex();
//	pLogParam->m_anTradeParam[1].m_anItem[0].m_nItemSnid = pItem->GetSNID();
//	pLogParam->m_anTradeParam[1].m_anItem[0].m_nCount = nCount;
//}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
//void FillGamePlayLog(GameWayLogRecoreInfo *pLogParam, Player *pPlayer, int32 nGameID)
//{
//	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//	__GUARD__	KCheck(pLogParam);
//	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//
//	KCheck(pPlayer);
//	KCheck(pPlayer->GetMap());
//	pLogParam->MapID = pPlayer->GetMap()->MapID();
//	pLogParam->CharSNID = pPlayer->GetSNID();
//	strncpy(pLogParam->Name, pPlayer->GetName(), sizeof(char) * MAX_KHAN_CHARACTER_NAME - 1);
//	pLogParam->GameID = nGameID;
//
//	__UNGUARD__
//}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
//void FillCSEnterLog(EnterMapLogRecordInfo *pLogParam, Player *pPlayer, int32 nGameID, int32 nDayCount)
//{
//	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//	__GUARD__	KCheck(pLogParam);
//	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//
//	KCheck(pPlayer);
//	KCheck(pPlayer->GetMap());
//	pLogParam->MapID = pPlayer->GetMap()->MapID();
//	pLogParam->CharSNID = pPlayer->GetSNID();
//	strncpy(pLogParam->Name, pPlayer->GetName(), sizeof(char) * MAX_KHAN_CHARACTER_NAME - 1);
//	pLogParam->GameID = nGameID;
//	pLogParam->DayCount = nDayCount;
//	__UNGUARD__
//}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void FillLawlessLog(CheatActLogRecordInfo *pLogParam, Player *pPlayer)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	KCheck(pLogParam);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	KCheck(pPlayer);

	tsnprintf_s(pLogParam->CharName, sizeof(pLogParam->CharName), "%s", pPlayer->GetName());
	pLogParam->OwnerSNID = pPlayer->GetSNID();
	pLogParam->MapID = pPlayer->GetMapID_i();
	pLogParam->Offset = pPlayer->GetVerifyOffset();
	pLogParam->PosX = pPlayer->GetGLPos()->m_fX;
	pLogParam->PosZ = pPlayer->GetGLPos()->m_fZ;
	__UNGUARD__
}
