/*$T MapServer/Server/Base/act_define.h GC 1.140 10/10/09 10:07:20 */

/* act_define.h $v1.0 97203 */
#ifndef _ACT_DEFINE_H_
#define _ACT_DEFINE_H_

#define CONVERT_ITEMACT_TO_CURRENCYACT(var) ((eCurrencyActType)(eCurrencyAct_SpecialItemConvert + var))

enum eSkillActType
{
	eSkillAct_Unknow		= 0,			/* 未知技能操作 */
	eSkillAct_ScriptTrain		= 10,			/* 技能学习 */
	eSkillAct_LevelUp		= 20,			/* 技能升级 */
	eSkillAct_ScriptAbandon		= 30,			/* 技能放弃 */
	eSkillAct_ScriptChangeFaction	= 31,			/* 脚本调用ChangeMenpai */
	eSkillAct_GMModify		= 40,			/* 通过GM命令 学习 */
	eSkillAct_GuildSkillLevelUp_Guild	= 50,			/* 帮会技能（天赋修行）升级 帮会*/
	eSkillAct_GuildSkillLevelUp_Person		= 51,			/* 帮会技能（天赋修行）升级 个人*/
	eSkillAct_InherenceBalance_LevelUp		= 60,			/* 天赋平衡升级*/
	eSkillAct_NUM
};
enum eAbilityActType
{
	eAbilityAct_Unknow		= 0,			/* 未知技能操作 */
	eAbilityAct_ScriptTrain		= 10,			/* 生活技能学习 */
	eAbilityAct_LevelUp		= 20,			/* 生活技能升级 */
	eAbilityAct_ScriptAbandon	= 30,			/* 生活技能放弃 */
	eAbilityAct_GMModify		= 40,			/* 通过GM命令 学习 */
	eAbilityAct_GMModifyAll		= 41,			/* 通过GM命令学习所有 */
	eAbilityAct_AddGuanqiaSpell = 42,			/* 关卡技能添加*/
	eAbilityAct_DelGuanqiaSpell = 43,			/* 关卡技能删除*/
	eAbilityAct_NUM
};
enum eCurrencyActType
{
	eCurrencyAct_Unknow = 0,	/* 未知 */
	eCurrencyAct_AltarHeroCard = 1, // 召唤英雄卡牌 [wangyiran]	
	eCurrencyAct_BattleGateAward =2,	// 战场获胜的奖励 [yangzhen]
	eCurrencyAct_OpenAwardBoxCreate = 3, // 开奖励宝箱所得 [yangzhen]
	eCurrencyAct_CompleteEliteBattle = 4, // 精英副本战役奖励 [yangzhen]
	eCurrencyAct_SystemAutoPhysical = 5,	// 自动增加体力 [xingzhishan]
	eCurrencyAct_SystemAutoSpirit = 6,	// 自动增加精力 [xingzhishan]
	eCurrencyAct_PhysicalChangeByLevelUP = 7,//升级改变体力 [xingzhishan]
	eCurrencyAct_SpiritChangeByLevelUP =8,// 升级改变精力 [xingzhishan]
	eCurrencyAct_BuyArenaChallengeCount =9,// 购买竞技场挑战次数改变钻石 [xingzhishan]
	eCurrencyAct_ArenaChallengeAward = 10, // 竞技场挑战奖励 [xingzhishan]
	eCurrencyAct_BattleReturn = 11, // 副本未完成就返回，返还体力精力 [wangyiran]
	eCurrencyAct_BankExchageGold = 12, // 摇钱树
	eCurrencyAct_GoldChangeByLevelUP =13,// 升级改变金币 [wangxiaoyong]
	eCurrencyAct_FirstPayAward = 14, //	首冲奖励 [xingzhishan]
	eCurrencyAct_SweepBattleGateAward = 15, //	副本扫荡 [wangyiran]
	eCurrencyAct_FromShopMallDiamond = 16, //商店购买钻石
	eCurrencyAct_SellItemGiveMoney = 17,// 卖物品给钱 [12/28/2015 superrobert]
	eCurrencyAct_CallScriptIncome = 18,// 脚本调用 [12/28/2015 superrobert]
	eCurrencyAct_GMGiveMoney = 19,// gm命令 [12/28/2015 superrobert]
	eCurrencyAct_ByFriend = 20,// 好友赠送 [12/29/2015 superrobert]
	eCurrencyAct_SportsHonor = 21, // 竞技挑战加荣誉币 [3/17/2016 scw]
	eCurrencyAct_GodTowerClear = 22, // 神域之塔周清 [3/17/2016 scw]
	eCurrencyAct_PhysicalChangeByItem = 23,// 使用道具改变体力 [8/9/2016 scw]
	eCurrencyAct_ShopBuyGift = 24,//  [6/3/2017 "tianshuai"] 商店购买礼包
	eCurrencyAct_ShopBuyTimeLimitGift = 25,// 大额限时礼包 [6/3/2017 "tianshuai"]
	eCurrencyAct_ShopBuySmallGift = 26,// 小额礼包 [6/3/2017 "tianshuai"]

	eCurrencyAct_FinishGuildBossBattle = 607, // 完成一场工会boss战斗 [wangyiran]
	eCurrencyAct_IndentureCompose = 608, // 契约卸下 [12/6/2016 "tianshuai"]
	eCurrencyAct_MgcLuckPanShip = 609, // mgc转盘 [12/26/2016 "tianshuai"]
	eCurrencyAct_SpecialShopBuy = 610, // 特殊商店活动购买 [12/26/2016 "tianshuai"]
	eCurrencyAct_SpecialShopRef = 611, // 特殊商店刷新 [12/26/2016 "tianshuai"]

