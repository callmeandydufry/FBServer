/*$T MapServer/FileDef.h GC 1.140 10/10/07 11:01:49 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __FILEDEF_H__
#define __FILEDEF_H__

/************************************************************************/
/* 场景目录声明                                                         */
/************************************************************************/
#define PATH_MAP							"./map/"

/************************************************************************/
/* 服务器配置相关                                                       */
/************************************************************************/
#define FILE_PLATFORMSERVERCONFIG			"./Config/PlatformServerConfig.ini"
#define FILE_BASESERVERCONFIG				"./Config/BaseServerConfig.ini"
#define FILE_EXTSERVERCONFIG				"./Config/ExtServerConfig.ini"
#define FILE_MULTIGROUPCONFIG				"./Config/MultiGroupConfig.ini"
#define FILE_DBSHARECONFIG					"./Config/DBShareConfig.ini"
#define FILE_GAMECONFIG						"./Config/GameConfig.ini"
#define FILE_DBCONFIG 						"./Config/DBConfig.ini"
#define FILE_SERVERINDEX					"./Config/ServerIndex.ini"
#define FILE_WORLDINDEX						"./Config/WorldIndex.ini"
#define FILE_GAMETOGGLE						"./Config/GameToggleConfig.ini"
#define FILE_IPSET							"./Config/IpSet.ini"
#define FILE_MERGE_SERVER_CONFIG			"../mergeserver/MergeServerConfig.ini"
#define FILE_MODULE_CONFIG					"./Config/ModuleConfig.ini"
#define FILE_ROBOT_CONFIG					"./Config/RobotConfig.ini"


/************************************************************************/
/* 服务器玩法配置                                                       */
/************************************************************************/
#define FILE_IPRANGE						"./Config/IPAuth.ini"
#define FILE_BASE_VALUE						"./Config/ProfessionBaseProperty.ini"
#define FILE_KHAN_ITEM_SERIAL					"./Config/ItemSN.ini"
#define FILE_MONITOR_INFO					"./Config/MonitorInfo.ini"
#define FILE_STOCK_MONITOR_INFO				"./Config/StockMonitorInfo.ini"

/************************************************************************/
/* 服务器相关内部表格                                                   */
/************************************************************************/
#define FILE_MAP_INFO					"./Config/mapdefines.tab"				// 默认的MapServer对应的配置表
#define FILE_MAP_INFO_6					"./Config/mapdefines_6.tab"				// 6 MapServer对应的配置表
#define FILE_MAP_INFO_10				"./Config/mapdefines_10.tab"			// 10 MapServer对应的配置表
#define FILE_GMLIST						"./Config/GMConfig.tab"

#define FILE_KHAN_ATTR_LEVEL_UP					"./Config/attr_lvup.tab"
#define FILE_MONSTER_DROP_GROUP				"./Config/monsterdrop_group.tab"
#define FILE_MONSTER_DROP_CONTENT			"./Config/monsterdrop_content.tab"
#define FILE_KHAN_DROP_ATTENUATION				"./Config/drop_fadeout.tab"
#define FILE_MONSTER_SKILL					"./Config/mon_wqskl.tab"
#define FILE_DROP_POS						"./Config/dorp_pos.tab"

#define FILE_STRING_FILTER_TABLE_NAME		"./Config/str_filter.tab"
#define FILE_TIMER_DOING_LIST				"./Config/Timer.tab"
#define FILE_MAP_COMMON					"./Config/map_common.tab"
#define FILE_ENCRYPT_INFO					"./Config/encrypt.tab"
#define FILE_LOGIN_ANSWERS_INFO				"./Config/LoginAnswers.tab"
#define FILE_MAP_ANSWERS_INFO				"./Config/SceneAnswers.tab"
#define FILE_CHEAT_ACT_DATA_TABLE_NAME		"./Config/CheatAct.tab"
#define FILE_WORLD_STOCK_INFO				"./Config/WorldStock.tab"
#define FILE_OPENBOX_ITEM					"./Config/OpenBoxItem.tab"
#define FILE_ONDIE_MONSTER					"./Config/OnDie_Monster.tab"
#define FILE_ONDIE_NOPUNISH					"./Config/OnDie_NoPunish.tab"
#define FILE_ONDIE_DIFFCOUNTRY				"./Config/OnDie_DiffCountry.tab"
#define FILE_ONDIE_SAMECOUNTRY				"./Config/OnDie_SameCountry.tab"
#define FILE_ONDIE_SAMECOUNTRY_KILLER		"./Config/OnDie_Killer.tab"
#define FILE_MENTAL_GAME_ANSWERS			"./Config/answers.tab"
#define FILE_RANDOM_MONSTER					"./Config/random_monster.tab"			//2010-11-26 by rj 随机怪配置表
#define FILE_TOPLIST_NOTICE					"./Config/TopListNotice.tab"			// 2010-3-27 by luoqian 排名通知表
//#define FILE_CHARIOT_EQUIP_BLUEATTR_BUFF	"./Config/ChariotEquip_BlueAttr_Effect.tab"	// by wy	 战车装备蓝属性BUFF表
#define FILE_POKER_ATTR_RANDOM_TABLE		"./Config/Poker_Attr_Random.tab"			//扑克牌属性随机生成表
//#define FILE_HORSEEQUIP_BLUE_SET			"./Config/horse_equip_blue_attr_set.tab"	// 坐骑装备蓝属性表
//#define	FILE_HORSEEQUIP_BLUE_LEVEL			"./Config/horse_equip_blue_attr_level.tab"	// 坐骑装备蓝属性值
//#define FILE_HORSEEQUIP_POINT_INFO			"./Config/Horse_EquipPoint_Value.tab"		// 装备点权重
#define FILE_BAK_SCENEINFO					"./Config/BakSceneInfo.tab"							//备份场景表
#define FILE_BATTLEEFFECT_TAB				"./Config/battle effect.tab"						//登陆礼包表
#define FILE_DYNAMICBUFF_REG				"./Config/DynamicBuffReg.tab"			// 动态 Buff 配置注册表
#define FILE_MINE_RESULT					"./Config/MineResult.tab"					// 家园矿山结果
#define FILE_CHANNELMERIDIAN_TAB			"./Config/ChannelMeridian.tab"				//经脉
#define FILE_PET_ATTR_MAX_TB				"./Config/PetAttrMax.tab"					// 宠物属性最大值表
//#define FILE_SERVER_OPENTIME			"./Config/ServerOpenTime.tab"				// 开服时间配置 -2013.02.20作废，移到worldname.tab
#define FILE_MAPLOADMANAGER_TAB         "./Config/MapLoadManager.tab"
#define FILE_HEROATTRGROW				"./Config/HeroGrow.tab"						//英雄属性养成
#define FILE_ARENATOPSELECT				"./Config/ArenaTopSelect.tab"				//竞技场选择玩家
#define FILE_ARENARANKUPAWARD			"./Config/ArenaRankUpAward.tab"				//竞技场提升奖励
#define FILE_PEAKARENARANKAWARD			"./Config/PeakArenaRankAward.tab"				//巅峰竞技场提升奖励
#define FILE_PlUNDER_AWARD				"./Config/PlunderAward.tab"				//竞技场选择玩家
#define FILE_PLATFORM_SHOP_PRODUCT_ID		"./Config/PlatformShopProductId.tab"		// 三方平台的商店价格表id [yangzhen]
#define FILE_PLATFORM_SHOP_CHANNEL_INDEX	"./Config/PlatformShopChannelIndex.tab"		// 三方平台的渠道id与商店价格表中列的对应 [yangzhen]

