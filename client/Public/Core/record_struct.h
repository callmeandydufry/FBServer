/*$T MapServer/Server/Base/record_struct.h GC 1.140 10/10/09 10:07:21 */

/*
$File Name:
	record_struct.h
$Purpose:
	定义所有游戏系统行为日志的格式
$Update history:
	v1.0 created.
*/
#ifndef _RECORD_STRUCT_H_
#define _RECORD_STRUCT_H_

/*
 * ifndef __RECORD_GAMEWAY_H_ ;
 * #error "Not direct include record_struct.h, Please include record_gameway.h" ;
 * #endif
 */
#include "act_define.h"

struct ItemLogRecordInfo
{
	SNID_t		CharSNID;			/* 自身角色,物品所有者 */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	char		TargetName[MAX_KHAN_CHARACTER_NAME]; /* 目标角色名字 */
	char		IP[IP_SIZE];			/* 所在server IP */
	SItemSnid	ItemSnid;			/* 物品编号 */
	uint32		ItemType;			/* 物品全局编号 */
	SNID_t		TargetSNID;			/* 目标角色 */
	eItemActType	OpType;			/* 操作类型 */
	char		OpDesc[128];		/* 操作描述 */
	eItemActType	SecOpType;		/* 转向类型 */
	uint16		ContainerPos;		/* 容器位置 */
	uint16		DestContainerPos;	/* 目标容器位置 */
	uint32		Count;			/* 变化个数 */
	ut_id_map		MapID;			/* 场景编号 */
	int32		NpcType;		/* npc 类型,掉落时填怪物类型 */
	float		XPos;			/* 事件发生位置 */
	float		ZPos;
	int32		ShopSNID;		/* 相关商店号 */
	uint8		GemMax;			/* 最大宝石个数(蓝装必须要) */
	int32		ScriptID;		/* 脚本ID */
	int32		WorldID;		/* 服务器ID */
	uchar		TempInvalid;	/* 是否为临时物品 */
	uint32		ExtData;		/* 扩展数据 */

	ItemLogRecordInfo (void)
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		CharSNID = TargetSNID = -1;
		OpType = OpType = eItemAct_Unknow;
		ContainerPos = DestContainerPos = -1;
		Count = 1;
		MapID = NpcType = -1;
		XPos = ZPos = 0.0f;
		ShopSNID = -1;
		GemMax = 0;
		ItemType = 0;
		ScriptID = -1;
		WorldID = 0;
		memset(Name, 0, sizeof(Name));
		memset(TargetName, 0, sizeof(TargetName));
		memset(IP, 0, sizeof(IP));
		memset(OpDesc, 0, sizeof(OpDesc));
		TempInvalid = FALSE;
		ExtData = 0;
	}
};


struct PostLogRecordInfo
{
	enum
	{
		eAddPostSuccess,
		eSystemDelPost,
		eUseDelPost,
	};

	PostLogRecordInfo()
	{
		m_Type = -1;
		m_Post.Clear();
	}

	int8 m_Type;
	SPost m_Post;
};


struct AwardPostLogRecordInfo
{
	enum
	{
		eAddAwardPostSuccess,
		eSystemDelPost,
		eUseDelPost,
	};

	AwardPostLogRecordInfo()
	{
		m_Type = -1;
		m_Post.Clear();
	}

	int8 m_Type;
	SAwardPost m_Post;
};

struct EquipLogRecordInfo
{
	SNID_t		CharSNID;	/* 自身角色,物品所有者 */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	char		IP[IP_SIZE];			/* 所在server IP */
	SItemSnid	ItemSnid;			/* 物品编号 */
	uint32		ItemType;			/* 物品全局编号 */
	eEquipActType	OpType;		/* 操作类型 */
	uint8		Level;		/* 等级 */
	EquipProperty m_Property[MAX_EQUIP_EXTRA_PROPERTY_NUM];
	EquipProperty m_RefinePropertyTmp[MAX_EQUIP_EXTRA_PROPERTY_NUM];
	
	EquipLogRecordInfo (void)
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		CharSNID = -1;
		OpType = eEquipAct_Unknow;
		ItemType = Level = 0;
		memset(Name, 0, sizeof(Name));
		memset(IP, 0, sizeof(IP));
		ItemSnid.Reset();