	eCurrencyAct_GoldChangeByPeakArena = 615,// 巅峰竞技场战斗改变金币 [chentongxin]
	eCurrencyAct_DotaCoinChangeByPeakArena = 616,// 巅峰竞技场战斗改变竞技场勋章[chentongxin]
	eCurrencyAct_PeakArenaShopBuyChangeMoney = 617, // 巅峰竞技场商店购买 [chentongxin]
	eCurrencyAct_BuyPeakArenaChallengeCount = 618,// 购买竞技场挑战次数改变钻石 [chentongxin]
	eCurrencyAct_MiningBuyRobCount = 619,// 购买掠夺次数[tianshuai]
	eCurrencyAct_ChangePeakArenaCoinByPlayer = 620, //用户主动领取巅峰币 [chentongxin]
	eCurrencyAct_ChangeBuyAbattoirTimes, //用户购买挑战次数
	eCurrencyAct_ChangeFreshAbattoirCD, // 用户购买刷新自己挑战cd
	eCurrencyAct_MoneyTreeCost,	// 摇钱树消耗

	eCurrencyAct_BuyItemFromGuildShop = 992,// 购买帮会物品 [wangyiran]
	eCurrencyAct_GuildEatDinner = 993,// 参加聚餐 [wangyiran]
	eCurrencyAct_GuildStartDinner = 994,// 开启钻石聚餐 [wangyiran]
	eCurrencyAct_GuildMakeDishSuccess = 995,// 烹饪成功领奖 [wangyiran]
	eCurrencyAct_BuyDinnerCount = 996, // 购买聚餐烹饪次数 [wangyiran]
	eCurrencyAct_upLevel_Spell = 997, //  技能升级 [8/10/2016 TianShuai]
	eCurrencyAct_ArenaRefEnemy = 998,	// 竞技场刷新对手消耗 [8/10/2016 TianShuai]
	eCurrencyAct_WonderfulActivityVipGift = 999,	// 精彩活动vip等级礼包 [7/26/2016 TianShuai]
	eCurrencyAct_EnterBattleCost =1000,	// 进入副本关卡消耗 [yangzhen]
	eCurrencyAct_SweepBattleGateCost = 1001, // 扫荡战役关卡消耗 [yangzhen
	eCurrencyAct_BuyPhysical = 1002,// 购买体力 [xingzhishan]
	eCurrencyAct_BuySpirit = 1003,// 购买精力 [xingzhishan]
	eCurrencyAct_BuyPhysicalChangeDiamond = 1004, // 购买体力改变钻石 [xingzhishan]
	eCurrencyAct_BuySpiritChangeDiamond = 1005, // 购买精力改变钻石 [xingzhishan]
	eCurrencyAct_StarCost =1006,// 觉醒消耗 [wangyiran]
	eCurrencyAct_BankDiamondCost = 1007, // 银行消耗钻石 [xingzhishan]
	eCurrencyAct_BuyXuYuanFu =1008, // 购买许愿符[wangxiaoyong]
	eCurrencyAct_BuyEliteBattleChangeMoney = 1009, // 精英副本挑战次数购买 [xingzhishan]
	eCurrencyAct_ShenyuzhichengCost = 1010, // 神谕之城许愿 [yangzhen]
	eCurrencyAct_ResetChallenageLastCountCost = 1011, // 重置竞技场挑战次数 [yangzhen]
	eCurrencyAct_DepleteChallenageLastCountCost = 1012, // 消费竞技场挑战次数 [yangzhen]
	eCurrencyAct_BuyArenaShopChangeMoney = 1013, // 竞技场商店购买 [xingzhishan]
	eCurrencyAct_FlushShopdeplete = 1014, //	商店刷新消耗 [xingzhishan]
	eCurrencyAct_CreateGuildCost =1015,// 创建帮会 [wangyiran]
	eCurrencyAct_BattleSweepCost =1016, //扫荡消费
	eCurrencyAct_1STBlackStoreBuyCost =1017,// 一级黑店购买消耗 [yangzhen]
	eCurrencyAct_2NDBlackStoreCost =1018,// 二级黑店购买消耗 [yangzhen]
	eCurrencyAct_3RDBlackStoreBuyCost =1019,// 三级黑店购买消耗 [yangzhen]
	eCurrencyAct_LuckPanCost =1020,//幸运盘购买消耗 [yangzhen]
	eCurrencyAct_HeroStarCost =1021, // 英雄觉醒消耗 [wangyiran]
	eCurrencyAct_SpellUpCost = 1022,// 升级技能 [12/28/2015 superrobert]
	eCurrencyAct_GMmodifyCost = 1023,// gm工具消耗 [12/28/2015 superrobert]
	eCurrencyAct_ShopCost = 1024,// 商店购买 [12/28/2015 superrobert]
	eCurrencyAct_GoldChangeByChatLabaCost = 1025,// 喇叭消耗 [12/29/2015 superrobert]
	eCurrencyAct_HeroDevelopCost = 1026,// 英雄培养 [12/29/2015 superrobert]
	eCurrencyAct_AwakingCost =1027,// 进化消耗 [wangyiran]
	eCurrencyAct_HeroBreakCost =1028,// 突破消耗 [wangyiran]
	eCurrencyAct_HeroTransferCost = 1029,//转移消耗[tianshuai]
	eCurrencyAct_EquipStrongerCost =1030,// 装备强化消耗金钱 [chentongxin]
	eCurrencyAct_FiendGemLvlUpCost =1031,// 友情宝石升级 [wangyiran]
	eCurrencyAct_FiendGemWashCost =1032,// 友情宝石洗练 [wangyiran]
	eCurrencyAct_ArtifactDestroyCost = 1033,// 神器打碎消耗钻石 [tianshuai]
	eCurrencyAct_ArtifactUpdateLevelCost = 1034,//神器升级消耗
	eCurrencyAct_ArtifactBuyCost = 1035,// 神器购买 [tianshuai]
	eCurrencyAct_ArtifactDestroyGive = 1036,// 神器打碎返还神之祝福 [tianshuai]
	eCurrencyAct_ChipGemCompoundCost = 1037,// 碎片合成消耗货币 [2/15/2016 HJ]
	eCurrencyAct_DepleteEnegyCost = 1038,//消耗能量
	eCurrencyAct_DepleteHounorCost = 1039,//消耗荣誉
	eCurrencyAct_AddEnegyByTimer = 1040,  //定时增加能量
	eCurrencyAct_GodTowerBuyCount = 1041, // 神域之塔购买次数 [3/2/2016 scw]
	eCurrencyAct_SportsRefreshCount = 1042, // 竞技挑战刷新次数 [3/12/2016 scw]
	eCurrencyAct_SportsRaise = 1043, // 竞技挑战鼓舞 [3/12/2016 scw]
	eCurrencyAct_WorldBossRaise = 1044, // 世界boss鼓舞 [4/6/2016 scw]
	eCurrencyAct_CityBuilding = 1045, // 城建消耗相关
	eCurrencyAct_FindTreasureFindGrid = 1046, // 寻宝探索格子获得物品 [3/30/2016 tianshuai]
	eCurrencyAct_RenameCostDiamond = 1047,// 改名字消耗钻石 [4/19/2016 HJ]
	eCurrencyAct_FindTreasureFindBuyBagItem = 1048, // 购买寻宝物资商店物品 [3/30/2016 tianshuai]
	eCurrencyAct_FindTreasureFindBuyBread = 1049, // 寻宝购买面包 [3/30/2016 tianshuai]
	eCurrencyAct_FindTreasureFindSpecialShopBuy = 1050, // 寻宝购买面包 [3/30/2016 tianshuai]
	eCurrencyAct_FindTreasureFindSpecialShopRef = 1051, // 寻宝商人刷新 [3/30/2016 tianshuai]
	eCurrencyAct_BattleAchieveGold = 1052,	// 战场获得金币 [ASSERT]
	eCurrencyAct_BattleAchieveExp = 1053,	// 战场获得的经验 [ASSERT]
	eCurrencyAct_TakeCardCostPower = 1054,// 友情召唤消耗 [4/28/2016 HJ]
	eCurrencyAct_BuyCommonBattleChangeMoney = 1055,// 普通副本购买挑战次数消耗 [4/29/2016 HJ]
	eCurrencyAct_SuperEgoVipBuy = 1056, // 超能试炼购买次数 [4/28/2016 scw]
	eCurrencyAct_BuyEnergyChangeDiamond = 1057, // 购买能量消耗月亮 [5/3/2016 HJ]
	eCurrencyAct_BuyEnergy = 1058,// 购买能量 [5/3/2016 HJ]
	eCurrencyAct_VipLevelGiftBuy = 1059,// vip特权礼包购买 [5/11/2016 TianShuai]
	eCurrencyAct_SpecialGiftBuy = 1060,// 特殊礼包购买 [5/20/2016 HJ]
	eCurrencyAct_WorldBossBuy = 1061, // 世界boss购买战斗次数 [4/6/2016 scw]
	eCurrencyAct_UnEquipCost = 1062, // 卸下装备消耗钻石 [4/6/2016 scw]
	eCurrencyAct_EquipEvolutionCost = 1063, // 进阶装备消耗钻石 [10/19/2016 scw]
	eCurrencyAct_FriendShopBuy = 1064, // 友情商店购买物品消耗 [11/29/2016 scw]
	eCurrencyAct_HeroReturn = 1065, // 英雄回收消耗
	eCurrencyAct_ActivitySaleGift = 1066, // 活动优惠礼包 消耗钻石等货币 [12/23/2016 scw]
	eCurrencyAct_HeroUpLv = 1067,	//英雄升级消耗的金币
	eCurrencyAct_EquipMixEquip = 1068,	// 装备合成装备 [2/16/2017 scw]
	eCurrencyAct_GuildGift = 1069,	//公会赠送
	eCurrencyAct_ResetGuildBoss = 1070,	//工会boss重置
	eCurrencyAct_GuildBossBuyCount = 1071,	//工会boss挑战次数购买
	eCurrencyAct_GuildBossInspire = 1072,	//工会boss祈祷消耗
	eCurrencyAct_BuyHellBattleChangeMoney = 1073, // 地狱副本挑战次数购买 tianshuai
	eCurrencyAct_NUM,
	eCurrencyAct_SpecialItemConvert =10000,// 掠夺刷新 [xingzhishan]
};
enum eQuestActType
{
	eQuestAct_Unknow				= 0,	/* 未知任务操作 */
	eQuestAct_AddQuest				= 10,	/* 增加任务 */
	eQuestAct_DelQuest				= 20,	/* 删除任务（有可能是完成后删除） */
	eQuestAct_ParamChange				= 30,	/* 任务参数变化 */
	eQuestAct_DoneFlag				= 40,	/* 任务完成标志变化 */
	eQuestAct_DataChange				= 50,	/* 任务全局数据变化 */
	eQuestAct_CycleDoneFlag				= 60,	/* 循环任务完成一次标志 */