#define FILE_WECHATREDITEMPACK_TAB		"./Config/WeChatRedItemPack.tab" // 红包活动中的道具奖励表 [yangzhen]
#define FILE_FAKEPLAYERNAME_TAB			"./Config/FakePlayerName.tab"		// 假玩家角色名表 [6/22/2015 yangzhen]
#define FILE_ACTIVITYSCRIPTCFG_TAB		"./Config/ActivityScriptCfg.tab" // 活动脚本配置表 [yangzhen]

/************************************************************************/
/* 客户端服务器端共用内部配置                                           */
/************************************************************************/
#define FILE_MONSTER_BASE_BEHAVIOR			"../Share/Config/monster_base_behavior.ini"
#define FILE_VERSIONCONFIG					"../Share/Config/VersionConfig.ini"

/************************************************************************/
/* 客户端服务器端共用内部表格                                           */
/************************************************************************/
#define FILE_CITY_BUILDING						"../Share/Config/CityBuilding.tab" //英雄表
#define FILE_HEROCHAR						"../Share/Config/Hero.tab" //英雄表
#define FILE_HERO_LEVELUP					"../Share/Config/HeroLvExp.tab"	//英雄升级表
#define FILE_HERO_STAR_GCONFIG				"../Share/Config/heroStar.tab" //英雄升级表
#define FILE_HERO_DEVELOPCONFIG				"../Share/Config/heroPeiYang.tab" //英雄培养表
#define FILE_HERO_AWAKEN_CONFIG				"../Share/Config/heroAwaken.tab" //英雄培养表
#define FILE_HERO_EVOLUTIONCONFIG			"../Share/Config/heroEvolution.tab" // 英雄进化 [1/6/2016 superrobert]
#define FILE_HERO_BREAKCONFIG				"../Share/Config/heroBreak.tab" // 英雄英雄升星 [1/6/2016 superrobert]
#define FILE_HERO_BREAKUPCONFIG				"../Share/Config/heroBreakUp.tab" // 英雄英雄升星升级 [hujian] --无用
#define FILE_FRIENDARMY_GEMCONFIG			"../Share/Config/friendArmyGem.tab" // 友情宝石 [hujian]
#define FILE_FRIENDARMY_GEMLEVELCONFIG		"../Share/Config/friendArmyGemLvlUp.tab" // 友情宝石等级 [hujian]
#define FILE_ARTIFACT_PROPCONFIG			"../Share/Config/ArtifactProp.tab" // 神器基础属性 [tianshuai]
#define FILE_ARTIFACT_BUYANDDESTROYCONFIG	"../Share/Config/ArtifactBuyAndDestroy.tab" // 神器购买和摧毁 [tianshuai]
#define FILE_ARTIFACT_SKILLCONFIG			"../Share/Config/ArtifactSkill.tab" // 神器技能升级 [tianshuai]
#define FILE_ARTIFACT_SKILLBASECONFIG		"../Share/Config/ArtifactSkillBase.tab" // 神器技能基本属性 [tianshuai]
#define FILE_ARTIFACT_UPDATECONFIG			"../Share/Config/ArtifactUpdate.tab" // 神器升级 [tianshuai]
#define FILE_STANDARD_EFFECT_DATA			"../Share/Config/buff_logic.tab"
#define FILE_EFFECT_DIR						"../Share/Config/buff_effect.tab"
#define FILE_KHAN_PLAYER_EXP_LEVEL				"../Share/Config/pl_explv.tab"
#define FILE_PLAYER_EXP_INHERENCE			"../Share/Config/pl_exp_inherence.tab"
#define FILE_INHERENCE_LEVEL_AVERAGE		"../Share/Config/inherence_level_average.tab"
#define FILE_KHAN_MONSTER_ATTREX					"../Share/Config/mon_attr_ex.tab"
#define FILE_BUS_ATTREX						"../Share/Config/bus_attr_ex.tab"
#define FILE_HORSE_JICHU					"../Share/Config/horse_jichu.tab"
#define FILE_HORSE_ITEM						"../Share/Config/horse_daoju.tab"
#define FILE_EQUIPSTRONGER					"../Share/Config/nwEquip_Stronger.tab"
#define FILE_EQUIP_EVOLUTION				"../Share/Config/nwEquip_Evolution.tab"
#define FILE_EQUIPEXTRACTION				"../Share/Config/nwEquip_Extraction.tab"
#define FILE_EQUIPEXTRACTIONMASTER			"../Share/Config/nwEquip_ExtractionMaster.tab"
#define FILE_EQUIPSUIT      				"../Share/Config/nwEquip_Suit.tab"