		for( int32 i = 0; i < MAX_EQUIP_EXTRA_PROPERTY_NUM; ++i )
		{
			m_Property[i].Clear();
			m_RefinePropertyTmp[i].Clear();
		}

		//RoseBand = 0;
	}
};

struct CurrencyLogRecordInfo
{
	SNID_t			CharSNID;			/* 自身角色,money所有者 */
	SNID_t			TargetSNID;			/* 目标角色 */
	eCurrencyActType	OPType;				/* 操作类型 */
	uint8			MoneyType;			/* 金钱类型 */
	int32			Count;				/* 数量 */
	int32			RemainCount;			/* 剩余金钱总数量 */
	ut_id_map			MapID;				/* 场景编号 */
	float			XPos;				/* 事件发生位置 */
	float			ZPos;
	char			Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	int32			ItemIndex;			/* 物品编号(当购买时候有效) */
	int32			ItemCount;			/* 购买物品数量 */
	ut_id_script		ScriptId;			/* 脚本id*/
	int16			nExt;				/* 保留nExt, 用于扩展更精确的金钱的记录*/
	int32			WorldID;			/* 服务器ID */

	CurrencyLogRecordInfo()
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		CharSNID = TargetSNID = INVALID_KHAN_ID;
		OPType = eCurrencyAct_Unknow;
		MoneyType = 0;
		Count = 0;
		MapID = -1;
		XPos = ZPos = 0.0f;
		memset(Name, 0, sizeof(Name));
		ItemIndex = -1;
		ItemCount = 0;
		ScriptId = 0;
		nExt = 0;	
		WorldID = 0;
	}
};

struct SpellLogRecordInfo
{
	SNID_t		CharSNID;	/* 自身角色,Skill所有者 */
	eSkillActType	OPType;		/* 操作类型 */
	ut_id_map		MapID;		/* 场景编号 */
	float		XPos;		/* 事件发生位置 */
	float		ZPos;
	ut_id_spell	SkillID;	/* 技能号 */
	int32		Level;		/* 级别 */
	int32		Count;		/* 技能 */
	int32		WorldID;	/* 服务器ID */

	SpellLogRecordInfo()
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		CharSNID = INVALID_KHAN_ID;
		OPType = eSkillAct_Unknow;
		MapID = -1;
		XPos = 0.0f;
		ZPos = 0.0f;
		SkillID = -1;
		Level = -1;
		Count = 0;
		WorldID = 0;
	}
};

struct SkillLogRecordInfo
{
	SNID_t		CharSNID;	/* 自身角色,Skill所有者 */
	eAbilityActType OPType;		/* 操作类型 */
	ut_id_map		MapID;		/* 场景编号 */
	float		XPos;		/* 事件发生位置 */
	float		ZPos;
	ut_id_spell	AbilityID;	/* 技能号 */
	int32		Level;		/* 级别 */
	int32		Count;
	int32		WorldID;

	SkillLogRecordInfo()
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		CharSNID = INVALID_KHAN_ID;
		OPType = eAbilityAct_Unknow;
		MapID = -1;
		XPos = 0.0f;
		ZPos = 0.0f;
		AbilityID = -1;
		Level = -1;
		Count = 0;
		WorldID = 0;
	}
};

struct TeamMemberLvlAndJob
{
	int32	Level;
	int32	Job;
	TeamMemberLvlAndJob (void)
	{
		clear();
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		Job = Level = -1;
	}
};

struct GameWayLogRecoreInfo
{
	SNID_t			CharSNID;			/* 自身角色 */
	char			Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	ut_id_map			MapID;	/* 场景编号 */
	int32			GameID;
	uint8			HaveTeam;
	uint8			member_cnt;
	int32			WorldID;		/* 服务器ID */
	TeamMemberLvlAndJob	member[TEAM_MAX_MEMBER_COUNT];

	GameWayLogRecoreInfo (void)
	{
		clear();
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		CharSNID = INVALID_KHAN_ID;		/* 自身角色,Skill所有者 */
		MapID = INVALID_KHAN_ID;		/* 场景编号 */
		GameID = INVALID_KHAN_ID;
		HaveTeam = 0;
		member_cnt = 0;
		WorldID = 0;
		for(int32 _Idx = 0; _Idx < TEAM_MAX_MEMBER_COUNT; ++_Idx) member[_Idx].clear();
	}
};