	eQuestAct_NUM
};
enum eItemActType
{
	eItemAct_Unknow = 0,	/* 未知物品拾取 */
	eItemAct_CreateItemByPost = 1,//通过邮件创建物品
	eItemAct_CreateItemByBattlePickItem = 2,// 将关卡中获得的拾取物品给玩家 [yangzhen]
	eItemAct_CreateItemByOpenBattleAwardBox = 3,// 获取战役宝箱 [yangzhen]
	eItemAct_CreateItemByCompleteBattleGate = 4,// 完成精英副本奖励 [yangzhen] //完成战役关卡（包括其他关卡）奖励
	eItemAct_CreateItemBySweepBattleGate = 5,// 扫荡副本关卡奖励 [yangzhen]
	eItemAct_CreateItemByAchievementAward = 6,// 成就奖励 [wangyiran]
	eItemAct_CreateItemByGodCity = 7,// 创建物品通过神域之城 [xingzhishan]
	eItemAct_CreateItemByCheckIn = 8,// 签到奖励 [wangyiran]
	eItemAct_CreateItemByCheckDailyTask = 9,// 日常任务奖励 [wangyiran]
	eItemAct_CreateItemByHeroCard = 10,// 卡牌转换 [wangyiran]
	eItemAct_CreateItemByRecoverBackupTemp = 11,// 恢复备份的临时物品 [yangzhen]
	eItemAct_CreateItemByGrowFundAwardDiamond = 12,// 成长基金奖励钻石 [wangyiran]
	eItemAct_CreateItemByMonthCardEveryDayAward = 13,// 领取月卡 [xingzhishan]
	eItemAct_CreateItemByZhanyiPreGuide = 14,// 领取战前引导奖励 [wangyiran]
	eItemAct_CreateItemByOfflineTimeAward = 15,// 离线时间奖励 [xingzhishan]
	eItemAct_CreateItemByDayContinueAssignGift = 16,// 日签到 [wangyiran]
	eItemAct_CreateItemByMonthAssignGift = 17,// 月签到 [wangyiran]
	eItemAct_CreateItemByLvlAssignGift = 18,// 等级礼包 [wangyiran]
	eItemAct_CreateItemByVIPGIFTBOX = 19,// VIP等级礼包 [wangyiran]
	eItemAct_CreateItemByCharge_Award = 20,// 充值活动奖励 [wangyiran]
	eItemAct_CreateItemByConsume_Award = 21,// 消费活动奖励 [wangyiran]
	eItemAct_CreateItemByLuck_Pan = 22,// 幸运转盘 [xingzhishan]
	eItemAct_CreateItemByYunYingActivityAward = 23,// 运营活动 [yangzhen]
	eItemAct_CreateItemByGhostMonth = 24,// 鬼月 [yangzhen]
	eItemAct_CreateItemByVipGiftActivity = 25,// vip gift [yangzhen]
	eItemAct_CreateItemByExchangeByHeroChip = 26,// 用英雄碎片兑换 [wangyiran]
	eItemAct_CreateItemByExchangeByNymph = 27,// 女神嫁到 [tianshuai]
	eItemAct_CreateItemByExchangeByChangeRune = 28,// 符文兑换 [tianshuai]
	eItemAct_CreateItemByDayChargeSection_Award = 29,// 分段单日充值 [wangyiran]
	eItemAct_CreateItemByWeekChargeSection_Award = 30,// 分段周充值 [wangyiran]
	eItemAct_CreateItemByDayConsumeSection_Award = 31,// 分段单日消费 [wangyiran]
	eItemAct_CreateItemByWeekConsumeSection_Award = 32,// 分段周消费 [wangyiran]
	eItemAct_CreateItemByOpenBox = 33,// 开宝箱给物品 [12/28/2015 superrobert]
	eItemAct_CreateItemCreateFromShop = 34,// 商店购买 [12/28/2015 superrobert]
	eItemAct_CreateItemBoxRecycle = 35,// 掉落 [12/28/2015 superrobert]
	eItemAct_CreateItemFromScript = 36,// 脚本调用 [12/29/2015 superrobert]
	eItemAct_CreateItemFromGM = 37,// gm命令 [12/29/2015 superrobert]
	eItemAct_ExchangeInventoryToEquip = 38, // 穿装备 [1/18/2016 superrobert]
	eItemAct_CreateItemFromUnUseEquip = 39,// 脱卸装备  [1/21/2016 superrobert] 
	eItemAct_TransferItemToOtherBag = 40,// 转移物品到其他背包 [4/13/2016 HJ]
	eItemAct_CreateItemByWeekCardEveryDayAward = 41,// 领取周卡 [5/10/2016 TianShuai] 
	eItemAct_CreateItemBySeasonCardEveryDayAward = 42,// 领取季卡 [5/10/2016 TianShuai] 
	eItemAct_CreateItemByYearCardEveryDayAward = 43,// 领取年卡 [5/10/2016 TianShuai] 
	eItemAct_CreateFromShop = 44,	/* 商店的创建 */
	eItemAct_CreateItemByPlayerGuide = 45, // 新手引导给物品 [9/28/2016 scw]
	eItemAct_CreateItemByGrowupReward = 46, // 成长领取奖励 [9/28/2016 scw]
	eItemAct_CreateItemByGrowupBox = 47, // 成长领取宝箱 [9/28/2016 scw]
	eItemAct_CreateItemByDreamStonePageBox = 48, // 梦石领取宝箱 [1/9/2017 scw]
	eItemAct_CreateItemByArenaTotalAwardBox = 49, // 竞技场累计胜利给宝箱 [2/26/2017 scw]
	eItemAct_CreateItemCreateFromShopGift = 50,// 商店购买礼包 [12/28/2015 superrobert]
	eItemAct_CreateItemCreateFromShopTimeLimitGift = 51,// 大额限时礼包 [6/13/2017 "tianshuai"]
	eItemAct_CreateItemCreateFromShopSmallGift = 52,// 小额礼包购买 [6/13/2017 "tianshuai"]
	eItemAct_CreateItemByDailyTask7 = 53, // 七天任务获得宝箱 [6/15/2017 scw]
	eItemAct_CreateItemByLimitLevelGift = 54, // 限时等级礼包 [8/25/2017 scw]