#define FILE_HORSE_ZIZHI					"../Share/Config/horse_zizhi.tab"
#define FILE_HORSE_MIX						"../Share/Config/horse_mix.tab"
#define FILE_HORSE_MIX_SPELL				"../Share/Config/horse_mix_spell.tab"
#define FILE_GROWPOINT						"../Share/Config/grow_point.tab"
#define FILE_GAMEOPEN						"../Server/Config/game_open.tab"
#define FILE_KHAN_SKILL_TEMPLATE					"../Share/Config/spell_logic.tab"
#define FILE_SKILL_STUDY					"../Share/Config/spell_learn.tab"
#define FILE_KHAN_SPECIAL_OBJ_DATA				"../Share/Config/sp_obj.tab"
#define FILE_EFFECT_SEDATA					"../Share/Config/buff_describe.tab"
#define FILE_KHAN_CAMP_AND_STAND					"../Share/Config/camp_stand.tab"
#define FILE_ABILITY						"../Share/Config/skill.tab"
#define FILE_HAIRSTYLE						"../Share/Config/char_hair.tab"
#define FILE_KHAN_PRESCRIPTION					"../Share/Config/item_mix.tab"
#define FILE_KHAN_ABILITY_EXP					"../Share/Config/skill_exp.tab"
#define FILE_BEASTIE_ATTR					"../Share/Config/beastie_attr.tab"
#define FILE_VIGOR_DATA						"../Share/Config/pl_energy.tab"
#define FILE_EQUIP_BASE						"../Share/Config/nwEquip.tab"
#define FILE_SUITEQUIP_BLUEATTR				"../Share/Config/Equip_Suit_Gem.tab"
#define FILE_EUIPHOLE_INFO					"../Share/Config/Equip_HoleReqInfo.tab"
#define FILE_EUIPLEVEUP_INFO				"../Share/Config/Equip_levelup.tab"
#define FILE_EUIPBIND_INFO					"../Share/Config/Equip_bind.tab"
#define FILE_SPECIALREPAIR_INFO				"../Share/Config/Equip_SpecialRepair.tab"
#define FILE_EQUIP_PRODUCT_INFO				"../Share/Config/equip_product_info.tab"
#define FILE_EXTRABAG_INFO					"../Share/Config/Extra_Bag.tab"
#define FILE_CHAIN_INFO						"../Share/Config/chain_info.tab"
#define FILE_SOUL_INFO						"../Share/Config/soul_info.tab"
#define FILE_CHANGEPAI_INFO					"../Share/Config/ChangePai_info.tab"
#define FILE_PAI_INFO						"../Share/Config/pai_info.tab"
#define FILE_MISCELL_ITEM					"../Share/Config/item_miscellaneous.tab"
#define FILE_QUEST_ITEM						"../Share/Config/item _quest.tab"
#define FILE_ITEM_RULE						"../Share/Config/item_formula.tab"
#define FILE_KHAN_EQUIP_SET_ATTR					"../Share/Config/equipset_attr.tab"

