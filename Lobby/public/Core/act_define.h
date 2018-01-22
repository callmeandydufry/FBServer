/*$T MapServer/Server/Base/act_define.h GC 1.140 10/10/09 10:07:20 */

/* act_define.h $v1.0 97203 */
#ifndef _ACT_DEFINE_H_
#define _ACT_DEFINE_H_

#define CONVERT_ITEMACT_TO_CURRENCYACT(var) ((eCurrencyActType)(eCurrencyAct_SpecialItemConvert + var))

enum eSkillActType
{
	eSkillAct_Unknow		= 0,			/* δ֪���ܲ��� */
	eSkillAct_ScriptTrain		= 10,			/* ����ѧϰ */
	eSkillAct_LevelUp		= 20,			/* �������� */
	eSkillAct_ScriptAbandon		= 30,			/* ���ܷ��� */
	eSkillAct_ScriptChangeFaction	= 31,			/* �ű�����ChangeMenpai */
	eSkillAct_GMModify		= 40,			/* ͨ��GM���� ѧϰ */
	eSkillAct_GuildSkillLevelUp_Guild	= 50,			/* ��Ἴ�ܣ��츳���У����� ���*/
	eSkillAct_GuildSkillLevelUp_Person		= 51,			/* ��Ἴ�ܣ��츳���У����� ����*/
	eSkillAct_InherenceBalance_LevelUp		= 60,			/* �츳ƽ������*/
	eSkillAct_NUM
};
enum eAbilityActType
{
	eAbilityAct_Unknow		= 0,			/* δ֪���ܲ��� */
	eAbilityAct_ScriptTrain		= 10,			/* �����ѧϰ */
	eAbilityAct_LevelUp		= 20,			/* ��������� */
	eAbilityAct_ScriptAbandon	= 30,			/* ����ܷ��� */
	eAbilityAct_GMModify		= 40,			/* ͨ��GM���� ѧϰ */
	eAbilityAct_GMModifyAll		= 41,			/* ͨ��GM����ѧϰ���� */
	eAbilityAct_AddGuanqiaSpell = 42,			/* �ؿ��������*/
	eAbilityAct_DelGuanqiaSpell = 43,			/* �ؿ�����ɾ��*/
	eAbilityAct_NUM
};
enum eCurrencyActType
{
	eCurrencyAct_Unknow = 0,	/* δ֪ */
	eCurrencyAct_AltarHeroCard = 1, // �ٻ�Ӣ�ۿ��� [wangyiran]	
	eCurrencyAct_BattleGateAward =2,	// ս����ʤ�Ľ��� [yangzhen]
	eCurrencyAct_OpenAwardBoxCreate = 3, // �������������� [yangzhen]
	eCurrencyAct_CompleteEliteBattle = 4, // ��Ӣ����ս�۽��� [yangzhen]
	eCurrencyAct_SystemAutoPhysical = 5,	// �Զ��������� [xingzhishan]
	eCurrencyAct_SystemAutoSpirit = 6,	// �Զ����Ӿ��� [xingzhishan]
	eCurrencyAct_PhysicalChangeByLevelUP = 7,//�����ı����� [xingzhishan]
	eCurrencyAct_SpiritChangeByLevelUP =8,// �����ı侫�� [xingzhishan]
	eCurrencyAct_BuyArenaChallengeCount =9,// ���򾺼�����ս�����ı���ʯ [xingzhishan]
	eCurrencyAct_ArenaChallengeAward = 10, // ��������ս���� [xingzhishan]
	eCurrencyAct_BattleReturn = 11, // ����δ��ɾͷ��أ������������� [wangyiran]
	eCurrencyAct_BankExchageGold = 12, // ҡǮ��
	eCurrencyAct_GoldChangeByLevelUP =13,// �����ı��� [wangxiaoyong]
	eCurrencyAct_FirstPayAward = 14, //	�׳影�� [xingzhishan]
	eCurrencyAct_SweepBattleGateAward = 15, //	����ɨ�� [wangyiran]
	eCurrencyAct_FromShopMallDiamond = 16, //�̵깺����ʯ
	eCurrencyAct_SellItemGiveMoney = 17,// ����Ʒ��Ǯ [12/28/2015 superrobert]
	eCurrencyAct_CallScriptIncome = 18,// �ű����� [12/28/2015 superrobert]
	eCurrencyAct_GMGiveMoney = 19,// gm���� [12/28/2015 superrobert]
	eCurrencyAct_ByFriend = 20,// �������� [12/29/2015 superrobert]
	eCurrencyAct_SportsHonor = 21, // ������ս�������� [3/17/2016 scw]
	eCurrencyAct_GodTowerClear = 22, // ����֮������ [3/17/2016 scw]
	eCurrencyAct_PhysicalChangeByItem = 23,// ʹ�õ��߸ı����� [8/9/2016 scw]
	eCurrencyAct_ShopBuyGift = 24,//  [6/3/2017 "tianshuai"] �̵깺�����
	eCurrencyAct_ShopBuyTimeLimitGift = 25,// �����ʱ��� [6/3/2017 "tianshuai"]
	eCurrencyAct_ShopBuySmallGift = 26,// С����� [6/3/2017 "tianshuai"]

	eCurrencyAct_FinishGuildBossBattle = 607, // ���һ������bossս�� [wangyiran]
	eCurrencyAct_IndentureCompose = 608, // ��Լж�� [12/6/2016 "tianshuai"]
	eCurrencyAct_MgcLuckPanShip = 609, // mgcת�� [12/26/2016 "tianshuai"]
	eCurrencyAct_SpecialShopBuy = 610, // �����̵����� [12/26/2016 "tianshuai"]
	eCurrencyAct_SpecialShopRef = 611, // �����̵�ˢ�� [12/26/2016 "tianshuai"]