	eItemAct_OpenBattleAwardBox = 284,
	eItemAct_CompleteGuildBossBattle = 332, // 完成工会boss战斗 [wangyiran]
	eItemAct_CreateItemFromZhanyiChapterAward = 333,// 战役章节奖励领取获得 [6/13/2017 "tianshuai"]
	eItemAct_FirstPay = 580,// 首冲 [6/17/2016 TianShuai]

	eItemAct_MgcSaleGiftAward = 581,// 优惠礼包 [12/22/2016 "tianshuai"]
	eItemAct_MgcRechargeAward = 582,// 梦工厂充值活动 [12/22/2016 "tianshuai"]
	eItemAct_MgcLuckPanAward = 583,// 梦工厂LuckPan [12/22/2016 "tianshuai"]
	eItemAct_SpecialShopAward = 584,// 特殊商店获得 [12/26/2016 "tianshuai"]
	eItemAct_InviteCodeAchievementAward = 585,// 邀请码成就奖励 [4/17/2017 "tianshuai"]


	eItemAct_RemoveBattlePickItem = 1000,// 将关卡中拾取的物品删除 [yangzhen]
	eItemAct_RemoveLogoutTempItem = 1001,// 离线时清理临时物品 [yangzhen]
	eItemAct_RemoveDepleteWishWin	= 1002,// 消耗战神许愿符 [xingzhishan]
	eItemAct_RemoveDepleteWishGod = 1003,// 消耗天神许愿符 [xingzhishan]
	eItemAct_RemoveChipByCreateHeroCard = 1004,// 召唤卡牌消耗 [wangyiran]
	eItemAct_RemoveSellBagItem = 1005,// 出售背包物品 [wangyiran]
	eItemAct_RemoveBySweepBattle = 1006,// 扫荡 [wangyiran]
	eItemAct_RemoveTempItemBeforeRecover = 1007,// 开始恢复临时物品前要删除现有的 [yangzhen]
	eItemAct_RemoveSpellLevelUp = 1008,// 升级技能 [yangzhen]
	eItemAct_RemoveGM_REMOVEITEM = 1009,// gm删除物品 [wangyiran]
	eItemAct_RemoveUseItem = 1010, // 通用使用物品 [tianshuai]
	eItemAct_RemovePlayerRenameUseItem = 1011,//玩家通过使用物品改名
	eItemAct_RemoveGuildRenameUseItem = 1012,//玩家通过使用物品改工会名
	eItemAct_RemoveByHeroStar = 1013,//玩家通过使用物品改工会名
	eItemAct_RemoveByOpenBox = 1014,// 开宝箱消耗物品 [12/28/2015 superrobert]
	eItemAct_RemoveByHeroLvlUp = 1015,// 英雄升级 [12/28/2015 superrobert]
	eItemAct_RemoveByUseLaBa = 1016,// 小喇叭 [12/28/2015 superrobert]
	eItemAct_RemoveByScriptDelete = 1017,// 脚本调用 [12/28/2015 superrobert]
	eItemAct_RemoveByStar = 1018,// 觉醒消耗 [12/28/2015 superrobert]
	eItemAct_RemoveByHeroDevelop = 1019,// 英雄培养消耗 [12/28/2015 superrobert]
	eItemAct_RemoveByHeroAwaking = 1020,// 进化消耗 [12/28/2015 superrobert]
	eItemAct_RemoveByHeroBreak = 1021,// 突破消耗 [12/28/2015 superrobert]
	eItemAct_RemoveByEquipExtraction = 1022,// 淬炼石消耗  [1/21/2016 admin] 
	eItemAct_CreateItemStar_Award = 1023,// 进化创建觉醒道具 [1/27/2016 HJ]
	eItemAct_CreateItemBreak_Award = 1024,// 进化创建突破道具 [1/27/2016 HJ]
	eItemAct_CreateItemEvolution_Award = 1025,//进化创建进化道具 [1/27/2016 HJ]
	eItemAct_CreateItemDevelop_Award = 1026,// 进化创建培养道具 [1/28/2016 HJ]
	eItemAct_RemoveByFriendGemLvlUp = 1027,// 友情宝石升级  [1/21/2016 superrobert] 
	eItemAct_RemoveByFriendGemWash = 1028,// 友情宝石洗练  [1/21/2016 superrobert] 
	eItemAct_GemCompoundDestorySelf = 1029,// 碎片合成消耗的物品 [2/15/2016 HJ]
	eItemAct_CreateAbilityComposite = 1030,// 装备碎片合成 [2/15/2016 HJ]
	eItemAct_CreateHandBookAward = 1031,// 图鉴领取奖励 [2/19/2016 HJ]
	eItemAct_SellEquipReturnItem = 1032,// 出售道具返还 [2/26/2016 HJ]
	eItemAct_CreateSportsWinAward = 1033, // 竞技挑战连胜奖励 [3/21/2016 scw]
	eItemAct_CreateByCityBuilding = 1034,  
	eItemAct_FindTreasureFindGrid = 1035, // 寻宝探索格子物品改变 [3/30/2016 tianshuai]
	eItemAct_FindTreasureReady = 1036, // 寻宝探索格子物品改变 [3/30/2016 tianshuai]
	eItemAct_FindTreasureGoBack = 1037, // 返回营地 [3/30/2016 tianshuai]
	eItemAct_FindTreasureFinder = 1038, // 寻宝挂机 [3/30/2016 tianshuai]
	eItemAct_FindTreasureBagShop = 1039, // 物资商店购买 [3/30/2016 tianshuai]
	eItemAct_FindTreasureBagUpLevel = 1040, // 物资背包升级 [3/30/2016 tianshuai]
	eItemAct_FindTreasureBuyBread = 1041, // 面包购买
	eItemAct_FindTreasureTransferItem = 1042, // 寻宝将物品在物资背包和胜利品之间转移
	eItemAct_VipLevelGiftBuy = 1043, // vip等级特权礼包购买
	eItemAct_WorldBossKillerAward = 1044, // 世界boss神补刀奖励 [8/20/2016 scw]
	eItemAct_EquipEvolutionCost = 1045, // 装备进阶消耗 [10/19/2016 scw]
	eItemAct_EquipMixEquip = 1046, // 装备合成装备 [2/16/2017 scw]
	eItemAct_EquipSmeltEquip = 1047, // 装备熔炼

