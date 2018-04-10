/*$T MapServer/FileDef.h GC 1.140 10/10/07 11:01:49 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __FILEDEF_H__
#define __FILEDEF_H__

/************************************************************************/
/* ����Ŀ¼����                                                         */
/************************************************************************/
#define PATH_MAP							"./map/"

/************************************************************************/
/* �������������                                                       */
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
/* �������淨����                                                       */
/************************************************************************/
#define FILE_IPRANGE						"./Config/IPAuth.ini"
#define FILE_BASE_VALUE						"./Config/ProfessionBaseProperty.ini"
#define FILE_KHAN_ITEM_SERIAL					"./Config/ItemSN.ini"
#define FILE_MONITOR_INFO					"./Config/MonitorInfo.ini"
#define FILE_STOCK_MONITOR_INFO				"./Config/StockMonitorInfo.ini"

/************************************************************************/
/* ����������ڲ����                                                   */
/************************************************************************/
#define FILE_MAP_INFO					"./Config/mapdefines.tab"				// Ĭ�ϵ�MapServer��Ӧ�����ñ�
#define FILE_MAP_INFO_6					"./Config/mapdefines_6.tab"				// 6 MapServer��Ӧ�����ñ�
#define FILE_MAP_INFO_10				"./Config/mapdefines_10.tab"			// 10 MapServer��Ӧ�����ñ�
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
#define FILE_RANDOM_MONSTER					"./Config/random_monster.tab"			//2010-11-26 by rj ��������ñ�
#define FILE_TOPLIST_NOTICE					"./Config/TopListNotice.tab"			// 2010-3-27 by luoqian ����֪ͨ��
//#define FILE_CHARIOT_EQUIP_BLUEATTR_BUFF	"./Config/ChariotEquip_BlueAttr_Effect.tab"	// by wy	 ս��װ��������BUFF��
#define FILE_POKER_ATTR_RANDOM_TABLE		"./Config/Poker_Attr_Random.tab"			//�˿�������������ɱ�
//#define FILE_HORSEEQUIP_BLUE_SET			"./Config/horse_equip_blue_attr_set.tab"	// ����װ�������Ա�
//#define	FILE_HORSEEQUIP_BLUE_LEVEL			"./Config/horse_equip_blue_attr_level.tab"	// ����װ��������ֵ
//#define FILE_HORSEEQUIP_POINT_INFO			"./Config/Horse_EquipPoint_Value.tab"		// װ����Ȩ��
#define FILE_BAK_SCENEINFO					"./Config/BakSceneInfo.tab"							//���ݳ�����
#define FILE_BATTLEEFFECT_TAB				"./Config/battle effect.tab"						//��½�����
#define FILE_DYNAMICBUFF_REG				"./Config/DynamicBuffReg.tab"			// ��̬ Buff ����ע���
#define FILE_MINE_RESULT					"./Config/MineResult.tab"					// ��԰��ɽ���
#define FILE_CHANNELMERIDIAN_TAB			"./Config/ChannelMeridian.tab"				//����
#define FILE_PET_ATTR_MAX_TB				"./Config/PetAttrMax.tab"					// �����������ֵ��
//#define FILE_SERVER_OPENTIME			"./Config/ServerOpenTime.tab"				// ����ʱ������ -2013.02.20���ϣ��Ƶ�worldname.tab
#define FILE_MAPLOADMANAGER_TAB         "./Config/MapLoadManager.tab"
#define FILE_HEROATTRGROW				"./Config/HeroGrow.tab"						//Ӣ����������
#define FILE_ARENATOPSELECT				"./Config/ArenaTopSelect.tab"				//������ѡ�����
#define FILE_ARENARANKUPAWARD			"./Config/ArenaRankUpAward.tab"				//��������������
#define FILE_PEAKARENARANKAWARD			"./Config/PeakArenaRankAward.tab"				//�۷徺������������
#define FILE_PlUNDER_AWARD				"./Config/PlunderAward.tab"				//������ѡ�����
#define FILE_PLATFORM_SHOP_PRODUCT_ID		"./Config/PlatformShopProductId.tab"		// ����ƽ̨���̵�۸��id [yangzhen]
#define FILE_PLATFORM_SHOP_CHANNEL_INDEX	"./Config/PlatformShopChannelIndex.tab"		// ����ƽ̨������id���̵�۸�����еĶ�Ӧ [yangzhen]