	eCurrencyAct_GoldChangeByPeakArena = 615,// �۷徺����ս���ı��� [chentongxin]
	eCurrencyAct_DotaCoinChangeByPeakArena = 616,// �۷徺����ս���ı侺����ѫ��[chentongxin]
	eCurrencyAct_PeakArenaShopBuyChangeMoney = 617, // �۷徺�����̵깺�� [chentongxin]
	eCurrencyAct_BuyPeakArenaChallengeCount = 618,// ���򾺼�����ս�����ı���ʯ [chentongxin]
	eCurrencyAct_MiningBuyRobCount = 619,// �����Ӷ����[tianshuai]
	eCurrencyAct_ChangePeakArenaCoinByPlayer = 620, //�û�������ȡ�۷�� [chentongxin]
	eCurrencyAct_ChangeBuyAbattoirTimes, //�û�������ս����
	eCurrencyAct_ChangeFreshAbattoirCD, // �û�����ˢ���Լ���սcd
	eCurrencyAct_MoneyTreeCost,	// ҡǮ������

	eCurrencyAct_BuyItemFromGuildShop = 992,// ��������Ʒ [wangyiran]
	eCurrencyAct_GuildEatDinner = 993,// �μӾ۲� [wangyiran]
	eCurrencyAct_GuildStartDinner = 994,// ������ʯ�۲� [wangyiran]
	eCurrencyAct_GuildMakeDishSuccess = 995,// ��⿳ɹ��콱 [wangyiran]
	eCurrencyAct_BuyDinnerCount = 996, // ����۲���⿴��� [wangyiran]
	eCurrencyAct_upLevel_Spell = 997, //  �������� [8/10/2016 TianShuai]
	eCurrencyAct_ArenaRefEnemy = 998,	// ������ˢ�¶������� [8/10/2016 TianShuai]
	eCurrencyAct_WonderfulActivityVipGift = 999,	// ���ʻvip�ȼ���� [7/26/2016 TianShuai]
	eCurrencyAct_EnterBattleCost =1000,	// ���븱���ؿ����� [yangzhen]
	eCurrencyAct_SweepBattleGateCost = 1001, // ɨ��ս�۹ؿ����� [yangzhen
	eCurrencyAct_BuyPhysical = 1002,// �������� [xingzhishan]
	eCurrencyAct_BuySpirit = 1003,// ������ [xingzhishan]
	eCurrencyAct_BuyPhysicalChangeDiamond = 1004, // ���������ı���ʯ [xingzhishan]
	eCurrencyAct_BuySpiritChangeDiamond = 1005, // �������ı���ʯ [xingzhishan]
	eCurrencyAct_StarCost =1006,// �������� [wangyiran]
	eCurrencyAct_BankDiamondCost = 1007, // ����������ʯ [xingzhishan]
	eCurrencyAct_BuyXuYuanFu =1008, // ������Ը��[wangxiaoyong]
	eCurrencyAct_BuyEliteBattleChangeMoney = 1009, // ��Ӣ������ս�������� [xingzhishan]
	eCurrencyAct_ShenyuzhichengCost = 1010, // ����֮����Ը [yangzhen]
	eCurrencyAct_ResetChallenageLastCountCost = 1011, // ���þ�������ս���� [yangzhen]
	eCurrencyAct_DepleteChallenageLastCountCost = 1012, // ���Ѿ�������ս���� [yangzhen]
	eCurrencyAct_BuyArenaShopChangeMoney = 1013, // �������̵깺�� [xingzhishan]
	eCurrencyAct_FlushShopdeplete = 1014, //	�̵�ˢ������ [xingzhishan]
	eCurrencyAct_CreateGuildCost =1015,// ������� [wangyiran]
	eCurrencyAct_BattleSweepCost =1016, //ɨ������
	eCurrencyAct_1STBlackStoreBuyCost =1017,// һ���ڵ깺������ [yangzhen]
	eCurrencyAct_2NDBlackStoreCost =1018,// �����ڵ깺������ [yangzhen]
	eCurrencyAct_3RDBlackStoreBuyCost =1019,// �����ڵ깺������ [yangzhen]
	eCurrencyAct_LuckPanCost =1020,//�����̹������� [yangzhen]
	eCurrencyAct_HeroStarCost =1021, // Ӣ�۾������� [wangyiran]
	eCurrencyAct_SpellUpCost = 1022,// �������� [12/28/2015 superrobert]
	eCurrencyAct_GMmodifyCost = 1023,// gm�������� [12/28/2015 superrobert]
	eCurrencyAct_ShopCost = 1024,// �̵깺�� [12/28/2015 superrobert]
	eCurrencyAct_GoldChangeByChatLabaCost = 1025,// �������� [12/29/2015 superrobert]
	eCurrencyAct_HeroDevelopCost = 1026,// Ӣ������ [12/29/2015 superrobert]
	eCurrencyAct_AwakingCost =1027,// �������� [wangyiran]
	eCurrencyAct_HeroBreakCost =1028,// ͻ������ [wangyiran]
	eCurrencyAct_HeroTransferCost = 1029,//ת������[tianshuai]
	eCurrencyAct_EquipStrongerCost =1030,// װ��ǿ�����Ľ�Ǯ [chentongxin]
	eCurrencyAct_FiendGemLvlUpCost =1031,// ���鱦ʯ���� [wangyiran]
	eCurrencyAct_FiendGemWashCost =1032,// ���鱦ʯϴ�� [wangyiran]
	eCurrencyAct_ArtifactDestroyCost = 1033,// ��������������ʯ [tianshuai]
	eCurrencyAct_ArtifactUpdateLevelCost = 1034,//������������
	eCurrencyAct_ArtifactBuyCost = 1035,// �������� [tianshuai]
	eCurrencyAct_ArtifactDestroyGive = 1036,// �������鷵����֮ף�� [tianshuai]
	eCurrencyAct_ChipGemCompoundCost = 1037,// ��Ƭ�ϳ����Ļ��� [2/15/2016 HJ]
	eCurrencyAct_DepleteEnegyCost = 1038,//��������
	eCurrencyAct_DepleteHounorCost = 1039,//��������
	eCurrencyAct_AddEnegyByTimer = 1040,  //��ʱ��������
	eCurrencyAct_GodTowerBuyCount = 1041, // ����֮��������� [3/2/2016 scw]
	eCurrencyAct_SportsRefreshCount = 1042, // ������սˢ�´��� [3/12/2016 scw]
	eCurrencyAct_SportsRaise = 1043, // ������ս���� [3/12/2016 scw]
	eCurrencyAct_WorldBossRaise = 1044, // ����boss���� [4/6/2016 scw]
	eCurrencyAct_CityBuilding = 1045, // �ǽ��������
	eCurrencyAct_FindTreasureFindGrid = 1046, // Ѱ��̽�����ӻ����Ʒ [3/30/2016 tianshuai]
	eCurrencyAct_RenameCostDiamond = 1047,// ������������ʯ [4/19/2016 HJ]
	eCurrencyAct_FindTreasureFindBuyBagItem = 1048, // ����Ѱ�������̵���Ʒ [3/30/2016 tianshuai]
	eCurrencyAct_FindTreasureFindBuyBread = 1049, // Ѱ��������� [3/30/2016 tianshuai]
	eCurrencyAct_FindTreasureFindSpecialShopBuy = 1050, // Ѱ��������� [3/30/2016 tianshuai]
	eCurrencyAct_FindTreasureFindSpecialShopRef = 1051, // Ѱ������ˢ�� [3/30/2016 tianshuai]
	eCurrencyAct_BattleAchieveGold = 1052,	// ս����ý�� [ASSERT]
	eCurrencyAct_BattleAchieveExp = 1053,	// ս����õľ��� [ASSERT]
	eCurrencyAct_TakeCardCostPower = 1054,// �����ٻ����� [4/28/2016 HJ]
	eCurrencyAct_BuyCommonBattleChangeMoney = 1055,// ��ͨ����������ս�������� [4/29/2016 HJ]
	eCurrencyAct_SuperEgoVipBuy = 1056, // ��������������� [4/28/2016 scw]
	eCurrencyAct_BuyEnergyChangeDiamond = 1057, // ���������������� [5/3/2016 HJ]
	eCurrencyAct_BuyEnergy = 1058,// �������� [5/3/2016 HJ]
	eCurrencyAct_VipLevelGiftBuy = 1059,// vip��Ȩ������� [5/11/2016 TianShuai]
	eCurrencyAct_SpecialGiftBuy = 1060,// ����������� [5/20/2016 HJ]
	eCurrencyAct_WorldBossBuy = 1061, // ����boss����ս������ [4/6/2016 scw]
	eCurrencyAct_UnEquipCost = 1062, // ж��װ��������ʯ [4/6/2016 scw]
	eCurrencyAct_EquipEvolutionCost = 1063, // ����װ��������ʯ [10/19/2016 scw]
	eCurrencyAct_FriendShopBuy = 1064, // �����̵깺����Ʒ���� [11/29/2016 scw]
	eCurrencyAct_HeroReturn = 1065, // Ӣ�ۻ�������
	eCurrencyAct_ActivitySaleGift = 1066, // ��Ż���� ������ʯ�Ȼ��� [12/23/2016 scw]
	eCurrencyAct_HeroUpLv = 1067,	//Ӣ���������ĵĽ��
	eCurrencyAct_EquipMixEquip = 1068,	// װ���ϳ�װ�� [2/16/2017 scw]
	eCurrencyAct_GuildGift = 1069,	//��������
	eCurrencyAct_ResetGuildBoss = 1070,	//����boss����
	eCurrencyAct_GuildBossBuyCount = 1071,	//����boss��ս��������
	eCurrencyAct_GuildBossInspire = 1072,	//����boss������
	eCurrencyAct_BuyHellBattleChangeMoney = 1073, // ����������ս�������� tianshuai
	eCurrencyAct_NUM,
	eCurrencyAct_SpecialItemConvert =10000,// �Ӷ�ˢ�� [xingzhishan]
};
enum eQuestActType
{
	eQuestAct_Unknow				= 0,	/* δ֪������� */
	eQuestAct_AddQuest				= 10,	/* �������� */
	eQuestAct_DelQuest				= 20,	/* ɾ�������п�������ɺ�ɾ���� */
	eQuestAct_ParamChange				= 30,	/* ��������仯 */
	eQuestAct_DoneFlag				= 40,	/* ������ɱ�־�仯 */
	eQuestAct_DataChange				= 50,	/* ����ȫ�����ݱ仯 */
	eQuestAct_CycleDoneFlag				= 60,	/* ѭ���������һ�α�־ */