	// 下面两个比较特殊，背包交换物品，实际没有增删物品 [12/28/2015 superrobert]
	eItemAct_Special_Swap_EquipBag = 2000,
	eItemAct_Special_ExchangeInventoryToInventory = 2001,

	eItemAct_SpecialGiftBuyGift = 2002,// 特殊礼包买东西 [5/20/2016 HJ]
	eItemAct_CreateHeroWearEquip = 2003,// 创建英雄时穿戴装备 [10/20/2016 HJ]

	eItemAct_HeroReturnItem = 2004,//英雄回收物品
	eItemAct_HeroTuJian = 2005,//英雄图鉴
	eItemAct_GuildGift = 2006,//公会赠送
	eItemAct_MoneyTreeCost, // 摇钱树消耗 [8/11/2017 chief]
	eItemAct_EquipTreasure, //装备夺宝发奖励
	eItemAct_EquipTreasureGetBoxReward, //装备夺宝领取宝箱奖励

	eItemAct_NUM,
};
enum eWorldShopCashActType
{
	eWorldShopCashAct_Unknow			= 0,	/* 未知任务操作 */
	eWorldShopCashAct_RequestSell,			/* 玩家请求卖现金 */
	eWorldShopCashAct_MarketSuccessed,		/* 现金上架成功 */
	eWorldShopCashAct_MarketFailed,			/* 现金上架失败 */
	eWorldShopCashAct_MarketReturnNullConnecter,	/* 返回上架结果时GameConnector指针为空 */
	eWorldShopCashAct_MarketReturnNullPlayer,	/* 返回上架结果时Human指针为空 */
	eWorldShopCashAct_MarketReturnNullMap,		/* 返回上架结果时Map指针为空 */
	eWorldShopCashAct_MarketReturnNullGamePlayer,	/* 返回上架结果时Player指针为空 */

