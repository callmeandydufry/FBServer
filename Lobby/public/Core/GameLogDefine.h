/*$T MapServer/Server/Base/LogDefine.h GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __RECORD_GAMEWAY_H_
#define __RECORD_GAMEWAY_H_

#include "TypeDefine.h"
#include "StructDB.h"
#include "record_format.h"
#include "record_struct.h"
#include "BaseTool.h"
#include "Setting.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */

template<typename PARAM>
inline void WRITE_GAMESYSTEM_LOG(const PARAM &Param)
{
}

template<typename PARAM>
inline void WRITE_GAMESYSTEM_LOG(const PARAM &Param0, const PARAM &Param1, ut_id_map, int32)
{
}

template<typename PARAM>
inline void WRITE_GAMESYSTEM_LOG(int opType,const PARAM &Param0, const PARAM &Param1, ut_id_map, int32)
{
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<>
inline void WRITE_GAMESYSTEM_LOG(const ItemLogRecordInfo &Param)
{
	__GUARD__;

	int32 WorldID = Param.WorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_WorldInfo.m_WorldID;

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_ITEMCHANGE,
			csRECORD_FORMAT_Item,
			GET_TIME().GetCTime(),
			Param.CharSNID,
			Param.Name,
			Param.IP,
			Param.MapID,
			Param.OpType,
			Param.OpDesc,
			Param.ItemType,
			Param.ItemSnid.m_World,
			Param.ItemSnid.m_Server,
			Param.ItemSnid.m_Serial,
			Param.Count,
			Param.TargetSNID,
			Param.SecOpType,
			Param.ContainerPos,
			Param.DestContainerPos,
			Param.NpcType,
			Param.XPos,
			Param.ZPos,
			Param.ShopSNID,
			Param.GemMax,
			Param.ScriptID,
			g_Config.m_ConfigInfo.m_ServerID,
			WorldID,
			Param.TempInvalid
		);

	__UNGUARD__;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<>
inline void WRITE_GAMESYSTEM_LOG(const PostLogRecordInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_POST_DB,
		csRECORD_FORMAT_Post,
		Param.m_Type,
		Param.m_Post.m_szSourName,
		Param.m_Post.m_szDestName,
		Param.m_Post.m_szTitle,
		Param.m_Post.m_uFlag,
		Param.m_Post.m_uCreateTime,
		Param.m_Post.m_PostAward.m_iItemNum,
		Param.m_Post.m_PostAward.m_iItemId[0],
		Param.m_Post.m_PostAward.m_uItemCount[0],
		Param.m_Post.m_PostAward.m_iItemId[1],
		Param.m_Post.m_PostAward.m_uItemCount[1],
		Param.m_Post.m_PostAward.m_iItemId[2],
		Param.m_Post.m_PostAward.m_uItemCount[2],
		Param.m_Post.m_PostAward.m_iItemId[3],
		Param.m_Post.m_PostAward.m_uItemCount[3]
		);	

	__UNGUARD__;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<>
inline void WRITE_GAMESYSTEM_LOG(const AwardPostLogRecordInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_AWARDPOST_DB,
		csRECORD_FORMAT_AwardPost,
		Param.m_Type,
		Param.m_Post.m_Snid,
		Param.m_Post.m_nPlayeType,
		Param.m_Post.m_szTitle,
		Param.m_Post.m_uCreateTime,
		Param.m_Post.m_Award.m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[0].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[0].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[1].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[1].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[2].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[2].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[3].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[3].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[4].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[4].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[5].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[5].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[6].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[6].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[7].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[7].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[8].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[8].m_nCount,
		Param.m_Post.m_Award.m_AwardPostItem[9].m_nItemID,
		Param.m_Post.m_Award.m_AwardPostItem[9].m_nCount
		);	

	__UNGUARD__;
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<>
inline void WRITE_GAMESYSTEM_LOG(const EquipLogRecordInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_EQUIP,
			csRECORD_FORMAT_Equip,
			GET_TIME().GetCTime(),
			Param.CharSNID,
			Param.Name,
			Param.ItemType,
			Param.ItemSnid.m_World,
			Param.ItemSnid.m_Server,
			Param.ItemSnid.m_Serial,
			Param.OpType,
			Param.Level,
			Param.m_Property[0].m_uPropertyID,
			Param.m_Property[0].m_uGrade,
			Param.m_Property[0].m_nValue,
			Param.m_Property[1].m_uPropertyID,
			Param.m_Property[1].m_uGrade,
			Param.m_Property[1].m_nValue,
			Param.m_Property[2].m_uPropertyID,
			Param.m_Property[2].m_uGrade,
			Param.m_Property[2].m_nValue,
			Param.m_Property[3].m_uPropertyID,
			Param.m_Property[3].m_uGrade,
			Param.m_Property[3].m_nValue,
			Param.m_Property[4].m_uPropertyID,
			Param.m_Property[4].m_uGrade,
			Param.m_Property[4].m_nValue,
			Param.m_RefinePropertyTmp[0].m_uPropertyID,
			Param.m_RefinePropertyTmp[0].m_uGrade,
			Param.m_RefinePropertyTmp[0].m_nValue,
			Param.m_RefinePropertyTmp[1].m_uPropertyID,
			Param.m_RefinePropertyTmp[1].m_uGrade,
			Param.m_RefinePropertyTmp[1].m_nValue,
			Param.m_RefinePropertyTmp[2].m_uPropertyID,
			Param.m_RefinePropertyTmp[2].m_uGrade,
			Param.m_RefinePropertyTmp[2].m_nValue,
			Param.m_RefinePropertyTmp[3].m_uPropertyID,
			Param.m_RefinePropertyTmp[3].m_uGrade,
			Param.m_RefinePropertyTmp[3].m_nValue,
			Param.m_RefinePropertyTmp[4].m_uPropertyID,
			Param.m_RefinePropertyTmp[4].m_uGrade,
			Param.m_RefinePropertyTmp[4].m_nValue
		);	

	__UNGUARD__;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<>
inline void WRITE_GAMESYSTEM_LOG(const CurrencyLogRecordInfo &Param)
{
	__GUARD__;

	int32 WorldID = Param.WorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_WorldInfo.m_WorldID;

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_MONEYCHANGE,
			csRECORD_FORMAT_Money,
			GET_TIME().GetCTime(),
			Param.CharSNID,
			Param.Name,
			Param.MapID,
			Param.OPType,
			Param.MoneyType,
			Param.Count,
			Param.RemainCount,
			Param.XPos,
			Param.ZPos,
			Param.TargetSNID,
			Param.ItemIndex,
			Param.ItemCount,
			Param.ScriptId,
			Param.nExt,
			g_Config.m_ConfigInfo.m_ServerID,
			WorldID
		);	
	__UNGUARD__;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<>
inline void WRITE_GAMESYSTEM_LOG(const PlayerLoginOutLogRecordInfo &Param)
{
	__GUARD__;

	int32 WorldID = Param.m_nWorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_WorldInfo.m_WorldID;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_PLAYERLOGINOUT,
		csRECORD_FORMAT_LoginOut,
		Param.m_nType,
		Param.m_nPlatformUid,
		Param.m_Account,
		Param.m_nCharSNID,
		Param.m_Name,
		Param.m_Mac,
		Param.m_Idfa,
		Param.m_Device,
		Param.m_DeviceOS,
		Param.m_nLoginTime,
		Param.m_nLogoutTime,
		Param.m_nOnlineTime,
		WorldID,
		Param.m_nChannel
		);	
	__UNGUARD__;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<>
inline void WRITE_GAMESYSTEM_LOG(const SpellLogRecordInfo &Param)
{
	__GUARD__;
#ifdef __GLSERVER__
	QLogSystem::SLOW_LOG_DB
		(
			QUICK_LOG_SKILL,
			SLOW_LOG_SKILL,
			csRECORD_FORMAT_Skill,
			Param.CharSNID,
			Param.OPType,
			Param.MapID,
			Param.XPos,
			Param.ZPos,
			Param.SkillID,
			Param.Level,
			Param.Count,
			2,
			g_Config.m_WorldInfo.m_WorldID
		);
#else
	int32 WorldID = Param.WorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_WorldInfo.m_WorldID;

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_SKILL,
			csRECORD_FORMAT_Skill,
			Param.CharSNID,
			Param.OPType,
			Param.MapID,
			Param.XPos,
			Param.ZPos,
			Param.SkillID,
			Param.Level,
			Param.Count,
			g_Config.m_ConfigInfo.m_ServerID,
			WorldID
		);
#endif
	__UNGUARD__;
}