#define FILE_WECHATREDITEMPACK_TAB		"./Config/WeChatRedItemPack.tab" // �����еĵ��߽����� [yangzhen]
#define FILE_FAKEPLAYERNAME_TAB			"./Config/FakePlayerName.tab"		// ����ҽ�ɫ���� [6/22/2015 yangzhen]
#define FILE_ACTIVITYSCRIPTCFG_TAB		"./Config/ActivityScriptCfg.tab" // ��ű����ñ� [yangzhen]

/************************************************************************/
/* �ͻ��˷������˹����ڲ�����                                           */
/************************************************************************/
#define FILE_MONSTER_BASE_BEHAVIOR			"../Share/Config/monster_base_behavior.ini"
#define FILE_VERSIONCONFIG					"../Share/Config/VersionConfig.ini"

/************************************************************************/
/* �ͻ��˷������˹����ڲ����                                           */
/************************************************************************/
#define FILE_CITY_BUILDING						"../Share/Config/CityBuilding.tab" //Ӣ�۱�
#define FILE_HEROCHAR						"../Share/Config/Hero.tab" //Ӣ�۱�
#define FILE_HERO_LEVELUP					"../Share/Config/HeroLvExp.tab"	//Ӣ��������
#define FILE_HERO_STAR_GCONFIG				"../Share/Config/heroStar.tab" //Ӣ��������
#define FILE_HERO_DEVELOPCONFIG				"../Share/Config/heroPeiYang.tab" //Ӣ��������
#define FILE_HERO_AWAKEN_CONFIG				"../Share/Config/heroAwaken.tab" //Ӣ��������
#define FILE_HERO_EVOLUTIONCONFIG			"../Share/Config/heroEvolution.tab" // Ӣ�۽��� [1/6/2016 superrobert]
#define FILE_HERO_BREAKCONFIG				"../Share/Config/heroBreak.tab" // Ӣ��Ӣ������ [1/6/2016 superrobert]
#define FILE_HERO_BREAKUPCONFIG				"../Share/Config/heroBreakUp.tab" // Ӣ��Ӣ���������� [hujian] --����
#define FILE_FRIENDARMY_GEMCONFIG			"../Share/Config/friendArmyGem.tab" // ���鱦ʯ [hujian]
#define FILE_FRIENDARMY_GEMLEVELCONFIG		"../Share/Config/friendArmyGemLvlUp.tab" // ���鱦ʯ�ȼ� [hujian]
#define FILE_ARTIFACT_PROPCONFIG			"../Share/Config/ArtifactProp.tab" // ������������ [tianshuai]
#define FILE_ARTIFACT_BUYANDDESTROYCONFIG	"../Share/Config/ArtifactBuyAndDestroy.tab" // ��������ʹݻ� [tianshuai]
#define FILE_ARTIFACT_SKILLCONFIG			"../Share/Config/ArtifactSkill.tab" // ������������ [tianshuai]
#define FILE_ARTIFACT_SKILLBASECONFIG		"../Share/Config/ArtifactSkillBase.tab" // �������ܻ������� [tianshuai]
#define FILE_ARTIFACT_UPDATECONFIG			"../Share/Config/ArtifactUpdate.tab" // �������� [tianshuai]
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
#define FILE_EQUIP_WANFA_ENERGYBALL			"../Share/Config/EnergyBall.tab"					//�����������淨�������������
#define FILE_EQUIP_WANFA_AMULET				"../Share/Config/AmuletInfo.tab"					//�����������淨�л����������
#define FILE_EQUIP_WANFA_ENERGYBALLSYN		"../Share/Config/EnergyBallSyn.tab"					//�����������淨�л������ɫ������
#define FILE_EQUIP_PRODUCE					"../Share/Config/EquipProduce.tab"					//����װ������
#define FILE_EQUIP_SWITCH					"../Share/Config/EquipSwitch.tab"					//װ��ת��
#define FILE_CHANGE_PROFESSION_TABLE_NAME   "../Share/Config/ChangeProfession.tab"				//ְҵת����
#define FILE_CHANGE_APPEARANCE_TABLE_NAME   "../Share/Config/change_appearance.tab"				//���������
#define FILE_GUILD_SKILL_INFO				"../Share/Config/gift_learn.tab"					//��Ἴ�ܱ�
#define FILE_EQUIP_PRODUCE_NEW				"../Share/Config/Equip_Produce_New.tab"				//��װ�����Դ��������
#define FILE_GIFT_BALANCE_INFO_1			"../Share/Config/gift_balance_learn_1.tab"			//�츳ƽ�� ���Ƽ��ܳ���
#define FILE_GIFT_BALANCE_INFO_2			"../Share/Config/gift_balance_learn_2.tab"			//�츳ƽ�� ���Ƽ���
#define FILE_EQUIP_CONST_INFO				"../Share/Config/Equip_Const_Info.tab"		        //������Ϣ
#define FILE_EQUIP_CONST_ACTIVE_POINT		"../Share/Config/Equip_Const_Active_Point.tab"		//��������������Ϣ
#define FILE_EQUIP_CONST_BIND				"../Share/Config/Equip_Const_Bind.tab"		        //����������Ϣ
#define FILE_EQUIP_HONOR_BIND				"../Share/Config/Equip_Honor_Bind.tab"		        //����������Ϣ
#define FILE_ITEM_SPECIAL					"../Share/Config/Item_Special.tab"					//���һЩ�淨���õ����������
#define FILE_GIFT_ZHUANGONG					"../Share/Config/gift_zhuangong.tab"				//�츳ר�� ר��
#define FILE_GIFT_ZHUANFANG					"../Share/Config/gift_zhuanfang.tab"				//�츳ר�� ר��
#define FILE_GUILD_BUS_SKILL_STUDY_CONSUME	"../Share/Config/Bus_Sill_Study_Consume.tab"		//���BUS����ѧϰ����
#define FILE_MILITARYRANK					"../Share/Config/MilitaryRank.tab"					//����������Ϣ
#define FILE_ROSEBAND						"../Share/Config/Equip_RoseBand.tab"				//װ��õ����������ñ�
#define FILE_EQUIP_HELL_PRODUCE				"../Share/Config/Equip_hell_produce.tab"			//ڤװ�����
#define FILE_EQUIP_HELL_LEVELUP				"../Share/Config/Hell_Equip_Levelup.tab"			//ڤװ������
#define FILE_EQUIP_HELL_LEVELUP_ATTR		"../Share/Config/Hell_Equip_Levelup_Attr.tab"		//ڤװ�������Ա�
#define FILE_EQUIP_METEORITE_STONE			"../Share/Config/Equip_hell_meteorite_stone.tab"	//����ʯ���ñ�
#define FILE_EQUIP_HELL_LEVEL				"../Share/Config/Equip_hell_level.tab"				//ڤ�����ȼ����ñ�
#define FILE_HELL_XUANTIE_EFFECT			"../Share/Config/Equip_hell_xuantie_effect.tab"		//���������Է�ֵӰ���
#define FILE_HELL_ATTACH_BLUE				"../Share/Config/Equip_hell_attachblue.tab"			//ڤװ������
#define FILE_HELL_BIND_BLUE_STAR			"../Share/Config/Equip_hell_bindbluestar.tab"		//���������ñ�
#define FILE_HELL_LANWAN					"../Share/Config/Equip_hell_lanwan.tab"				//������Ʒ��
#define FILE_HELL_LANWAN_PROPERTY			"../Share/Config/Equip_hell_lanwan_property.tab"	//����ǿ����
#define FILE_EQUIP_DISMANTLE_ITEM			"../Share/Config/Equip_dismantle_item.tab"			//װ�������߱�
#define FILE_HELL_SUBSKILL					"../Share/Config/Equip_hell_subskill.tab"			//ڤװ�䷽��
#define FILE_CHARIOT_BASEATTR				"../Share/Config/Chariot_Base_Attr.tab"				//ս���������Ա�
#define FILE_CHARIOT_EQUIPATTR				"../Share/Config/Chariot_Equip_Attr.tab"			//ս��װ�����Ա�
#define FILE_COUNTRY_MANOR					"../Share/Config/CountryManor.tab"
#define FILE_CHARIOT_EQUIPBUILD				"../Share/Config/Chariot_Equip_Build.tab"			//ս��װ�������
#define FILE_CHARIOT_RANDOMATTR				"../Share/Config/Chariot_Random_Attr.tab"			//ս��װ��������Ա�
#define FILE_CHARIOT_RANDOMITEM				"../Share/Config/Chariot_Equip_RandItem.tab"		//ս��װ�����װ����
#define FILE_CHARIOT_BODYBUILD				"../Share/Config/Chariot_Body_Build.tab"			//ս����������
#define FILE_FADE_ITEM                      "../Share/Config/fakeitem.tab"                      // ���û�δ������Ʒ��װ������ʯ���б�
#define FILE_FADE_HORSE                     "../Share/Config/fakehorse.tab"                     // ���û�δ��������б�
#define FILE_CHARIOT_EQUIP_RECYCLE			"../Share/Config/Chariot_Equip_Recycle.tab"			//ս��װ�����ձ�
#define FILE_CHARIOT_COEFFICIENT			"../Share/Config/chariotcoefficient.tab"			// �������Զ�ս�����Ե�Ӱ��ϵ��
#define FILE_POKER_ACTIVATION				"../Share/Config/Poker_Activation.tab"				//�˿��Ƽ����
#define FILE_GEM_REWORK_WEIGHT				"../Share/Config/GemReworkWeight.tab"				//��ʯ��¯Ȩ�ر�
#define FILE_GEM_REWORK_COST				"../Share/Config/GemReworkCost.tab"					//��ʯ��¯���ı�
#define FILE_ROOM_CONFIG					"../Share/Config/Room_Config.tab"					//�������ñ�
#define FILE_ROOM_LOGIC						"../Share/Config/Room_Logic.tab"					//����ؿ��߼���

#define FILE_GEM_RAND_ATTR					"../Share/Config/Gem_Attr_Rand.tab"					// ��ʯϴ��������Ա�
#define FILE_RAND_ATTR_GEMLIST				"../Share/Config/Gem_RandAttr_GemList.tab"			// ��ϴ�ϱ�ʯ�б�
#define FILE_GEM_RAND_ATTR_COST				"../Share/Config/Gem_RandAttr_Cost.tab"				// ��ʯϴ�ϻ���
#define FILE_GEM_CHANGE_TB				"../Share/Config/GemChange.tab"							// ��ʯת��
#define FILE_HORSE_EQUIP_BASE				"../Share/Config/horse_equip_base.tab"				// ����װ����
#define FILE_GUANQIA_SKILL_INFO				"../Share/Config/guanqiamaps.tab"					// �ؿ����ܱ�
#define FILE_HOMEMONSTER_FOOD				"../Share/Config/HomeMonsterFood.tab"		//��԰�ػ���ʳ��

#define FILE_PLANT_RECLAIM					"../Share/Config/PlantReclaim.tab"					// ��԰��ֲ����
#define FILE_PLANT_LEVELUP					"../Share/Config/PlantLevelUp.tab"					// ��԰��ֲ����
#define FILE_PLANT_RESULT					"../Share/Config/PlantResult.tab"					// ��԰�ջ���
#define FILE_MINE_RECLAIM					"../Share/Config/MineReclaim.tab"					// ��԰��ɽ����
#define FILE_GARDEN_LEVELUP					"../Share/Config/GardenLevelUp.tab"					// ��԰������
#define FILE_HOEMSHOP_TAB					"../Share/Config/HomeShop.tab"						// ��԰�̵�
#define FILE_HOMESHOPBACK_TAB				"../Share/Config/HomeShopBack.tab"					// ��԰��Ʒ����
#define FILE_GARDENTREE_TAB					"../Share/Config/GardenTree.tab"					// ��԰������
#define FILE_GARDEFRUIT_TAB					"../Share/Config/GardenFruit.tab"					// ��԰��ʵ����
#define FILE_GARDEN_APPLY_TAB				"../Share/Config/GardenApply.tab"					// ��԰����


#define FILE_CARD_MIX_TABLE					"../Share/Config/CardMix.tab"						// ���ƺϳɱ�
#define FILE_CARD_LEVELUP_TABLE				"../Share/Config/CardLevelup.tab"					// ����������
#define FILE_MINE_CARDINFO					"../Share/Config/CardInfo.tab"						// ����
#define FILE_KHAN_CARDSUIT					"../Share/Config/CardSuitActive.tab"				// ���Ƽ���
#define FILE_GIFTPACK_ONLINE				"../Share/Config/online_gift.tab"					// �����������
#define FILE_THREETOVISIT_CONSUME			"../Share/Config/ThreeToVisit_Consume.tab"			// ����é®���ı�
#define FILE_THREETOVISIT_HORTATION			"../Share/Config/ThreeToVisit_Hortation.tab"		// ����é®������
#define FILE_CHANGE_SILLER_TAB				"../Share/Config/ChangeSilver.tab"					// �����һ���
#define FILE_LOGINGIFT_TAB					"../Share/Config/LoginGift.tab"						//��½�����
#define FILE_PET_JICHU						"../Share/Config/pet_jichu.tab"					// ����������
#define FILE_PET_TRAIN						"../Share/Config/PetTrain.tab"						// ����������
#define FILE_CONSUMMATION_TAB				"../Share/Config/Consummation_list.tab"				//�ɾ���Ϣ��
#define FILE_PLAYER_ATTRIBUTE_UP_TAB	    "../Share/Config/PlayerAttribute_up.tab"			//�������Ե����ɱ� 
#define FILE_CHANNELMERIDIAN_COLOR_TAB			"../Share/Config/ChannelMeridianColor.tab"				//����

#define FILE_FUBENPINGFEN_RANK_TAB				"../Share/Config/FubenPingFenRank.tab"			//�������ֱ��
#define FILE_FUBENPINGFEN_AWARD_TAB				"../Share/Config/FubenPingFenAward.tab"	//�������ֱ��

#define FILE_MD_TO_CLIENT_TAB				"../Share/Config/MD_To_Client.tab"			//��Ҫͬ�����ͻ��˵�MD���
#define FILE_PARTY_HONOR_TAB				"../Share/Config/PartyHonor.tab"				//��������

#define FILE_EQUIP_CONSTELLATION_SEAL_ACTIVE_TB					"../Share/Config/Equip_ConstellationSealActive.tab"    //������ӡ

#define FILE_EQUIP_ZODIAC_SEAL_ACTIVE_TB					"../Share/Config/Equip_ZodiacSealActive.tab"    //��Ф��ӡ
#define FILE_SLG_BUILD_INFO					"../Share/Config/slg_buildinfo.tab" // slg������Ϣ
#define FILE_ARMS_INFO					"../Share/Config/ArmsInfo.tab"  //�������
#define FILE_BARRACK_INFO					"../Share/Config/BarrackInfo.tab"  //��Ӫ��Ϣ
#define FILE_FAREASTWAR_MONEDIT				"../Share/Config/FarEastWar_Mon_Edit.tab"
#define FILE_PET_HATCH_INFO				"../Share/Config/PetHatchInfo.tab" // �������

#define FILE_GARDEN_QUEST_TB		"../Share/Config/GardenQuest.tab"		//��԰����


#define FILE_FAREASTWAR_ARMS_INFO	"../Share/Config/FarEastWarMonster.tab"		//Զ������ս�����

#define FILE_PET_ATTRIBUTE_UP_TAB	    "../Share/Config/PetAttribute_up.tab"			//�������Ե����ɱ� 
#define FILE_WORLDNAME_TAB				"../Share/Config/worldname.tab"
#define FILE_SOLDIERATTRIBUTE_TAB          "../Share/Config/SoldierAttribute.tab"		//Ӷ�����Ա�
#define FILE_SOLDIERBATTLE_TAB          "../Share/Config/SoldierBattle.tab"
#define FILE_SOLDIERBATTLEBODY_TAB      "../Share/Config/SoldierBattleBody.tab"         //Ӷ��ʵ���
#define FILE_SOLDIERBATTLESPELLGROUP_TAB  "../Share/Config/SoldierBattleSpellGroup.tab"  //Ӷ���������

#define FILE_SOLDIER_BASEATTR_TAB       "../Share/Config/Soldier_BaseAttr.tab"          //Ӷ���������Ա�
#define FILE_SOLDIER_TOOLS_TAB          "../Share/Config/SoldierTools.tab"              //Ӷ�����߱�

#define FILE_ZHANYI_GATE_TAB			"../Share/Config/ZhanyiGate.tab"				//�ؿ����ñ�
#define FILE_ZHANYI_LEVEL_TAB			"../Share/Config/ZhanyiLevel.tab"				// ս�����ñ� [yangzhen]

