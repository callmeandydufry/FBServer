/*$T Common/DefineResult.cpp GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "TypeDefine.h"
#include <map>
#include "DefineResult.h"

struct SResultText
{
	eOPTRESULT	nCode;
	const char* pszText;
};

static SResultText	s_aResultText[] =
{
	{ ORESULT_OK, "ORESULT_OK" },
	{ ORESULT_ERROR, "ORESULT_ERROR" },
	{ ORESULT_DIE, "ORESULT_DIE" },
	{ ORESULT_INVALID_SKILL, "ORESULT_INVALID_SKILL" },
	{ ORESULT_TARGET_DIE, "ORESULT_TARGET_DIE" },
	{ ORESULT_LACK_MANA, "ORESULT_LACK_MANA" },
	{ ORESULT_COOL_DOWNING, "ORESULT_COOL_DOWNING" },
	{ ORESULT_NO_TARGET, "ORESULT_NO_TARGET" },
	{ ORESULT_INVALID_TARGET, "ORESULT_INVALID_TARGET" },
	{ ORESULT_OUT_RANGE, "ORESULT_OUT_RANGE" },
	{ ORESULT_IN_RANGE, "ORESULT_IN_RANGE" },
	{ ORESULT_NO_PLATFORM, "ORESULT_NO_PLATFORM" },
	{ ORESULT_OUT_PLATFORM, "ORESULT_OUT_PLATFORM" },
	{ ORESULT_NO_TOOL, "ORESULT_NO_TOOL" },
	{ ORESULT_STUFF_LACK, "ORESULT_STUFF_LACK" },
	{ ORESULT_BAG_OUT_OF_SPACE, "ORESULT_BAG_OUT_OF_SPACE" },
	{ ORESULT_WARNING, "ORESULT_WARNING" },
	{ ORESULT_BUSY, "ORESULT_BUSY" },
	{ ORESULT_CHAR_DO_NOT_KNOW_THIS_SKILL, "ORESULT_CHAR_DO_NOT_KNOW_THIS_SKILL" },
	{ ORESULT_NOT_ENOUGH_HP, "ORESULT_NOT_ENOUGH_HP" },
	{ ORESULT_NOT_ENOUGH_RAGE, "ORESULT_NOT_ENOUGH_RAGE" },
	{ ORESULT_NOT_ENOUGH_STRIKE_POINT, "ORESULT_NOT_ENOUGH_STRIKE_POINT" },
	{ ORESULT_NOT_ENOUGH_ITEM, "ORESULT_NOT_ENOUGH_ITEM" },
	{ ORESULT_FAILURE, "ORESULT_FAILURE" },
	{ ORESULT_INVALID_TARGET_POS, "ORESULT_INVALID_TARGET_POS" },
	{ ORESULT_LIMIT_MOVE, "ORESULT_LIMIT_MOVE" },
	{ ORESULT_LIMIT_USE_SKILL, "ORESULT_LIMIT_USE_SKILL" },
	{ ORESULT_INACTIVE_SKILL, "ORESULT_INACTIVE_SKILL" },
	{ ORESULT_U_CANNT_DO_THIS_RIGHT_NOW, "ORESULT_U_CANNT_DO_THIS_RIGHT_NOW" },
	{ ORESULT_NEED_HIGH_LEVEL, "ORESULT_NEED_HIGH_LEVEL" },
	{ ORESULT_EXP_FULL, "ORESULT_EXP_FULL" },
	{ ORESULT_IMPASSABLE_ZONE, "ORESULT_IMPASSABLE_ZONE" },
	{ ORESULT_NOT_ENOUGH_MONEY, "ORESULT_NOT_ENOUGH_MONEY" },
	{ ORESULT_TRY_CATCH_RETURN, "ORESULT_TRY_CATCH_RETURN" },
	{ ORESULT_NULL_POINTER, "ORESULT_NULL_POINTER" },
	{ ORESULT_NULL_MAP, "ORESULT_NULL_MAP" },
	{ ORESULT_HAVENOT_LEARN_THIS_SKILL, "ORESULT_HAVENOT_LEARN_THIS_SKILL" },
	{ ORESULT_DONOT_HAVE_ENOUGH_SKILL_POINT, "ORESULT_DONOT_HAVE_ENOUGH_SKILL_POINT" },
	{ ORESULT_HAVENOT_LEARN_PARENT_SKILL, "ORESULT_HAVENOT_LEARN_PARENT_SKILL" },
	{ ORESULT_CANNOT_FIND_THIS_SKILL_IN_STUDY_TABLE, "ORESULT_CANNOT_FIND_THIS_SKILL_IN_STUDY_TABLE" },
	{ ORESULT_THIS_SKILL_IS_MAX_LEVEL, "ORESULT_THIS_SKILL_IS_MAX_LEVEL" },
	{ ORESULT_CHANGE_STATE_FAIL, "ORESULT_CHANGE_STATE_FAIL" },
	{ ORESULT_CHARACTER_IS_UNACTIVE, "ORESULT_CHARACTER_IS_UNACTIVE" },
	{ ORESULT_SKILL_STATE_ERROR, "ORESULT_SKILL_STATE_ERROR" },
	{ ORESULT_CANNOT_FIND_MOVE_PATH, "ORESULT_CANNOT_FIND_MOVE_PATH" },
	{ ORESULT_DEFAULT_ERROR, "ORESULT_DEFAULT_ERROR" },
	{ ORESULT_IS_ENEMY_CAMP, "ORESULT_IS_ENEMY_CAMP" },
	{ ORESULT_TEAM_FOLLOW_STATE, "ORESULT_TEAM_FOLLOW_STATE" },
	{ ORESULT_CANNOT_FIND_NPC, "ORESULT_CANNOT_FIND_NPC" },
	{ ORESULT_IS_PASSIVE_SKILL, "ORESULT_IS_PASSIVE_SKILL" },
	{ ORESULT_SCAN_ERROR, "ORESULT_SCAN_ERROR" },
	{ ORESULT_TALK_TOO_FAST, "ORESULT_TALK_TOO_FAST" },
	{ ORESULT_POST_TOO_FAST, "ORESULT_POST_TOO_FAST" },
	{ ORSEULT_NOT_Star_CONDITION_LESS ,"HeroGrow_12"},
	{ ORESULT_HEROCARD_NOT_EXITS, "HeroGrow_1"},
	{ ORESULT_HEROCARD_POS_ERROR, "HeroGrow_1"},
	{ ORESULT_HEROCARD_SPELL_NOT_EXITS, "HeroCard_Spell_Not_Exits"},
	{ ORESULT_HEROCARD_SPELL_LEVEL_MAX, "HeroCard_Spell_Max_Level"},
	{ ORESULT_NEEDITEM_LESS, "no_item"},
	{ ORESULT_GEM_LESSMONEY, "Baoshi_Lostmoney"},
	{ ORESULT_SYS_ERROR, "MESSAGE_BOX_TITLE_TXT"},
	{ ORESULT_BAG_FULL, "stall_package_full"},
	{ ORESULT_ALREADY_HAVE_ITEM, "Bag_error_already_have"},
	{ ORESULT_BAG_TYPE_ERROR, "Bag_error_type"},
	{ ORESULT_ITEM_NOT_EXITS, "BagItem_10"},
	{ ORESULT_BODY_POS_ERROR, "Body_error_pos"},
	{ ORESULT_NOT_ENOUGH_CU_GOLD, "no_diamon" }, 
	{ ORESULT_NOT_ENOUGH_CU_SILLER, "no_gold" }, 
	{ ORESULT_NOT_ENOUGH_CU_BEYONGOD, "no_herosoul"},
	{ ORESULT_NOT_ENOUGH_CU_CRYSTAL, "no_crystal"},
	{ ORESULT_NOT_ENOUGH_CU_DotaCoin, "no_DotaCoin"},
	{ ORESULT_NOT_ENOUGH_CU_HONOREXP, "HeroGrow_4"},
	{ ORESULT_NOT_ENOUGH_CU_MAGICSTAL, "no_magicstal"},
	{ ORESULT_NOT_ENOUGH_CU_RECALLPAPER, "no_recallpaper"},
	{ ORESULT_NOT_ENOUGH_CU_PHYSICALPOWER, "no_physicalpower"},
	{ ORESULT_NOT_ENOUGH_CU_SPIRIT, "no_spirit"},
	{ ORESULT_BATTLE_ENTER_FAILED_LESS_LEVEL, "battle_level_failed"},
	{ ORESULT_BATTLE_ENTER_FAILED_NO_PREBATTLE, "battle_pre_battle_not_finish"},
	{ ORESULT_BATTLE_ENTER_SUCCESS, "battle_enter_success" },
	{ ORESULT_BATTLE_OPEN_AWARDBOX_ALREADYOPENED, "battle_awardbox_alreadyopened"},
	{ ORESULT_BATTLE_OPEN_AWARDBOX_STARNOTENOUGH, "battle_awardbox_starnotenough"},
	{ ORESULT_STAR_MAX, "HeroGrow_5"},
	{ ORESULT_CONFIG_ERROR, "HeroGrow_6"},
	{ ORESULT_Star_ITEMLESS, "HeroGrow_7"},
	{ ORESULT_PLAYER_LEVEL_LESS, "HeroGrow_13"},
	{ ORESULT_FORMALBAGFULL_EQUIP, "BagItem_9"},
	{ ORESULT_FORMALBAGFULL_MEDIC, "formal_bag_full_medic"},
	{ ORESULT_FORMALBAGFULL_GEM, "formal_bag_full_gem"},
	{ ORESULT_FORMALBAGFULL_TREASURE, "formal_bag_full_treasure"},
	{ ORESULT_ACHIEVE_NOT_DONE, "achieve_not_done"},
	{ ORESULT_ACHIEVE_HAS_FINISH, "achieve_has_finish" },
	{ ORESULT_FORMALBAGFULL_HERO, "hero_bag_full" },
	{ ORESULT_BUY_ARENACOUNT_OK, "arena_buy_count_ok"},
	{ ORESULT_DAILYTASK_NOT_OPEN, "error_dailytask_notopen"},
	{ ORESULT_DAILYTASK_NOT_FINISH, "error_dailytask_notfinish"},
	{ ORESULT_DAILYTASK_HASGOT, "error_dailytask_hasgot"},
	{ ORESULT_HEROLEVEL_LESS, "error_hero_lvl_less"},
	{ ORESULT_BUY_SUCCESS, "Buy_Success"},
	{ ORESULT_HEROCARD_LVL_MAX, "HeroGrow_2"},
	{ ORESULT_CDKEY_NO_EXIST, "error_cdkey_no_exist"},
	{ ORESULT_CDKEY_VALID, "error_cdkey_valid"},
	{ ORESULT_CDKEY_TIME_OUT, "error_cdkey_time_out"},
	{ ORESULT_CDKEY_OK, "cdkey_ok"},
	{ ORESULT_CDKEY_ONLY_ONE, "cdkey_only_one"},
	{ ORESULT_CDKEY_OVER_COUNT, "cdkey_over_count"},
	{ ORESULT_NOT_ENOUGH_MATERIAL_FAIL, "error_not_enough_material_fail"},
	{ ORESULT_GUILD_CREATE_SUCCESS, "guild_create_success"},
	{ ORESULT_GUILD_CREATEFULL, "guild_server_full"},
	{ ORESULT_GUILD_DUPLICATED_NAME, "guild_name_exit"},
	{ ORESULT_GUILD_PLAYER_INGUILD, "guild_char_inguild"},
	{ ORESULT_GUILD_JOIN_GUILD_LVLLESS, "guild_join_lvl_less"},
	{ ORESULT_GUILD_JOIN_GUILD_LIMIT, "guild_join_limit"},
	{ ORESULT_GUILD_NOT_EXITS, "GuildNoGuild_8"},
	{ ORESULT_GUILD_MEMBER_FULL, "guild_member_full"},
	{ ORESULT_GUILD_NOT_AMEMBER, "guild_error_not_amember"},
	{ ORESULT_GUILD_POSITION_LESS, "guild_error_position_less"},
	{ ORESULT_GUILD_CANNOT_DELSELF, "guild_error_cannot_delself"},
	{ ORESULT_GUILD_LEAVE_TIME_LESS, "guild_error_leave_less2hour"},
	{ ORESULT_FORBIDCHAT_LEFT_MIN, "ORESULT_FORBIDCHAT_LEFT_MIN" },
	{ ORESULT_FORBIDCHAT_LEFT_HOUR, "ORESULT_FORBIDCHAT_LEFT_HOUR" },
	{ ORESULT_ASSIGN_ALREADYGET, "Assign_AlreadyGet_error" },
	{ ORESULT_MONTHASSIGN_DAY_ERROR, "Assign_MonthDay_error" },
	{ ORESULT_LVLASSIGN_ERROR, "Assign_Lvl_error" },
	{ ORESULT_VIPLESS_ERROR, "vip_level_less" },
	{ ORESULT_GUILD_COCHAIRMAN_MAX, "guild_cochairman_max"},
	{ ORESULT_GUILD_MEMBER_EXIT, "guild_member_exit"},
	{ ORESULT_FRIEND_ERROR_NOTOPEN, "Friend_1"},
	{ ORESULT_FRIEND_ERROR_DETAILERROR, "Friend_4"},
	{ ORESULT_FRIEND_ERROR_LVL_LESS, "Friend_3"},
	{ ORESULT_FRIEND_ERROR_APPLY_FULL, "Friend_32"},
	{ ORESULT_FRIEND_ERROR_ALREADY_FRIEND, "Friend_33"},
	{ ORESULT_FRIEND_ERROR_SELF_FRIENDFULL, "Friend_13"},
	{ ORESULT_FRIEND_ERROR_OTHER_FRIENDFULL, "Friend_34"},
	{ ORESULT_FRIEND_ERROR_NOT_INAPPLY, "Friend_35"},
	{ ORESULT_FRIEND_ERROR_GIVEN_PHYSICAL, "Friend_36"},
	{ ORESULT_FRIEND_ERROR_CANNOT_APPLYSELF, "Friend_37"},
	{ ORESULT_FRIEND_ERROR_APPLY_SUCCESS, "Friend_14"},
	{ ORESULT_FRIEND_ERROR_AGREE_SUCCESS, "Friend_15"},
	{ ORESULT_FRIEND_ERROR_ONEKEYAGREE_SUCCESS, "Friend_16"},
	{ ORESULT_FRIEND_ERROR_NOT_RECV_PHYSICAL, "Friend_19"},
	{ ORESULT_FRIEND_ERROR_ALREADY_GET_PHYSICAL, "Friend_20"},
	{ ORESULT_FRIEND_ERROR_GET_PHYSICAL_MAX, "Friend_21"},
	{ ORESULT_FRIEND_ERROR_GET_PHYSICAL_SUCCESS, "Friend_22"},
	{ ORESULT_FRIEND_ERROR_GIVE_PHYSICAL_SUCCESS, "Friend_25"},
	{ ORESULT_FRIEND_ERROR_SELF, "Friend_28"},
	{ ORESULT_FRIEND_ERROR_NOT_ONLINE, "Friend_30"},
	{ ORESULT_USEITEM_COUNT_LIMIT, "CommonWanFa_1"},
	{ ORESULT_USEITEM_PHYSICALPOWER_MAX, "CommonWanFa_2"},
	{ ORESULT_HERO_DVELOP_PROPMAX, "HeroGrow_15"},
	{ ORESULT_HERO_DVELOP_ITEMLESS, "HeroGrow_16"},
	{ ORESULT_MS_Stall, "ORESULT_MS_Stall" },
	{ ORESULT_MS_Fuben, "ORESULT_MS_Fuben" },
	{ ORESULT_MS_BattleFieldMap, "ORESULT_MS_BattleFieldMap" },
	{ ORESULT_MS_ArenaMap, "ORESULT_MS_ArenaMap" },
	{ ORESULT_MS_CityMap, "ORESULT_MS_CityMap" },
	{ ORESULT_MS_PrisonMap, "ORESULT_MS_PrisonMap" },
	{ ORESULT_MS_Die, "ORESULT_MS_Die" },
	{ ORESULT_MS_Bus, "ORESULT_MS_Bus" },
	{ ORESULT_MS_Cashbox, "ORESULT_MS_Cashbox" },
	{ ORESULT_MS_Exchange, "ORESULT_MS_Exchange" },
	{ ORESULT_MS_MentalGame, "ORESULT_MS_MentalGame" },
	{ ORESULT_MS_TeamFollow, "ORESULT_MS_TeamFollow" },
	{ ORESULT_MS_AutoPlay, "ORESULT_MS_AutoPlay" },
	{ ORESULT_MS_SubTrain, "ORESULT_MS_SubTrain" },
	{ ORESULT_MS_Prison, "ORESULT_MS_Prison" },
	{ ORESULT_MS_OpenBox, "ORESULT_MS_OpenBox" },
	{ ORESULT_MS_ProtectTime, "ORESULT_MS_ProtectTime" },
	{ ORESULT_MS_KeyboardMove, "ORESULT_MS_KeyboardMove" },
	{ ORESULT_MS_Quest_TieSan, "ORESULT_MS_Quest_TieSan" },
	{ ORESULT_MS_ReadyFuben, "ORESULT_MS_ReadyFuben" },
	{ ORESULT_MS_Quest_DuoQiBuff, "ORESULT_MS_Quest_DuoQiBuff" },
	{ ORESULT_MS_Quest_TongQuBuff, "ORESULT_MS_Quest_TongQuBuff" },
	{ ORESULT_MS_PKMode, "ORESULT_MS_PKMode" },
	{ ORESULT_MS_ForbidRide, "ORESULT_MS_ForbidRide" },
	{ ORESULT_MS_Wedding, "ORESULT_MS_Wedding" },
	{ ORESULT_PWD_ErrorProtect, "ORESULT_PWD_ErrorProtect" },
	{ ORESULT_PWD_ErrorMax, "ORESULT_PWD_ErrorMax" },
	{ ORESULT_MS_Enjail, "ORESULT_MS_Enjail" },
	{ ORESULT_MS_Award, "ORESULT_MS_Award" },
	{ ORESULT_MS_TransGold, "ORESULT_MS_TransGold" },
	{ ORESULT_MS_BlackGold, "ORESULT_MS_BlackGold" },
	{ ORESULT_HERO_AWAKING_MAX, "HeroGrow_20" },
	{ ORESULT_HERO_AWAKING_ITEMLESS, "HeroGrow_21" },
	{ ORESULT_HERO_BREAK_MAX, "HeroGrow_23" },
	{ ORESULT_HERO_BREAK_ITEMLESS, "HeroGrow_24" },
	{ ORESULT_ONEKEY_WEAREQUIP_SUCCESS, "OneKeyWearEquipSuccess" },
	{ ORESULT_EQUIP_EXTRACTION_ITEMLESS, "Equip_1"},
	{ ORESULT__EQUIP_EXTRACTION_FAIL, "Equip_2"},
	{ ORESULT__EQUIP_STRONGER_MAX, "EQUIP_STRONG_3" },
	{ ORESULT_EQUIP_EVOLUTION_FAILED, "Equip_38" },
	{ ORESULT_ERROR_FRIENDGEM_LVLMAX, "FriendGem_1"},
	{ ORESULT_ERROR_FRIENDGEM_ITEMLESS, "FriendGem_2"},
	{ ORESULT_ERROR_FRIENDGEM_WASHITEMLESS, "FriendGem_3"},
	{ ORESULT_NOT_ENOUGH_CU_HONOR, "no_honor" },
	{ ORESULT_GODTOWER_SHOPPOINT_NOT, "no_godtowershoppoint" },
	{ ORESULT_EQUIP_EXTRACTION_SUCCESS, "Equip_31" },	
	{ ORESULT_HEROCARD_NUM_MAX, "BagItem_11" },
	{ ORESULT_SPORTS_SETDEFENDERDATA_FAIL, "Sports_9" },
	{ ORESULT_NOT_ENOUGH_CU_KUNG_FU_RELIC, "FindTreasure1" },
	{ ORESULT_NOT_ENOUGH_CU_WACKY_GADGET, "FindTreasure2" },
	{ ORESULT_NOT_ENOUGH_CU_VIKING_HEIRLOOM, "FindTreasure3" },
	{ ORESULT_NOT_ENOUGH_CU_ZOO_SOUVENIR, "FindTreasure4" },
	{ ORESULT_PLAYER_SHOPPING_FLUSH_MAX, "windowMysteryShopping_10" },
	{ ORESULT_PLAYER_TEAM_LEVEL_MAX, "Player_Max_Teamlevel" },
	{ ORESULT_GUILD_SEARCH_NULL, "guild_search_null" },

	{ ORESULT_WORLD_BOSS_RAIS_SUCCESS, "WorldBoss_5" },
	{ ORESULT_FRIEND_ERROR_PVPTARGET_GETFAIL, "Friend_31" },
	{ ORESULT_SPECIAL_GIFT_NOT_EXIST, "Special_Gift_1" },
	{ ORESULT_SPECIAL_GIFT_HAS_BUY, "Special_Gift_2"},
	{ ORESULT_SPECIAL_GIFT_BAG_FULL, "Special_Gift_6"},

	{ ORESULT_GET_DAILY_PHYSICAL_NOT_OPEN, "DailyTask_01" },
	{ ORESULT_GET_DAILY_PHYSICAL_CONFIG_ERROR, "DailyTask_02" },
	{ ORESULT_GET_DAILY_PHYSICAL_MAX_COUNT, "DailyTask_03" },
	{ ORESULT_GET_DAILY_PHYSICAL_TIME, "DailyTask_04" },
	{ ORESULT_NOT_ENOUGH_CU_FRIEND, "no_friendpoint" },
	{ ORESULT_FRIEND_ERROR_GET_COUNT_MAX, "Friend_53" },
	{ ORESULT_FRIEND_ERROR_GIVE_COUNT_MAX, "Friend_54" },
	{ ORESULT_FRIEND_NO_POINT_GET, "Friend_55"},
	{ ORESULT_CURRENCY_NOT_ENOUGH, "no_money" },
	{ ORESULT_WORLD_BOSS_RAIS_FAILED, "WorldBoss_13" },

	{ ORESULT_GUILD_DINNER_MAKING, "guild_error_dinner_making" },
	{ ORESULT_GUILD_DINNER_HASAWARD, "guild_error_dinner_hasaward" },
	{ ORESULT_GUILD_DINNER_MAKECOUNT_LESS, "guild_error_dinner_makecount_less" },
	{ ORESULT_GUILD_DINNER_TIME_ERROR, "guild_error_dinner_time" },
	{ ORESULT_GUILD_DINNER_WAITING, "guild_error_dinner_waiting" },
	{ ORESULT_GUILD_DINNER_EAT_ALREADY, "guild_error_dinner_eat_already" },

	{ ORESULT_GUILDBOSS_CHALLENGE_COUNT_LESS, "GuildBoss_9" },

	{ ORESULT_GUILD_SHOP_ITEM_LOST, "guild_shop_item_lost" },
	{ ORESULT_GUILD_SHOP_HAS_BUY, "guild_shop_item_hasbuy" },
	{ ORESULT_GUILDBOSS_ALREADYOPEN, "GuildBoss_1" },
	{ ORESULT_GUILDBOSS_PRE_NOT_PASS, "GuildBoss_2" },
	{ ORESULT_GUILDBOSS_GATE_ERROR, "GuildBoss_3" },
	{ ORESULT_GUILDBOSS_ACTIVITY_LESS, "GuildBoss_4" },
	{ ORESULT_GUILDBOSS_NOT_OPEN, "GuildBoss_5" },
	{ ORESULT_GUILDBOSS_IN_BATTLE, "GuildBoss_6" },
	{ ORESULT_GUILDBOSS_LVL_LESS, "GuildBoss_7" },
	{ ORESULT_GUILDBOSS_ALREADYPASS, "GuildBoss_8" },
	{ ORESULT_GUILDBOSS_CHALLENGE_COUNT_LESS, "GuildBoss_9" },
	{ ORESULT_GUILDBOSS_NOT_PASS, "GuildBoss_10" },
	{ ORESULT_RAISE_FAILED, "raise_failed" },
	{ ORESULT_RAISE_SUCCESS, "raise_success" },
	{ ORESULT_ITEM_MIX_HAS_NOT, "BagItem_14" },
	{ ORESULT_ITEM_MIX_ALL_HAS_NOT, "BagItem_15" },
	{ ORESULT_AREAN_REFRESH_ENEMY_SUCC, "arena_6" },

	{ ORESULT_INVITE_CODE_FRIEND_ALREADY, "InviteCodeFriend_1" },
	{ ORESULT_INVITE_CODE_FRIEND_COUNT_FULL, "InviteCodeFriend_2" },
	{ ORESULT_INVITE_CODE_FRIEND_NOT_FIND, "InviteCodeFriend_3" },
	{ ORESULT_GOD_ARENA_CHALLENGE_FAILED, "GOD_ARENA_10" },

	{ ORESULT_EQUIP_STRONGER_FAIL, "EQUIP_STRONG_4" },

	{ ORESULT_NOT_ENOUGH_CU_PeakArenaCoin, "" },
	{ ORESULT_PEAKARENA_NO_FIND_PLAYER, "" },
	{ ORESULT_GUILD_APPLY_ADD_NUM, "GUILD_STR_8" },
	{ ORESULT_GUILD_APPLY_EXIST, "GUILD_STR_9" },
	{ ORESULT_EQUIP_TREASURE_SCORE_FULL, "EUIP_TREASURE_TIPS11" },

	
};

class			RetriveResultStrManager
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:
	typedef std::map<int32, const char *>	COResultTextMap;
	COResultTextMap m_map_OResultText;

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	RetriveResultStrManager()
	{
		/*~~~~~~~~~~~*/
		int32 _Idx, nSize;
		/*~~~~~~~~~~~*/

		nSize = sizeof(s_aResultText) / sizeof(SResultText);
		for(_Idx = 0; _Idx < nSize; _Idx++)
		{
			m_map_OResultText.insert(COResultTextMap::value_type(s_aResultText[_Idx].nCode, s_aResultText[_Idx].pszText));
		}
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	~RetriveResultStrManager()
	{
		m_map_OResultText.erase(m_map_OResultText.begin(), m_map_OResultText.end());
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	LPCSTR Get_OperateResultText(eOPTRESULT oResult)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		static const char		*szORESULT_ERROR = "unkown error";
		COResultTextMap::iterator	itFind;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		itFind = m_map_OResultText.find(oResult);
		if(itFind != m_map_OResultText.end())
		{
			return itFind->second;
		}
		else
		{
			return szORESULT_ERROR;
		}
	}
};

static RetriveResultStrManager g_ORTTextManager;

/*
 =======================================================================================================================
 =======================================================================================================================
 */

const char *GetORTText(eOPTRESULT code)
{
	return g_ORTTextManager.Get_OperateResultText(code);
}