	eWorldShopCashAct_RequestBuy,			/* 玩家请求买现金 */
	eWorldShopCashAct_BuySuccessed,			/* 购买成功 */
	eWorldShopCashAct_BuyFailed,			/* 购买失败 */
	eWorldShopCashAct_BankTakeReturnNullConnector,	/* 银行领取时GameConnector指针为空 */
	eWorldShopCashAct_BankTakeReturnNullGamePlayer, /* 银行领取时HUMAN指针为空 */
	eWorldShopCashAct_BankTakeReturnNullMap,	/* 银行领取时SCENE指针为空 */
	eWorldShopCashAct_BankTakeReturnNullPlayer,	/* 银行领取时PLAYER指针为空 */
	eWorldShopCashAct_BankTakeSuccessed,		/* 银行领取时成功 */
	eWorldShopCashAct_NullUser,			/* pKPlayer为空 */
	eWorldShopCashAct_ClearCash,			/* 从商店中清除寄售现金 */
	eWorldShopCashAct_SellTimeoutReturnToBank,	/* 寄售超时，现金从商店转入银行 */
	eWorldShopCashAct_SystemBuy,			/* 系统回购，现金从商店转入银行 */
	eWorldShopCashAct_NullShop,			/* Shop为空 */
	eWorldShopCashAct_NullSrvConnecter,		/* ServerPlayer为空 */
	eWorldShopCashAct_ShopFull,			/* 商店满 */
	eWorldShopCashAct_BankFull,			/* 银行满 */
	eWorldShopCashAct_UserFull,			/* 玩家在该商店中的寄售货物已达到上限 */
	eWorldShopCashAct_ClearInTheBank,		/* 从银行中清除寄售现金 */
	eWorldShopCashAct_LoadShopFromDB,		/* 启动时从DB中实际LOAD一条商店数据 */
	eWorldShopCashAct_LoadBankFromDB,		/* 启动时从DB中实际LOAD一条银行数据 */
	eWorldShopCashAct_ShouldLoadShopFromDB,		/* 启动时从DB中应该LOAD的一条商店数据 */
	eWorldShopCashAct_ShouldLoadBankFromDB,		/* 启动时从DB中应该LOAD的一条银行数据 */
	eWorldShopCashAct_AddShopFailed,		/* 加入商店失败 */
	eWorldShopCashAct_DelShopFailed,		/* 从商店删除时失败 */
	eWorldShopCashAct_AddBankFailed,		/* 加入银行失败 */
	eWorldShopCashAct_NUM
};
enum eEquipActType
{
	eEquipAct_Unknow			= 0,	/* 未知任务操作 */
	eEquipAct_LevelUp			= 10,	/* 升级*/
	eEquipAct_AttrRefine		= 20,	/* 洗练 */
	eEquipAct_AttrRefineConfirm	= 30,	/* 洗练确定 */
	eEquipAct_NUM
};