	eQuestAct_NUM
};
enum eItemActType
{
	eItemAct_Unknow = 0,	/* δ֪��Ʒʰȡ */
	eItemAct_CreateItemByPost = 1,//ͨ���ʼ�������Ʒ
	eItemAct_CreateItemByBattlePickItem = 2,// ���ؿ��л�õ�ʰȡ��Ʒ����� [yangzhen]
	eItemAct_CreateItemByOpenBattleAwardBox = 3,// ��ȡս�۱��� [yangzhen]
	eItemAct_CreateItemByCompleteBattleGate = 4,// ��ɾ�Ӣ�������� [yangzhen] //���ս�۹ؿ������������ؿ�������
	eItemAct_CreateItemBySweepBattleGate = 5,// ɨ�������ؿ����� [yangzhen]
	eItemAct_CreateItemByAchievementAward = 6,// �ɾͽ��� [wangyiran]
	eItemAct_CreateItemByGodCity = 7,// ������Ʒͨ������֮�� [xingzhishan]
	eItemAct_CreateItemByCheckIn = 8,// ǩ������ [wangyiran]
	eItemAct_CreateItemByCheckDailyTask = 9,// �ճ������� [wangyiran]
	eItemAct_CreateItemByHeroCard = 10,// ����ת�� [wangyiran]
	eItemAct_CreateItemByRecoverBackupTemp = 11,// �ָ����ݵ���ʱ��Ʒ [yangzhen]
	eItemAct_CreateItemByGrowFundAwardDiamond = 12,// �ɳ���������ʯ [wangyiran]
	eItemAct_CreateItemByMonthCardEveryDayAward = 13,// ��ȡ�¿� [xingzhishan]
	eItemAct_CreateItemByZhanyiPreGuide = 14,// ��ȡսǰ�������� [wangyiran]
	eItemAct_CreateItemByOfflineTimeAward = 15,// ����ʱ�佱�� [xingzhishan]
	eItemAct_CreateItemByDayContinueAssignGift = 16,// ��ǩ�� [wangyiran]
	eItemAct_CreateItemByMonthAssignGift = 17,// ��ǩ�� [wangyiran]
	eItemAct_CreateItemByLvlAssignGift = 18,// �ȼ���� [wangyiran]
	eItemAct_CreateItemByVIPGIFTBOX = 19,// VIP�ȼ���� [wangyiran]
	eItemAct_CreateItemByCharge_Award = 20,// ��ֵ����� [wangyiran]
	eItemAct_CreateItemByConsume_Award = 21,// ���ѻ���� [wangyiran]
	eItemAct_CreateItemByLuck_Pan = 22,// ����ת�� [xingzhishan]
	eItemAct_CreateItemByYunYingActivityAward = 23,// ��Ӫ� [yangzhen]
	eItemAct_CreateItemByGhostMonth = 24,// ���� [yangzhen]
	eItemAct_CreateItemByVipGiftActivity = 25,// vip gift [yangzhen]
	eItemAct_CreateItemByExchangeByHeroChip = 26,// ��Ӣ����Ƭ�һ� [wangyiran]
	eItemAct_CreateItemByExchangeByNymph = 27,// Ů��޵� [tianshuai]
	eItemAct_CreateItemByExchangeByChangeRune = 28,// ���Ķһ� [tianshuai]
	eItemAct_CreateItemByDayChargeSection_Award = 29,// �ֶε��ճ�ֵ [wangyiran]
	eItemAct_CreateItemByWeekChargeSection_Award = 30,// �ֶ��ܳ�ֵ [wangyiran]
	eItemAct_CreateItemByDayConsumeSection_Award = 31,// �ֶε������� [wangyiran]
	eItemAct_CreateItemByWeekConsumeSection_Award = 32,// �ֶ������� [wangyiran]
	eItemAct_CreateItemByOpenBox = 33,// ���������Ʒ [12/28/2015 superrobert]
	eItemAct_CreateItemCreateFromShop = 34,// �̵깺�� [12/28/2015 superrobert]
	eItemAct_CreateItemBoxRecycle = 35,// ���� [12/28/2015 superrobert]
	eItemAct_CreateItemFromScript = 36,// �ű����� [12/29/2015 superrobert]
	eItemAct_CreateItemFromGM = 37,// gm���� [12/29/2015 superrobert]
	eItemAct_ExchangeInventoryToEquip = 38, // ��װ�� [1/18/2016 superrobert]
	eItemAct_CreateItemFromUnUseEquip = 39,// ��жװ��  [1/21/2016 superrobert] 
	eItemAct_TransferItemToOtherBag = 40,// ת����Ʒ���������� [4/13/2016 HJ]
	eItemAct_CreateItemByWeekCardEveryDayAward = 41,// ��ȡ�ܿ� [5/10/2016 TianShuai] 
	eItemAct_CreateItemBySeasonCardEveryDayAward = 42,// ��ȡ���� [5/10/2016 TianShuai] 
	eItemAct_CreateItemByYearCardEveryDayAward = 43,// ��ȡ�꿨 [5/10/2016 TianShuai] 
	eItemAct_CreateFromShop = 44,	/* �̵�Ĵ��� */
	eItemAct_CreateItemByPlayerGuide = 45, // ������������Ʒ [9/28/2016 scw]
	eItemAct_CreateItemByGrowupReward = 46, // �ɳ���ȡ���� [9/28/2016 scw]
	eItemAct_CreateItemByGrowupBox = 47, // �ɳ���ȡ���� [9/28/2016 scw]
	eItemAct_CreateItemByDreamStonePageBox = 48, // ��ʯ��ȡ���� [1/9/2017 scw]
	eItemAct_CreateItemByArenaTotalAwardBox = 49, // �������ۼ�ʤ�������� [2/26/2017 scw]
	eItemAct_CreateItemCreateFromShopGift = 50,// �̵깺����� [12/28/2015 superrobert]
	eItemAct_CreateItemCreateFromShopTimeLimitGift = 51,// �����ʱ��� [6/13/2017 "tianshuai"]
	eItemAct_CreateItemCreateFromShopSmallGift = 52,// С��������� [6/13/2017 "tianshuai"]
	eItemAct_CreateItemByDailyTask7 = 53, // ���������ñ��� [6/15/2017 scw]
	eItemAct_CreateItemByLimitLevelGift = 54, // ��ʱ�ȼ���� [8/25/2017 scw]