struct EnterMapLogRecordInfo
{
	SNID_t	CharSNID;			/* 自身角色 */
	char	Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	ut_id_map MapID;	/* 场景编号 */
	int32	GameID;
	int32	DayCount;
	int32	WorldID;

	EnterMapLogRecordInfo (void)
	{
		clear();
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear()
	{
		CharSNID = INVALID_KHAN_ID;	/* 自身角色,Skill所有者 */
		MapID = INVALID_KHAN_ID;	/* 场景编号 */
		GameID = INVALID_KHAN_ID;
		DayCount = 0;
		WorldID = 0;
	}
};

struct GoldCardLogRecordInfo
{
	SNID_t	snidChar;		/* 玩家SNID */
	char	Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	ut_id_map mapId;		/* 场景ID */
	int32	iChangeType;	/* 变更类型 */
	int32	iCostTypeId;	/* 金钱类型 */
	uint32	uPayment;	/* 金钱变更数量 */
	uint32	uItemTypeID;	/* 获得物品ID */
	int32	iItemCount;	/* 获得物品数量 */
	int32	WorldID;

	GoldCardLogRecordInfo (void)
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		snidChar = INVALID_KHAN_ID;
		mapId = INVALID_KHAN_ID;
		iChangeType = INVALID_KHAN_ID;
		uItemTypeID = 0;
		uPayment = 0;
		iCostTypeId = INVALID_KHAN_ID;
		memset(Name, 0, sizeof(Name));
		WorldID = 0;
	}
};

enum ManageLogType
{
	MLT_ONLENE_USER_COUNT	= 0,
	MLT_ONLENE_ITEM,
	MLT_ONLENE_MONEY,
	MLT_ONLENE_IB,
	MLT_ONLENE_TRADE,
	MLT_ONLENE_EQUIP,
};

struct TradeLogRecordInfo
{
	struct ItemParam
	{

		/*
		 =======================================================================================================
		 =======================================================================================================
		 */
		void clear(void)
		{
			m_nItemId = INVALID_KHAN_ID;
			m_nCount = 0;
			m_nItemSnid.Reset();
		}

		int32		m_nItemId;
		SItemSnid	m_nItemSnid;
		int32		m_nCount;
	};
	struct TradeParam
	{
		enum eTradeType { eExchange = 0, eStallSiller, eStallGold };

		/*
		 =======================================================================================================
		 =======================================================================================================
		 */
		void clear(void)
		{
			m_nCharSnid = INVALID_SNID;
			memset(m_szCharName, 0, sizeof(m_szCharName));
			m_nMapId = INVALID_KHAN_ID;
			m_nTradeType = eExchange;
			m_nPreGold = 0;
			m_nAftGold = 0;
			for(int32 _Idx = 0; _Idx < EXCHANGE_KHAN_BOX_SIZE; ++_Idx) m_anItem[_Idx].clear();
		}

		SNID_t		m_nCharSnid;
		char		m_szCharName[MAX_KHAN_CHARACTER_NAME];
		ut_id_map		m_nMapId;
		int32		m_nTradeType;
		int32		m_nPreGold;
		int32		m_nAftGold;
		ItemParam	m_anItem[EXCHANGE_KHAN_BOX_SIZE];
	};

	TradeLogRecordInfo (void)
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		for(int32 _Idx = 0; _Idx < 2; ++_Idx) m_anTradeParam[_Idx].clear();
		m_WorldID = 0;
	}

	TradeParam	m_anTradeParam[2];
	int32		m_WorldID;
};

struct WorldShopLogRecordInfo
{
	SNID_t			OwnerSNID;	/* 所有者SNID */
	SNID_t			BuyerSNID;	/* 购买者SNID */
	eWorldShopCashActType	OPType;		/* 操作类型 */
	int32			ShopId;		/* 商店ID */
	uint32			SerialId;	/* 商店中的位置,-1表示不需要位置 */
	int32			SellGold;	/* 寄售/银行领取现金数 */
	int32			GoldNum;	/* 银行领取时记录的现金笔数 */
	int32			Price;		/* 售价 */
	int32			SillerNum;	/* 银行领取时记录的现银笔数 */
	int32			Charge;		/* 手继费 */
	uint32			CreateTime;	/* 创建时间 */
	uint32			DeleteTime;	/* 删除时间 */
	ut_id_map			MapID;		/* 场景ID */
	int32			WorldID;	/* 服务器ID */