#define FILE_KHAN_PLAYER_LEVEL_MONEY				"../Share/Config/lv_maxmoney.tab"
#define FILE_KHAN_CHAT_CONSUME					"../Share/Config/chat_cfg.tab"
#define FILE_EFFECT_MARRY					"../Share/Config/MarryEffect.tab"
#define FILE_EFFECT_FRIEND					"../Share/Config/FriendEffect.tab"
#define FILE_NPC_QUEST						"../Share/Config/obj_quest.tab"
#define FILE_MAPMAP_NPC					"../Share/Config/scenemap_npc.tab"
#define FILE_ONRELIVE_COST					"../Share/Config/brith_cost.tab"
#define FILE_DEATH_DEAL						"../Share/Config/death_deal.tab"
#define FILE_MENTAL_GAME_QUESTIONS			"../Share/Config/questions.tab"
#define FILE_DAY_TIPS						"../Share/Config/DayTips.tab"
#define FILE_TOP_LIST						"../Share/Config/TopList.tab"
#define FILE_MAP_INFO_LIST					"../Share/Config/WorldPos.tab"
#define FILE_GUILD_LEVEL					"../Share/Config/guild_level_info.tab"
#define FILE_GUILD_MERIT					"../Share/Config/guild_merit_info.tab"
#define FILE_SCRIPTFILE						"../Share/Config/ScriptFile.tab"
#define FILE_PATH_GUILD_AUTH_RULER			"../Share/Config/auth_ruler_guild.tab"
#define FILE_PATH_COUNTRY_AUTH_RULER		"../Share/Config/auth_ruler_country.tab"
#define FILE_PATH_EMPIRE_AUTH_RULER			"../Share/Config/auth_ruler_empire.tab"
#define FILE_AWARD_INFO						"../Share/Config/trophy.tab"
#define FILE_WORLD_SHOP_INFO				"../Share/Config/WorldShop.tab"
#define FILE_EQUIP_ACTIVATION_TB			"../Share/Config/equip_activation.tab"
#define FILE_EQUIP_COLORCHANGE_TB			"../Share/Config/Equip_ColorChange.tab"
#define FILE_EQUIP_PURPLEREWORK_TB			"../Share/Config/Equip_PurpleRework.tab"
#define FILE_GEM_REWORK_TB					"../Share/Config/GemRework.tab"
#define FILE_EQUIP_COPY_TB					"../Share/Config/EquipCopy.tab"
#define FILE_EQUIP_RECALL_TB				"../Share/Config/EquipRecall.tab"
#define FILE_TRANSITEM_POS					"../Share/Config/TransItemPos.tab"
#define FILE_EQUIP_WANFA_ENERGYBALL			"../Share/Config/EnergyBall.tab"					//蓝属性吸收玩法中能量球的配置
#define FILE_EQUIP_WANFA_AMULET				"../Share/Config/AmuletInfo.tab"					//蓝属性吸收玩法中护身符的配置
#define FILE_EQUIP_WANFA_ENERGYBALLSYN		"../Share/Config/EnergyBallSyn.tab"					//蓝属性吸收玩法中护身符补色的配置
#define FILE_EQUIP_PRODUCE					"../Share/Config/EquipProduce.tab"					//打造装备配置
#define FILE_EQUIP_SWITCH					"../Share/Config/EquipSwitch.tab"					//装备转化
#define FILE_CHANGE_PROFESSION_TABLE_NAME   "../Share/Config/ChangeProfession.tab"				//职业转换表
#define FILE_CHANGE_APPEARANCE_TABLE_NAME   "../Share/Config/change_appearance.tab"				//更换形象表
#define FILE_GUILD_SKILL_INFO				"../Share/Config/gift_learn.tab"					//帮会技能表
#define FILE_EQUIP_PRODUCE_NEW				"../Share/Config/Equip_Produce_New.tab"				//橙装蓝属性打造的配置
#define FILE_GIFT_BALANCE_INFO_1			"../Share/Config/gift_balance_learn_1.tab"			//天赋平衡 控制技能除外
#define FILE_GIFT_BALANCE_INFO_2			"../Share/Config/gift_balance_learn_2.tab"			//天赋平衡 控制技能
#define FILE_EQUIP_CONST_INFO				"../Share/Config/Equip_Const_Info.tab"		        //星座信息
#define FILE_EQUIP_CONST_ACTIVE_POINT		"../Share/Config/Equip_Const_Active_Point.tab"		//星座部件激活信息
#define FILE_EQUIP_CONST_BIND				"../Share/Config/Equip_Const_Bind.tab"		        //星座铭记信息
#define FILE_EQUIP_HONOR_BIND				"../Share/Config/Equip_Honor_Bind.tab"		        //荣誉铭记信息
#define FILE_ITEM_SPECIAL					"../Share/Config/Item_Special.tab"					//存放一些玩法的用到的特珠道具
#define FILE_GIFT_ZHUANGONG					"../Share/Config/gift_zhuangong.tab"				//天赋专精 专攻
#define FILE_GIFT_ZHUANFANG					"../Share/Config/gift_zhuanfang.tab"				//天赋专精 专防
#define FILE_GUILD_BUS_SKILL_STUDY_CONSUME	"../Share/Config/Bus_Sill_Study_Consume.tab"		//帮会BUS技能学习消耗
#define FILE_MILITARYRANK					"../Share/Config/MilitaryRank.tab"					//军阶配置信息
#define FILE_ROSEBAND						"../Share/Config/Equip_RoseBand.tab"				//装备玫瑰星镶边配置表
#define FILE_EQUIP_HELL_PRODUCE				"../Share/Config/Equip_hell_produce.tab"			//冥装打造表
#define FILE_EQUIP_HELL_LEVELUP				"../Share/Config/Hell_Equip_Levelup.tab"			//冥装升级表
#define FILE_EQUIP_HELL_LEVELUP_ATTR		"../Share/Config/Hell_Equip_Levelup_Attr.tab"		//冥装升级属性表
#define FILE_EQUIP_METEORITE_STONE			"../Share/Config/Equip_hell_meteorite_stone.tab"	//陨星石配置表
#define FILE_EQUIP_HELL_LEVEL				"../Share/Config/Equip_hell_level.tab"				//冥甲术等级配置表
#define FILE_HELL_XUANTIE_EFFECT			"../Share/Config/Equip_hell_xuantie_effect.tab"		//玄铁对属性分值影响表
#define FILE_HELL_ATTACH_BLUE				"../Share/Config/Equip_hell_attachblue.tab"			//冥装附蓝表
#define FILE_HELL_BIND_BLUE_STAR			"../Share/Config/Equip_hell_bindbluestar.tab"		//定蓝星配置表
#define FILE_HELL_LANWAN					"../Share/Config/Equip_hell_lanwan.tab"				//蓝丸物品表
#define FILE_HELL_LANWAN_PROPERTY			"../Share/Config/Equip_hell_lanwan_property.tab"	//蓝丸强化表
#define FILE_EQUIP_DISMANTLE_ITEM			"../Share/Config/Equip_dismantle_item.tab"			//装备拆解道具表
#define FILE_HELL_SUBSKILL					"../Share/Config/Equip_hell_subskill.tab"			//冥装配方表
#define FILE_CHARIOT_BASEATTR				"../Share/Config/Chariot_Base_Attr.tab"				//战车基本属性表
#define FILE_CHARIOT_EQUIPATTR				"../Share/Config/Chariot_Equip_Attr.tab"			//战车装备属性表
#define FILE_COUNTRY_MANOR					"../Share/Config/CountryManor.tab"
#define FILE_CHARIOT_EQUIPBUILD				"../Share/Config/Chariot_Equip_Build.tab"			//战车装备打造表
#define FILE_CHARIOT_RANDOMATTR				"../Share/Config/Chariot_Random_Attr.tab"			//战车装备随机属性表
#define FILE_CHARIOT_RANDOMITEM				"../Share/Config/Chariot_Equip_RandItem.tab"		//战车装备随机装备表
#define FILE_CHARIOT_BODYBUILD				"../Share/Config/Chariot_Body_Build.tab"			//战车车身打造表
#define FILE_FADE_ITEM                      "../Share/Config/fakeitem.tab"                      // 禁用或未开放物品、装备及宝石等列表
#define FILE_FADE_HORSE                     "../Share/Config/fakehorse.tab"                     // 禁用或未开放骑乘列表
#define FILE_CHARIOT_EQUIP_RECYCLE			"../Share/Config/Chariot_Equip_Recycle.tab"			//战车装备回收表
#define FILE_CHARIOT_COEFFICIENT			"../Share/Config/chariotcoefficient.tab"			// 人物属性对战车属性的影响系数
#define FILE_POKER_ACTIVATION				"../Share/Config/Poker_Activation.tab"				//扑克牌激活表
#define FILE_GEM_REWORK_WEIGHT				"../Share/Config/GemReworkWeight.tab"				//宝石回炉权重表
#define FILE_GEM_REWORK_COST				"../Share/Config/GemReworkCost.tab"					//宝石回炉消耗表
#define FILE_ROOM_CONFIG					"../Share/Config/Room_Config.tab"					//房间配置表
#define FILE_ROOM_LOGIC						"../Share/Config/Room_Logic.tab"					//跨服关卡逻辑表

