/*$T MapServer/Server/Base/record_struct.h GC 1.140 10/10/09 10:07:21 */

/*
$File Name:
	record_struct.h
$Purpose:
	����������Ϸϵͳ��Ϊ��־�ĸ�ʽ
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
	SNID_t		CharSNID;			/* �����ɫ,��Ʒ������ */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	char		TargetName[MAX_KHAN_CHARACTER_NAME]; /* Ŀ���ɫ���� */
	char		IP[IP_SIZE];			/* ����server IP */
	SItemSnid	ItemSnid;			/* ��Ʒ��� */
	uint32		ItemType;			/* ��Ʒȫ�ֱ�� */
	SNID_t		TargetSNID;			/* Ŀ���ɫ */
	eItemActType	OpType;			/* �������� */
	char		OpDesc[128];		/* �������� */
	eItemActType	SecOpType;		/* ת������ */
	uint16		ContainerPos;		/* ����λ�� */
	uint16		DestContainerPos;	/* Ŀ������λ�� */
	uint32		Count;			/* �仯���� */
	ut_id_map		MapID;			/* ������� */
	int32		NpcType;		/* npc ����,����ʱ��������� */
	float		XPos;			/* �¼�����λ�� */
	float		ZPos;
	int32		ShopSNID;		/* ����̵�� */
	uint8		GemMax;			/* ���ʯ����(��װ����Ҫ) */
	int32		ScriptID;		/* �ű�ID */
	int32		WorldID;		/* ������ID */
	uchar		TempInvalid;	/* �Ƿ�Ϊ��ʱ��Ʒ */
	uint32		ExtData;		/* ��չ���� */

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
	SNID_t		CharSNID;	/* �����ɫ,��Ʒ������ */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	char		IP[IP_SIZE];			/* ����server IP */
	SItemSnid	ItemSnid;			/* ��Ʒ��� */
	uint32		ItemType;			/* ��Ʒȫ�ֱ�� */
	eEquipActType	OpType;		/* �������� */
	uint8		Level;		/* �ȼ� */
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
	SNID_t			CharSNID;			/* �����ɫ,money������ */
	SNID_t			TargetSNID;			/* Ŀ���ɫ */
	eCurrencyActType	OPType;				/* �������� */
	uint8			MoneyType;			/* ��Ǯ���� */
	int32			Count;				/* ���� */
	int32			RemainCount;			/* ʣ���Ǯ������ */
	ut_id_map			MapID;				/* ������� */
	float			XPos;				/* �¼�����λ�� */
	float			ZPos;
	char			Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	int32			ItemIndex;			/* ��Ʒ���(������ʱ����Ч) */
	int32			ItemCount;			/* ������Ʒ���� */
	ut_id_script		ScriptId;			/* �ű�id*/
	int16			nExt;				/* ����nExt, ������չ����ȷ�Ľ�Ǯ�ļ�¼*/
	int32			WorldID;			/* ������ID */

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
	SNID_t		CharSNID;	/* �����ɫ,Skill������ */
	eSkillActType	OPType;		/* �������� */
	ut_id_map		MapID;		/* ������� */
	float		XPos;		/* �¼�����λ�� */
	float		ZPos;
	ut_id_spell	SkillID;	/* ���ܺ� */
	int32		Level;		/* ���� */
	int32		Count;		/* ���� */
	int32		WorldID;	/* ������ID */

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
	SNID_t		CharSNID;	/* �����ɫ,Skill������ */
	eAbilityActType OPType;		/* �������� */
	ut_id_map		MapID;		/* ������� */
	float		XPos;		/* �¼�����λ�� */
	float		ZPos;
	ut_id_spell	AbilityID;	/* ���ܺ� */
	int32		Level;		/* ���� */
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
	SNID_t			CharSNID;			/* �����ɫ */
	char			Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	ut_id_map			MapID;	/* ������� */
	int32			GameID;
	uint8			HaveTeam;
	uint8			member_cnt;
	int32			WorldID;		/* ������ID */
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
		CharSNID = INVALID_KHAN_ID;		/* �����ɫ,Skill������ */
		MapID = INVALID_KHAN_ID;		/* ������� */
		GameID = INVALID_KHAN_ID;
		HaveTeam = 0;
		member_cnt = 0;
		WorldID = 0;
		for(int32 _Idx = 0; _Idx < TEAM_MAX_MEMBER_COUNT; ++_Idx) member[_Idx].clear();
	}
};

