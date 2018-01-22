/*$T Share/DefineResult.h GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __GAME_DEF_RESULT_H__
#define __GAME_DEF_RESULT_H__

#define ORT_SUCCEEDED(x) (((eOPTRESULT) (x)) >= 0)
#define ORT_FAILED(x)	(((eOPTRESULT) (x)) < 0)
#define ORT_TestResult(a) \
	if((eOPTRESULT) (a) < 0) \
	{ \
		return a; \
	}

enum eOPTRESULT
{
	ORESULT_OK						= 0,
	ORESULT_ERROR					= -1,
	ORESULT_DIE						= -2,
	ORESULT_INVALID_SKILL			 = -3,
	ORESULT_TARGET_DIE					= -4,
	ORESULT_LACK_MANA					= -5,
	ORESULT_COOL_DOWNING					= -6,
	ORESULT_NO_TARGET					= -7,
	ORESULT_INVALID_TARGET				= -8,
	ORESULT_OUT_RANGE					= -9,
	ORESULT_IN_RANGE					= -10,
	ORESULT_NO_PLATFORM					= -11,
	ORESULT_OUT_PLATFORM					= -12,
	ORESULT_NO_TOOL					= -13,
	ORESULT_STUFF_LACK					= -14,
	ORESULT_BAG_OUT_OF_SPACE				= -15,
	ORESULT_WARNING					= -16,
	ORESULT_BUSY						= -17,
	ORESULT_CHAR_DO_NOT_KNOW_THIS_SKILL			= -18,
	ORESULT_NOT_ENOUGH_HP				= -19,
	ORESULT_NOT_ENOUGH_RAGE				= -20,
	ORESULT_NOT_ENOUGH_STRIKE_POINT			= -21,
	ORESULT_NOT_ENOUGH_ITEM				= -22,
	ORESULT_FAILURE					= -23,
	ORESULT_INVALID_TARGET_POS				= -24,
	ORESULT_LIMIT_MOVE					= -25,
	ORESULT_LIMIT_USE_SKILL				= -26,
	ORESULT_INACTIVE_SKILL				= -27,
	ORESULT_U_CANNT_DO_THIS_RIGHT_NOW	= -28,
	ORESULT_NEED_HIGH_LEVEL				= -29,
	ORESULT_EXP_FULL					= -30,
	ORESULT_IMPASSABLE_ZONE				= -31,
	ORESULT_NOT_ENOUGH_MONEY			= -32,
	ORESULT_TALK_TOO_FAST				= -33,
	ORESULT_NEED_STOP_MOVE				= -34,
	ORESULT_TRY_CATCH_RETURN			= -35,
	ORESULT_NULL_POINTER				= -36,
	ORESULT_NULL_MAP					= -37,
	ORESULT_HAVENOT_LEARN_THIS_SKILL			= -38,
	ORESULT_DONOT_HAVE_ENOUGH_SKILL_POINT		= -39,
	ORESULT_HAVENOT_LEARN_PARENT_SKILL			= -40,
	ORESULT_CANNOT_FIND_THIS_SKILL_IN_STUDY_TABLE	= -41,
	ORESULT_THIS_SKILL_IS_MAX_LEVEL			= -42,
	ORESULT_CHANGE_STATE_FAIL				= -43,
	ORESULT_CHARACTER_IS_UNACTIVE			= -44,
	ORESULT_SKILL_STATE_ERROR				= -45,
	ORESULT_CANNOT_FIND_MOVE_PATH			= -46,
	ORESULT_DEFAULT_ERROR				= -47,
	ORESULT_IS_ENEMY_CAMP				= -48,
	ORESULT_TEAM_FOLLOW_STATE			= -49,
	ORESULT_CANNOT_FIND_NPC				= -50,
	ORESULT_IS_PASSIVE_SKILL			= -51,
	ORESULT_SCAN_ERROR					= -52,
	ORESULT_POST_TOO_FAST				= -53,

	ORSEULT_NOT_Star_CONDITION_LESS	= -54, // 英雄升星条件不足
	ORESULT_HEROCARD_Star_FIRST          = -55, //先英雄升星英雄
	ORESULT_HEROCARD_NOT_EXITS				= -56, //卡牌不存在
	ORESULT_HEROCARD_POS_ERROR				= -57, //卡牌位置错误
	ORESULT_HEROCARD_LVL_MAX				= -58,  //卡牌已到最大等级
	ORESULT_HEROCARD_SPELL_NOT_EXITS		= -59,	//卡牌技能不存在
	ORESULT_HEROCARD_SPELL_LEVEL_MAX		= -60, //技能已经达到最高级

	ORESULT_NEEDITEM_LESS					= -61, //道具不足 [wangyiran]
	ORESULT_GEM_LESSMONEY					= -62, //金币不足 [wangyiran]
	ORESULT_SYS_ERROR						= -63,
	ORESULT_BAG_FULL						= -64, //背包满 [wangyiran]

	ORESULT_NOT_ENOUGH_CU_GOLD				= -65, //钻石不足 [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_SILLER			= -66, //金币不足 [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_BEYONGOD			= -67, //超神b不足 [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_CRYSTAL			= -68, //水晶不足 [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_DotaCoin			= -69, //刀币不足 [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_HONOREXP			= -70, // 英雄经验不足 [wangyiran]
	ORESULT_NOT_ENOUGH_CU_MAGICSTAL			= -71, // 秘法石不足 [wangyiran]
	ORESULT_NOT_ENOUGH_CU_RECALLPAPER		= -72, // 召唤符不足 [wangyiran]
	ORESULT_NOT_ENOUGH_CU_PHYSICALPOWER		= -73, // 体力不足 [wangyiran]
	ORESULT_NOT_ENOUGH_CU_SPIRIT			= -74, // 精力不足 [wangyiran]
	ORESULT_NOT_LESS_PLAYER_LEVEL			= -75, //不能大于玩家等级 [xingzhishan]  
	ORESULT_ALREADY_HAVE_ITEM				= -76, // 背包已经有此种物品 [wangyiran]
	ORESULT_BAG_TYPE_ERROR					= -77, // 背包类型错误 [wangyiran]
	ORESULT_ITEM_NOT_EXITS					= -78, // 物品不存在 [wangyiran]
	ORESULT_BODY_POS_ERROR					= -79, // 身体位置错误 [wangyiran]
	ORESULT_EQUIP_STRONGER_MORE_THEN_PLAYER_LEVEL = -80, //装备强化比玩家等级大 [xingzhishan]  
	ORESULT_EQUIP_STRONGER_SUCCESS			= -81, // 强化成功 [xingzhishan]
	ORESULT_EQUIP_ATTR_REFINE_SUCCESS		= -82,// 装备洗练成功 [xingzhishan]
	ORESULT_EQUIP_ATTR_REFINE_COUNT_ERROR   = -83, //装备洗练条数错误 [xingzhishan]

	ORESULT_BATTLE_ENTER_FAILED_LESS_LEVEL	= -84,	// 进入战场等级不够 [yangzhen]
	ORESULT_BATTLE_ENTER_FAILED_NO_PREBATTLE = -85,	// 进入战场前置战役没完成 [yangzhen]
	ORESULT_BATTLE_ENTER_SUCCESS			= -86, //成功进入 [yangzhen]
	ORESULT_BATTLE_OPEN_AWARDBOX_ALREADYOPENED	= -87, //开启了一个已经被开过的宝箱 [yangzhen]
	ORESULT_BATTLE_OPEN_AWARDBOX_STARNOTENOUGH	= -88, //开启一个星星条件不满足的宝箱 [yangzhen]
	ORESULT_STAR_MAX						= -89, // 英雄升星到达上限 [wangyiran]
	ORESULT_CONFIG_ERROR			= -90, // 配置错误 [wangyiran]
	ORESULT_Star_ITEMLESS				= -91, // 英雄升星材料不足 [wangyiran]
	ORESULT_PLAYER_LEVEL_LESS				= -92, // 战队等级不足 [wangyiran]

	ORESULT_FORMALBAGFULL_EQUIP				= -93,	// 用户可用装备背包已满 [yangzhen]
	ORESULT_FORMALBAGFULL_MEDIC				= -94,	// 用户可用材料背包已满 [yangzhen]
	ORESULT_FORMALBAGFULL_GEM				= -95,	// 用户可用宝石背包已满 [yangzhen]
	ORESULT_FORMALBAGFULL_TREASURE			= -96,	// 用户可用宝物背包已满 [yangzhen]
	ORESULT_CHARACTER_AI_IN_CLIENT			= -97,	// 角色的AI在客户端控制，服务器对角色的操作范围错误 [yangzhen]
	ORESULT_ACHIEVE_NOT_DONE				= -98, // 成就未完成 [wangyiran]
	ORESULT_ACHIEVE_HAS_FINISH				= -99, // 成就奖励已经被领取 [wangyiran]
	ORESULT_FORMALBAGFULL_HERO				= -100,	// 用户可携带英雄到达上限[wangxiaoyong]
	ORESULT_BUY_ARENACOUNT_OK				= -101,	// 购买竞技场挑战成功 [wangyiran]
	ORESULT_DAILYTASK_NOT_OPEN				= -102,	// 此日常任务尚未开启 [wangyiran]
	ORESULT_DAILYTASK_NOT_FINISH			= -103,	// 此日常任务正在进行中 [wangyiran]
	ORESULT_DAILYTASK_HASGOT				= -104,	// 此日常任务奖励已经领取 [wangyiran]
	ORESULT_OPERATE_FINISH					= -105,	// 没用特殊含义，只是服务器给客户端的一个反馈 [yangzhen]
	ORESULT_HEROLEVEL_LESS					= -106,	// 英雄等级不足 [yangzhen]
	ORESULT_BUY_SUCCESS						= -107, // 购买成功 [wangyiran]
	ORESULT_CDKEY_NO_EXIST					= -108,// cdkey不存在 [xingzhishan]
	ORESULT_CDKEY_VALID						= -109,// cdkey无效 [xingzhishan]
	ORESULT_CDKEY_TIME_OUT					= -110,// cdkey过期 [xingzhishan]
	ORESULT_CDKEY_OK						= -111,// cdkey成功 [xingzhishan]
	ORESULT_CDKEY_ONLY_ONE					= -112,// cdkey相同类型只能领取1个 [xingzhishan]
	ORESULT_NOT_ENOUGH_MATERIAL_FAIL		= -113,// 材料不足升级失败[xingzhishan]

	// 帮会相关 [wangyiran]
	ORESULT_GUILD_CREATE_SUCCESS			= -114,// 创建成功 [wangyiran]
	ORESULT_GUILD_CREATEFULL				= -115,// 服务器帮会已达上限 [wangyiran]
	ORESULT_GUILD_DUPLICATED_NAME = -116,// 帮会名字已经存在 [wangyiran]
	ORESULT_GUILD_PLAYER_INGUILD = -117,// 你已经在帮会里 [wangyiran]
	ORESULT_GUILD_JOIN_GUILD_LVLLESS = -118,// 进入帮会等级不足 [wangyiran]
	ORESULT_GUILD_JOIN_GUILD_LIMIT = -119,// 会长设置不允许加入 [wangyiran]
	ORESULT_GUILD_NOT_EXITS = -120,// 公会不存在 [wangyiran]
	ORESULT_GUILD_MEMBER_FULL = -121,// 公会成员已满 [wangyiran]
	ORESULT_GUILD_NOT_AMEMBER = -122,// 你不在公会里 [wangyiran]
	ORESULT_GUILD_POSITION_LESS = -123,// 权限不足 [wangyiran]
	ORESULT_GUILD_CANNOT_DELSELF = -124,// 不能删除自己 [wangyiran]
	ORESULT_GUILD_LEAVE_TIME_LESS = -125,// 退出公会后2小时内无法加入新公会 [wangyiran]
	ORESULT_CDKEY_OVER_COUNT = -126,// cdkey领取数量超了 [xingzhishan]
	ORESULT_FORBIDCHAT_LEFT_MIN = -127,//禁言分钟 [xingzhishan]
	ORESULT_FORBIDCHAT_LEFT_HOUR = -128,//禁言小时 [xingzhishan]
	ORESULT_ONLINE_PLAYER_LIMIT = -129,//在线人数过多 [xingzhishan]
	ORESULT_ASSIGN_ALREADYGET = -130,//奖励已经领取 [wangyiran]
	ORESULT_MONTHASSIGN_DAY_ERROR = -131,//尚未到达这天 [wangyiran]
	ORESULT_LVLASSIGN_ERROR = -132,//此等级没有奖励 [wangyiran]
	ORESULT_VIPLESS_ERROR = -133,//vip等级不足 [wangyiran]
	ORESULT_HIDE_LOCKSCREEN_WINDOW = -134,//解小菊花 [wangyiran]
	ORESULT_GUILD_COCHAIRMAN_MAX = -135,//副会长已满 [wangyiran]
	ORESULT_GUILD_MEMBER_EXIT = -136,//会员已经退出公会 [wangyiran]

	ORESULT_FRIEND_ERROR_NOTOPEN = -137,// 功能尚未开启 [wangyiran]
	ORESULT_FRIEND_ERROR_DETAILERROR = -138,// 玩家信息不存在 [wangyiran]
	ORESULT_FRIEND_ERROR_LVL_LESS = -139,// 对方等级不足 [wangyiran]
	ORESULT_FRIEND_ERROR_APPLY_FULL = -140,// 对方申请列表满 [wangyiran]
	ORESULT_FRIEND_ERROR_ALREADY_FRIEND = -141,// 已经是好友 [wangyiran]
	ORESULT_FRIEND_ERROR_SELF_FRIENDFULL = -142,// 你的好友列表满 [wangyiran]
	ORESULT_FRIEND_ERROR_OTHER_FRIENDFULL = -143,// 对方好友列表满 [wangyiran]
	ORESULT_FRIEND_ERROR_NOT_INAPPLY = -144,// 不在申请列表里 [wangyiran]
	ORESULT_FRIEND_ERROR_GIVEN_PHYSICAL = -145,// 对方好友列表满 [wangyiran]
	ORESULT_FRIEND_ERROR_CANNOT_APPLYSELF = -146,// 不能申请自己为好友 [wangyiran]
	ORESULT_FRIEND_ERROR_APPLY_SUCCESS = -147,// 不能申请自己为好友 [wangyiran]
	ORESULT_FRIEND_ERROR_AGREE_SUCCESS = -148,// 不能申请自己为好友 [wangyiran]
	ORESULT_FRIEND_ERROR_ONEKEYAGREE_SUCCESS = -149,// 一键添加成功 [wangyiran]
	ORESULT_FRIEND_ERROR_NOT_RECV_PHYSICAL = -150,// 没被赠送体力 [wangyiran]
	ORESULT_FRIEND_ERROR_ALREADY_GET_PHYSICAL = -151,// 已经领取过了 [wangyiran]
	ORESULT_FRIEND_ERROR_GET_PHYSICAL_MAX = -152,// 已经领取过了 [wangyiran]
	ORESULT_FRIEND_ERROR_GET_PHYSICAL_SUCCESS = -153,// 领取成功 [wangyiran]
	ORESULT_FRIEND_ERROR_GIVE_PHYSICAL_SUCCESS = -154,// 赠送成功 [wangyiran]
	ORESULT_FRIEND_ERROR_SELF = -155,// 不能添加自己 [wangyiran]
	ORESULT_FRIEND_ERROR_NOT_ONLINE = -156,// 玩家不再线 [wangyiran]

	//===================普通小玩法==================//
	ORESULT_USEITEM_COUNT_LIMIT = -157,		//今日使用次数完
	ORESULT_USEITEM_PHYSICALPOWER_MAX = -158,		//体力值超上限

	ORESULT_HERO_DVELOP_PROPMAX = -159,// 属性已达上限 [1/4/2016 superrobert]
	ORESULT_HERO_DVELOP_ITEMLESS = -160,// 培养丹不足 [1/4/2016 superrobert]
	// 保留之前东西 [1/5/2016 superrobert]
	ORESULT_MS_Stall = -161,
	ORESULT_MS_Fuben = -162,
	ORESULT_MS_BattleFieldMap = -163,
	ORESULT_MS_ArenaMap = -164,
	ORESULT_MS_CityMap = -165,
	ORESULT_MS_PrisonMap = -166,
	ORESULT_MS_Die = -167,
	ORESULT_MS_Bus = -168,
	ORESULT_MS_Cashbox = -169,
	ORESULT_MS_Exchange = -170,
	ORESULT_MS_MentalGame = -171,
	ORESULT_MS_TeamFollow = -172,
	ORESULT_MS_AutoPlay = -173,
	ORESULT_MS_SubTrain = -174,
	ORESULT_MS_Prison = -175,
	ORESULT_MS_OpenBox = -176,
	ORESULT_MS_ProtectTime = -177,
	ORESULT_MS_KeyboardMove = -178,
	ORESULT_MS_Quest_TieSan = -179,
	ORESULT_MS_ReadyFuben = -180,
	ORESULT_MS_Quest_DuoQiBuff = -181,
	ORESULT_MS_Quest_TongQuBuff = -182,
	ORESULT_MS_PKMode = -183,
	ORESULT_MS_ForbidRide = -184,
	ORESULT_MS_Wedding = -185,
	ORESULT_PWD_ErrorProtect = -186,
	ORESULT_PWD_ErrorMax = -187,
	ORESULT_MS_Enjail = -188,
	ORESULT_MS_Award = -189,
	ORESULT_MS_TransGold = -190,
	ORESULT_MS_InDancery = -191,
	ORESULT_MS_BlackGold = -192,

	ORESULT_HERO_AWAKING_MAX = -193,// 进化到最大了 [1/6/2016 superrobert]
	ORESULT_HERO_AWAKING_ITEMLESS = -194,// 材料不足 [1/6/2016 superrobert]
	ORESULT_HERO_BREAK_MAX = -195,// 突破最大 [1/6/2016 superrobert]
	ORESULT_HERO_BREAK_ITEMLESS = -196,// 突破最大 [1/6/2016 superrobert]
	ORESULT_EQUIP_EXTRACTION_ITEMLESS = -197,// 淬炼石不足 [1/21/2016 admin]
	ORESULT_ONEKEY_WEAREQUIP_SUCCESS = -198,// 一键穿戴成功 [1/6/2016 superrobert]
	ORESULT_HERO_BREAKUP_NOT_EXITS = -199,//突破升级整卡不存在 [1/21/2016 HJ]
	ORESULT__EQUIP_EXTRACTION_FAIL = -200,//淬炼失败 [1/21/2016 chentongxin]
	ORESULT__EQUIP_STRONGER_MAX = -201,//淬炼失败 [1/21/2016 chentongxin]
	ORESULT_ERROR_FRIENDGEM_LVLMAX = -202,// 等级最高 [1/26/2016 superrobert]
	ORESULT_ERROR_FRIENDGEM_ITEMLESS = -203,// 材料不足 [1/26/2016 superrobert]
	ORESULT_FRIENDGEM_LVLSUCCESS = -204,// 宝石升级成功 [1/26/2016 superrobert]
	ORESULT_ERROR_FRIENDGEM_WASHITEMLESS = -205,// 材料不足 [1/26/2016 superrobert]
	ORESULT_FRIENDGEM_WASHSUCCESS = -206,// 宝石洗练成功 [1/26/2016 superrobert]
	ORESULT_HERO_AWAKING_FAIL = -207,// 进化失败 [1/26/2016 HJ]
	ORESULT_ITEM_MIX_NOT_CHIP = -208,// 物品合成不是碎片 [2/15/2016 HJ]
	ORESULT_ITEM_MIX_LESS = -209,// 合成材料不足 [2/15/2016 HJ]
	ORESULT_NOT_ENOUGH_CU_HONOR = -210,//荣誉值不足
	ORESULT_HANDBOOK_AWARD_NOT = -210,// 未达到领奖条件 [2/19/2016 HJ]
	ORESULT_GODTOWER_SHOPPOINT_NOT = -211, // 神域之塔商店点不足 [2/23/2016 wangshaocong]
	ORESULT_EQUIP_EXTRACTION_SUCCESS = -212, //淬炼成功
	ORESULT_HEROCARD_NUM_MAX = -213,
	ORESULT_SPORTS_SETDEFENDERDATA_FAIL = -214, //竞技挑战查找被挑战者失败
	ORESULT_BATTLEREADY_CHANGETEAM = -215,// 副本选择战队 [3/25/2016 HJ]
	ORESULT_NOT_ENOUGH_CU_KUNG_FU_RELIC = -216,//功夫遗迹不足 [tianshuai 2016.4.12]
	ORESULT_NOT_ENOUGH_CU_WACKY_GADGET = -217,//奇怪的工具不足 [tianshuai 2016.4.12]
	ORESULT_NOT_ENOUGH_CU_VIKING_HEIRLOOM = -218,//维京的遗物不足 [tianshuai 2016.4.12]
	ORESULT_NOT_ENOUGH_CU_ZOO_SOUVENIR = -219,//动物园纪念品不足 [tianshuai 2016.4.12]
	ORESULT_NOT_TRANSFER_ITEM = -220,// 不能转移 [4/13/2016 HJ]
	ORESULT_PLAYER_HEAD_EQUAL = -221,// 修改的头像与当前头像相同 [4/19/2016 HJ]
	ORESULT_PLAYER_SHOPPING_FLUSH_MAX = -222,// 商店刷新次数已达上限 [4/22/2016 tianshuai]
	ORESULT_PLAYER_TEAM_LEVEL_MAX = -223,// 玩家战队等级已达最大 [5/7/2016 HJ]
	ORESULT_GUILD_SEARCH_NULL = -224,//搜索工会 没有符合要求的
	ORESULT_WORLD_BOSS_RAIS_SUCCESS = -225, // 世界boss购买鼓舞成功提示 [5/9/2016 scw]
	ORESULT_FRIEND_ERROR_PVPTARGET_GETFAIL = -226,// 好友切磋没找到对手信息 [5/10/2016 HJ]
	ORESULT_SPECIAL_GIFT_NOT_EXIST = -227,// 当前特殊礼包不存在 [5/19/2016 HJ]
	ORESULT_SPECIAL_GIFT_HAS_BUY = -228,// 当前特殊礼包已经购买过 [5/19/2016 HJ]
	ORESULT_SPECIAL_GIFT_BAG_FULL = -229,// 购买礼包检测背包已满 [5/20/2016 HJ]
	ORESULT_GET_DAILY_PHYSICAL_NOT_OPEN = -230,// 日常体力领取，日常开启条件不满足
	ORESULT_GET_DAILY_PHYSICAL_CONFIG_ERROR = -231,// 日常体力领取，日常配置文件错误
	ORESULT_GET_DAILY_PHYSICAL_MAX_COUNT = -232,// 日常体力领取，次数已满
	ORESULT_GET_DAILY_PHYSICAL_TIME = -233,// 日常体力领取，时间未到
	ORESULT_NOT_ENOUGH_CU_FRIEND = -234,// 友情点数不足 [5/24/2016 HJ]
	ORESULT_FRIEND_ERROR_GET_COUNT_MAX = -235,// 当前领取次数已达上限 [5/26/2016 HJ]
	ORESULT_FRIEND_ERROR_GIVE_COUNT_MAX = -236,// 当前赠送次数已达上限 [5/26/2016 HJ]
	ORESULT_FRIEND_NO_POINT_GET = -237,// 当前没有友情点数可领取 [5/26/2016 HJ]
	ORESULT_CURRENCY_NOT_ENOUGH = -238,// 货币不足 [8/12/2016 TianShuai]
	ORESULT_WORLD_BOSS_RAIS_FAILED = -239,// 世界boss购买鼓舞失败 [8/18/2016 scw]

	ORESULT_GUILD_DINNER_HASAWARD = -240,// 请先领取佳肴奖励 [yangzhen]
	ORESULT_GUILD_DINNER_MAKING = -241,// 有佳肴正在烹饪中 [yangzhen]
	ORESULT_GUILD_DINNER_MAKECOUNT_LESS = -242,// 烹饪次数已经用完 [yangzhen]
	ORESULT_GUILD_DINNER_TIME_ERROR = -243,// 聚餐时间未到[wangyiran]
	ORESULT_GUILD_DINNER_WAITING = -244,// 等待会长大人发起聚餐[wangyiran]
	ORESULT_GUILD_DINNER_EAT_ALREADY = -245,// 您已经参加过聚餐了[wangyiran]
	ORESULT_GUILDBOSS_CHALLENGE_COUNT_LESS = -246,// 挑战次数不足 [wangyiran]
	ORESULT_GUILD_SHOP_ITEM_LOST = -247,// 物品不存在 [wangyiran]
	ORESULT_GUILD_SHOP_HAS_BUY = -248,// 物品不存在 [wangyiran]
	ORESULT_GUILDBOSS_PRE_NOT_PASS = -249,// 先通关上一管 [wangyiran]
	ORESULT_GUILDBOSS_GATE_ERROR = -250,// 关卡不存在 [wangyiran]
	ORESULT_GUILDBOSS_ACTIVITY_LESS = -251,// 活跃度不够 [wangyiran]
	ORESULT_GUILDBOSS_NOT_OPEN = -252,// 关卡尚未开启 [wangyiran]
	ORESULT_GUILDBOSS_IN_BATTLE = -253,// boss正在战斗中 [wangyiran]
	ORESULT_GUILDBOSS_LVL_LESS = -254,// 等级不足 [wangyiran]
	ORESULT_GUILDBOSS_ALREADYPASS = -255,// 已经通关了 [wangyiran]
	ORESULT_GUILDBOSS_NOT_PASS = -256,// 关卡尚未通关 [wangyiran]
	ORESULT_GUILDBOSS_ALREADYOPEN = -257, // 关卡已经开启 [wangyiran]
	ORESULT_RAISE_FAILED = -258,// 鼓舞失败 [9/19/2016 scw]
	ORESULT_RAISE_SUCCESS = -259, // 鼓舞成功 [9/19/2016 scw]
	ORESULT_ITEM_MIX_HAS_NOT = -260, // 英雄已经存在不能合成 [9/22/2016 scw]
	ORESULT_ITEM_MIX_ALL_HAS_NOT = -261,
	ORESULT_AREAN_REFRESH_ENEMY_SUCC = -262, // 竞技场刷新敌人成功 [10/10/2016 scw]
	ORESULT_EQUIP_EVOLUTION_FAILED = -263, // 装备进阶失败 [10/17/2016 scw]
	ORESULT_AFTER_CLIENTLOADFINISH = -264, // 服务器得到了客户端领地加载完成的消息并回复客户端 [10/21/2016 yz]
	ORESULT_CONDITION_NOTHING = -265, // 条件不足 [11/14/2016 scw]
	ORESULT_GUILD_BOSS_INSPIRE_COUNT_MAX = -267, //工会boss鼓舞次数已达上限  [3/27/2017 "tianshuai"]
	ORESULT_GUILD_BOSS_BUY_CHALLENGE_COUNT_MAX = -268, //工会boss鼓舞次数已达上限  [3/27/2017 "tianshuai"]
	ORESULT_GUILD_BOSS_RESET_MAX = -269,// 公会boss重置次数已达上限 [3/28/2017 "tianshuai"]

	ORESULT_INVITE_CODE_FRIEND_COUNT_FULL = -270,// 对方邀请好友已满
	ORESULT_INVITE_CODE_FRIEND_ALREADY = -271,// 已经是邀请码好友了
	ORESULT_INVITE_CODE_FRIEND_ADD_SUCCESS = -272,// 添加成功
	ORESULT_INVITE_CODE_FRIEND_NOT_FIND = -273,// 错误的邀请码 [4/18/2017 "tianshuai"]
	ORESULT_GOD_ARENA_CHALLENGE_FAILED = -274,// 战神竞技场发起挑战失败 [7/15/2017 scw]


	ORESULT_NOT_ENOUGH_CU_PeakArenaCoin		= -600, // 巅峰竞技场勋章不足 [xingzhishan]
	ORESULT_PEAKARENA_NO_FIND_PLAYER		= -796, //巅峰竞技场找不到玩家 [chentongxin]

	ORESULT_HERO_RETURN_NO_HERO = -797,	//英雄回收，找不到英雄
	ORESULT_HERO_RETURN_OK = -798,	//英雄回收成功
	ORESULT_HERO_RETURN_GOLD_NO = -799,	//英雄回收金钱不足
	ORESULT_HERO_RETURN_QIYUE_BAG_FULL = -800, //契约背包已满
	ORESULT_EQUIP_STRONGER_FAIL = -801, //装备强化失败
	ORESULT_GUILD_APPLY_ADD_NUM = -802, //申请消息上限
	ORESULT_GUILD_APPLY_EXIST = -803,	//已经申请过了
	ORESULT_EQUIP_TREASURE_SCORE_FULL = -804, //装备熔炼 积分已满
	ORESULT_MAX_COUNT
};

typedef eOPTRESULT	OPT_RESULT;

const char		*GetORTText(eOPTRESULT code);
#endif