#define FILE_GEM_RAND_ATTR					"../Share/Config/Gem_Attr_Rand.tab"					// 宝石洗紫随机属性表
#define FILE_RAND_ATTR_GEMLIST				"../Share/Config/Gem_RandAttr_GemList.tab"			// 可洗紫宝石列表
#define FILE_GEM_RAND_ATTR_COST				"../Share/Config/Gem_RandAttr_Cost.tab"				// 宝石洗紫花费
#define FILE_GEM_CHANGE_TB				"../Share/Config/GemChange.tab"							// 宝石转换
#define FILE_HORSE_EQUIP_BASE				"../Share/Config/horse_equip_base.tab"				// 坐骑装备表
#define FILE_GUANQIA_SKILL_INFO				"../Share/Config/guanqiamaps.tab"					// 关卡技能表
#define FILE_HOMEMONSTER_FOOD				"../Share/Config/HomeMonsterFood.tab"		//家园守护兽食物

#define FILE_PLANT_RECLAIM					"../Share/Config/PlantReclaim.tab"					// 家园种植开垦
#define FILE_PLANT_LEVELUP					"../Share/Config/PlantLevelUp.tab"					// 家园种植升级
#define FILE_PLANT_RESULT					"../Share/Config/PlantResult.tab"					// 家园收获结果
#define FILE_MINE_RECLAIM					"../Share/Config/MineReclaim.tab"					// 家园矿山开垦
#define FILE_GARDEN_LEVELUP					"../Share/Config/GardenLevelUp.tab"					// 家园升级表
#define FILE_HOEMSHOP_TAB					"../Share/Config/HomeShop.tab"						// 家园商店
#define FILE_HOMESHOPBACK_TAB				"../Share/Config/HomeShopBack.tab"					// 家园物品回收
#define FILE_GARDENTREE_TAB					"../Share/Config/GardenTree.tab"					// 家园树升级
#define FILE_GARDEFRUIT_TAB					"../Share/Config/GardenFruit.tab"					// 家园果实升级
#define FILE_GARDEN_APPLY_TAB				"../Share/Config/GardenApply.tab"					// 家园供给


#define FILE_CARD_MIX_TABLE					"../Share/Config/CardMix.tab"						// 卡牌合成表
#define FILE_CARD_LEVELUP_TABLE				"../Share/Config/CardLevelup.tab"					// 卡牌升级表
#define FILE_MINE_CARDINFO					"../Share/Config/CardInfo.tab"						// 卡牌
#define FILE_KHAN_CARDSUIT					"../Share/Config/CardSuitActive.tab"				// 卡牌激活
#define FILE_GIFTPACK_ONLINE				"../Share/Config/online_gift.tab"					// 在线礼包配置
#define FILE_THREETOVISIT_CONSUME			"../Share/Config/ThreeToVisit_Consume.tab"			// 三顾茅庐消耗表
#define FILE_THREETOVISIT_HORTATION			"../Share/Config/ThreeToVisit_Hortation.tab"		// 三顾茅庐奖励表
#define FILE_CHANGE_SILLER_TAB				"../Share/Config/ChangeSilver.tab"					// 金换银兑换表
#define FILE_LOGINGIFT_TAB					"../Share/Config/LoginGift.tab"						//登陆礼包表
#define FILE_PET_JICHU						"../Share/Config/pet_jichu.tab"					// 宠物基础表格
#define FILE_PET_TRAIN						"../Share/Config/PetTrain.tab"						// 宠物培养表
#define FILE_CONSUMMATION_TAB				"../Share/Config/Consummation_list.tab"				//成就信息表
#define FILE_PLAYER_ATTRIBUTE_UP_TAB	    "../Share/Config/PlayerAttribute_up.tab"			//人物属性点养成表 
#define FILE_CHANNELMERIDIAN_COLOR_TAB			"../Share/Config/ChannelMeridianColor.tab"				//经脉

#define FILE_FUBENPINGFEN_RANK_TAB				"../Share/Config/FubenPingFenRank.tab"			//副本评分表格
#define FILE_FUBENPINGFEN_AWARD_TAB				"../Share/Config/FubenPingFenAward.tab"	//副本评分表格

#define FILE_MD_TO_CLIENT_TAB				"../Share/Config/MD_To_Client.tab"			//需要同步到客户端的MD表格
#define FILE_PARTY_HONOR_TAB				"../Share/Config/PartyHonor.tab"				//工会荣誉

#define FILE_EQUIP_CONSTELLATION_SEAL_ACTIVE_TB					"../Share/Config/Equip_ConstellationSealActive.tab"    //星座封印

