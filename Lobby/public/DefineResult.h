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

	ORSEULT_NOT_Star_CONDITION_LESS	= -54, // Ӣ��������������
	ORESULT_HEROCARD_Star_FIRST          = -55, //��Ӣ������Ӣ��
	ORESULT_HEROCARD_NOT_EXITS				= -56, //���Ʋ�����
	ORESULT_HEROCARD_POS_ERROR				= -57, //����λ�ô���
	ORESULT_HEROCARD_LVL_MAX				= -58,  //�����ѵ����ȼ�
	ORESULT_HEROCARD_SPELL_NOT_EXITS		= -59,	//���Ƽ��ܲ�����
	ORESULT_HEROCARD_SPELL_LEVEL_MAX		= -60, //�����Ѿ��ﵽ��߼�

	ORESULT_NEEDITEM_LESS					= -61, //���߲��� [wangyiran]
	ORESULT_GEM_LESSMONEY					= -62, //��Ҳ��� [wangyiran]
	ORESULT_SYS_ERROR						= -63,
	ORESULT_BAG_FULL						= -64, //������ [wangyiran]

	ORESULT_NOT_ENOUGH_CU_GOLD				= -65, //��ʯ���� [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_SILLER			= -66, //��Ҳ��� [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_BEYONGOD			= -67, //����b���� [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_CRYSTAL			= -68, //ˮ������ [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_DotaCoin			= -69, //���Ҳ��� [xingzhishan]
	ORESULT_NOT_ENOUGH_CU_HONOREXP			= -70, // Ӣ�۾��鲻�� [wangyiran]
	ORESULT_NOT_ENOUGH_CU_MAGICSTAL			= -71, // �ط�ʯ���� [wangyiran]
	ORESULT_NOT_ENOUGH_CU_RECALLPAPER		= -72, // �ٻ������� [wangyiran]
	ORESULT_NOT_ENOUGH_CU_PHYSICALPOWER		= -73, // �������� [wangyiran]
	ORESULT_NOT_ENOUGH_CU_SPIRIT			= -74, // �������� [wangyiran]
	ORESULT_NOT_LESS_PLAYER_LEVEL			= -75, //���ܴ�����ҵȼ� [xingzhishan]  
	ORESULT_ALREADY_HAVE_ITEM				= -76, // �����Ѿ��д�����Ʒ [wangyiran]
	ORESULT_BAG_TYPE_ERROR					= -77, // �������ʹ��� [wangyiran]
	ORESULT_ITEM_NOT_EXITS					= -78, // ��Ʒ������ [wangyiran]
	ORESULT_BODY_POS_ERROR					= -79, // ����λ�ô��� [wangyiran]
	ORESULT_EQUIP_STRONGER_MORE_THEN_PLAYER_LEVEL = -80, //װ��ǿ������ҵȼ��� [xingzhishan]  
	ORESULT_EQUIP_STRONGER_SUCCESS			= -81, // ǿ���ɹ� [xingzhishan]
	ORESULT_EQUIP_ATTR_REFINE_SUCCESS		= -82,// װ��ϴ���ɹ� [xingzhishan]
	ORESULT_EQUIP_ATTR_REFINE_COUNT_ERROR   = -83, //װ��ϴ���������� [xingzhishan]

	ORESULT_BATTLE_ENTER_FAILED_LESS_LEVEL	= -84,	// ����ս���ȼ����� [yangzhen]
	ORESULT_BATTLE_ENTER_FAILED_NO_PREBATTLE = -85,	// ����ս��ǰ��ս��û��� [yangzhen]
	ORESULT_BATTLE_ENTER_SUCCESS			= -86, //�ɹ����� [yangzhen]
	ORESULT_BATTLE_OPEN_AWARDBOX_ALREADYOPENED	= -87, //������һ���Ѿ��������ı��� [yangzhen]
	ORESULT_BATTLE_OPEN_AWARDBOX_STARNOTENOUGH	= -88, //����һ����������������ı��� [yangzhen]
	ORESULT_STAR_MAX						= -89, // Ӣ�����ǵ������� [wangyiran]
	ORESULT_CONFIG_ERROR			= -90, // ���ô��� [wangyiran]
	ORESULT_Star_ITEMLESS				= -91, // Ӣ�����ǲ��ϲ��� [wangyiran]
	ORESULT_PLAYER_LEVEL_LESS				= -92, // ս�ӵȼ����� [wangyiran]

	ORESULT_FORMALBAGFULL_EQUIP				= -93,	// �û�����װ���������� [yangzhen]
	ORESULT_FORMALBAGFULL_MEDIC				= -94,	// �û����ò��ϱ������� [yangzhen]
	ORESULT_FORMALBAGFULL_GEM				= -95,	// �û����ñ�ʯ�������� [yangzhen]
	ORESULT_FORMALBAGFULL_TREASURE			= -96,	// �û����ñ��ﱳ������ [yangzhen]
	ORESULT_CHARACTER_AI_IN_CLIENT			= -97,	// ��ɫ��AI�ڿͻ��˿��ƣ��������Խ�ɫ�Ĳ�����Χ���� [yangzhen]
	ORESULT_ACHIEVE_NOT_DONE				= -98, // �ɾ�δ��� [wangyiran]
	ORESULT_ACHIEVE_HAS_FINISH				= -99, // �ɾͽ����Ѿ�����ȡ [wangyiran]
	ORESULT_FORMALBAGFULL_HERO				= -100,	// �û���Я��Ӣ�۵�������[wangxiaoyong]
	ORESULT_BUY_ARENACOUNT_OK				= -101,	// ���򾺼�����ս�ɹ� [wangyiran]
	ORESULT_DAILYTASK_NOT_OPEN				= -102,	// ���ճ�������δ���� [wangyiran]
	ORESULT_DAILYTASK_NOT_FINISH			= -103,	// ���ճ��������ڽ����� [wangyiran]
	ORESULT_DAILYTASK_HASGOT				= -104,	// ���ճ��������Ѿ���ȡ [wangyiran]
	ORESULT_OPERATE_FINISH					= -105,	// û�����⺬�壬ֻ�Ƿ��������ͻ��˵�һ������ [yangzhen]
	ORESULT_HEROLEVEL_LESS					= -106,	// Ӣ�۵ȼ����� [yangzhen]
	ORESULT_BUY_SUCCESS						= -107, // ����ɹ� [wangyiran]
	ORESULT_CDKEY_NO_EXIST					= -108,// cdkey������ [xingzhishan]
	ORESULT_CDKEY_VALID						= -109,// cdkey��Ч [xingzhishan]
	ORESULT_CDKEY_TIME_OUT					= -110,// cdkey���� [xingzhishan]
	ORESULT_CDKEY_OK						= -111,// cdkey�ɹ� [xingzhishan]
	ORESULT_CDKEY_ONLY_ONE					= -112,// cdkey��ͬ����ֻ����ȡ1�� [xingzhishan]
	ORESULT_NOT_ENOUGH_MATERIAL_FAIL		= -113,// ���ϲ�������ʧ��[xingzhishan]

	// ������ [wangyiran]
	ORESULT_GUILD_CREATE_SUCCESS			= -114,// �����ɹ� [wangyiran]
	ORESULT_GUILD_CREATEFULL				= -115,// ����������Ѵ����� [wangyiran]
	ORESULT_GUILD_DUPLICATED_NAME = -116,// ��������Ѿ����� [wangyiran]
	ORESULT_GUILD_PLAYER_INGUILD = -117,// ���Ѿ��ڰ���� [wangyiran]
	ORESULT_GUILD_JOIN_GUILD_LVLLESS = -118,// ������ȼ����� [wangyiran]
	ORESULT_GUILD_JOIN_GUILD_LIMIT = -119,// �᳤���ò�������� [wangyiran]
	ORESULT_GUILD_NOT_EXITS = -120,// ���᲻���� [wangyiran]
	ORESULT_GUILD_MEMBER_FULL = -121,// �����Ա���� [wangyiran]
	ORESULT_GUILD_NOT_AMEMBER = -122,// �㲻�ڹ����� [wangyiran]
	ORESULT_GUILD_POSITION_LESS = -123,// Ȩ�޲��� [wangyiran]
	ORESULT_GUILD_CANNOT_DELSELF = -124,// ����ɾ���Լ� [wangyiran]
	ORESULT_GUILD_LEAVE_TIME_LESS = -125,// �˳������2Сʱ���޷������¹��� [wangyiran]
	ORESULT_CDKEY_OVER_COUNT = -126,// cdkey��ȡ�������� [xingzhishan]
	ORESULT_FORBIDCHAT_LEFT_MIN = -127,//���Է��� [xingzhishan]
	ORESULT_FORBIDCHAT_LEFT_HOUR = -128,//����Сʱ [xingzhishan]
	ORESULT_ONLINE_PLAYER_LIMIT = -129,//������������ [xingzhishan]
	ORESULT_ASSIGN_ALREADYGET = -130,//�����Ѿ���ȡ [wangyiran]
	ORESULT_MONTHASSIGN_DAY_ERROR = -131,//��δ�������� [wangyiran]
	ORESULT_LVLASSIGN_ERROR = -132,//�˵ȼ�û�н��� [wangyiran]
	ORESULT_VIPLESS_ERROR = -133,//vip�ȼ����� [wangyiran]
	ORESULT_HIDE_LOCKSCREEN_WINDOW = -134,//��С�ջ� [wangyiran]
	ORESULT_GUILD_COCHAIRMAN_MAX = -135,//���᳤���� [wangyiran]
	ORESULT_GUILD_MEMBER_EXIT = -136,//��Ա�Ѿ��˳����� [wangyiran]

	ORESULT_FRIEND_ERROR_NOTOPEN = -137,// ������δ���� [wangyiran]
	ORESULT_FRIEND_ERROR_DETAILERROR = -138,// �����Ϣ������ [wangyiran]
	ORESULT_FRIEND_ERROR_LVL_LESS = -139,// �Է��ȼ����� [wangyiran]
	ORESULT_FRIEND_ERROR_APPLY_FULL = -140,// �Է������б��� [wangyiran]
	ORESULT_FRIEND_ERROR_ALREADY_FRIEND = -141,// �Ѿ��Ǻ��� [wangyiran]
	ORESULT_FRIEND_ERROR_SELF_FRIENDFULL = -142,// ��ĺ����б��� [wangyiran]
	ORESULT_FRIEND_ERROR_OTHER_FRIENDFULL = -143,// �Է������б��� [wangyiran]
	ORESULT_FRIEND_ERROR_NOT_INAPPLY = -144,// ���������б��� [wangyiran]
	ORESULT_FRIEND_ERROR_GIVEN_PHYSICAL = -145,// �Է������б��� [wangyiran]
	ORESULT_FRIEND_ERROR_CANNOT_APPLYSELF = -146,// ���������Լ�Ϊ���� [wangyiran]
	ORESULT_FRIEND_ERROR_APPLY_SUCCESS = -147,// ���������Լ�Ϊ���� [wangyiran]
	ORESULT_FRIEND_ERROR_AGREE_SUCCESS = -148,// ���������Լ�Ϊ���� [wangyiran]
	ORESULT_FRIEND_ERROR_ONEKEYAGREE_SUCCESS = -149,// һ����ӳɹ� [wangyiran]
	ORESULT_FRIEND_ERROR_NOT_RECV_PHYSICAL = -150,// û���������� [wangyiran]
	ORESULT_FRIEND_ERROR_ALREADY_GET_PHYSICAL = -151,// �Ѿ���ȡ���� [wangyiran]
	ORESULT_FRIEND_ERROR_GET_PHYSICAL_MAX = -152,// �Ѿ���ȡ���� [wangyiran]
	ORESULT_FRIEND_ERROR_GET_PHYSICAL_SUCCESS = -153,// ��ȡ�ɹ� [wangyiran]
	ORESULT_FRIEND_ERROR_GIVE_PHYSICAL_SUCCESS = -154,// ���ͳɹ� [wangyiran]
	ORESULT_FRIEND_ERROR_SELF = -155,// ��������Լ� [wangyiran]
	ORESULT_FRIEND_ERROR_NOT_ONLINE = -156,// ��Ҳ����� [wangyiran]

	//===================��ͨС�淨==================//
	ORESULT_USEITEM_COUNT_LIMIT = -157,		//����ʹ�ô�����
	ORESULT_USEITEM_PHYSICALPOWER_MAX = -158,		//����ֵ������

	ORESULT_HERO_DVELOP_PROPMAX = -159,// �����Ѵ����� [1/4/2016 superrobert]
	ORESULT_HERO_DVELOP_ITEMLESS = -160,// ���������� [1/4/2016 superrobert]
	// ����֮ǰ���� [1/5/2016 superrobert]
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

	ORESULT_HERO_AWAKING_MAX = -193,// ����������� [1/6/2016 superrobert]
	ORESULT_HERO_AWAKING_ITEMLESS = -194,// ���ϲ��� [1/6/2016 superrobert]
	ORESULT_HERO_BREAK_MAX = -195,// ͻ����� [1/6/2016 superrobert]
	ORESULT_HERO_BREAK_ITEMLESS = -196,// ͻ����� [1/6/2016 superrobert]
	ORESULT_EQUIP_EXTRACTION_ITEMLESS = -197,// ����ʯ���� [1/21/2016 admin]
	ORESULT_ONEKEY_WEAREQUIP_SUCCESS = -198,// һ�������ɹ� [1/6/2016 superrobert]
	ORESULT_HERO_BREAKUP_NOT_EXITS = -199,//ͻ���������������� [1/21/2016 HJ]
	ORESULT__EQUIP_EXTRACTION_FAIL = -200,//����ʧ�� [1/21/2016 chentongxin]
	ORESULT__EQUIP_STRONGER_MAX = -201,//����ʧ�� [1/21/2016 chentongxin]
	ORESULT_ERROR_FRIENDGEM_LVLMAX = -202,// �ȼ���� [1/26/2016 superrobert]
	ORESULT_ERROR_FRIENDGEM_ITEMLESS = -203,// ���ϲ��� [1/26/2016 superrobert]
	ORESULT_FRIENDGEM_LVLSUCCESS = -204,// ��ʯ�����ɹ� [1/26/2016 superrobert]
	ORESULT_ERROR_FRIENDGEM_WASHITEMLESS = -205,// ���ϲ��� [1/26/2016 superrobert]
	ORESULT_FRIENDGEM_WASHSUCCESS = -206,// ��ʯϴ���ɹ� [1/26/2016 superrobert]
	ORESULT_HERO_AWAKING_FAIL = -207,// ����ʧ�� [1/26/2016 HJ]
	ORESULT_ITEM_MIX_NOT_CHIP = -208,// ��Ʒ�ϳɲ�����Ƭ [2/15/2016 HJ]
	ORESULT_ITEM_MIX_LESS = -209,// �ϳɲ��ϲ��� [2/15/2016 HJ]
	ORESULT_NOT_ENOUGH_CU_HONOR = -210,//����ֵ����
	ORESULT_HANDBOOK_AWARD_NOT = -210,// δ�ﵽ�콱���� [2/19/2016 HJ]
	ORESULT_GODTOWER_SHOPPOINT_NOT = -211, // ����֮���̵�㲻�� [2/23/2016 wangshaocong]
	ORESULT_EQUIP_EXTRACTION_SUCCESS = -212, //�����ɹ�
	ORESULT_HEROCARD_NUM_MAX = -213,
	ORESULT_SPORTS_SETDEFENDERDATA_FAIL = -214, //������ս���ұ���ս��ʧ��
	ORESULT_BATTLEREADY_CHANGETEAM = -215,// ����ѡ��ս�� [3/25/2016 HJ]
	ORESULT_NOT_ENOUGH_CU_KUNG_FU_RELIC = -216,//�����ż����� [tianshuai 2016.4.12]
	ORESULT_NOT_ENOUGH_CU_WACKY_GADGET = -217,//��ֵĹ��߲��� [tianshuai 2016.4.12]
	ORESULT_NOT_ENOUGH_CU_VIKING_HEIRLOOM = -218,//ά�������ﲻ�� [tianshuai 2016.4.12]
	ORESULT_NOT_ENOUGH_CU_ZOO_SOUVENIR = -219,//����԰����Ʒ���� [tianshuai 2016.4.12]
	ORESULT_NOT_TRANSFER_ITEM = -220,// ����ת�� [4/13/2016 HJ]
	ORESULT_PLAYER_HEAD_EQUAL = -221,// �޸ĵ�ͷ���뵱ǰͷ����ͬ [4/19/2016 HJ]
	ORESULT_PLAYER_SHOPPING_FLUSH_MAX = -222,// �̵�ˢ�´����Ѵ����� [4/22/2016 tianshuai]
	ORESULT_PLAYER_TEAM_LEVEL_MAX = -223,// ���ս�ӵȼ��Ѵ���� [5/7/2016 HJ]
	ORESULT_GUILD_SEARCH_NULL = -224,//�������� û�з���Ҫ���
	ORESULT_WORLD_BOSS_RAIS_SUCCESS = -225, // ����boss�������ɹ���ʾ [5/9/2016 scw]
	ORESULT_FRIEND_ERROR_PVPTARGET_GETFAIL = -226,// �����д�û�ҵ�������Ϣ [5/10/2016 HJ]
	ORESULT_SPECIAL_GIFT_NOT_EXIST = -227,// ��ǰ������������� [5/19/2016 HJ]
	ORESULT_SPECIAL_GIFT_HAS_BUY = -228,// ��ǰ��������Ѿ������ [5/19/2016 HJ]
	ORESULT_SPECIAL_GIFT_BAG_FULL = -229,// ���������ⱳ������ [5/20/2016 HJ]
	ORESULT_GET_DAILY_PHYSICAL_NOT_OPEN = -230,// �ճ�������ȡ���ճ���������������
	ORESULT_GET_DAILY_PHYSICAL_CONFIG_ERROR = -231,// �ճ�������ȡ���ճ������ļ�����
	ORESULT_GET_DAILY_PHYSICAL_MAX_COUNT = -232,// �ճ�������ȡ����������
	ORESULT_GET_DAILY_PHYSICAL_TIME = -233,// �ճ�������ȡ��ʱ��δ��
	ORESULT_NOT_ENOUGH_CU_FRIEND = -234,// ����������� [5/24/2016 HJ]
	ORESULT_FRIEND_ERROR_GET_COUNT_MAX = -235,// ��ǰ��ȡ�����Ѵ����� [5/26/2016 HJ]
	ORESULT_FRIEND_ERROR_GIVE_COUNT_MAX = -236,// ��ǰ���ʹ����Ѵ����� [5/26/2016 HJ]
	ORESULT_FRIEND_NO_POINT_GET = -237,// ��ǰû�������������ȡ [5/26/2016 HJ]
	ORESULT_CURRENCY_NOT_ENOUGH = -238,// ���Ҳ��� [8/12/2016 TianShuai]
	ORESULT_WORLD_BOSS_RAIS_FAILED = -239,// ����boss�������ʧ�� [8/18/2016 scw]

	ORESULT_GUILD_DINNER_HASAWARD = -240,// ������ȡ���Ƚ��� [yangzhen]
	ORESULT_GUILD_DINNER_MAKING = -241,// �м������������ [yangzhen]
	ORESULT_GUILD_DINNER_MAKECOUNT_LESS = -242,// ��⿴����Ѿ����� [yangzhen]
	ORESULT_GUILD_DINNER_TIME_ERROR = -243,// �۲�ʱ��δ��[wangyiran]
	ORESULT_GUILD_DINNER_WAITING = -244,// �ȴ��᳤���˷���۲�[wangyiran]
	ORESULT_GUILD_DINNER_EAT_ALREADY = -245,// ���Ѿ��μӹ��۲���[wangyiran]
	ORESULT_GUILDBOSS_CHALLENGE_COUNT_LESS = -246,// ��ս�������� [wangyiran]
	ORESULT_GUILD_SHOP_ITEM_LOST = -247,// ��Ʒ������ [wangyiran]
	ORESULT_GUILD_SHOP_HAS_BUY = -248,// ��Ʒ������ [wangyiran]
	ORESULT_GUILDBOSS_PRE_NOT_PASS = -249,// ��ͨ����һ�� [wangyiran]
	ORESULT_GUILDBOSS_GATE_ERROR = -250,// �ؿ������� [wangyiran]
	ORESULT_GUILDBOSS_ACTIVITY_LESS = -251,// ��Ծ�Ȳ��� [wangyiran]
	ORESULT_GUILDBOSS_NOT_OPEN = -252,// �ؿ���δ���� [wangyiran]
	ORESULT_GUILDBOSS_IN_BATTLE = -253,// boss����ս���� [wangyiran]
	ORESULT_GUILDBOSS_LVL_LESS = -254,// �ȼ����� [wangyiran]
	ORESULT_GUILDBOSS_ALREADYPASS = -255,// �Ѿ�ͨ���� [wangyiran]
	ORESULT_GUILDBOSS_NOT_PASS = -256,// �ؿ���δͨ�� [wangyiran]
	ORESULT_GUILDBOSS_ALREADYOPEN = -257, // �ؿ��Ѿ����� [wangyiran]
	ORESULT_RAISE_FAILED = -258,// ����ʧ�� [9/19/2016 scw]
	ORESULT_RAISE_SUCCESS = -259, // ����ɹ� [9/19/2016 scw]
	ORESULT_ITEM_MIX_HAS_NOT = -260, // Ӣ���Ѿ����ڲ��ܺϳ� [9/22/2016 scw]
	ORESULT_ITEM_MIX_ALL_HAS_NOT = -261,
	ORESULT_AREAN_REFRESH_ENEMY_SUCC = -262, // ������ˢ�µ��˳ɹ� [10/10/2016 scw]
	ORESULT_EQUIP_EVOLUTION_FAILED = -263, // װ������ʧ�� [10/17/2016 scw]
	ORESULT_AFTER_CLIENTLOADFINISH = -264, // �������õ��˿ͻ�����ؼ�����ɵ���Ϣ���ظ��ͻ��� [10/21/2016 yz]
	ORESULT_CONDITION_NOTHING = -265, // �������� [11/14/2016 scw]
	ORESULT_GUILD_BOSS_INSPIRE_COUNT_MAX = -267, //����boss��������Ѵ�����  [3/27/2017 "tianshuai"]
	ORESULT_GUILD_BOSS_BUY_CHALLENGE_COUNT_MAX = -268, //����boss��������Ѵ�����  [3/27/2017 "tianshuai"]
	ORESULT_GUILD_BOSS_RESET_MAX = -269,// ����boss���ô����Ѵ����� [3/28/2017 "tianshuai"]

	ORESULT_INVITE_CODE_FRIEND_COUNT_FULL = -270,// �Է������������
	ORESULT_INVITE_CODE_FRIEND_ALREADY = -271,// �Ѿ��������������
	ORESULT_INVITE_CODE_FRIEND_ADD_SUCCESS = -272,// ��ӳɹ�
	ORESULT_INVITE_CODE_FRIEND_NOT_FIND = -273,// ����������� [4/18/2017 "tianshuai"]
	ORESULT_GOD_ARENA_CHALLENGE_FAILED = -274,// ս�񾺼���������սʧ�� [7/15/2017 scw]


	ORESULT_NOT_ENOUGH_CU_PeakArenaCoin		= -600, // �۷徺����ѫ�²��� [xingzhishan]
	ORESULT_PEAKARENA_NO_FIND_PLAYER		= -796, //�۷徺�����Ҳ������ [chentongxin]

	ORESULT_HERO_RETURN_NO_HERO = -797,	//Ӣ�ۻ��գ��Ҳ���Ӣ��
	ORESULT_HERO_RETURN_OK = -798,	//Ӣ�ۻ��ճɹ�
	ORESULT_HERO_RETURN_GOLD_NO = -799,	//Ӣ�ۻ��ս�Ǯ����
	ORESULT_HERO_RETURN_QIYUE_BAG_FULL = -800, //��Լ��������
	ORESULT_EQUIP_STRONGER_FAIL = -801, //װ��ǿ��ʧ��
	ORESULT_GUILD_APPLY_ADD_NUM = -802, //������Ϣ����
	ORESULT_GUILD_APPLY_EXIST = -803,	//�Ѿ��������
	ORESULT_EQUIP_TREASURE_SCORE_FULL = -804, //װ������ ��������
	ORESULT_MAX_COUNT
};

typedef eOPTRESULT	OPT_RESULT;

const char		*GetORTText(eOPTRESULT code);
#endif