enum eCheatActOpType
{
	eCheatActOp_Unknow				= 0,	/* 未知操作 */
	eCheatActOp_Log,	/* 只写LOG */
	eCheatActOp_Kill,	/* 踢掉 */
	eCheatActOp_Answer,	/* 答题 */
};

enum ePetActType
{
	ePetAct_Unknow					= 0,	/* 宠物未知操作 */
	ePetAct_Catch					= 10,	/* 宠物捕捉 */
	ePetAct_ScriptCreate				= 11,	/* 脚本创建宠物 */
	ePetAct_ScriptDestroy				= 12,	/* 脚本删除宠物 */
	ePetAct_PlayerFree				= 13,	/* 玩家放生 */
	ePetAct_ResumeAllHorseHappy			= 14,	/* 恢复所有骑乘快乐度 */
	ePetAct_GeneraAdd				= 20,	/* 宠物繁殖时增加宠物 */
	ePetAct_GeneraLost				= 21,	/* 宠物繁殖时失去宠物 */
	ePetAct_ComposeMain				= 22,	/* 宠物炼化时主宠物 */
	ePetAct_ComposeSub				= 23,	/* 宠物炼化时副宠物 */
	ePetAct_ComposeNew				= 24,	/* 宠物炼化时新宠物 */
	ePetAct_ComposeRep				= 25,	/* 宠物炼化后恢复主宠物 */
	ePetAct_ComposeDel				= 26,	/* 宠物炼化后反悔删掉新宠物 */
	ePetAct_TradeOther				= 30,	/* 宠物交易获得 */
	ePetAct_StallTradeAcquire			= 40,	/* 摆摊交易获得 */
	ePetAct_PlayerShopTradeAcquire			= 50,	/* 玩家商店交易获得 */
	ePetAct_PlayerShopMarket			= 51,	/* 玩家商店上架 */
	ePetAct_PlayerShopMarketGoWay			= 52,	/* 玩家商店下架 */
	ePetAct_GMCreate				= 60,	/* GM 修改添加 */
	ePetAct_GMDestroy				= 61,	/* GM 修改销毁 */

	ePetAct_PlayerLock				= 62,	/* 骑乘锁定 */
	ePetAct_PlayerUnlock			= 63,	/* 骑乘解锁 */

	ePetAct_ReturnToChild			= 70,	/* 骑乘还童 */
	ePetAct_ReturnToChildExt		= 71,	/* 骑乘还龙 */

	ePetAct_NUM
};

//股票交易相关的操作
enum eStockActType
{
	eStockAct_Unknow			= 0,	// 未知任务操作 
	eStockAct_LoadDataFromDB,			// 启动时从DB中实际LOAD一条数据
	eStockAct_AutoBuyMarketOK,			// 买单上架成功 
	eStockAct_AutoBuyMarketFailed,		// 买单上架失败
	eStockAct_AutoSellMarketOK,			// 卖单上架成功 
	eStockAct_AutoSellMarketFailed,		// 卖单上架失败
	eStockAct_ClearOvertime,			// 超时清除挂单
	eStockAct_ClearTrade,				// 交易清除挂单
	eStockAct_ClearRepeal,				// 玩家撤单时清除挂单
	eStockAct_BuyListFull,				// 买单太多列表爆了
	eStockAct_SellListFull,				// 卖单太多列表爆了
	eStockAct_UserBuyFull,				// 用户买单已达上限
	eStockAct_UserSellFull,				// 用户卖单已达上限
	eStockAct_SystemBuy,				// 系统回购
	eStockAct_TradeOK,					// 交易成功
	eStockAct_LoadFromDBNodeFull,		// 启动时从DB中LOAD一条数据失败(结点已被占用)
	eStockAct_SMUNodeNull,				// 共享内存结点为空
	eStockAct_AutoBuyMarketSMUNull,		// 买单上架时 共享内存结点为空
	eStockAct_AutoSellMarketSMUNull,	// 卖单上架时共享内存结点为空
	eStockAct_NUM
};