#define FILE_EQUIP_ZODIAC_SEAL_ACTIVE_TB					"../Share/Config/Equip_ZodiacSealActive.tab"    //生肖封印
#define FILE_SLG_BUILD_INFO					"../Share/Config/slg_buildinfo.tab" // slg建筑信息
#define FILE_ARMS_INFO					"../Share/Config/ArmsInfo.tab"  //兵种相关
#define FILE_BARRACK_INFO					"../Share/Config/BarrackInfo.tab"  //兵营信息
#define FILE_FAREASTWAR_MONEDIT				"../Share/Config/FarEastWar_Mon_Edit.tab"
#define FILE_PET_HATCH_INFO				"../Share/Config/PetHatchInfo.tab" // 宠物孵化

#define FILE_GARDEN_QUEST_TB		"../Share/Config/GardenQuest.tab"		//家园内政


#define FILE_FAREASTWAR_ARMS_INFO	"../Share/Config/FarEastWarMonster.tab"		//远东争霸战怪物表

#define FILE_PET_ATTRIBUTE_UP_TAB	    "../Share/Config/PetAttribute_up.tab"			//宠物属性点养成表 
#define FILE_WORLDNAME_TAB				"../Share/Config/worldname.tab"
#define FILE_SOLDIERATTRIBUTE_TAB          "../Share/Config/SoldierAttribute.tab"		//佣兵属性表
#define FILE_SOLDIERBATTLE_TAB          "../Share/Config/SoldierBattle.tab"
#define FILE_SOLDIERBATTLEBODY_TAB      "../Share/Config/SoldierBattleBody.tab"         //佣兵实体表
#define FILE_SOLDIERBATTLESPELLGROUP_TAB  "../Share/Config/SoldierBattleSpellGroup.tab"  //佣兵技能组表

#define FILE_SOLDIER_BASEATTR_TAB       "../Share/Config/Soldier_BaseAttr.tab"          //佣兵基础属性表
#define FILE_SOLDIER_TOOLS_TAB          "../Share/Config/SoldierTools.tab"              //佣兵道具表

#define FILE_ZHANYI_GATE_TAB			"../Share/Config/ZhanyiGate.tab"				//关卡配置表
#define FILE_ZHANYI_LEVEL_TAB			"../Share/Config/ZhanyiLevel.tab"				// 战役配置表 [yangzhen]

#define FILE_EQUIP_NEW_BASE				"../Share/Config/equip_new_base.tab"			//新装备表
#define FILE_MISCELL_NWITEM				"../Share/Config/nwitem_miscellaneous.tab"		//新材料和消耗物品表
#define FILE_SPELL_LEVELUP_TAB			"../Share/Config/spell_lvlup.tab"				//技能升级表
#define FILE_NWGEM_INFO_TAB				"../Share/Config/newgem_info.tab"				//新宝石表
#define FILE_NEW_ITEM_MIX_TAB			"../Share/Config/newitem_mix.tab"				//新合成分解表
#define FILE_NWTREASURE_INFO_TAB		"../Share/Config/treasure_item.tab"			// 宝物表 [wangyiran]


#define FILE_NW_BOXDROPDETAIL			"../Share/Config/box_droplist.tab"	// 宝箱掉落 [wangyiran]
#define FILE_PHYSICAL_POWER				"../Share/Config/physicalpower.tab" // 体力	 [xingzhishan]
#define FILE_ENERGY_POWER				"../Share/Config/energypower.tab" // 能量 [5/3/2016 HJ]
#define FILE_PLAYER_GUIDE_TAB			"../Share/Config/PlayerGuide.tab"	// 新手引导 [yangzhen]
#define FILE_ARENA_CHALLENGE_BUY_COUNT_TAB	"../Share/Config/ArenaChallengeBuyCount.tab"	// 竞技场挑战购买次数 [xingzhishan]
#define FILE_ARENA_CHALLENGE_AWARD_TAB	"../Share/Config/ArenaChallengeAward.tab" // 竞技场挑战奖励 [xingzhishan]
#define FILE_ARENA_AWARD_TAB			"../Share/Config/ArenaAward.tab" // 竞技场每周段位奖励 [2/19/2016 chentongxin]
#define FILE_ARENA_PLACEAWARD_TAB		"../Share/Config/ArenaPlaceAward.tab" // 竞技场每周排行奖励奖励 [2/19/2016 chentongxin]
#define FILE_AWARD_SYSTEM_TAB			"../Share/Config/AwardSystem.tab" // 离线奖励 [xingzhishan]
#define FILE_ARENA_LEVEL_TAB			"../Share/Config/ArenaLevel.tab" // 竞技场段位等级[chentongxin]