	WorldShopLogRecordInfo (void)
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		OwnerSNID = INVALID_KHAN_ID;
		BuyerSNID = INVALID_KHAN_ID;
		OPType = eWorldShopCashAct_Unknow;
		ShopId = -1;
		SerialId = -1;
		SellGold = 0;
		GoldNum = 0;
		Price = 0;
		SillerNum = 0;
		Charge = 0;
		CreateTime = -1;
		DeleteTime = -1;
		MapID = -1;
		WorldID = 0;
	}
};

struct GLGoldShopLogRecordInfo
{
	SNID_t			OwnerSNID;	/* 所有者SNID */
	SNID_t			BuyerSNID;	/* 购买者SNID */
	eWorldShopCashActType	OPType;		/* 操作类型 */
	int32			ShopId;		/* 商店ID */
	uint32			SerialId;	/* 商店中的位置,-1表示不需要位置 */
	int32			SellGold;	/* 寄售/银行领取现金数 */
	int32			GoldNum;	/* 银行领取时记录的现金笔数 */
	int32			Price;		/* 售价 */
	int32			SillerNum;	/* 银行领取时记录的现银笔数 */
	int32			Charge;		/* 手继费 */
	uint32			CreateTime;	/* 创建时间 */
	uint32			DeleteTime;	/* 删除时间 */

	GLGoldShopLogRecordInfo (void)
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		OwnerSNID = INVALID_KHAN_ID;
		BuyerSNID = INVALID_KHAN_ID;
		OPType = eWorldShopCashAct_Unknow;
		ShopId = -1;
		SerialId = -1;
		SellGold = 0;
		GoldNum = 0;
		Price = 0;
		SillerNum = 0;
		Charge = 0;
		CreateTime = -1;
		DeleteTime = -1;
	}
};

struct CheatActLogRecordInfo
{
	char		Account[MAX_ACCOUNT + 1];		/* 帐号 */
	char		CharName[MAX_KHAN_CHARACTER_NAME + 1];	/* 角色名字 */
	SNID_t		OwnerSNID;	/* 所有者SNID */
	eCheatActType	OPType;		/* 操作类型 */
	ut_id_map		MapID;		/* 场景ID */
	uint32		Offset;		/* 随机的偏移量 */
	float		PosX;		/* 角色当前X坐标 */
	float		PosZ;		/* 角色当前Z坐标 */
	int32		WorldID;	/* 服务器ID */

	/*
	 * 移动时起点坐标X+Z，或者是心跳、锁定时的ObiId ;
	 * 登陆验证： 0:基础加密模式，1：随机加密模式，2：时间加密模式
	 */
	uint32		Param;

	CheatActLogRecordInfo()
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		memset(Account, 0, sizeof(Account));
		memset(CharName, 0, sizeof(CharName));
		OwnerSNID = INVALID_KHAN_ID;
		OPType = eCheatAct_Unknow;
		MapID = INVALID_KHAN_ID;
		Offset = -1;
		PosX = 0;
		PosZ = 0;
		Param = 0;
		WorldID = 0;
	}
};

struct IBInItemLogRecordInfo
{
	SNID_t		CharSNID;			/* 自身角色,物品所有者 */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	SItemSnid	ItemSnid;			/* 物品编号 */
	uint32		ItemType;			/* 物品全局编号 */
	uint32		OpType;			/* 操作类型 */
	uint32		Count;			/* 个数 */
	uint32		GoldCash;		/* 现金数 */
	uint32		TermTime;		/* 有效期 */
	ut_id_map		MapID;			/* 场景ID */
	int32		WorldID;		/* 服务器ID */

	IBInItemLogRecordInfo (void)
	{
		clear();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void clear(void)
	{
		CharSNID =  -1;
		Count = 1;
		ItemType = 0;
		TermTime = 0;
		GoldCash = 0;
		WorldID = 0;
		MapID = -1;
		OpType = eItemAct_Unknow;
		memset(Name, 0, sizeof(Name));
	}
};

struct IBOutItemLogRecordInfo
{
	SNID_t		CharSNID;			/* 自身角色,物品所有者 */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* 角色名字 */
	SItemSnid	ItemSnid;			/* 物品编号 */
	uint32		ItemType;			/* 物品全局编号 */
	uint32		OpType;			/* 操作类型 */
	uint32		Count;			/* 个数 */
	uint32		GoldCash;		/* 现金数 */
	ut_id_map		MapID;			/* 场景ID */
	int32		WorldID;		/* 服务器ID */