	eItemAct_OpenBattleAwardBox = 284,
	eItemAct_CompleteGuildBossBattle = 332, // ��ɹ���bossս�� [wangyiran]
	eItemAct_CreateItemFromZhanyiChapterAward = 333,// ս���½ڽ�����ȡ��� [6/13/2017 "tianshuai"]
	eItemAct_FirstPay = 580,// �׳� [6/17/2016 TianShuai]

	eItemAct_MgcSaleGiftAward = 581,// �Ż���� [12/22/2016 "tianshuai"]
	eItemAct_MgcRechargeAward = 582,// �ι�����ֵ� [12/22/2016 "tianshuai"]
	eItemAct_MgcLuckPanAward = 583,// �ι���LuckPan [12/22/2016 "tianshuai"]
	eItemAct_SpecialShopAward = 584,// �����̵��� [12/26/2016 "tianshuai"]
	eItemAct_InviteCodeAchievementAward = 585,// ������ɾͽ��� [4/17/2017 "tianshuai"]


	eItemAct_RemoveBattlePickItem = 1000,// ���ؿ���ʰȡ����Ʒɾ�� [yangzhen]
	eItemAct_RemoveLogoutTempItem = 1001,// ����ʱ������ʱ��Ʒ [yangzhen]
	eItemAct_RemoveDepleteWishWin	= 1002,// ����ս����Ը�� [xingzhishan]
	eItemAct_RemoveDepleteWishGod = 1003,// ����������Ը�� [xingzhishan]
	eItemAct_RemoveChipByCreateHeroCard = 1004,// �ٻ��������� [wangyiran]
	eItemAct_RemoveSellBagItem = 1005,// ���۱�����Ʒ [wangyiran]
	eItemAct_RemoveBySweepBattle = 1006,// ɨ�� [wangyiran]
	eItemAct_RemoveTempItemBeforeRecover = 1007,// ��ʼ�ָ���ʱ��ƷǰҪɾ�����е� [yangzhen]
	eItemAct_RemoveSpellLevelUp = 1008,// �������� [yangzhen]
	eItemAct_RemoveGM_REMOVEITEM = 1009,// gmɾ����Ʒ [wangyiran]
	eItemAct_RemoveUseItem = 1010, // ͨ��ʹ����Ʒ [tianshuai]
	eItemAct_RemovePlayerRenameUseItem = 1011,//���ͨ��ʹ����Ʒ����
	eItemAct_RemoveGuildRenameUseItem = 1012,//���ͨ��ʹ����Ʒ�Ĺ�����
	eItemAct_RemoveByHeroStar = 1013,//���ͨ��ʹ����Ʒ�Ĺ�����
	eItemAct_RemoveByOpenBox = 1014,// ������������Ʒ [12/28/2015 superrobert]
	eItemAct_RemoveByHeroLvlUp = 1015,// Ӣ������ [12/28/2015 superrobert]
	eItemAct_RemoveByUseLaBa = 1016,// С���� [12/28/2015 superrobert]
	eItemAct_RemoveByScriptDelete = 1017,// �ű����� [12/28/2015 superrobert]
	eItemAct_RemoveByStar = 1018,// �������� [12/28/2015 superrobert]
	eItemAct_RemoveByHeroDevelop = 1019,// Ӣ���������� [12/28/2015 superrobert]
	eItemAct_RemoveByHeroAwaking = 1020,// �������� [12/28/2015 superrobert]
	eItemAct_RemoveByHeroBreak = 1021,// ͻ������ [12/28/2015 superrobert]
	eItemAct_RemoveByEquipExtraction = 1022,// ����ʯ����  [1/21/2016 admin] 
	eItemAct_CreateItemStar_Award = 1023,// �����������ѵ��� [1/27/2016 HJ]
	eItemAct_CreateItemBreak_Award = 1024,// ��������ͻ�Ƶ��� [1/27/2016 HJ]
	eItemAct_CreateItemEvolution_Award = 1025,//���������������� [1/27/2016 HJ]
	eItemAct_CreateItemDevelop_Award = 1026,// ���������������� [1/28/2016 HJ]
	eItemAct_RemoveByFriendGemLvlUp = 1027,// ���鱦ʯ����  [1/21/2016 superrobert] 
	eItemAct_RemoveByFriendGemWash = 1028,// ���鱦ʯϴ��  [1/21/2016 superrobert] 
	eItemAct_GemCompoundDestorySelf = 1029,// ��Ƭ�ϳ����ĵ���Ʒ [2/15/2016 HJ]
	eItemAct_CreateAbilityComposite = 1030,// װ����Ƭ�ϳ� [2/15/2016 HJ]
	eItemAct_CreateHandBookAward = 1031,// ͼ����ȡ���� [2/19/2016 HJ]
	eItemAct_SellEquipReturnItem = 1032,// ���۵��߷��� [2/26/2016 HJ]
	eItemAct_CreateSportsWinAward = 1033, // ������ս��ʤ���� [3/21/2016 scw]
	eItemAct_CreateByCityBuilding = 1034,  
	eItemAct_FindTreasureFindGrid = 1035, // Ѱ��̽��������Ʒ�ı� [3/30/2016 tianshuai]
	eItemAct_FindTreasureReady = 1036, // Ѱ��̽��������Ʒ�ı� [3/30/2016 tianshuai]
	eItemAct_FindTreasureGoBack = 1037, // ����Ӫ�� [3/30/2016 tianshuai]
	eItemAct_FindTreasureFinder = 1038, // Ѱ���һ� [3/30/2016 tianshuai]
	eItemAct_FindTreasureBagShop = 1039, // �����̵깺�� [3/30/2016 tianshuai]
	eItemAct_FindTreasureBagUpLevel = 1040, // ���ʱ������� [3/30/2016 tianshuai]
	eItemAct_FindTreasureBuyBread = 1041, // �������
	eItemAct_FindTreasureTransferItem = 1042, // Ѱ������Ʒ�����ʱ�����ʤ��Ʒ֮��ת��
	eItemAct_VipLevelGiftBuy = 1043, // vip�ȼ���Ȩ�������
	eItemAct_WorldBossKillerAward = 1044, // ����boss�񲹵����� [8/20/2016 scw]
	eItemAct_EquipEvolutionCost = 1045, // װ���������� [10/19/2016 scw]
	eItemAct_EquipMixEquip = 1046, // װ���ϳ�װ�� [2/16/2017 scw]
	eItemAct_EquipSmeltEquip = 1047, // װ������

	// ���������Ƚ����⣬����������Ʒ��ʵ��û����ɾ��Ʒ [12/28/2015 superrobert]
	eItemAct_Special_Swap_EquipBag = 2000,
	eItemAct_Special_ExchangeInventoryToInventory = 2001,

	eItemAct_SpecialGiftBuyGift = 2002,// ����������� [5/20/2016 HJ]
	eItemAct_CreateHeroWearEquip = 2003,// ����Ӣ��ʱ����װ�� [10/20/2016 HJ]

	eItemAct_HeroReturnItem = 2004,//Ӣ�ۻ�����Ʒ
	eItemAct_HeroTuJian = 2005,//Ӣ��ͼ��
	eItemAct_GuildGift = 2006,//��������
	eItemAct_MoneyTreeCost, // ҡǮ������ [8/11/2017 chief]
	eItemAct_EquipTreasure, //װ���ᱦ������
	eItemAct_EquipTreasureGetBoxReward, //װ���ᱦ��ȡ���佱��

	eItemAct_NUM,
};
enum eWorldShopCashActType
{
	eWorldShopCashAct_Unknow			= 0,	/* δ֪������� */
	eWorldShopCashAct_RequestSell,			/* ����������ֽ� */
	eWorldShopCashAct_MarketSuccessed,		/* �ֽ��ϼܳɹ� */
	eWorldShopCashAct_MarketFailed,			/* �ֽ��ϼ�ʧ�� */
	eWorldShopCashAct_MarketReturnNullConnecter,	/* �����ϼܽ��ʱGameConnectorָ��Ϊ�� */
	eWorldShopCashAct_MarketReturnNullPlayer,	/* �����ϼܽ��ʱHumanָ��Ϊ�� */
	eWorldShopCashAct_MarketReturnNullMap,		/* �����ϼܽ��ʱMapָ��Ϊ�� */
	eWorldShopCashAct_MarketReturnNullGamePlayer,	/* �����ϼܽ��ʱPlayerָ��Ϊ�� */