#define FILE_KHAN_ID_COLLECTION			"../Share/Config/id_gather.tab"
#define FILE_ACHIEVEMENT_TAB			"../Share/Config/Achievement.tab" // 成就系统 [wangyiran]
#define FILE_HERO_TREASURE_WEAR_TAB		"../Share/Config/treasureWereInfo.tab" // 英雄穿戴宝物系统 [wangyiran]
#define FILE_BANK_EXCHANGE_TAB			"../Share/Config/BankExchange.tab" // 银行兑换 [xingzhishan]
#define FILE_BANK_CRITICAL_AWARD_TAB	"../Share/Config/BankCriticalAwawrd.tab" // 银行暴击奖励 [xingzhishan]
#define FILE_PLAYER_LEVEL_CONFIG		"../Share/Config/PlayerLevelConfig.tab" // 玩家等级配置 [xingzhishan]
#define FILE_CHECHINS_AWARD_TAB			"../Share/Config/CheckinsAward.tab"	// 签到 [xingzhishan]
#define FILE_PLAYER_UPlEVEL             "../Share/Config/PlayerLevelUp.tab"	//团队升级表格[wangxiaoyong]
#define FILE_DAILYTASK_TAB				"../Share/Config/DailyTask.tab" // 日常任务系统 [wangyiran]
#define FILE_DAILYTASK7_TAB				"../Share/Config/DailyTask7.tab" // 日常任务系统 [wangyiran]
#define FILE_GROWUP_TAB					"../Share/Config/Growup.tab" // 成长 [11/11/2016 scw]
#define FILE_WISH_WIN					"../Share/Config/WishWin.tab"	// 战神召唤 [xingzhishan]
#define FILE_WISH_GOD				    "../Share/Config/WishGod.tab"	// 天神召唤 [xingzhishan]
#define FILE_LOGIN_TDANALYTISS		    "../Share/Config/Analytiss.tab"	// td统计 [wangyiran]
#define FILE_HEROATLAS				    "../Share/Config/HeroAtlas.tab"	// 英雄图鉴 [wangyiran]
#define FILE_NWVIP_LEVELUP				"../Share/Config/NWVipLevelUp.tab" // vip升级表 [wangxiaoyong]
#define FILE_GODTOWER_TAB				"../Share/Config/GodTower.tab"	// 神域之塔 [2/2/2016 wangshaocong]
#define FILE_HANDBOOK_TAB               "../Share/Config/heroHandBook.tab"
#define FILE_HANDBOOKAWARD_TAB          "../Share/Config/heroHandBookAward.tab"
#define FILE_SPORTS_TAB					"../Share/Config/Sports.tab" // 竞技挑战 [3/9/2016 scw]
#define FILE_SPORTS_WIN_TAB				"../Share/Config/SportsWin.tab" //竞技挑战连胜
#define FILE_SPORTS_FINISH_TAB			"../Share/Config/SportsFinish.tab" //竞技挑战完成
#define FILE_WORLDBOSS_TAB				"../Share/Config/WorldBoss.tab" // 世界boss [4/6/2016 scw]
#define FILE_HERO_MIX_TAB				"../Share/Config/HeroMix.tab" // 英雄合成 [6/26/2017 scw]

#define FILE_CHAR_DEFAULT				"../Share/Config/PlayerTemplte.tab"
#define FILE_BEHAVIOR_AI_TEMPLATE		"../Share/Config/monster_ai_template.tab"
#define FILE_ONEATTR_2_TWOATTR			"../Share/Config/OneAttr2TwoAttr.tab"

#define FILE_ARENA_SPRINT_MATCH_AWARD	"./ActivityConfig/ArenaSprintMatchAward.tab"
#define FILE_LEVEL_SPRINT_MATCH_AWARD	"./ActivityConfig/LevelSprintMatchAward.tab"

// 召唤 [4/26/2016 HJ]
#define FILE_NOVICE_TAKECARD			"./Config/NoviceTakeCard.tab"
#define FILE_TAKECARD_ONEGOLD			"./Config/TakeCardOneGold.tab"
#define FILE_TAKECARD_TENGOLD			"./Config/TakeCardTenGold.tab"
#define FILE_TAKECARD_FRIEND            "./Config/TakeCardOneFriend.tab"
#define FILE_TAKECARD_ADVANCED           "./Config/TakeCardOneAdvanced.tab"
#define FILE_TAKECARD_ONEDIAMOND			"./Config/TakeCardOneDiamond.tab"
#define FILE_TAKECARD_TENDIAMOND			"./Config/TakeCardTenDiamond.tab"
#define FILE_WISH_ARES				"./Config/WishAres.tab"
#define FILE_WISH_HOTWEEK			"./Config/WishAresWeekGood.tab"
#define FILE_WISH_HOTDAY			"./Config/WishAresDayGood.tab"
#define FILE_ELITE_CHANLLENGE_COUNT			"../Share/Config/BattleEliteChallengeCount.tab"
#define FILE_ELITE_COMMON_CHANLLENGE_COUNT			"../Share/Config/BattleCommonChallengeCount.tab"

#define FILE_OFFLINE_TIME_AWARD			"../Share/Config/OfflineTimeAward.tab"
#define FILE_VIP_LEVEL_RIGHT			"../Share/Config/NWVipLevelRight.tab"
#define FILE_MONSTER_LVLPROP			"../Share/Config/monster_lvlprop.tab"

#define FILE_GROUP_SPELL				"../Share/Config/HeroGroupSpell.tab"
#define FILE_TOPLIST_CONFIG				"../Share/Config/TopListConfig.tab"
#define FILE_TOPLIST_AWARD_HOUR_CONFIG		"../Share/Config/TopListAwardHour.tab"
#define FILE_TOPLIST_AWARD_DAY_CONFIG		"../Share/Config/TopListAwardDay.tab"
#define FILE_TOPLIST_AWARD_WEEK_CONFIG		"../Share/Config/TopListAwardWeek.tab"
#define FILE_TOPLIST_AWARD_WEEKS_CONFIG		"../Share/Config/TopListAwardWeeks.tab"

#define FILE_ALLPLAYER_DUMP				"./TempArchive/PlayerDump"
#define FILE_SHOP						"../Share/Config/shop_tab.tab"
#define FILE_SHOP_ITEM					"../Share/Config/shopItem_tab.tab"

#define FILE_LVLGIFT_TAB				"../Share/Config/AssignGiftLvl.tab" // 等级礼包奖励 [wangyiran]
#define FILE_MONTHASSIGNGIFT_TAB		"../Share/Config/AssignGiftMonth.tab" // 月签到奖励 [wangyiran]
#define FILE_DAYASSIGNGIFT_TAB			"../Share/Config/AssignGiftDayContinue.tab" // 日连续签到奖励 [wangyiran]
#define FILE_UPDATEPLAYER_HEAD_TAB		"../Share/Config/PlayerHead.tab"// 玩家更换头像 [4/18/2016 HJ]