	IBOutItemLogRecordInfo (void)
	{
		clear();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void clear(void)
	{
		CharSNID =  -1;
		Count = 1;
		ItemType = 0;
		OpType = eItemAct_Unknow;
		memset(Name, 0, sizeof(Name));
		GoldCash = 0;
		WorldID = 0;
		MapID = -1;
	}
};


struct GLStockTradeLogRecordInfo
{
	SNID_t			OwnerSNID;	/* 所有者SNID */
	SNID_t			OtherSNID;	/* 交易另一方SNID */
	eStockActType	OPType;		/* 操作类型 */
	uchar			TradeType;	/* 交易类型 */
	int32			OwnerPrice;	/* 所有者出价：单价 */
	int32			OtherPrice;	/* 交易另一方出价：单价 */
	int32			Gold;		/* 交易的现金总数 */
	uint32			CreateTime;	/* 创建时间 */
	uint32			DeleteTime;	/* 删除时间 */

	GLStockTradeLogRecordInfo(void)
	{
		clear();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void clear(void)
	{
		OwnerSNID = INVALID_KHAN_ID;
		OtherSNID = INVALID_KHAN_ID;
		OPType = eStockAct_Unknow;
		TradeType = STOCK_TRADE_TYPE_BUY;
		OwnerPrice = 0;
		OtherPrice = 0;
		Gold = 0;
		CreateTime = -1;
		DeleteTime = -1;
	}
};

//账户记录LOG
struct MapStockAccLogRecordInfo
{
	SNID_t			OwnerSNID;	/* 所有者SNID */
	eStockAccActType OPType;	/* 操作类型 */
	int32			TransGold;	/* 转账的现金 */
	int32			TransSiller;/* 转账的现银 */
	int32			StockGold;	/* 账户余额 现金 */
	int32			StockSiller;/* 账户余额 现银 */
	ut_id_map			MapID;		/* 场景ID */
	int32			WorldID;	/* 服务器ID */

	MapStockAccLogRecordInfo()
	{
		clear();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void clear()
	{
		OwnerSNID = INVALID_KHAN_ID;
		OPType = eStockAccAct_Unknow;
		TransGold = 0;
		TransSiller = 0;
		StockGold = 0;
		StockSiller = 0;
		MapID = -1;
		WorldID = 0;
	}
};

//账户记录LOG
struct GLStockAccLogRecordInfo
{
	SNID_t			OwnerSNID;	/* 所有者SNID */
	eStockAccActType OPType;	/* 操作类型 */
	int32			TransGold;	/* 转账的现金 */
	int32			TransSiller;/* 转账的现银 */
	int32			StockGold;	/* 账户余额 现金 */
	int32			StockSiller;/* 账户余额 现银 */

	GLStockAccLogRecordInfo()
	{
		clear();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void clear()
	{
		OwnerSNID = INVALID_KHAN_ID;
		OPType = eStockAccAct_Unknow;
		TransGold = 0;
		TransSiller = 0;
		StockGold = 0;
		StockSiller = 0;
	}
};

// GL帮会属性Log
struct GLGuildLogRecordInfo
{
	ut_id_guild		m_GuildId;								//帮会ID
	char			m_szGuildName[MAX_KHAN_GUILD_NAME_SIZE];		//帮会名
	eGuildActType	m_OpType;								//操作类型
	int32			m_nCount;								//变化值
	int32			m_nRemainCount;							//变化后的值
	int32			m_nParam;								//操作相关的参数
	int32			m_nScriptId;							//脚本Id


	GLGuildLogRecordInfo (void)
	{
		clear();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void clear(void)
	{
		m_GuildId = INVALID_GUILD_ID;
		memset( m_szGuildName, 0, MAX_KHAN_GUILD_NAME_SIZE );
		m_OpType = eGuildAct_Unknow;
		m_nCount = 0;
		m_nRemainCount = 0;
		m_nParam = 0;
		m_nScriptId = -1;
	}
};

struct ZhuanJingLogRecordInfo
{
	SNID_t		CharSNID;	/* 自身角色 */
	eZhuanJingActType	OPType;		/* 操作类型 */
	ut_id_map		MapID;		/* 场景编号 */
	float		XPos;		/* 事件发生位置 */
	float		ZPos;
	int32		TotalPoint;
	int32		Profession[PROFESSION_NUMBER];
	int32		Reserve_param0;
	int32		Reserve_param1;
	int32		WorldID;	/* 服务器ID */