#define FILE_EQUIP_NEW_BASE				"../Share/Config/equip_new_base.tab"			//��װ����
#define FILE_MISCELL_NWITEM				"../Share/Config/nwitem_miscellaneous.tab"		//�²��Ϻ�������Ʒ��
#define FILE_SPELL_LEVELUP_TAB			"../Share/Config/spell_lvlup.tab"				//����������
#define FILE_NWGEM_INFO_TAB				"../Share/Config/newgem_info.tab"				//�±�ʯ��
#define FILE_NEW_ITEM_MIX_TAB			"../Share/Config/newitem_mix.tab"				//�ºϳɷֽ��
#define FILE_NWTREASURE_INFO_TAB		"../Share/Config/treasure_item.tab"			// ����� [wangyiran]


#define FILE_NW_BOXDROPDETAIL			"../Share/Config/box_droplist.tab"	// ������� [wangyiran]
#define FILE_PHYSICAL_POWER				"../Share/Config/physicalpower.tab" // ����	 [xingzhishan]
#define FILE_ENERGY_POWER				"../Share/Config/energypower.tab" // ���� [5/3/2016 HJ]
#define FILE_PLAYER_GUIDE_TAB			"../Share/Config/PlayerGuide.tab"	// �������� [yangzhen]
#define FILE_ARENA_CHALLENGE_BUY_COUNT_TAB	"../Share/Config/ArenaChallengeBuyCount.tab"	// ��������ս������� [xingzhishan]
#define FILE_ARENA_CHALLENGE_AWARD_TAB	"../Share/Config/ArenaChallengeAward.tab" // ��������ս���� [xingzhishan]
#define FILE_ARENA_AWARD_TAB			"../Share/Config/ArenaAward.tab" // ������ÿ�ܶ�λ���� [2/19/2016 chentongxin]
#define FILE_ARENA_PLACEAWARD_TAB		"../Share/Config/ArenaPlaceAward.tab" // ������ÿ�����н������� [2/19/2016 chentongxin]
#define FILE_AWARD_SYSTEM_TAB			"../Share/Config/AwardSystem.tab" // ���߽��� [xingzhishan]
#define FILE_ARENA_LEVEL_TAB			"../Share/Config/ArenaLevel.tab" // ��������λ�ȼ�[chentongxin]

#define FILE_KHAN_ID_COLLECTION			"../Share/Config/id_gather.tab"
#define FILE_ACHIEVEMENT_TAB			"../Share/Config/Achievement.tab" // �ɾ�ϵͳ [wangyiran]
#define FILE_HERO_TREASURE_WEAR_TAB		"../Share/Config/treasureWereInfo.tab" // Ӣ�۴�������ϵͳ [wangyiran]
#define FILE_BANK_EXCHANGE_TAB			"../Share/Config/BankExchange.tab" // ���жһ� [xingzhishan]
#define FILE_BANK_CRITICAL_AWARD_TAB	"../Share/Config/BankCriticalAwawrd.tab" // ���б������� [xingzhishan]
#define FILE_PLAYER_LEVEL_CONFIG		"../Share/Config/PlayerLevelConfig.tab" // ��ҵȼ����� [xingzhishan]
#define FILE_CHECHINS_AWARD_TAB			"../Share/Config/CheckinsAward.tab"	// ǩ�� [xingzhishan]
#define FILE_PLAYER_UPlEVEL             "../Share/Config/PlayerLevelUp.tab"	//�Ŷ��������[wangxiaoyong]
#define FILE_DAILYTASK_TAB				"../Share/Config/DailyTask.tab" // �ճ�����ϵͳ [wangyiran]
#define FILE_DAILYTASK7_TAB				"../Share/Config/DailyTask7.tab" // �ճ�����ϵͳ [wangyiran]
#define FILE_GROWUP_TAB					"../Share/Config/Growup.tab" // �ɳ� [11/11/2016 scw]
#define FILE_WISH_WIN					"../Share/Config/WishWin.tab"	// ս���ٻ� [xingzhishan]
#define FILE_WISH_GOD				    "../Share/Config/WishGod.tab"	// �����ٻ� [xingzhishan]
#define FILE_LOGIN_TDANALYTISS		    "../Share/Config/Analytiss.tab"	// tdͳ�� [wangyiran]
#define FILE_HEROATLAS				    "../Share/Config/HeroAtlas.tab"	// Ӣ��ͼ�� [wangyiran]
#define FILE_NWVIP_LEVELUP				"../Share/Config/NWVipLevelUp.tab" // vip������ [wangxiaoyong]
#define FILE_GODTOWER_TAB				"../Share/Config/GodTower.tab"	// ����֮�� [2/2/2016 wangshaocong]
#define FILE_HANDBOOK_TAB               "../Share/Config/heroHandBook.tab"
#define FILE_HANDBOOKAWARD_TAB          "../Share/Config/heroHandBookAward.tab"
#define FILE_SPORTS_TAB					"../Share/Config/Sports.tab" // ������ս [3/9/2016 scw]
#define FILE_SPORTS_WIN_TAB				"../Share/Config/SportsWin.tab" //������ս��ʤ
#define FILE_SPORTS_FINISH_TAB			"../Share/Config/SportsFinish.tab" //������ս���
#define FILE_WORLDBOSS_TAB				"../Share/Config/WorldBoss.tab" // ����boss [4/6/2016 scw]
#define FILE_HERO_MIX_TAB				"../Share/Config/HeroMix.tab" // Ӣ�ۺϳ� [6/26/2017 scw]