template<>
inline void WRITE_GAMESYSTEM_LOG(const CheatActLogRecordInfo &Param)
{
	__GUARD__;
	int32 WorldID = Param.WorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_WorldInfo.m_WorldID;

	QLogSystem::QUICK_LOG
		(
			QUICK_LOG_MAPLAWLESSOP,
			csRECORD_FORMAT_Cheat,
			Param.CharName,
			Param.OwnerSNID,
			Param.OPType,
			Param.MapID,
			Param.Offset,
			Param.PosX,
			Param.PosZ,
			Param.Param,
			g_Config.m_ConfigInfo.m_ServerID,
			WorldID
		);
	__UNGUARD__;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
template<>
inline void WRITE_GAMESYSTEM_LOG(const IBInItemLogRecordInfo &Param)
{
	__GUARD__;

#ifdef __GLSERVER__
	QLogSystem::SLOW_LOG_DB
		(
		QUICK_LOG_IBMONEY,
		SLOW_LOG_GL_IBMONEY,
		csRECORD_FORMAT_IBMoney,
		Param.CharSNID,
		Param.Name,
		Param.OpType,
		Param.ItemType,
		Param.ItemSnid.m_World,
		Param.ItemSnid.m_Server,
		Param.ItemSnid.m_Serial,
		Param.Count,
		Param.GoldCash,
		Param.TermTime,
		2,
		g_Config.m_WorldInfo.m_WorldID
		);
#else
	int32 WorldID = Param.WorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_WorldInfo.m_WorldID;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_IBMONEY,
		csRECORD_FORMAT_IBMoney,
		Param.CharSNID,
		Param.Name,
		Param.OpType,
		Param.ItemType,
		Param.ItemSnid.m_World,
		Param.ItemSnid.m_Server,
		Param.ItemSnid.m_Serial,
		Param.Count,
		Param.GoldCash,
		Param.TermTime,
		g_Config.m_ConfigInfo.m_ServerID,
		WorldID
		);	
#endif

	__UNGUARD__;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
template<>
inline void WRITE_GAMESYSTEM_LOG(const IBOutItemLogRecordInfo &Param)
{
	__GUARD__;

#ifdef __GLSERVER__
	QLogSystem::SLOW_LOG_DB
		(
		QUICK_LOG_IBCOST,
		SLOW_LOG_GL_IBCOST,
		csRECORD_FORMAT_IBCost,
		Param.CharSNID,
		Param.Name,
		Param.OpType,
		Param.ItemType,
		Param.ItemSnid.m_World,
		Param.ItemSnid.m_Server,
		Param.ItemSnid.m_Serial,
		Param.Count,
		Param.GoldCash,
		2,
		g_Config.m_WorldInfo.m_WorldID
		);
#else
	int32 WorldID = Param.WorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_WorldInfo.m_WorldID;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_IBCOST,
		csRECORD_FORMAT_IBCost,
		Param.CharSNID,
		Param.Name,
		Param.OpType,
		Param.ItemType,
		Param.ItemSnid.m_World,
		Param.ItemSnid.m_Server,
		Param.ItemSnid.m_Serial,
		Param.Count,
		Param.GoldCash,
		g_Config.m_ConfigInfo.m_ServerID,
		WorldID
		);	
#endif

	__UNGUARD__;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
//template<>
//inline void WRITE_GAMESYSTEM_LOG(const GLStockTradeLogRecordInfo &Param)
//{
//	__GUARD__;
//	QLogSystem::SLOW_LOG_DB
//		(
//		-1,
//		SLOW_LOG_GL_STOCK_TRADE,
//		csRECORD_FORMAT_StockTrade,
//		Param.OwnerSNID,
//		Param.OtherSNID,
//		Param.OPType,
//		Param.TradeType,
//		Param.OwnerPrice,
//		Param.OtherPrice,
//		Param.Gold,
//		Param.CreateTime,
//		Param.DeleteTime,
//		2,
//		g_Config.m_WorldInfo.m_WorldID
//		);
//	__UNGUARD__;
//}

/*
=======================================================================================================================
=======================================================================================================================
*/
//template<>
//inline void WRITE_GAMESYSTEM_LOG(const MapStockAccLogRecordInfo &Param)
//{
//	__GUARD__;
//	int32 WorldID = Param.WorldID;
//	if( WorldID == 0 )
//		WorldID = g_Config.m_WorldInfo.m_WorldID;
//
//	QLogSystem::QUICK_LOG
//		(
//		QUICK_LOG_MAPSTOCKACC,
//		csRECORD_FORMAT_StockAcc,
//		Param.OwnerSNID,
//		Param.OPType,
//		Param.TransGold,
//		Param.TransSiller,
//		Param.StockGold,
//		Param.StockSiller,
//		g_Config.m_ConfigInfo.m_ServerID,
//		WorldID
//		);
//
//	__UNGUARD__;
//}

/*
=======================================================================================================================
=======================================================================================================================
*/
//template<>
//inline void WRITE_GAMESYSTEM_LOG(const GLStockAccLogRecordInfo &Param)
//{
//	__GUARD__;
//	QLogSystem::SLOW_LOG_DB
//		(
//		-1,
//		SLOW_LOG_GL_STOCK_ACC,
//		csRECORD_FORMAT_StockAcc,
//		Param.OwnerSNID,
//		Param.OPType,
//		Param.TransGold,
//		Param.TransSiller,
//		Param.StockGold,
//		Param.StockSiller,
//		2,
//		g_Config.m_WorldInfo.m_WorldID
//		);
//	__UNGUARD__;
//}

/*
=======================================================================================================================
=======================================================================================================================
*/
//template<>
//inline void WRITE_GAMESYSTEM_LOG(const GLGuildLogRecordInfo &Param)
//{
//	__GUARD__;
//	QLogSystem::SLOW_LOG_DB
//		(
//		-1,
//		SLOW_LOG_GL_GUILD,
//		csRECORD_FORMAT_Guild,
//		Param.m_GuildId,
//		Param.m_szGuildName,
//		Param.m_OpType,
//		Param.m_nCount,
//		Param.m_nRemainCount,
//		Param.m_nParam,
//		Param.m_nScriptId,
//		2,
//		g_Config.m_WorldInfo.m_WorldID
//		);
//
//	__UNGUARD__;
//}

/*
=======================================================================================================================
=======================================================================================================================
*/
//template<>
//inline void WRITE_GAMESYSTEM_LOG(const ZhuanJingLogRecordInfo &Param)
//{
//	__GUARD__;
//
//	int32 WorldID = Param.WorldID;
//	if( WorldID == 0 )
//		WorldID = g_Config.m_WorldInfo.m_WorldID;
//
//	QLogSystem::QUICK_LOG
//		(
//		QUICK_LOG_ZHUANJING,
//		csRECORD_FORMAT_ZhuanJing,
//		Param.CharSNID,
//		Param.OPType,
//		Param.MapID,
//		Param.XPos,
//		Param.ZPos,
//		Param.TotalPoint,
//		Param.Profession[PROFESSION_WUSHI],
//		Param.Profession[PROFESSION_JIANXIA],
//		Param.Profession[PROFESSION_QISHE],
//		Param.Profession[PROFESSION_HUOQIANG],
//		Param.Profession[PROFESSION_XIANZHI],
//		Param.Profession[PROFESSION_SAMAN],
//		Param.Profession[PROFESSION_SHIZIJUN],
//		Param.Profession[PROFESSION_SHENGHUOSHI],
//		Param.Profession[PROFESSION_CIKE],
//		Param.Profession[PROFESSION_JINWEIJUN],
//		Param.Profession[PROFESSION_YINXIUSHI],
//		Param.Profession[PROFESSION_LAMA],
//		Param.Reserve_param0,
//		Param.Reserve_param1,
//		g_Config.m_ConfigInfo.m_ServerID,
//		WorldID
//		);	
//
//	__UNGUARD__;
//}

/*
=======================================================================================================================
=======================================================================================================================
*/
template<>
inline void WRITE_GAMESYSTEM_LOG(const PlayerUseSkillCount &Param)
{
	__GUARD__;

	int32 WorldID = Param.m_WorldID;
	if( WorldID == 0 )
		WorldID = g_Config.m_ConfigInfo.m_ServerID;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_PLAYERUSESKILL,
		csRECORD_FORMAT_PlayerUseSkillCount,
		Param.m_CharGUID,
		Param.m_MapID,
		Param.m_XPos,
		Param.m_ZPos,
		Param.m_Count,
		Param.m_Profession,
		Param.m_SkillID,
		g_Config.m_ConfigInfo.m_ServerID
		);

	__UNGUARD__;
}