	ZhuanJingLogRecordInfo()
	{
		clear();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void clear(void)
	{
		CharSNID = INVALID_KHAN_ID;
		OPType = eZhuanjingAct_Unknow;
		MapID = -1;
		XPos = 0.0f;
		ZPos = 0.0f;
		TotalPoint = 0;
		Reserve_param0 = 0;
		Reserve_param1 = 0;
		WorldID = 0;
		memset(Profession, 0, sizeof(Profession));
	}
};

struct PlayerUseSkillCount
{
	SNID_t			m_CharGUID;		/* 自身角色 */
	ut_id_map		m_MapID;		/* 场景编号 */
	float			m_XPos;			/* 事件发生位置 */
	float			m_ZPos;
	int32			m_Count;		/* 技能释放的数量 */
	int32			m_WorldID;		/* 服务器ID */
	int32			m_SkillID;		/* 使用的技能能ID */
	int32			m_Profession;	/* 玩家的职业 */

	PlayerUseSkillCount()
	{
		Clear();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void Clear()
	{
		m_CharGUID = INVALID_KHAN_ID;
		m_MapID = -1;
		m_XPos = 0.0f;
		m_ZPos = 0.0f;
		m_WorldID = 0;
	}
};

// 玩家登陆登出db日志 [wangyiran]
struct PlayerLoginOutLogRecordInfo
{
	SNID_t	m_nCharSNID; // 角色Id [wangyiran]
	char	m_Name[MAX_KHAN_CHARACTER_NAME+1];// 名字 [wangyiran]
	char	m_Account[MAX_KHAN_CHARACTER_NAME+1];	// 账号 [wangyiran]
	char	m_Mac[MAX_KHAN_CHARACTER_NAME+1];	// mac [wangyiran]
	char	m_Idfa[MAX_DEVICE_IDFA_LENGTH];	// idfa [wangyiran]
	char	m_Device[MAX_KHAN_CHARACTER_NAME+1];	// Device [wangyiran]
	int32	m_DeviceOS;	// DeviceOS [wangyiran]
	int32	m_nWorldID;// 服务器id [wangyiran]
	uint32	m_nLoginTime;// 登陆时间 [wangyiran]
	uint32	m_nLogoutTime; // 登出时间 [wangyiran]
	int32	m_nOnlineTime;// 在线时间 [wangyiran]
	int32	m_nType;// 0 登陆 1登出 [wangyiran]
	int64	m_nPlatformUid;// 平台账号id [wangyiran]
	int32	m_nChannel;// 渠道 [wangyiran]

	PlayerLoginOutLogRecordInfo()
	{
		clear();
	}

	void clear(void)
	{
		m_nCharSNID = INVALID_KHAN_ID;
		memset(m_Name, 0, sizeof(m_Name));
		memset(m_Account, 0, sizeof(m_Account));
		memset(m_Mac, 0, sizeof(m_Mac));
		memset(m_Idfa, 0, sizeof(m_Idfa));
		memset(m_Device, 0, sizeof(m_Device));
		m_DeviceOS = 0;
		m_nWorldID = 0;
		m_nLoginTime = 0;
		m_nLogoutTime = 0;
		m_nOnlineTime = 0;
		m_nType = 0;
		m_nPlatformUid = 0;
		m_nChannel = 0;
	}
};

// billing推送的订单充值日志[wangxiaoyong]
struct BillingOrderInfo
{
	char	m_OrderId[MAX_ORDER_ID_SIZE];		// 订单号 [wangxiaoyong]
	SNID_t	m_nUserId;							// 玩家SNID [wangxiaoyong]
	char	m_ProductId[MAX_PRODUCT_ID_SIZE];	// 商品ID [wangxiaoyong]
	double	m_nPayMoney;						// 支付金额 [wangxiaoyong]
	int32	m_nBindPoints;						// 绑定符石点数 [wangxiaoyong]
	int32	m_nPoints;							// 充值符石点数 [wangxiaoyong]
	int32	m_nAdditionalPoints;				// 充值赠送符石点数 [wangxiaoyong]
	char	m_PayTime[MAX_TD_ITEM_SIZE];		// 支付时间 [wangxiaoyong]
	int32	m_nChannelId;						// 渠道ID [wangxiaoyong]