	eWorldShopCashAct_RequestBuy,			/* ����������ֽ� */
	eWorldShopCashAct_BuySuccessed,			/* ����ɹ� */
	eWorldShopCashAct_BuyFailed,			/* ����ʧ�� */
	eWorldShopCashAct_BankTakeReturnNullConnector,	/* ������ȡʱGameConnectorָ��Ϊ�� */
	eWorldShopCashAct_BankTakeReturnNullGamePlayer, /* ������ȡʱHUMANָ��Ϊ�� */
	eWorldShopCashAct_BankTakeReturnNullMap,	/* ������ȡʱSCENEָ��Ϊ�� */
	eWorldShopCashAct_BankTakeReturnNullPlayer,	/* ������ȡʱPLAYERָ��Ϊ�� */
	eWorldShopCashAct_BankTakeSuccessed,		/* ������ȡʱ�ɹ� */
	eWorldShopCashAct_NullUser,			/* pKPlayerΪ�� */
	eWorldShopCashAct_ClearCash,			/* ���̵�����������ֽ� */
	eWorldShopCashAct_SellTimeoutReturnToBank,	/* ���۳�ʱ���ֽ���̵�ת������ */
	eWorldShopCashAct_SystemBuy,			/* ϵͳ�ع����ֽ���̵�ת������ */
	eWorldShopCashAct_NullShop,			/* ShopΪ�� */
	eWorldShopCashAct_NullSrvConnecter,		/* ServerPlayerΪ�� */
	eWorldShopCashAct_ShopFull,			/* �̵��� */
	eWorldShopCashAct_BankFull,			/* ������ */
	eWorldShopCashAct_UserFull,			/* ����ڸ��̵��еļ��ۻ����Ѵﵽ���� */
	eWorldShopCashAct_ClearInTheBank,		/* ����������������ֽ� */
	eWorldShopCashAct_LoadShopFromDB,		/* ����ʱ��DB��ʵ��LOADһ���̵����� */
	eWorldShopCashAct_LoadBankFromDB,		/* ����ʱ��DB��ʵ��LOADһ���������� */
	eWorldShopCashAct_ShouldLoadShopFromDB,		/* ����ʱ��DB��Ӧ��LOAD��һ���̵����� */
	eWorldShopCashAct_ShouldLoadBankFromDB,		/* ����ʱ��DB��Ӧ��LOAD��һ���������� */
	eWorldShopCashAct_AddShopFailed,		/* �����̵�ʧ�� */
	eWorldShopCashAct_DelShopFailed,		/* ���̵�ɾ��ʱʧ�� */
	eWorldShopCashAct_AddBankFailed,		/* ��������ʧ�� */
	eWorldShopCashAct_NUM
};
enum eEquipActType
{
	eEquipAct_Unknow			= 0,	/* δ֪������� */
	eEquipAct_LevelUp			= 10,	/* ����*/
	eEquipAct_AttrRefine		= 20,	/* ϴ�� */
	eEquipAct_AttrRefineConfirm	= 30,	/* ϴ��ȷ�� */
	eEquipAct_NUM
};