template<>
inline void WRITE_GAMESYSTEM_LOG(const BillingOrderInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_ORDER_BILLING,
		csRECORD_FORMAT_BillingOrderInfo,
		Param.m_OrderId,
		Param.m_nUserId,
		Param.m_ProductId,
		Param.m_nPayMoney,
		Param.m_nBindPoints,
		Param.m_nPoints,
		Param.m_nAdditionalPoints,
		Param.m_PayTime,
		Param.m_nChannelId
		);

	__UNGUARD__;
}

// 记录英雄养成db日志 [wangyiran]
template<>
inline void WRITE_GAMESYSTEM_LOG(const HeroGrowLogInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		QUICK_LOG_DB_HEROGROW,
		csRECORD_FORMAT_HeroGrowInfo,
		Param.m_nPlayerId,
		Param.m_PlayerName,
		Param.m_nHeroTabIndex,
		Param.m_nHeroLvl,
		Param.m_nHeroGrade,
		Param.m_nConstellation,
		Param.m_nConstellatiionStar,
		Param.m_nHeroSkill1Lvl,
		Param.m_nHeroSkill2Lvl,
		(int32)Param.op
		);

	__UNGUARD__;
}

// 记录神器养成日志 tianshuai
template<>
inline void WRITE_GAMESYSTEM_LOG(const ArtifactLogInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		SLOW_LOG_PLAYER_ARTIFACT,
		csRECORD_FORMAT_ArtifactLogInfo,
		Param.m_nPlayerId,
		Param.m_PlayerName,
		Param.ArtifactId,
		Param.ArtifactLevel,
		Param.m_nPlayerLevel,
		(int32)Param.op
		);

	__UNGUARD__;
}