struct EnterMapLogRecordInfo
{
	SNID_t	CharSNID;			/* �����ɫ */
	char	Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	ut_id_map MapID;	/* ������� */
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
		CharSNID = INVALID_KHAN_ID;	/* �����ɫ,Skill������ */
		MapID = INVALID_KHAN_ID;	/* ������� */
		GameID = INVALID_KHAN_ID;
		DayCount = 0;
		WorldID = 0;
	}
};

struct GoldCardLogRecordInfo
{
	SNID_t	snidChar;		/* ���SNID */
	char	Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	ut_id_map mapId;		/* ����ID */
	int32	iChangeType;	/* ������� */
	int32	iCostTypeId;	/* ��Ǯ���� */
	uint32	uPayment;	/* ��Ǯ������� */
	uint32	uItemTypeID;	/* �����ƷID */
	int32	iItemCount;	/* �����Ʒ���� */
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
	SNID_t			OwnerSNID;	/* ������SNID */
	SNID_t			BuyerSNID;	/* ������SNID */
	eWorldShopCashActType	OPType;		/* �������� */
	int32			ShopId;		/* �̵�ID */
	uint32			SerialId;	/* �̵��е�λ��,-1��ʾ����Ҫλ�� */
	int32			SellGold;	/* ����/������ȡ�ֽ��� */
	int32			GoldNum;	/* ������ȡʱ��¼���ֽ���� */
	int32			Price;		/* �ۼ� */
	int32			SillerNum;	/* ������ȡʱ��¼���������� */
	int32			Charge;		/* �̷ּ� */
	uint32			CreateTime;	/* ����ʱ�� */
	uint32			DeleteTime;	/* ɾ��ʱ�� */
	ut_id_map			MapID;		/* ����ID */
	int32			WorldID;	/* ������ID */

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
	SNID_t			OwnerSNID;	/* ������SNID */
	SNID_t			BuyerSNID;	/* ������SNID */
	eWorldShopCashActType	OPType;		/* �������� */
	int32			ShopId;		/* �̵�ID */
	uint32			SerialId;	/* �̵��е�λ��,-1��ʾ����Ҫλ�� */
	int32			SellGold;	/* ����/������ȡ�ֽ��� */
	int32			GoldNum;	/* ������ȡʱ��¼���ֽ���� */
	int32			Price;		/* �ۼ� */
	int32			SillerNum;	/* ������ȡʱ��¼���������� */
	int32			Charge;		/* �̷ּ� */
	uint32			CreateTime;	/* ����ʱ�� */
	uint32			DeleteTime;	/* ɾ��ʱ�� */

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
	char		Account[MAX_ACCOUNT + 1];		/* �ʺ� */
	char		CharName[MAX_KHAN_CHARACTER_NAME + 1];	/* ��ɫ���� */
	SNID_t		OwnerSNID;	/* ������SNID */
	eCheatActType	OPType;		/* �������� */
	ut_id_map		MapID;		/* ����ID */
	uint32		Offset;		/* �����ƫ���� */
	float		PosX;		/* ��ɫ��ǰX���� */
	float		PosZ;		/* ��ɫ��ǰZ���� */
	int32		WorldID;	/* ������ID */