enum eCheatActOpType
{
	eCheatActOp_Unknow				= 0,	/* δ֪���� */
	eCheatActOp_Log,	/* ֻдLOG */
	eCheatActOp_Kill,	/* �ߵ� */
	eCheatActOp_Answer,	/* ���� */
};

enum ePetActType
{
	ePetAct_Unknow					= 0,	/* ����δ֪���� */
	ePetAct_Catch					= 10,	/* ���ﲶ׽ */
	ePetAct_ScriptCreate				= 11,	/* �ű��������� */
	ePetAct_ScriptDestroy				= 12,	/* �ű�ɾ������ */
	ePetAct_PlayerFree				= 13,	/* ��ҷ��� */
	ePetAct_ResumeAllHorseHappy			= 14,	/* �ָ�������˿��ֶ� */
	ePetAct_GeneraAdd				= 20,	/* ���ﷱֳʱ���ӳ��� */
	ePetAct_GeneraLost				= 21,	/* ���ﷱֳʱʧȥ���� */
	ePetAct_ComposeMain				= 22,	/* ��������ʱ������ */
	ePetAct_ComposeSub				= 23,	/* ��������ʱ������ */
	ePetAct_ComposeNew				= 24,	/* ��������ʱ�³��� */
	ePetAct_ComposeRep				= 25,	/* ����������ָ������� */
	ePetAct_ComposeDel				= 26,	/* ���������󷴻�ɾ���³��� */
	ePetAct_TradeOther				= 30,	/* ���ｻ�׻�� */
	ePetAct_StallTradeAcquire			= 40,	/* ��̯���׻�� */
	ePetAct_PlayerShopTradeAcquire			= 50,	/* ����̵꽻�׻�� */
	ePetAct_PlayerShopMarket			= 51,	/* ����̵��ϼ� */
	ePetAct_PlayerShopMarketGoWay			= 52,	/* ����̵��¼� */
	ePetAct_GMCreate				= 60,	/* GM �޸���� */
	ePetAct_GMDestroy				= 61,	/* GM �޸����� */

	ePetAct_PlayerLock				= 62,	/* ������� */
	ePetAct_PlayerUnlock			= 63,	/* ��˽��� */

	ePetAct_ReturnToChild			= 70,	/* ��˻�ͯ */
	ePetAct_ReturnToChildExt		= 71,	/* ��˻��� */

	ePetAct_NUM
};

//��Ʊ������صĲ���
enum eStockActType
{
	eStockAct_Unknow			= 0,	// δ֪������� 
	eStockAct_LoadDataFromDB,			// ����ʱ��DB��ʵ��LOADһ������
	eStockAct_AutoBuyMarketOK,			// ���ϼܳɹ� 
	eStockAct_AutoBuyMarketFailed,		// ���ϼ�ʧ��
	eStockAct_AutoSellMarketOK,			// �����ϼܳɹ� 
	eStockAct_AutoSellMarketFailed,		// �����ϼ�ʧ��
	eStockAct_ClearOvertime,			// ��ʱ����ҵ�
	eStockAct_ClearTrade,				// ��������ҵ�
	eStockAct_ClearRepeal,				// ��ҳ���ʱ����ҵ�
	eStockAct_BuyListFull,				// ��̫���б���
	eStockAct_SellListFull,				// ����̫���б���
	eStockAct_UserBuyFull,				// �û����Ѵ�����
	eStockAct_UserSellFull,				// �û������Ѵ�����
	eStockAct_SystemBuy,				// ϵͳ�ع�
	eStockAct_TradeOK,					// ���׳ɹ�
	eStockAct_LoadFromDBNodeFull,		// ����ʱ��DB��LOADһ������ʧ��(����ѱ�ռ��)
	eStockAct_SMUNodeNull,				// �����ڴ���Ϊ��
	eStockAct_AutoBuyMarketSMUNull,		// ���ϼ�ʱ �����ڴ���Ϊ��
	eStockAct_AutoSellMarketSMUNull,	// �����ϼ�ʱ�����ڴ���Ϊ��
	eStockAct_NUM
};