//  [4/22/2016 tianshuai] 记录寻宝日志
template<>
inline void WRITE_GAMESYSTEM_LOG(const FindTreasureLogInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		SLOW_LOG_PLAYER_FIND_TREASURE,
		csRECORD_FORMAT_FindTreasureLogInfo,
		Param.m_nPlayerId,
		Param.m_PlayerName,
		Param.m_nParam1,
		Param.m_nParam2,
		(int32)Param.op
		);

	__UNGUARD__;
}


//  [4/22/2016 tianshuai] 记录英雄转移日志
template<>
inline void WRITE_GAMESYSTEM_LOG(const HeroTransferLogInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		SLOW_LOG_PLAYER_HERO_TRANSFER,
		csRECORD_FORMAT_HeroTransferLogInfo,
		Param.m_nPlayerId,
		Param.m_PlayerName,
		Param.m_nFromHeroTabIndex,
		Param.m_nFParam1,
		Param.m_nFParam2,
		Param.m_nFParam3,
		Param.m_nFParam4,
		Param.m_nFParam5,
		Param.m_nFParam6,
		Param.m_nFParam7,
		Param.m_nFParam8,
		Param.m_nFParam9,
		Param.m_nFParam10,
		Param.m_nFParam11,
		Param.m_nFParam12,
		Param.m_nFParam13,
		Param.m_nFParam14,
		Param.m_nToHeroTabIndex,
		Param.m_nTParam1,
		Param.m_nTParam2,
		Param.m_nTParam3,
		Param.m_nTParam4,
		Param.m_nTParam5,
		Param.m_nTParam6,
		Param.m_nTParam7,
		Param.m_nTParam8,
		Param.m_nTParam9,
		Param.m_nTParam10,
		Param.m_nTParam11,
		Param.m_nTParam12,
		Param.m_nTParam13,
		Param.m_nTParam14,
		(int32)Param.op
		);

	__UNGUARD__;
}

