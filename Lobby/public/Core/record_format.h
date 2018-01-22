/*$T MapServer/Server/Base/record_format.h GC 1.140 10/10/09 10:07:21 */

/* record_format.h $v1.0 13407899 */
#ifndef _RECORD_FORMAT_H_
#define _RECORD_FORMAT_H_

/*
 * TOL: ;
 * ���1SNID����SNID��,��ɫ����,���ڵ�ͼsceneid,������ͣ�0���ף�1��̯��,����ǰ��������,���׺���������
 * ;
 * ��Ʒ1ID,��ƷSNID1,��Ʒ1�������,��Ʒ2ID,��ƷSNID2,��Ʒ2�������,��Ʒ3ID,��ƷSNID3,��Ʒ3�������,��Ʒ4ID,
 * ;
 * ��ƷSNID4,��Ʒ4�������,��Ʒ5ID,��ƷSNID5,��Ʒ5�������,��ƥID,��ƥSNID, ;
 * ���2SNID������SNID��,��ɫ����,���ڵ�ͼsceneid,������ͣ�0���ף�1��̯��,����ǰ��������,���׺���������
 * ;
 * ��Ʒ1ID,��ƷSNID1,��Ʒ1�������,��Ʒ2ID,��ƷSNID2,��Ʒ2�������,��Ʒ3ID,��ƷSNID3,��Ʒ3�������,��Ʒ4ID,
 * ;
 * ��ƷSNID4,��Ʒ4�������,��Ʒ5ID,��ƷSNID5,��Ʒ5�������,��ƥID,��ƥSNID ;
 * Trade ��ʽΪ, log����,
 * ���1��Ϣ,��Ʒ1��Ϣ,...��Ʒ5��Ϣ,pet��Ϣ,���2��Ϣ,��Ʒ1��Ϣ,...��Ʒ5��Ϣ,pet��Ϣ,
 * ;
 * IOL:GL��,Server��,��Ʒ���,��ɫSNID,�Է�SNID,��������,��󱳰�λ��,������, ;
 * ���NPC,XPOS,ZPOS,����̵��(��������֮����-1),�;öȣ���Ƕ�׸��� ;
 * ��������1������1��������������4������4 ;
 * MIL:��ɫSNID���������ͣ�������xλ��,zλ�ã������ţ�����ű��������־ ;
 * ����0������1������2������3������4������5������6������7�� ;
 * ��ɱ�־��������ɱ�־ֵ��ȫ����������������ȫ����������ֵ ;
 * MOL:�����ɫ�������ɫ���������ͣ�����������������xλ��,zλ�� ;
 * POL:�����ɫ�������ɫ���������ͣ�������xλ��,zλ�� ,������ ;
 * SOL:�����ɫ���������ͣ�������xλ��,zλ��,���ܱ�� ;
 * XOL:�����ɫ���������ͣ�������xλ��,zλ��,���ܱ�� ;
 * GPL:��ɫSNID����ɫ���֣��������淨��ţ��Ƿ���ӣ�������� ;
 * ��Ա1�ȼ�����Ա1ְҵ����Ա2�ȼ�����Ա2ְҵ����Ա3�ȼ��� ;
 * ��Ա3ְҵ����Ա4�ȼ�����Ա4ְҵ����Ա5�ȼ�����Ա5ְҵ����Ա6�ȼ�����Ա6ְҵ��
 */
static const char	*csRECORD_FORMAT_Quest = "MIL:%u,%d,%d,%.2f,%.2f,%d,%d,%d,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_Item = "%u,%llu,%s,%s,%d,%u,%s,%d,%d,%d,%d,%d,%llu,%u,%u,%u,%u,%.2f,%.2f,%d,%u,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_Equip = "%llu,%u,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_Money = "%llu,%u,%s,%d,%d,%d,%d,%d,%.2f,%.2f,%u,%d,%d,%u,%u,%d,%d";
static const char	*csRECORD_FORMAT_Pet = "%u,%u,%d,%d,%.2f,%.2f,%u,%u,%d,%d,%d";
static const char	*csRECORD_FORMAT_Skill = "%llu,%d,%d,%.2f,%.2f,%u,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_Ability = "%u,%d,%d,%.2f,%.2f,%u,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_GameWay = "%u,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_IllegalEnter = "%u,%s,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_IB = "%u,%u,%s,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_Cheat = "%s,%llu,%d,%u,%u,%f,%f,%u,%d,%d";
static const char	*csRECORD_FORMAT_Trade = "%u,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%u,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%d,%d";
static const char	*csRECORD_FORMAT_WorlShop = "%u,%u,%d,%d,%u,%d,%d,%d,%d,%d,%u,%u,%d,%d";
static const char	*csRECORD_FORMAT_ReturnToChild = "%d,%u,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_HorseSkillChange = "%u,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_IBMoney = "%llu,%s,%d,%u,%u,%u,%d,%u,%u,%u,%d,%d";
static const char	*csRECORD_FORMAT_IBCost = "%llu,%s,%d,%u,%u,%u,%d,%u,%d,%d,%d";
static const char	*csRECORD_FORMAT_StockTrade = "%u,%u,%d,%d,%d,%d,%d,%u,%u,%d,%d";
static const char	*csRECORD_FORMAT_StockAcc = "%u,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_Guild = "%d,%s,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_ZhuanJing = "%llu,%d,%d,%.2f,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_PlayerUseSkillCount = "%llu,%d,%.2f,%.2f,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_RideHorse = "%u,%u,%d,%d,%.2f,%.2f,%u,%u,%d,%d,%d";
static const char	*csRECORD_FORMAT_LoginOut = "%d,%lld,%s,%llu,%s,%s,%s,%s,%d,%u,%u,%d,%d,%d";
static const char	*csRECORD_FORMAT_BillingOrderInfo = "%s,%llu,%s,%f,%d,%d,%d,%s,%d";
static const char	*csRECORD_FORMAT_HeroGrowInfo = "%llu,%s,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_AwardPost ="%d,%llu,%d,%s,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_Post = "%d,%s,%s,%s,%d,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_ArtifactLogInfo = "%llu,%s,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_FindTreasureLogInfo = "%llu,%s,%d,%d,%d";
static const char	*csRECORD_FORMAT_HeroTransferLogInfo = "%llu,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_IndentureLogInfo = "%llu,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
static const char	*csRECORD_FORMAT_PlayerEffectUp = "%llu,%s,%d,%d,%u,%u";
#endif /* _RECORD_FORMAT_H_??? */