// 寻宝 [3/1/2016 TianShuai]
#define FILE_FIND_TREASURE_LAYER		"../Share/Config/FindTreasureLayer.tab" //寻宝层
#define FILE_FIND_TREASURE_GRID			"../Share/Config/FindTreasureGrid.tab" //寻宝格子
#define FILE_FIND_TREASURE_ITEM			"../Share/Config/FindTreasureItem.tab" //物资
#define FILE_FIND_TREASURE_CAMPSITE		"../Share/Config/FindTreasureCampsite.tab" //营地
#define FILE_FIND_TREASURE_BAG			"../Share/Config/FindTreasureBag.tab" //背包
#define FILE_FIND_TREASURE_AUTO			"../Share/Config/FindTreasureAuto.tab" //扫荡

#define FILE_SUPER_EGO_TAB				"../Share/Config/SuperEgo.tab" // 超能试炼 [4/22/2016 scw]
#define FILE_VIP_LEVEL_GIFT_BUY_TAB		"../Share/Config/VipLevelGiftBuy.tab" // vip等级特权礼包购买 [5/11/2016 TianShuai]
#define FILE_DREAM_TEMPLE_TAB			"../Share/Config/DreamTemple.tab" // 梦幻神殿 [5/17/2016 scw]

#define FILE_HERO_SPELL_LEVEL_UP_WEIGHT "../Share/Config/spell_lvlup_weight.tab" // 英雄技能升级 [8/12/2016 TianShuai]
// 推送配置 [5/16/2016 HJ]
#define FILE_PUSH_NOTIFICATIONS         "./Config/GamePushNotifications.tab"

// 特殊礼包 [5/18/2016 HJ]
#define FILE_SPECIAL_GIFT_MALL_TAB			"./Config/SpecialGiftMall.tab"
// 玩家数据健值配置表[用过lua接口 SetPlayerGameDataByKey GetPlayerGameDataByKey 访问]
#define FILE_SCRIPTMEMDATA_TAB			"./Config/ScriptMemData.tab"

#define FILE_GUILDDISH_TAB				"../Share/Config/guildDishInfo.tab"// 公会菜肴 [wangyiran]
#define FILE_GUILDDISHCOST_TAB			"../Share/Config/guildDishCost.tab"// 公会购买次数消耗钻石 [wangyiran]
#define FILE_GUILDSHOP_TAB				"../Share/Config/guildShopItem.tab"// 公会商店 [wangyiran]
#define FILE_GUILDBOSS_TAB				"../Share/Config/guildBoss.tab" // 工会boss [wangyiran]
#define FILE_GUILDBOSS_AWARD_TAB		"../Share/Config/guildBossAward.tab" // 工会boss奖励 [3/27/2017 "tianshuai"]

#define FILE_HERO_ATALS_PROP_ADD		"../Share/Config/HeroAtlasPropAdd.tab" // 收集英雄属性加成 [11/22/2016 TianShuai]


#define FILE_INDENTURE_DATA_TABLE		"../Share/Config/IndentureItem.tab" // 契约普通品质表 [11/22/2016 TianShuai]
#define FILE_INDENTURE_SPECIAL_DATA_TABLE		"../Share/Config/IndentureSpecialItem.tab" // 契约特殊品质表 [11/22/2016 TianShuai]

#define FILE_HERO_SPELL_LEVEL_UP_COST_TABLE		"../Share/Config/spell_lvlup_cost.tab" // 英雄技能升级消耗表 [12/12/2016 "tianshuai"]

#define FILE_HERO_TUJIAN_ITEM_TABLE "../Share/Config/TuJianItem.tab"
#define FILE_HERO_EXP_CALC_TABLE "../Share/Config/HeroExpCalc.tab"

#define FILE_EQUIP_PRIMARY_ATTR_RANDOM_TABLE "../Share/Config/nwEquip_PrimaryAttrRandom.tab" // 装备主属性随机表 [2/10/2017 "tianshuai"]
#define FILE_EQUIP_PRIMARY_ATTR_GROW_TABLE "../Share/Config/nwEquip_PrimaryAttrGrow.tab"
#define FILE_EQUIP_ADDITIONAL_ATTR_TABLE "../Share/Config/nwEquip_AdditionalAttr.tab" // 装备附加属性表 [2/10/2017 "tianshuai"]

#define FILE_HERO_BIRTH_STAR_TABLE "../Share/Config/heroBirthStar.tab" //英雄出生星级表

#define FILE_INVITECODE_ACHIEVEMENT_TABLE "../Share/Config/InviteCodeAchievement.tab" //邀请码好友成就
#define FILE_ZHANYI_CHAPTER_AWARD_TABLE "../Share/Config/ZhanyiChapterAward.tab" //战役章节胜利奖励
#define FILE_ROBOT_HELP_FIGHT_TABLE "../Share/Config/RobotHelpFight.tab" //机器人助战
#define FILE_GOD_ARENA_AWARD_TABLE "../Share/Config/GodArenaAward.tab" // 战神竞技场奖励 [7/12/2017 scw]
#define FILE_MONEY_TREE_TABLE "../Share/Config/MoneyTree.tab" // 摇钱树 [8/11/2017 Chief]

#define FILE_EQUIP_TREASURE_TABLE "../Share/Config/EquipTreasure.tab" // 装备夺宝概率表 [8/16/2017 PGT]
#define FILE_EQUIP_TREASURE_ITEM_TABLE "../Share/Config/EquipTreasureItem.tab" // 装备夺宝奖励物品 [8/16/2017 PGT]
#define FILE_EQUIP_TREASURE_BOX_TABLE "../Share/Config/EquipTreasureBox.tab" // 装备夺宝宝箱 [8/16/2017 PGT]

/************************************************************************/
/* 相关函数宏定义                                                       */
/************************************************************************/
#define _GetFullPath(fullpath, filepath, filename) \
	do \
	{ \
		sprintf(fullpath, "%s%s", (filepath), (filename)); \
	} while(0)
#define GetAIScriptFullPath(fullpath, filename)	_GetFullPath(fullpath, PATH_AI_SCRIPT, (filename))
#define GetMapFullPath(fullpath, filename)		_GetFullPath(fullpath, PATH_MAP, (filename))
#endif
	
