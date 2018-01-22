/*$T Share/DBSystem/DataBase/SqlTemplate.h GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef _SQL_khan2__TEMPLATE_H_
#define _SQL_khan2__TEMPLATE_H_
#include "TypeDefine.h"

#define CHAR_TABLE						"t_player"
#define ITEM_TABLE						"t_item"
#define SPELL_TABLE						"t_spell"
#define TALENT_TABLE					"t_talent"
#define SKILL_TABLE						"t_skill"
#define QUEST_TABLE						"t_quest"
#define RELATION_KHAN_TABLE				"t_friend"
#define MASTER_TABLE					"t_master"
#define PRENTICE_TABLE					"t_prentice"
#define MARRY_TABLE						"t_marry"
#define EFFECT_TABLE					"t_effect"
#define HORSE_TABLE						"t_horse"
#define ITEMSERIAL_TABLE				"t_itemserial"
#define POST_TABLE						"t_mail"
#define AWARDPOST_TABLE					"t_awardpost"
#define GUILD_TABLE						"t_guild"
#define GUILD_MEMBER_TABLE				"t_guildmember"
#define GUILD_SKILL_TABLE				"t_guildskill"
#define COUNTRY_TABLE					"t_country"
#define MAPVAR_TABLE					"t_mapvar"
#define TOPLIST_TABLE					"t_toplist"
#define ARENATOPLIST_TABLE				"t_arenatoplist"
#define PEAKARENATOPLIST_TABLE			"t_peakarenatoplist"   //�۷徺����
#define SHOP_TABLE						"t_shop"
#define BANK_TABLE						"t_bank"
#define MASTER_CANDIDATE_TABLE			"t_master_candidate"
#define PRENTICE_CANDIDATE_TABLE		"t_prentice_candidate"
#define STOCK_TRADE_TABLE				"t_stocktrade"			//��ǰ���ڽ��׵Ĺ�Ʊ��Ϣ
#define STOCK_RECORD_TABLE				"t_stockrecord"			//��Ʊ���׵���ʷ��¼
#define STOCK_ACCOUNT_TABLE				"t_stockaccount"		//�û���Ʊ�˻���Ϣ:��Ҫ�Ǽ�¼��ҹ�Ʊ�˻����
#define CHAR_EXT_TABLE					"t_player_ext"            //����������չ��:��Ҫ��¼�̵��ղؼУ���ʷ��¼����Ϣ
#define DYNAMIC_SHOP_TABLE				"t_dynamicshop"        //��̬�۸��̵�
#define CHARIOT_TABLE					"t_chariot"				// 2011-4-14 by rj ս�����ݱ�
#define LOSTITEM_TABLE					"t_lostitem"
#define MPVPPOINT_TABLE					"t_mpvppoint"			//���ս���ֱ�
#define HORSE_EQUIP_TABLE				"t_horseequip"			// ����װ����
#define SUBSTITUTE_TABLE				"t_substitute"			// �����
#define GARDEN_TABLE					"t_garden"				// ��԰��
#define GARDENRES_TABLE					"t_gardenres"			// ��԰��Դ��
#define GARDENSUBRES_TABLE				"t_gardensubres"		// ��԰����Դ��
#define GARDENEVENT_TABLE				"t_gardenevent"			// ��԰�¼���
#define PET_TABLE						"t_pet"					// �����
#define CHANNELMERIDIAN_TABLE			"t_channelmeridian"		// ������
#define PLAYER_NUMERICAL				"t_playernumerical"
#define GLOBALVAL_TABLE					"t_globalval"			// ȫ�ֱ�����
#define MASK_TABLE						"t_mask"				// ��߱�
#define MPVPLEAGUEPOINT_TABLE			"t_mpvpleague_result"			// ���������� [11/24/2012 sunlei]
#define SLG_BUILDINFO_TABLE				"t_slg_buildinfo"		// slg�淨-������
#define SOLDIER_TABLE					"t_soldier"				// ʿ�������֣���
#define PETHATCH_TABLE					"t_pethatch"			// ���������
#define SLGBATTLE_TABLE					"t_slgbattle"			// Զ������
#define HEROCARD_TABLE					"t_herocard"			// Ӣ�ۿ���
#define CDKEY_TABLE						"t_cdkey"				// ckkey��
#define POSTRELATION_TABLE				"t_postrelation"
#define ABATTOIRTOPLIST_TABLE			"t_abattoirtoplist"		// �սᾺ����[7/15/2017 chief]

extern char	*g_StrLoadPlayerList;
extern char *g_StrSelectByPlayerName;
extern char	*g_StrLoadPlayerListDynamicCondition;				// ��������б�Ķ�̬����

extern char	*g_StrLoadPlayerEquipList;
extern char	*LoadCharGuild;
extern char	*LoadCharCreateTime;

//orderlist
extern char* g_IsOrderExist;
extern char* g_InsertOrder;

// cdkey��� [xingzhishan]
extern char* g_CheckCDKey;
extern char* g_LoadCDKeyCount;
extern char* g_LoadCDKey;
extern char* g_InsertCDKey;
extern char* g_UpdateCDKey;

// ȫ��������������� [xingzhishan]
extern char	*LoadGlobalPlayerInfo;
extern char	*g_StrLoadPlayerFullData;
extern char *g_StrLoadPlayerFullDataExt;
extern char *g_StrLoadPlayerCount;
extern char *g_StrLoadPlayerMaxAid;
extern char *g_StrLoadPlayerMinAid;
extern char *g_StrLoadPlayerSnidByAid;

extern char *g_StrLoadPlayerLockData;


extern char	*g_StrLoadPlayerAccountName;
extern char *g_StrSavePlayerAccountName;


extern char	*g_StrLoadPlayerItemList;
extern char	*g_StrDeletePlayerItemList;
extern char	*SaveItemInfo;

extern char	*g_StrLoadPlayerHeroCardList;
extern char	*g_StrDeletePlayerHeroCardList;
extern char	*SaveHeroCardInfo;

extern char	*g_StrLoadPlayerSkillList;
extern char	*g_StrDeletePlayerSpellList;
extern char	*g_StrNewPlayerSkill;

extern char	*LoadCharTalentList;
extern char	*DeleteCharTalentList;
extern char	*NewCharTalent;

extern char	*g_StrLoadPlayerAbilityList;
extern char	*g_StrLoadPlayerPresList;
extern char	*g_StrDeletePlayerAbilityList;
extern char	*g_StrNewPlayerAbility;
extern char	*g_StrUpdatePlayerPresList;

extern char	*g_StrLoadPlayerTaskDataAndFlag;
extern char	*g_StrLoadPlayerTaskList;
extern char	*g_StrDeletePlayerTaskList;
extern char	*g_StrNewPlayerTask;
extern char	*g_StrUpdatePlayerTaskData;
extern char	*g_StrUpdatePlayerTaskFlag;

extern char	*g_StrLoadPlayerRelationList;
extern char	*NewCharRelationList;
extern char	*g_StrDeletePlayerRelationList;

extern char	*g_StrLoadPlayerRelationData;
extern char	*g_StrUpdatePlayerRelationData;

extern char	*LoadCharMasterInfo;
extern char	*NewCharMasterInfo;
extern char	*DeleteCharMasterInfo;

extern char	*LoadCharPrenticeInfo;
extern char	*NewCharPrenticeInfo;
extern char	*DeleteCharPrenticeInfo;

extern char	*LoadCharMarryInfo;
extern char	*NewCharMarryInfo;
extern char	*DeleteCharMarryInfo;

extern char	*g_StrLoadPlayerEffectList;
extern char	*g_StrDeletePlayerEffectList;
extern char	*g_StrNewPlayerEffect;

extern char	*g_StrLoadPlayerHorseList;
extern char	*g_StrDeletePlayerHorseList;
extern char	*g_StrNewPlayerHorse;

extern char	*g_StrLoadPlayerPrivateInfo;
extern char	*g_StrUpdatePlayerPrivateInfo;

extern char	*g_StrLoadPlayerTitleInfo;
extern char	*g_StrUpdatePlayerTitleInfo;

extern char	*g_StrLoadPlayerCoolDownInfo;
extern char	*g_StrUpdatePlayerCoolDownInfo;

extern char	*g_StrCreateNewPlayer;
extern char	*g_StrCreateMergePlayer;
extern char	*g_StrFetchNewGuid;

extern char	*g_StrDeleteBasePlayer;
extern char	*g_StrUpdatePlayerFullData;
extern char	*g_StrUpdatePlayerFullDataExt;
extern char *g_StrUnLockPlayer;
extern char *g_StrLockPlayer;

extern char *g_StrForbidPlayerLoginTime;
extern char *g_StrForbidPlayerChatTime;

extern char *g_StrQueryPlayerVersion;

extern char	*g_StrLoadServerItemSerial;
extern char	*g_StrUpdateServerItemSN;

extern char	*g_StrLoadWorldPostInfo;
extern char	*g_StrSaveGLPostInfo;
extern char	*g_StrDeleteWorldPostInfo;
extern char *ClearInvalidPostSql;
extern char	*DeletePostByCharName;
// �ʼ���Ӧ��ϵ [wangyiran]
extern char	*g_StrLoadWorldPostRelationInfo;
extern char	*g_StrSaveGLPostRelationInfo;
// �����ʼ� [xingzhishan]
extern char	*g_StrLoadAwardPostInfo;
extern char	*g_StrUpdateAwardPostInfo;
extern char	*g_StrDeleteAwardPostInfo;

extern char *g_StrLoadGlabalValue;
extern char *g_StrSaveGlabalValue;

extern char	*g_StrLoadGuildIdBegin;
extern char	*g_StrSaveGuildIdBegin;
extern char	*g_StrLoadWorldGuildInfo;
extern char	*g_StrSaveGLGuildInfo;
extern char	*g_StrDeleteWorldGuildInfo;
extern char	*g_StrDeleteALLWorldGuildInfo;

extern char	*LoadGuildMember;
extern char	*DeleteGuildMember;
extern char	*InsertGuildMember;
extern char	*DeleteAllGuildMember;
extern char *g_StrDeleteInvalidMembers;

extern char	*LoadGuildSkill;
extern char	*DeleteGuildSkill;
extern char	*InsertGuildSkill;

extern char	*g_StrLoadGlobalSerialIdBegin;
extern char	*g_StrSaveGlobalSerialIdBegin;

extern char	*LoadWorldCountryInfo;
extern char	*SaveWorldCountryInfo;
extern char	*DeleteWorldCountryInfo;

extern char	*LoadMapVarInfo;
extern char	*SaveMapVarInfo;
extern char	*DeleteMapVarInfo;

extern char* LoadArenaTopList;
extern char* InquireArenaRank;
extern char* SaveArenaTopList;
extern char* DelArenaTopListInfo;

extern char* LoadPeakArenaTopList;
extern char* InquirePeakArenaRank;
extern char* SavePeakArenaTopList;
extern char* DelPeakArenaTopListInfo;

extern char	*LoadTopList;
extern char	*SaveTopListInfo;
extern char	*DelTopListInfo;
extern char	*CheckDBConnection;
extern char	*CheckDBVersion;

extern char	*ChangeCharName;
extern char	*ChangeGuildName;
extern char	*InsertShopData;
extern char	*g_StrLoadShopInfo;
extern char	*DeleteShopData;

extern char	*InsertBankData;
extern char	*LoadBankInfo;
extern char	*DeleteBankData;

extern char	*LoadMasterCandidate;
extern char	*SaveMasterCandidate;
extern char	*DeleteMasterCandidate;

extern char	*LoadPrenticeCandidate;
extern char	*SavePrenticeCandidate;
extern char	*DeletePrenticeCandidate;

extern char	*InsertStockTradeData;
extern char	*LoadStockTradeInfo;
extern char	*SaveStockTradeData;
extern char	*DeleteStockTradeData;

extern char	*InsertStockAccData;
extern char	*LoadStockAccInfo;
extern char	*UpdateStockAccData;
extern char	*DeleteStockAccData;

extern char	*InsertStockRecordData;
extern char	*LoadStockRecordInfo;
extern char	*DeleteStockRecordData;

extern char	*LoadCharExtInfo;
extern char *DeleteCharExtInfo;
extern char *InsertCharExtInfo;

extern char	*LoadDynamicShopInfo;
extern char	*SaveDynamicShopInfo;
extern char	*DeleteDynamicShopInfo;

extern char	*g_StrLoadPlayerChariotList;
extern char	*g_StrDeletePlayerChariotList;
extern char	*g_StrNewPlayerChariot;

extern char *g_LoadLostItemInfo;
extern char *g_DelLostItemInfo;
extern char *g_UpdateLostItemInfo;

extern char	*LoadMPVPPointInfo;
extern char	*SaveMPVPPointInfo;
extern char	*DelMPVPPointInfo;

extern char	*g_StrLoadHorseItemList;
extern char	*g_StrDeleteHorseItemList;
extern char	*SaveHorseItemInfo;

extern char	*LoadSubstituteInfo;
extern char	*SaveSubstituteInfo;
extern char	*DelSubstituteInfo;

extern char	*LoadGardenInfo;
extern char	*SaveGardenInfo;

extern char	*LoadGardenResInfo;
extern char	*SaveGardenResInfo;

extern char	*LoadGardenSubResInfo;
extern char	*SaveGardenSubResInfo;

extern char *LoadGardenEventInfo;
extern char *SaveGardenEventInfo;

extern char	*g_StrLoadPlayerPetList;
extern char	*g_StrDeletePlayerPetList;
extern char	*g_StrNewPlayerPet;


extern char *DelChannelMeridian;
extern char *LoadChannelMeridian;
extern char *SaveChannelMeridian;

extern char *LoadPlayerNumericalInfo;
extern char *DeletePlayerNumericalInfo;
extern char *SavePlayerNumericalInfo;

extern char	*g_StrLoadGlobal_1v1Data;
extern char	*g_StrSaveGlobal_1v1Data;

extern char	*LoadMaskInfo;
extern char	*DelMaskInfo;
extern char	*SaveMaskInfo;

extern char	*LoadMPVPLeaguePointInfo;
extern char	*SaveMPVPLeaguePointInfo;
extern char	*DelMPVPLeaguePointInfo;

extern char *LoadSlgBuildInfo;
extern char *DelSlgBuildInfo;
extern char *SaveSlgBuildInfo;

extern char	*g_LoadSoldierInfo;
extern char	*g_DelSoldierInfo;
extern char	*g_SaveSoldierInfo;

extern char *g_LoadPetHatchAll;
extern char *g_SavePetHatchInfo;
extern char *g_DelPetHatchInfo;

extern char	*g_StrLoadSlgBattle;
extern char	*g_StrSaveSlgBattle;

extern char	*g_StrAddLostItemByGM;

// ΢�ź�� [wangyiran]
extern char	*g_StrLoadWorldWeChatRedInfo;
extern char	*g_StrSaveGLWeChatRedInfo;

// ��ɫ���ϵĺ���б� [yangzhen]
extern char	*g_StrLoadPlayerWeChatRedInfo;
extern char	*g_StrSavePlayerWeChatRedInfo;

// ���� [wangyiran]
extern char *g_StrLoadWorldFriendInfo;
extern char *g_StrSaveWorldFriendInfo;


//�ڿ��淨[tianshuai]
extern char* g_StrLoadMiningInfo;
extern char* g_StrSaveMiningInfo;
//������SNID
extern char* g_StrLoadFakeSnid;

// ���� [wangyiran]
extern char *g_StrLoadBuildingInfo;
extern char *g_StrSaveBuildingInfo;

// playerext2 [12/6/2016 "tianshuai"]
extern char *g_StrLoadPlayerFullDataExt2;
extern char *g_StrUpdatePlayerFullDataExt2;

// � [12/19/2016 scw]
extern char *g_StrLoadActivityInfo;
extern char *g_StrSaveActivityInfo;
extern char *g_StrFindActivityInfo;

// �սᾺ���� [7/15/2017 chief]
extern char* LoadAbattoirTopList;
extern char* SaveAbattoirTopList;
extern char* DelAbattoirTopListInfo;

#endif