//��Ʊ�˻���صĲ���
enum eStockAccActType
{
	eStockAccAct_Unknow			= 0,		// δ֪������� 
	eStockAccAct_LoadDataFromDB,			// ����ʱ��DB��ʵ��LOADһ������
	eStockAccAct_ClearOvertime,				// ��ʱ����˻�
	eStockAccAct_ClearAccount,				// ����˻�����ҳ����˻�
	eStockAccAct_CharMoneyToStockCost,		// ��ֵʱ��MAP�۳���ɫ�ֽ������
	eStockAccAct_CharMoneyToStockFailedGL,	// ��GL��ֵʧ��
	eStockAccAct_CharMoneyToStockAdd,		// ��ֵʱ��GL���˻������ֽ������
	eStockAccAct_CharMoneyToStockFailedMAP,	// ��ֵʧ�ܣ�MAP�ع�Money
	eStockAccAct_StockMoneyToCharkCost,		// ���ɫת��ʱ��GL�۳��˻��ֽ������
	eStockAccAct_StockMoneyToCharFailedkGL,	// ���ɫת��ʱ��GLʧ��
	eStockAccAct_StockMoneyToCharkAdd,		// ���ɫת�˳ɹ���MAP���ӽ�ɫ�ֽ������
	eStockAccAct_StockMoneyToCharkFailed,	// ���ɫת��ʧ��
	eStockAccAct_GoldToCharkFailedAdd,		// ���ɫת��ʧ��,GL�ع��ֽ�
	eStockAccAct_SillerToCharkFailedAdd,	// ���ɫת��ʧ��,GL�ع�����
	eStockAccAct_NullUser,					// pKPlayerΪ�� 
	eStockAccAct_AutoSaleCostGold,			// ί������: �۳������������ �ֽ�
	eStockAccAct_AutoBuyCostSiller,			// ί�����: �۳������������ ����
	eStockAccAct_AutoSaleAddSiller,			// ί������: ��������
	eStockAccAct_AutoBuyAddGold,			// ί�����: �����ֽ�
	eStockAccAct_FastBuyCostSiller,			// �ֶ����: �۳������������ ����
	eStockAccAct_FastSaleCostGold,			// �ֶ�����: �۳������������ �ֽ�
	eStockAccAct_FastBuyAddGold,			// �ֶ����ɹ�: �����ֽ�
	eStockAccAct_FastSaleAddSiller,			// �ֶ�����ɹ�: ��������
	eStockAccAct_GoldLimit,					// �ֽ𳬹�����
	eStockAccAct_SillerLimit,				// ������������
	eStockAccAct_GoldLack,					// �ֽ���
	eStockAccAct_SillerLack,				// ��������
	eStockAccAct_BuyOvertimeAdd,			// �򵥳�ʱ,����ת���˻�
	eStockAccAct_SellOvertimeAdd,			// ������ʱ,�ֽ�ת���˻�
	eStockAccAct_RepealTradeAddGold,		// ��ҳ������ֽ�ת���˻�
	eStockAccAct_RepealTradeAddSiller,		// ��ҳ���������ת���˻�
	eStockAccAct_AddSillerOwnerNull,		// ���˻���������ʱ����˻�Ϊ��
	eStockAccAct_AddGoldOwnerNull,			// ���˻������ֽ�ʱ����˻�Ϊ��
	eStockAccAct_RegisterOK,				// �����ɹ�
	eStockAccAct_LoadFromDbNullAcc,			// ������DB��LOAD����ʱ���û���Ʊ�˻�Ϊ��
	eStockAccAct_StockRegisterCostMoney,	// ����������
	eStockAccAct_StockRegisterFailedAddMoney,//����ʧ�ܷ�������
	eStockAccAct_CoolingCostGold,			 //������ȴ״̬��۳��ֽ�
	eStockAccAct_CoolingCostSiller,			 //������ȴ״̬��۳�����
	eStockAccAct_StockAccDieAddMoney,		 //��Ʊ�˻��������ʽ𷵻ؽ�ɫ
	eStockAccAct_NUM
};

enum eGuildActType
{
	eGuildAct_Unknow			= 0,		// δ֪������� 
	eGuildAct_Exp_GM			= 1,		// ��ᾭ�飬GMָ��ı�
	eGuildAct_Exp_Script		= 2,		// ��ᾭ�飬�ű�����
	eGuildAct_Money_GM			= 3,		// ����Ǯ��GMָ��ı�
	eGuildAct_Money_Script		= 4,		// ����Ǯ���ű�����
	eGuildAct_ShiMing_GM		= 5,		// ���ʹ���㣬GMָ��ı�
	eGuildAct_ShiMing_Script	= 6,		// ���ʹ���㣬�ű�����
	eGuildAct_Money_Leveldown	= 7,		// ��ή�������������ƵĽ�Ǯ�۳�
	eGuildAct_ShiMing_RemoveMember = 8,		// �뿪��Ա��ʹ����۳�
	eGuildAct_ShiMing_AddMember = 9,		// ���ӳ�Ա��ʹ��������
	eGuildAct_ShiMing_SystemCheck = 10,		// ϵͳÿ�¼������
	eGuildAct_Exp_Skill		= 11,			// ��ᾭ�飬��Ἴ������
	eGuildAct_Money_Skill		= 12,			// ����Ǯ����Ἴ������
	eGuildAct_BusSkill_LevelUp_Money = 13,	// ������޼�������,���İ���Ǯ
	eGuildAct_BusSkill_LevelUp_Exp = 14,	// ������޼�������,���İ�ᾭ��
	eSkillAct_GuildBusSkill_LevelUp = 15,	/// ������޼�������
	eGuildAct_Exp_PriseByShiMing	= 16,	/// ��ᾭ�飬����
	eGuildAct_Fund_CurMonth		= 17,		// ���°�����ı�
	eGuildAct_Fund_LastMonth	= 18,		// ���°�����ı�
	eGuildAct_NUM
};

enum eZhuanJingActType
{
	eZhuanjingAct_Unknow		= 0,			/* δ֪���� */
	eZhuanjingAct_ZhuanGong_Get		= 10,			/* ר��������� */
	eZhuanjingAct_ZhuanFang_Get		= 20,			/* ר��������� */
	eZhuanjingAct_ZhuanGong_Coefficient		= 30,			/* ר��ϵ������ */
	eZhuanjingAct_ZhuanFang_Reallocate		= 40,			/* ר���������� */
	eZhuanjingAct_ZhuanGong_GM		= 50,			/* ͨ�� GM���� ר��������� */
	eZhuanjingAct_ZhuanFang_GM		= 60,			/* ͨ�� GM���� ר��������� */
	eZhuanjingAct_Zhuangong_LevelUp = 70,			/* ר������ */
	eZhuanjingAct_Zhuanfang_LevelUP = 80,		/* ר������ */
	eZhuanjingAct_NUM
};

#endif /* _ACT_DEFINE_H_ */

/* end ??? */