#define FILE_CHAR_DEFAULT				"../Share/Config/PlayerTemplte.tab"
#define FILE_BEHAVIOR_AI_TEMPLATE		"../Share/Config/monster_ai_template.tab"
#define FILE_ONEATTR_2_TWOATTR			"../Share/Config/OneAttr2TwoAttr.tab"

#define FILE_ARENA_SPRINT_MATCH_AWARD	"./ActivityConfig/ArenaSprintMatchAward.tab"
#define FILE_LEVEL_SPRINT_MATCH_AWARD	"./ActivityConfig/LevelSprintMatchAward.tab"

// �ٻ� [4/26/2016 HJ]
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

#define FILE_LVLGIFT_TAB				"../Share/Config/AssignGiftLvl.tab" // �ȼ�������� [wangyiran]
#define FILE_MONTHASSIGNGIFT_TAB		"../Share/Config/AssignGiftMonth.tab" // ��ǩ������ [wangyiran]
#define FILE_DAYASSIGNGIFT_TAB			"../Share/Config/AssignGiftDayContinue.tab" // ������ǩ������ [wangyiran]
#define FILE_UPDATEPLAYER_HEAD_TAB		"../Share/Config/PlayerHead.tab"// ��Ҹ���ͷ�� [4/18/2016 HJ]

// Ѱ�� [3/1/2016 TianShuai]
#define FILE_FIND_TREASURE_LAYER		"../Share/Config/FindTreasureLayer.tab" //Ѱ����
#define FILE_FIND_TREASURE_GRID			"../Share/Config/FindTreasureGrid.tab" //Ѱ������
#define FILE_FIND_TREASURE_ITEM			"../Share/Config/FindTreasureItem.tab" //����
#define FILE_FIND_TREASURE_CAMPSITE		"../Share/Config/FindTreasureCampsite.tab" //Ӫ��
#define FILE_FIND_TREASURE_BAG			"../Share/Config/FindTreasureBag.tab" //����
#define FILE_FIND_TREASURE_AUTO			"../Share/Config/FindTreasureAuto.tab" //ɨ��

#define FILE_SUPER_EGO_TAB				"../Share/Config/SuperEgo.tab" // �������� [4/22/2016 scw]
#define FILE_VIP_LEVEL_GIFT_BUY_TAB		"../Share/Config/VipLevelGiftBuy.tab" // vip�ȼ���Ȩ������� [5/11/2016 TianShuai]
#define FILE_DREAM_TEMPLE_TAB			"../Share/Config/DreamTemple.tab" // �λ���� [5/17/2016 scw]

#define FILE_HERO_SPELL_LEVEL_UP_WEIGHT "../Share/Config/spell_lvlup_weight.tab" // Ӣ�ۼ������� [8/12/2016 TianShuai]
// �������� [5/16/2016 HJ]
#define FILE_PUSH_NOTIFICATIONS         "./Config/GamePushNotifications.tab"

// ������� [5/18/2016 HJ]
#define FILE_SPECIAL_GIFT_MALL_TAB			"./Config/SpecialGiftMall.tab"
// ������ݽ�ֵ���ñ�[�ù�lua�ӿ� SetPlayerGameDataByKey GetPlayerGameDataByKey ����]
#define FILE_SCRIPTMEMDATA_TAB			"./Config/ScriptMemData.tab"