// 契约养成 [11/23/2016 TianShuai]
template<>
inline void WRITE_GAMESYSTEM_LOG(const IndentureLogInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		SLOW_LOG_PLAYER_INDENTURE,
		csRECORD_FORMAT_IndentureLogInfo,
		Param.m_nPlayerId,
		Param.m_PlayerName,

		Param.m_nIndentureIndex,
		Param.m_nIndentureType,
		Param.m_nIndentureQuality,
		Param.m_nIndentureUseCount,
		Param.m_nIndentureFuseType1,
		Param.m_nIndentureFuseQuality1,
		Param.m_nIndentureFuseType2,
		Param.m_nIndentureFuseQuality2,
		Param.m_nIndentureFuseType3,
		Param.m_nIndentureFuseQuality3,
		Param.m_nIndentureFuseType4,
		Param.m_nIndentureFuseQuality4,
		(int32)Param.op
		);

	__UNGUARD__;
}

// 特殊效果 如金币加成 [7/6/2017 "tianshuai"]
template<>
inline void WRITE_GAMESYSTEM_LOG(const EffectUpInfo &Param)
{
	__GUARD__;

	QLogSystem::QUICK_LOG
		(
		SLOW_LOG_PLAYER_EFFECT_UP_DATA,
		csRECORD_FORMAT_PlayerEffectUp,
		Param.m_nPlayerId,
		Param.m_PlayerName,
		Param.m_nEffectType,
		Param.m_nAddTime,
		Param.m_nLastTime,
		Param.m_nEndTime,
		(int32)Param.op
		);

	__UNGUARD__;
}

#endif