	BillingOrderInfo()
	{
		Clear();
	}

	void Clear()
	{
		memset(m_OrderId, 0, sizeof(m_OrderId));
		memset(m_ProductId, 0, sizeof(m_ProductId));
		memset(m_PayTime, 0, sizeof(m_PayTime));
		m_nUserId = INVALID_KHAN_ID;
		m_nPayMoney = 0;
		m_nBindPoints = 0;
		m_nPoints = 0;
		m_nAdditionalPoints = 0;
		m_nChannelId = 0;
	}
};

// 英雄养成日志[wangxiaoyong]
struct HeroGrowLogInfo
{
	char	m_PlayerName[MAX_KHAN_CHARACTER_NAME+1];
	SNID_t	m_nPlayerId;
	int32 m_nHeroTabIndex;
	int32 m_nHeroLvl;
	int32 m_nHeroGrade;
	int32 m_nConstellation;
	int32 m_nConstellatiionStar;
	int32 m_nHeroSkill1Lvl;
	int32 m_nHeroSkill2Lvl;
	eHeroGrowOperate op;

	HeroGrowLogInfo()
	{
		Clear();
	}

	void Clear()
	{
		memset(m_PlayerName, 0, sizeof(m_PlayerName));
		m_nPlayerId = 0;
		m_nHeroTabIndex = 0;
		m_nHeroSkill1Lvl = 0;
		m_nHeroSkill2Lvl = 0;
		m_nHeroGrade = 0;
		m_nHeroLvl = 0;
		m_nConstellation = 0;
		m_nConstellatiionStar = 0;
		op = eHeroGrowOperate_None;
	}
};


// 神器养成日志[tianshuai]
struct ArtifactLogInfo
{
	char	m_PlayerName[MAX_KHAN_CHARACTER_NAME + 1];
	SNID_t	m_nPlayerId;
	int32 ArtifactId;
	int32 ArtifactLevel;
	int32 m_nPlayerLevel;
	ArtifactOperate op;

	ArtifactLogInfo()
	{
		Clear();
	}

	void Clear()
	{
		memset(m_PlayerName, 0, sizeof(m_PlayerName));
		m_nPlayerId = 0;
		ArtifactId = -1;
		m_nPlayerLevel = 0;
		ArtifactLevel = 0;
		op = ArtifactOperate_None;
	}
};


//  [4/22/2016 tianshuai] 寻宝日志
struct FindTreasureLogInfo
{
	char	m_PlayerName[MAX_KHAN_CHARACTER_NAME + 1];
	SNID_t	m_nPlayerId;
	int32 m_nParam1;
	int32 m_nParam2;
	FIND_TREASURE_OPERATE op;

	FindTreasureLogInfo()
	{
		Clear();
	}

	void Clear()
	{
		memset(m_PlayerName, 0, sizeof(m_PlayerName));
		m_nPlayerId = 0;
		m_nParam1 = 0;
		m_nParam2 = 0;
		op = FIND_TREASURE_OPERATE_NONE;
	}
};

//  [4/22/2016 tianshuai] 英雄转移日志
struct HeroTransferLogInfo
{
	SNID_t	m_nPlayerId;
	char	m_PlayerName[MAX_KHAN_CHARACTER_NAME + 1];
	int32 m_nFromHeroTabIndex;   //被转移英雄
	int32 m_nFParam1;//转移前等级
	int32 m_nFParam2;//转移前升华(突破)
	int32 m_nFParam3;//转移前进阶(觉醒)
	int32 m_nFParam4;//转移前攻击
	int32 m_nFParam5;//转移前生命
	int32 m_nFParam6;//转移前能量回复
	int32 m_nFParam7;//转移前生命回复
	int32 m_nFParam8;//转移后等级
	int32 m_nFParam9;//转移后升华
	int32 m_nFParam10;//转移后进阶
	int32 m_nFParam11;//转移后攻击
	int32 m_nFParam12;//转移后生命
	int32 m_nFParam13;//转移后能量回复
	int32 m_nFParam14;//转移后生命回复