#define FILE_GUILDDISH_TAB				"../Share/Config/guildDishInfo.tab"// ������� [wangyiran]
#define FILE_GUILDDISHCOST_TAB			"../Share/Config/guildDishCost.tab"// ���Ṻ�����������ʯ [wangyiran]
#define FILE_GUILDSHOP_TAB				"../Share/Config/guildShopItem.tab"// �����̵� [wangyiran]
#define FILE_GUILDBOSS_TAB				"../Share/Config/guildBoss.tab" // ����boss [wangyiran]
#define FILE_GUILDBOSS_AWARD_TAB		"../Share/Config/guildBossAward.tab" // ����boss���� [3/27/2017 "tianshuai"]

#define FILE_HERO_ATALS_PROP_ADD		"../Share/Config/HeroAtlasPropAdd.tab" // �ռ�Ӣ�����Լӳ� [11/22/2016 TianShuai]


#define FILE_INDENTURE_DATA_TABLE		"../Share/Config/IndentureItem.tab" // ��Լ��ͨƷ�ʱ� [11/22/2016 TianShuai]
#define FILE_INDENTURE_SPECIAL_DATA_TABLE		"../Share/Config/IndentureSpecialItem.tab" // ��Լ����Ʒ�ʱ� [11/22/2016 TianShuai]

#define FILE_HERO_SPELL_LEVEL_UP_COST_TABLE		"../Share/Config/spell_lvlup_cost.tab" // Ӣ�ۼ����������ı� [12/12/2016 "tianshuai"]

#define FILE_HERO_TUJIAN_ITEM_TABLE "../Share/Config/TuJianItem.tab"
#define FILE_HERO_EXP_CALC_TABLE "../Share/Config/HeroExpCalc.tab"

#define FILE_EQUIP_PRIMARY_ATTR_RANDOM_TABLE "../Share/Config/nwEquip_PrimaryAttrRandom.tab" // װ������������� [2/10/2017 "tianshuai"]
#define FILE_EQUIP_PRIMARY_ATTR_GROW_TABLE "../Share/Config/nwEquip_PrimaryAttrGrow.tab"
#define FILE_EQUIP_ADDITIONAL_ATTR_TABLE "../Share/Config/nwEquip_AdditionalAttr.tab" // װ���������Ա� [2/10/2017 "tianshuai"]

#define FILE_HERO_BIRTH_STAR_TABLE "../Share/Config/heroBirthStar.tab" //Ӣ�۳����Ǽ���

#define FILE_INVITECODE_ACHIEVEMENT_TABLE "../Share/Config/InviteCodeAchievement.tab" //��������ѳɾ�
#define FILE_ZHANYI_CHAPTER_AWARD_TABLE "../Share/Config/ZhanyiChapterAward.tab" //ս���½�ʤ������
#define FILE_ROBOT_HELP_FIGHT_TABLE "../Share/Config/RobotHelpFight.tab" //��������ս
#define FILE_GOD_ARENA_AWARD_TABLE "../Share/Config/GodArenaAward.tab" // ս�񾺼������� [7/12/2017 scw]
#define FILE_MONEY_TREE_TABLE "../Share/Config/MoneyTree.tab" // ҡǮ�� [8/11/2017 Chief]

#define FILE_EQUIP_TREASURE_TABLE "../Share/Config/EquipTreasure.tab" // װ���ᱦ���ʱ� [8/16/2017 PGT]
#define FILE_EQUIP_TREASURE_ITEM_TABLE "../Share/Config/EquipTreasureItem.tab" // װ���ᱦ������Ʒ [8/16/2017 PGT]
#define FILE_EQUIP_TREASURE_BOX_TABLE "../Share/Config/EquipTreasureBox.tab" // װ���ᱦ���� [8/16/2017 PGT]

/************************************************************************/
/* ��غ����궨��                                                       */
/************************************************************************/
#define _GetFullPath(fullpath, filepath, filename) \
	do \
	{ \
		sprintf(fullpath, "%s%s", (filepath), (filename)); \
	} while(0)
#define GetAIScriptFullPath(fullpath, filename)	_GetFullPath(fullpath, PATH_AI_SCRIPT, (filename))
#define GetMapFullPath(fullpath, filename)		_GetFullPath(fullpath, PATH_MAP, (filename))
#endif
	