//股票账户相关的操作
enum eStockAccActType
{
	eStockAccAct_Unknow			= 0,		// 未知任务操作 
	eStockAccAct_LoadDataFromDB,			// 启动时从DB中实际LOAD一条数据
	eStockAccAct_ClearOvertime,				// 超时清除账户
	eStockAccAct_ClearAccount,				// 清除账户，玩家撤消账户
	eStockAccAct_CharMoneyToStockCost,		// 充值时，MAP扣除角色现金和现银
	eStockAccAct_CharMoneyToStockFailedGL,	// 向GL充值失败
	eStockAccAct_CharMoneyToStockAdd,		// 充值时，GL向账户增加现金和现银
	eStockAccAct_CharMoneyToStockFailedMAP,	// 充值失败，MAP回滚Money
	eStockAccAct_StockMoneyToCharkCost,		// 向角色转账时，GL扣除账户现金和现银
	eStockAccAct_StockMoneyToCharFailedkGL,	// 向角色转账时，GL失败
	eStockAccAct_StockMoneyToCharkAdd,		// 向角色转账成功，MAP增加角色现金和现银
	eStockAccAct_StockMoneyToCharkFailed,	// 向角色转账失败
	eStockAccAct_GoldToCharkFailedAdd,		// 向角色转账失败,GL回滚现金
	eStockAccAct_SillerToCharkFailedAdd,	// 向角色转账失败,GL回滚现银
	eStockAccAct_NullUser,					// pKPlayer为空 
	eStockAccAct_AutoSaleCostGold,			// 委托卖金: 扣除本金和手续费 现金
	eStockAccAct_AutoBuyCostSiller,			// 委托买金: 扣除本金和手续费 现银
	eStockAccAct_AutoSaleAddSiller,			// 委托卖金: 增加现银
	eStockAccAct_AutoBuyAddGold,			// 委托买金: 增加现金
	eStockAccAct_FastBuyCostSiller,			// 手动买金: 扣除本金和手续费 现银
	eStockAccAct_FastSaleCostGold,			// 手动卖金: 扣除本金和手续费 现金
	eStockAccAct_FastBuyAddGold,			// 手动买金成功: 增加现金
	eStockAccAct_FastSaleAddSiller,			// 手动卖金成功: 增加现银
	eStockAccAct_GoldLimit,					// 现金超过上限
	eStockAccAct_SillerLimit,				// 现银超过上限
	eStockAccAct_GoldLack,					// 现金不足
	eStockAccAct_SillerLack,				// 现银不足
	eStockAccAct_BuyOvertimeAdd,			// 买单超时,现银转入账户
	eStockAccAct_SellOvertimeAdd,			// 卖单超时,现金转入账户
	eStockAccAct_RepealTradeAddGold,		// 玩家撤单，现金转入账户
	eStockAccAct_RepealTradeAddSiller,		// 玩家撤单，现银转入账户
	eStockAccAct_AddSillerOwnerNull,		// 向账户增加现银时玩家账户为空
	eStockAccAct_AddGoldOwnerNull,			// 向账户增加现金时玩家账户为空
	eStockAccAct_RegisterOK,				// 开户成功
	eStockAccAct_LoadFromDbNullAcc,			// 启动从DB中LOAD数据时，用户股票账户为空
	eStockAccAct_StockRegisterCostMoney,	// 开户扣现银
	eStockAccAct_StockRegisterFailedAddMoney,//开户失败返回现银
	eStockAccAct_CoolingCostGold,			 //进入冷却状态后扣除现金
	eStockAccAct_CoolingCostSiller,			 //进入冷却状态后扣除现银
	eStockAccAct_StockAccDieAddMoney,		 //股票账户消亡：资金返回角色
	eStockAccAct_NUM
};

enum eGuildActType
{
	eGuildAct_Unknow			= 0,		// 未知任务操作 
	eGuildAct_Exp_GM			= 1,		// 帮会经验，GM指令改变
	eGuildAct_Exp_Script		= 2,		// 帮会经验，脚本调用
	eGuildAct_Money_GM			= 3,		// 帮会金钱，GM指令改变
	eGuildAct_Money_Script		= 4,		// 帮会金钱，脚本调用
	eGuildAct_ShiMing_GM		= 5,		// 帮会使命点，GM指令改变
	eGuildAct_ShiMing_Script	= 6,		// 帮会使命点，脚本调用
	eGuildAct_Money_Leveldown	= 7,		// 帮会降级，将超过限制的金钱扣除
	eGuildAct_ShiMing_RemoveMember = 8,		// 离开成员，使命点扣除
	eGuildAct_ShiMing_AddMember = 9,		// 增加成员，使命点增加
	eGuildAct_ShiMing_SystemCheck = 10,		// 系统每月检查清零
	eGuildAct_Exp_Skill		= 11,			// 帮会经验，帮会技能升级
	eGuildAct_Money_Skill		= 12,			// 帮会金钱，帮会技能升级
	eGuildAct_BusSkill_LevelUp_Money = 13,	// 帮会神兽技能升级,消耗帮会金钱
	eGuildAct_BusSkill_LevelUp_Exp = 14,	// 帮会神兽技能升级,消耗帮会经验
	eSkillAct_GuildBusSkill_LevelUp = 15,	/// 帮会神兽技能升级
	eGuildAct_Exp_PriseByShiMing	= 16,	/// 帮会经验，奖励
	eGuildAct_Fund_CurMonth		= 17,		// 本月帮会基金改变
	eGuildAct_Fund_LastMonth	= 18,		// 上月帮会基金改变
	eGuildAct_NUM
};

enum eZhuanJingActType
{
	eZhuanjingAct_Unknow		= 0,			/* 未知操作 */
	eZhuanjingAct_ZhuanGong_Get		= 10,			/* 专攻点数获得 */
	eZhuanjingAct_ZhuanFang_Get		= 20,			/* 专防点数获得 */
	eZhuanjingAct_ZhuanGong_Coefficient		= 30,			/* 专攻系数分配 */
	eZhuanjingAct_ZhuanFang_Reallocate		= 40,			/* 专防点数分配 */
	eZhuanjingAct_ZhuanGong_GM		= 50,			/* 通过 GM命令 专攻点数获得 */
	eZhuanjingAct_ZhuanFang_GM		= 60,			/* 通过 GM命令 专防点数获得 */
	eZhuanjingAct_Zhuangong_LevelUp = 70,			/* 专攻升级 */
	eZhuanjingAct_Zhuanfang_LevelUP = 80,		/* 专防升级 */
	eZhuanjingAct_NUM
};

#endif /* _ACT_DEFINE_H_ */

/* end ??? */
