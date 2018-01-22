/*$T MapServer/Server/Base/LogAssistant.h GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef LogAssistant_h__15_10_2008_11_37
#define LogAssistant_h__15_10_2008_11_37

#include "TypeDefine.h"
#include "StructDB.h"

struct ItemLogRecordInfo;
class Player;

void FillItemLogCommonInf(ItemLogRecordInfo *pLogParam, Player *pPlayer);
//void FillEquipLogCommonInf(EquipLogRecordInfo *pLogParam, Player *pPlayer, Item *pEquip);
void FillServerIPOfItemLog(ItemLogRecordInfo *pLogParam);
void FillMoneyLogCommonInf(CurrencyLogRecordInfo *pLogParam, Player *pPlayer);

//void	FillStallLogCommonInf(TradeLogRecordInfo *pLogParam, Player *pBuyer, Player *pSeller, Item *pItem, int32 nCount, uchar MoneyType);
//void FillGamePlayLog(GameWayLogRecoreInfo *pLogParam, Player *pPlayer, int32 nGameID);
//void FillCSEnterLog(EnterMapLogRecordInfo *pLogParam, Player *pPlayer, int32 nGameID, int32 nDayCount);
void FillLawlessLog(CheatActLogRecordInfo *pLogParam, Player *pPlayer);
#endif