	int32 m_nToHeroTabIndex;//接受转移英雄
	int32 m_nTParam1;//转移前等级
	int32 m_nTParam2;//转移前升华
	int32 m_nTParam3;//转移前进阶
	int32 m_nTParam4;//转移前攻击
	int32 m_nTParam5;//转移前生命
	int32 m_nTParam6;//转移前能量回复
	int32 m_nTParam7;//转移前生命回复
	int32 m_nTParam8;//转移后等级
	int32 m_nTParam9;//转移后升华
	int32 m_nTParam10;//转移后进阶
	int32 m_nTParam11;//转移后攻击
	int32 m_nTParam12;//转移后生命
	int32 m_nTParam13;//转移后能量回复
	int32 m_nTParam14;//转移后生命回复

	HERO_TRANSFER_OPERATE op;

	HeroTransferLogInfo()
	{
		Clear();
	}

	void Clear()
	{
		m_nPlayerId = 0;
		memset(m_PlayerName, 0, sizeof(m_PlayerName));
		m_nFromHeroTabIndex = 0;
		m_nFParam1 = 0;
		m_nFParam2 = 0;
		m_nFParam3 = 0;
		m_nFParam4 = 0;
		m_nFParam5 = 0;
		m_nFParam6 = 0;
		m_nFParam7 = 0;
		m_nFParam8 = 0;
		m_nFParam9 = 0;
		m_nFParam10 = 0;
		m_nFParam11 = 0;
		m_nFParam12 = 0;
		m_nFParam13 = 0;
		m_nFParam14 = 0;

		m_nToHeroTabIndex = 0;
		m_nTParam1 = 0;
		m_nTParam2 = 0;
		m_nTParam3 = 0;
		m_nTParam4 = 0;
		m_nTParam5 = 0;
		m_nTParam6 = 0;
		m_nTParam7 = 0;
		m_nTParam8 = 0;
		m_nTParam9 = 0;
		m_nTParam10 = 0;
		m_nTParam11 = 0;
		m_nTParam12 = 0;
		m_nTParam13 = 0;
		m_nTParam14 = 0;
		op = HERO_TRANSFER_OPERATE_NONE;
	}
};


// 神器养成日志[tianshuai]
struct IndentureLogInfo
{
	char	m_PlayerName[MAX_KHAN_CHARACTER_NAME + 1];
	SNID_t	m_nPlayerId;
	int32 m_nIndentureIndex;
	int32 m_nIndentureType;
	int32 m_nIndentureQuality;
	int32 m_nIndentureUseCount;
	int32 m_nIndentureFuseQuality1;
	int32 m_nIndentureFuseQuality2;
	int32 m_nIndentureFuseQuality3;
	int32 m_nIndentureFuseQuality4;
	int32 m_nIndentureFuseType1;
	int32 m_nIndentureFuseType2;
	int32 m_nIndentureFuseType3;
	int32 m_nIndentureFuseType4;
	IndentureOperate op;

	IndentureLogInfo()
	{
		Clear();
	}

	void Clear()
	{
		memset(m_PlayerName, 0, sizeof(m_PlayerName));
		m_nPlayerId = 0;
		m_nIndentureIndex = -1;
		m_nIndentureType = -1;
		m_nIndentureQuality = -1;
		m_nIndentureUseCount = 0;
		m_nIndentureFuseQuality1 = -1;
		m_nIndentureFuseQuality2 = -1;
		m_nIndentureFuseQuality3 = -1;
		m_nIndentureFuseQuality4 = -1;
		m_nIndentureFuseType1 = -1;
		m_nIndentureFuseType2 = -1;
		m_nIndentureFuseType3 = -1;
		m_nIndentureFuseType4 = -1;
		op = IndentureOperate_None;
	}
};

// 特殊效果日志[tianshuai]
struct EffectUpInfo
{
	char m_PlayerName[MAX_KHAN_CHARACTER_NAME + 1];
	SNID_t	m_nPlayerId;
	int32 m_nEffectType;
	int32 m_nAddTime;
	uint32 m_nLastTime;
	uint32 m_nEndTime;
	PlayerEffectUpOperate op;

	EffectUpInfo()
	{
		Clear();
	}

	void Clear()
	{
		memset(m_PlayerName, 0, sizeof(m_PlayerName));
		m_nPlayerId = 0;
		m_nEffectType = 0;
		m_nAddTime = 0;
		m_nLastTime = 0;
		m_nEndTime = 0;
		op = PlayerEffectUpOperate_None;
	}
};
#endif /* _RECORD_STRUCT_H_ */

/* ???? */