	/*
	 * �ƶ�ʱ�������X+Z������������������ʱ��ObiId ;
	 * ��½��֤�� 0:��������ģʽ��1���������ģʽ��2��ʱ�����ģʽ
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
	SNID_t		CharSNID;			/* �����ɫ,��Ʒ������ */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	SItemSnid	ItemSnid;			/* ��Ʒ��� */
	uint32		ItemType;			/* ��Ʒȫ�ֱ�� */
	uint32		OpType;			/* �������� */
	uint32		Count;			/* ���� */
	uint32		GoldCash;		/* �ֽ��� */
	uint32		TermTime;		/* ��Ч�� */
	ut_id_map		MapID;			/* ����ID */
	int32		WorldID;		/* ������ID */

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
	SNID_t		CharSNID;			/* �����ɫ,��Ʒ������ */
	char		Name[MAX_KHAN_CHARACTER_NAME];	/* ��ɫ���� */
	SItemSnid	ItemSnid;			/* ��Ʒ��� */
	uint32		ItemType;			/* ��Ʒȫ�ֱ�� */
	uint32		OpType;			/* �������� */
	uint32		Count;			/* ���� */
	uint32		GoldCash;		/* �ֽ��� */
	ut_id_map		MapID;			/* ����ID */
	int32		WorldID;		/* ������ID */

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
	SNID_t			OwnerSNID;	/* ������SNID */
	SNID_t			OtherSNID;	/* ������һ��SNID */
	eStockActType	OPType;		/* �������� */
	uchar			TradeType;	/* �������� */
	int32			OwnerPrice;	/* �����߳��ۣ����� */
	int32			OtherPrice;	/* ������һ�����ۣ����� */
	int32			Gold;		/* ���׵��ֽ����� */
	uint32			CreateTime;	/* ����ʱ�� */
	uint32			DeleteTime;	/* ɾ��ʱ�� */

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

//�˻���¼LOG
struct MapStockAccLogRecordInfo
{
	SNID_t			OwnerSNID;	/* ������SNID */
	eStockAccActType OPType;	/* �������� */
	int32			TransGold;	/* ת�˵��ֽ� */
	int32			TransSiller;/* ת�˵����� */
	int32			StockGold;	/* �˻���� �ֽ� */
	int32			StockSiller;/* �˻���� ���� */
	ut_id_map			MapID;		/* ����ID */
	int32			WorldID;	/* ������ID */

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

//�˻���¼LOG
struct GLStockAccLogRecordInfo
{
	SNID_t			OwnerSNID;	/* ������SNID */
	eStockAccActType OPType;	/* �������� */
	int32			TransGold;	/* ת�˵��ֽ� */
	int32			TransSiller;/* ת�˵����� */
	int32			StockGold;	/* �˻���� �ֽ� */
	int32			StockSiller;/* �˻���� ���� */

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

// GL�������Log
struct GLGuildLogRecordInfo
{
	ut_id_guild		m_GuildId;								//���ID
	char			m_szGuildName[MAX_KHAN_GUILD_NAME_SIZE];		//�����
	eGuildActType	m_OpType;								//��������
	int32			m_nCount;								//�仯ֵ
	int32			m_nRemainCount;							//�仯���ֵ
	int32			m_nParam;								//������صĲ���
	int32			m_nScriptId;							//�ű�Id


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
	SNID_t		CharSNID;	/* �����ɫ */
	eZhuanJingActType	OPType;		/* �������� */
	ut_id_map		MapID;		/* ������� */
	float		XPos;		/* �¼�����λ�� */
	float		ZPos;
	int32		TotalPoint;
	int32		Profession[PROFESSION_NUMBER];
	int32		Reserve_param0;
	int32		Reserve_param1;
	int32		WorldID;	/* ������ID */


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
	SNID_t			m_CharGUID;		/* �����ɫ */
	ut_id_map		m_MapID;		/* ������� */
	float			m_XPos;			/* �¼�����λ�� */
	float			m_ZPos;
	int32			m_Count;		/* �����ͷŵ����� */
	int32			m_WorldID;		/* ������ID */
	int32			m_SkillID;		/* ʹ�õļ�����ID */
	int32			m_Profession;	/* ��ҵ�ְҵ */

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

// ��ҵ�½�ǳ�db��־ [wangyiran]
struct PlayerLoginOutLogRecordInfo
{
	SNID_t	m_nCharSNID; // ��ɫId [wangyiran]
	char	m_Name[MAX_KHAN_CHARACTER_NAME+1];// ���� [wangyiran]
	char	m_Account[MAX_KHAN_CHARACTER_NAME+1];	// �˺� [wangyiran]
	char	m_Mac[MAX_KHAN_CHARACTER_NAME+1];	// mac [wangyiran]
	char	m_Idfa[MAX_DEVICE_IDFA_LENGTH];	// idfa [wangyiran]
	char	m_Device[MAX_KHAN_CHARACTER_NAME+1];	// Device [wangyiran]
	int32	m_DeviceOS;	// DeviceOS [wangyiran]
	int32	m_nWorldID;// ������id [wangyiran]
	uint32	m_nLoginTime;// ��½ʱ�� [wangyiran]
	uint32	m_nLogoutTime; // �ǳ�ʱ�� [wangyiran]
	int32	m_nOnlineTime;// ����ʱ�� [wangyiran]
	int32	m_nType;// 0 ��½ 1�ǳ� [wangyiran]
	int64	m_nPlatformUid;// ƽ̨�˺�id [wangyiran]
	int32	m_nChannel;// ���� [wangyiran]

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

// billing���͵Ķ�����ֵ��־[wangxiaoyong]
struct BillingOrderInfo
{
	char	m_OrderId[MAX_ORDER_ID_SIZE];		// ������ [wangxiaoyong]
	SNID_t	m_nUserId;							// ���SNID [wangxiaoyong]
	char	m_ProductId[MAX_PRODUCT_ID_SIZE];	// ��ƷID [wangxiaoyong]
	double	m_nPayMoney;						// ֧����� [wangxiaoyong]
	int32	m_nBindPoints;						// �󶨷�ʯ���� [wangxiaoyong]
	int32	m_nPoints;							// ��ֵ��ʯ���� [wangxiaoyong]
	int32	m_nAdditionalPoints;				// ��ֵ���ͷ�ʯ���� [wangxiaoyong]
	char	m_PayTime[MAX_TD_ITEM_SIZE];		// ֧��ʱ�� [wangxiaoyong]
	int32	m_nChannelId;						// ����ID [wangxiaoyong]

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

// Ӣ��������־[wangxiaoyong]
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


// ����������־[tianshuai]
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


//  [4/22/2016 tianshuai] Ѱ����־
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

//  [4/22/2016 tianshuai] Ӣ��ת����־
struct HeroTransferLogInfo
{
	SNID_t	m_nPlayerId;
	char	m_PlayerName[MAX_KHAN_CHARACTER_NAME + 1];
	int32 m_nFromHeroTabIndex;   //��ת��Ӣ��
	int32 m_nFParam1;//ת��ǰ�ȼ�
	int32 m_nFParam2;//ת��ǰ����(ͻ��)
	int32 m_nFParam3;//ת��ǰ����(����)
	int32 m_nFParam4;//ת��ǰ����
	int32 m_nFParam5;//ת��ǰ����
	int32 m_nFParam6;//ת��ǰ�����ظ�
	int32 m_nFParam7;//ת��ǰ�����ظ�
	int32 m_nFParam8;//ת�ƺ�ȼ�
	int32 m_nFParam9;//ת�ƺ�����
	int32 m_nFParam10;//ת�ƺ����
	int32 m_nFParam11;//ת�ƺ󹥻�
	int32 m_nFParam12;//ת�ƺ�����
	int32 m_nFParam13;//ת�ƺ������ظ�
	int32 m_nFParam14;//ת�ƺ������ظ�

	int32 m_nToHeroTabIndex;//����ת��Ӣ��
	int32 m_nTParam1;//ת��ǰ�ȼ�
	int32 m_nTParam2;//ת��ǰ����
	int32 m_nTParam3;//ת��ǰ����
	int32 m_nTParam4;//ת��ǰ����
	int32 m_nTParam5;//ת��ǰ����
	int32 m_nTParam6;//ת��ǰ�����ظ�
	int32 m_nTParam7;//ת��ǰ�����ظ�
	int32 m_nTParam8;//ת�ƺ�ȼ�
	int32 m_nTParam9;//ת�ƺ�����
	int32 m_nTParam10;//ת�ƺ����
	int32 m_nTParam11;//ת�ƺ󹥻�
	int32 m_nTParam12;//ת�ƺ�����
	int32 m_nTParam13;//ת�ƺ������ظ�
	int32 m_nTParam14;//ת�ƺ������ظ�

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


// ����������־[tianshuai]
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

// ����Ч����־[tianshuai]
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
