/*$T Share/DBSystem/DataBase/SqlTemplate.cpp GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "SqlStatement.h"

//char	*g_StrLoadPlayerList = "select playerguid,sex,playername,level,haircolordata,facecolordata,hairmodeldata,facemodeldata,map,profession, headid,country, isfirstlogin,settings,soulid,gemaff,setsuit,levelaff,isneedrename,func_get_mountmodelid(playerguid,hhorseguid,lhorseguid) from %s where accountname =\'%s\' and valid =1";
char	*g_StrLoadPlayerList = "select playerguid,playername,level,map,isfirstlogin from %s where accountname =\'%s\' and valid =1";
char	*g_StrSelectByPlayerName = "select playerguid from t_player where playername = '%s'";

char	*g_StrLoadPlayerListDynamicCondition = " and country=%d";
char	*g_StrLoadPlayerEquipList = "select itemclass,slot from %s where playerguid =%llu and slot>=%d and slot<%d and valid =1";

char	*LoadCharCreateTime = "select createtime from t_player where playerguid=%llu";

char	* g_IsOrderExist = "select count(*) from t_orderlist where orderid =\'%s\'";
char	* g_InsertOrder = "insert into t_orderlist(orderid, productid, buyoktime, chargediamond, donatediamond, price) values (\'%s\', %d, \'%s\', %d, %d, %d)";

char	*g_CheckCDKey = "select count(*) from t_cdkey where cdkeytype =%d and guid =%u";
char	*g_LoadCDKeyCount = "select count(*) from t_cdkey where cdkey =\'%s\'";
char	*g_LoadCDKey = "select cdkey,title,cdkeyclass,cdkeytype,iscount,begintime,endtime,guid,item1,itemcount1,item2,itemcount2,item3,itemcount3,item4,itemcount4,item5,itemcount5,item6,itemcount6,item7,itemcount7,item8,itemcount8,item9,itemcount9,item10,itemcount10,valid from t_cdkey where cdkey =\'%s\'";
char	*g_InsertCDKey = "insert into t_cdkey(cdkey,title,cdkeyclass,cdkeytype,iscount,begintime,endtime,guid,item1,itemcount1,item2,itemcount2,item3,itemcount3,item4,itemcount4,item5,itemcount5,item6,itemcount6,item7,itemcount7,item8,itemcount8,item9,itemcount9,item10,itemcount10,valid) values(\'%s\',\'%s\',%d,%d,%d,%u,%u,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,1)";
char	*g_UpdateCDKey = "update t_cdkey set guid=%u,valid = 0 where cdkey =\'%s\'";

char	*LoadCharGuild = "select guildid,guildname,isneedrename from %s where chiefguid =%u";

char	*LoadGlobalPlayerInfo = "select accountname,playerguid,playername,playersimpledata from t_player where valid =1";

char	*g_StrLoadPlayerFullData = "select accountname,isfirstlogin,playerguid,playername,createtime,level,exp,silver,diamon,rmbdiamon,magicstone,herosoul,recallpaper,peakarenacoin,honor,crystal,heroexp,spirit,spiritbuycount,physicalpower,physicalbuycount,arenabuycount,arenaleftcount,arenawinstreak,arenamaxrank,arenachallengecd,energy,hounor,peakarenabuycount,peakarenaleftcount,peakarenawinstreak,peakarenamaxrank,peakarenachallengecd,zhanyidetail,achievement,dailytask,growup,orderlist,shopdata,bankallmoney,questdata,onlinetimetoday,logintime,logouttime,lastloginip,map,x,z,hp,totalonlinetime,crc,isdelchar,lockdata,lastresetyearday,nwvipexp,nwviplevel,playersimpledata,rmb,chargesum,donatediamond,dailygetphysicaltime,arenadata,macaddress,channelid,idfa,deviceos,platuserid,guildid,leaveguildtime,guildmakedishcount,guildiseatdinner,guildlastmakedishtime,guildbossbattlecount,guildcontribute,brothercoin,forbidlogintime,forbidchattime,vipawardflag,dailygetphysicalcount, arenarefcount,guildbossbattlebuycount,chargeallsum, donateallsum, chargeallsumcurrday, donateallsumcurrday, gameserverid, battleteamindex, godblessing, kungfurelic, wackygadge, vikingheirloom, zoosouvenir,friendpoint,invitecodeplayerid, facebookid, delfriendnum, robothelpfightnexttime, imei, accountid, registerip, telecomoper, devicehardware, devicesoftware, screenwidth, screenhight, density, cpuhardware, memory, glrender, glversion, deviceid, copp  from %s where playerguid = %llu and valid = 1";

char	*g_StrLoadPlayerMaxSnid = "select globalvalue from t_globalval where globaltype = 0";

char	*g_StrLoadPlayerMaxAid = "select aid from t_player order by aid desc limit 1";

char	*g_StrLoadPlayerMinAid = "select aid from t_player order by aid asc limit 1";

char	*g_StrLoadPlayerSnidByAid = "select playerguid from t_player where aid = %d and valid =1";

char	*g_StrLoadPlayerCount = "select count(*) from t_player";

char	*g_StrUpdatePlayerFullData = "update %s  set isfirstlogin=%d,playername=\'%s\',level=%d,exp=%llu,silver=%d,diamon=%d,rmbdiamon=%d,magicstone=%d,herosoul=%d,recallpaper=%d,peakarenacoin=%d,honor=%d,crystal=%d,heroexp=%d,spirit=%d,spiritbuycount=%d,physicalpower=%d,physicalbuycount=%d,arenabuycount=%d,arenaleftcount=%d,arenawinstreak=%d,arenamaxrank=%d,arenachallengecd=%d,energy=%d,hounor=%d,peakarenabuycount=%d,peakarenaleftcount=%d,peakarenawinstreak=%d,peakarenamaxrank=%d,peakarenachallengecd=%d,zhanyidetail=\'%s\',achievement=\'%s\', dailytask=\'%s\', growup=\'%s\', orderlist=\'%s\',shopdata=\'%s\',bankallmoney=%llu,onlinetime=%u,onlinetimetoday=%u,totalonlinetime=%u,logintime=%d,logouttime=%d,lastloginip=\'%s\',version=%d,map=%d,x=%d,z=%d,hp=%d,crc=%u,playerworld=%d,lastresetyearday=%d,nwvipexp=%u,nwviplevel=%d,playersimpledata=\'%s\',rmb=%d,chargesum=%d,donatediamond=%d,dailygetphysicaltime=%u,arenadata=\'%s\',guildid=%d,leaveguildtime=%u,guildmakedishcount=%d,guildiseatdinner=%d,guildlastmakedishtime=%u,guildbossbattlecount=%d,brothercoin=%d,vipawardflag=%d, dailygetphysicalcount=%d, arenarefcount=%d, guildbossbattlebuycount=%d,chargeallsum=%d, donateallsum=%d, chargeallsumcurrday=%d, donateallsumcurrday=%d, guildcontribute=%d, battleteamindex=%d, godblessing=%d, kungfurelic=%d, wackygadge=%d, vikingheirloom=%d, zoosouvenir=%d, friendpoint=%d,invitecodeplayerid=%llu, facebookid=\'%s\', delfriendnum=%d,robothelpfightnexttime=%u, questdata = \'%s\' where playerguid = %llu and valid = 1 and version<=%d";

char	*g_StrLoadPlayerLockData = "select lockdata, forbidlogintime from t_player where playerguid = %llu and valid =1";

char	*g_StrLoadPlayerAccountName = "select accountname from t_player where aid = %d and valid =1";
char	*g_StrSavePlayerAccountName = "update t_player set accountname=\'%s\' where aid = %d and valid =1";

char	*g_StrLoadPlayerFullDataExt = "select offlineaward,playercdkey,assigngift,zhanyifinishdata,friendgeminfo,artifactlist,godtower,herohandbook,herohandbookreward,sports,worldboss,findtreasure,superego,shopcard,vipext,pushnotifications,specialgiftmall,dreamtemple,guildbossinfo,peakarenadata from t_playerext where playerguid = %llu";

char	*g_StrUpdatePlayerFullDataExt = "call proc_updata_playerext(%llu,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')";

char	*g_StrUnLockPlayer = "update %s  set lockdata = 0 where playerguid = %llu and valid = 1";

char	*g_StrLockPlayer = "update %s  set lockdata = 1 where playerguid = %llu and valid = 1";

char	*g_StrForbidPlayerLoginTime = "update %s  set forbidlogintime = %u where playerguid = %llu and valid = 1";

char	*g_StrForbidPlayerChatTime = "update %s  set forbidchattime = %u where playerguid = %llu and valid = 1";

char	*g_StrQueryPlayerVersion = "select version from %s where playerguid=%llu";

char	*g_StrLoadPlayerItemList = "select playerguid,world,server,guid,itemclass,bagtype,slot,dataproperty,creator,data,count from %s where playerguid = %llu and valid=1 and version=%d";

char	*g_StrDeletePlayerItemList = "update %s set valid=0 where playerguid = %llu";

char	*g_StrLoadPlayerHeroCardList = "select playerguid,heroid,slot,battleslot1,battleslot2,battleslot3,dataproperty,skillproperty,indenturelist from %s where playerguid = %llu and valid=1 and version=%d";

char	*g_StrDeletePlayerHeroCardList = "update %s set valid=0 where playerguid = %llu";

char	*g_StrLoadPlayerSkillList = "select playerguid,skid,skstate,sklevel from %s where playerguid = %llu and version=%d order by aid";

char	*g_StrDeletePlayerSpellList = "update %s set version=-1 where playerguid = %llu";

char	*LoadCharTalentList = "select playerguid,talentid,talentlvl from %s where playerguid = %llu and version=%d";

char	*DeleteCharTalentList = "delete from %s where playerguid = %llu and version<=%d";

char	*g_StrLoadPlayerAbilityList = "select playerguid,skillid,skilllvl,skillexp from %s where playerguid = %llu and version=%d";

char	*g_StrLoadPlayerPresList = "select playerguid,skillpres from %s where playerguid = %llu and version=%d";

char	*g_StrDeletePlayerAbilityList = "update %s set version=-1 where playerguid = %llu";

char	*g_StrLoadPlayerTaskList = "select playerguid,questid,scriptid,flag,param1,param2,param3,param4,param5,param6,param7,param8 from %s where playerguid = %llu and version=%d";

char	*g_StrLoadPlayerTaskDataAndFlag = "select playerguid,questdata from %s where playerguid = %llu and version=%d";

char	*g_StrLoadPlayerPrivateInfo = "select playerguid,privateinfo from %s where playerguid=%llu and version=%d";

char	*g_StrLoadPlayerTitleInfo = "select playerguid,titleinfo from %s where playerguid=%llu and version=%d";

char	*g_StrLoadPlayerCoolDownInfo = "select playerguid,cooldown from %s where playerguid=%llu and version=%d";

char	*g_StrDeletePlayerTaskList = "update %s set version=-1 where playerguid = %llu";

char	*NewCharRelationList = "call proc_save_friendinfo(%u,%u,%d,\'%s\',%d,%d,%d,%u,\'%s\',\'%s\',%d,%d,%d,%d,%u,%d,%d,%d,%d)";

char	*g_StrLoadPlayerRelationList = "select playerguid,fguid,ftype,fname,flevel,fprofession,fportrait,fguild,fguildname,ftitle,fmapid,fteamsize,fgovernmentpos,fcountry,fmasterguid,fincreasefptime,flock,ffriendpoint from t_friend where playerguid = %llu and version=%d";

char	*g_StrDeletePlayerRelationList = "update %s set version=-1 where playerguid = %llu";

char	*g_StrLoadPlayerRelationData = "select playerguid,dispelmastertime,dispelprenticetime,totalprentice,sendgifttime,sendgifttimes,patrolid,curpathnode from %s where playerguid = %llu and valid =1 and version=%d";

char	*g_StrUpdatePlayerRelationData = "update %s set dispelmastertime=%u, dispelprenticetime=%u, totalprentice=%d, sendgifttime=%u, sendgifttimes=%d, patrolid=%d, curpathnode=%d where playerguid=%llu and valid=1 and version=%d";

char	*LoadCharMasterInfo = "select masterguid from %s where playerguid = %llu and valid=1";

char	*NewCharMasterInfo = "call proc_save_masterinfo(%u,%u)";

char	*DeleteCharMasterInfo = "update %s set valid=0 where playerguid = %llu";

char	*LoadCharPrenticeInfo = "select prenticeguid,offlinetime from %s where playerguid = %llu and valid=1";

char	*NewCharPrenticeInfo = "call proc_save_prenticeinfo(%u,%u,%u)";

char	*DeleteCharPrenticeInfo = "update %s set valid=0 where playerguid = %llu";

char	*LoadCharMarryInfo = "select spouseguid,marrytime,ishavewedding from %s where playerguid = %llu and valid=1";

char	*NewCharMarryInfo = "call proc_save_marryinfo(%u,%u,%u,%d)";

char	*DeleteCharMarryInfo = "update %s set valid=0 where playerguid = %llu";

char	*g_StrUpdatePlayerPresList = "update %s set skillpres = \'%s\' where playerguid = %llu and  valid = 1 and version = %d";

char	*g_StrLoadPlayerEffectList = "select playerguid,effectdata from %s where playerguid = %llu and version = %d";
char	*g_StrDeletePlayerEffectList = "update %s set version=-1 where playerguid = %llu";

char	*g_StrLoadPlayerHorseList = "select playerguid ,hhorseguid,lhorseguid,dataid,horsename,horsenick,level,takelevel,propertytype,behaviortype,campdata,genera,enjoyvalue,strpervalue,conpervalue,dexpervalue,iprpervalue,growratevalue,repointvalue,exp,strvalue,convalue,dexvalue,iprvalue,skilldata,strpoint,smartpoint,mindpoint,conpoint,modelid,mat,happinesslefttime,stallorder,islock,unlocktime from %s where playerguid =%llu and version=%d order by stallorder asc";

char	*g_StrDeletePlayerHorseList = "update %s set version=-1 where playerguid = %llu";

char	*g_StrCreateNewPlayer = "call proc_create_newplayer(\'%s\',\'%s\',\'%s\',\'%s\',%d,%d,%d,%llu,%d,%d,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,%d,%f,\'%s\',%d,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')";

char	*g_StrCreateMergePlayer = "call proc_create_mergeplayer(%llu,\'%s\',\'%s\',\'%s\',\'%s\',%d,%d,%d,%llu,%d,%d,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,%d,%f,\'%s\',%d,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')";

char	*g_StrFetchNewGuid = "call proc_fetch_guid()";

char	*g_StrDeleteBasePlayer = "call proc_delete_player_new(\'%s\',%llu)";

char	*SaveItemInfo = "call proc_save_iteminfo(%llu,%d,%d,%d,%d,%d,%d,\'%s\',%d,\'%s\',\'%s\',%d)";

char	*SaveHeroCardInfo = "call proc_save_herocardinfo(%llu,%d,%d,%d,%d,%d,%d,\'%s\',\'%s\',\'%s\')";

char	*g_StrNewPlayerSkill = "call proc_save_spellinfo(%u,%d,%d,%d,%d)";

char	*NewCharTalent = "insert into %s(playerguid,talentid,talentlvl,version) values(%llu,%d,%d,%d)";

char	*g_StrNewPlayerAbility = "call proc_save_skillinfo(%u,%d,%d,%d,%d)";

char	*g_StrNewPlayerTask = "call proc_save_questinfo(%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)";

char	*g_StrUpdatePlayerTaskData = "update %s  set questdata=\'%s\' where playerguid = %llu and valid = 1 and version=%d";

char	*g_StrUpdatePlayerTaskFlag = "update %s set questflag=\'%s\' where playerguid = %llu and valid = 1 and version=%d";

char	*g_StrUpdatePlayerPrivateInfo = "update %s set privateinfo=\'%s\' where playerguid=%llu and valid=1 and version=%d";

char	*g_StrUpdatePlayerTitleInfo = "update %s set titleinfo=\'%s\'where playerguid=%llu and valid=1 and version=%d";

char	*g_StrUpdatePlayerCoolDownInfo = "update %s set cooldown=\'%s\'where playerguid=%llu and valid=1 and version=%d";

char	*g_StrNewPlayerEffect = "call proc_save_effectinfo(%u,\'%s\',%d)";

char	*g_StrNewPlayerHorse = "call proc_save_horseinfo(%u,%u,%u,%d,\'%s\',\'%s\',%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u)";

char	*g_StrLoadServerItemSerial = "select sid,sn from %s where sid = %u";

char	*g_StrUpdateServerItemSN = "update %s set sn = %u where sid = %u";
// 普通邮件 [xingzhishan]
char	*g_StrLoadWorldPostInfo = "select pindex,mailid,sendername,receivername,mailtitle,mailcontent,playerguid,portrait,flag,createtime,isread,isimport,awarddata from %s where valid=1";

char	*g_StrSaveGLPostInfo = "call proc_save_mailinfo(\'%s\',\'%s\',\'%s\',\'%s\',%llu,%u,%d,%u,%u,%d,\'%s\',%d,%d,%d)";

char	*g_StrDeleteWorldPostInfo = "update t_mail set valid = 0 where mailid=%u";

char	*ClearInvalidPostSql = "update t_mail set valid = 0 where valid = 1 and playerguid <> 18446744073709551615 and receivername not in (select playername from t_player)";

char	*DeletePostByCharName = "delete from t_mail where receivername=\'%s\'";

// 邮件对应关系 [wangyiran]
char	*g_StrLoadWorldPostRelationInfo = "select playerguid,relationdata,readflag,importflag from %s";

char	*g_StrSaveGLPostRelationInfo = "call proc_save_postrelation(%llu, \'%s\', \'%s\', \'%s\')";

// 奖励邮件 [xingzhishan]
char	*g_StrLoadAwardPostInfo = "select aid,playerguid,flag,title,awarddata,createtime,losetime,activityflag,awardtype from %s where valid=1";

char	*g_StrUpdateAwardPostInfo = "update t_awardpost set playerguid=%llu, flag=%d, title=\'%s\', awarddata=\'%s\', createtime=%u, losetime=%u, activityflag=%d, awardtype=%d, valid=%d  where aid=%d";

char	*g_StrDeleteAwardPostInfo = "update t_awardpost set valid = 0 where aid=%d";

char	*g_StrLoadGlabalValue = "select CAST(globalvalue as UNSIGNED) from t_globalval where globaltype = %d";

char	*g_StrSaveGlabalValue = "call proc_save_globalval(%d, \'%s\')";

char	*g_StrLoadWorldGuildInfo = "select guildid,guildname,guildnotice,chairmanid,chairmanname,guildlvl,guildicon,guildbackicon,memcount,time,joinlvl,joinlimit,lastresetyearday,activity,guildfund,dinnertype,dinnerdata,nutritiontoplist,troopnum,bossinfo,bossawardinfo,bossitemlog,prayinfo,bossresetcount,bossnextresettime from %s where valid=1";

char	*g_StrSaveGLGuildInfo = "call proc_save_guildinfo(%d,\'%s\',\'%s\',%llu,\'%s\',%d,%d,%d,%d,%u,%d,%d,%d,%d,%d,%d,\'%s\',\'%s\',%d,\'%s\',\'%s\',\'%s\',\'%s\', %d, %d, %d)";

char	*g_StrDeleteWorldGuildInfo = "update t_guild set valid=0 where guildid=%u";
char	*g_StrDeleteALLWorldGuildInfo = "update t_guild set valid=0";

char	*LoadGuildMember = "select guildid,userguid,username,position,lastlogintime,level,heroid,fight,herograde,herostar,areanpoint,allcontribution,weekcontribution,contributecount,applyitem,applynum from %s";

char	*InsertGuildMember = "insert into %s(guildid,userguid,username,position,lastlogintime,level,heroid,fight,herograde,herostar,areanpoint,allcontribution,weekcontribution,contributecount,applyitem,applynum) values(%d,%llu,\'%s\',%d,%d,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)";

char	*DeleteGuildMember = "delete from %s where guildid=%d";
char	*DeleteAllGuildMember = "delete from %s";
char	*g_StrDeleteInvalidMembers = "DELETE FROM t_guildmember WHERE guildid IN (SELECT guildid FROM t_guild WHERE valid = 0)";

char	*LoadGuildSkill = "select guildid,skillid,skillindex,skilllevel,skilllayer from %s";

char	*DeleteGuildSkill = "delete from %s where guildid=%d";

char	*InsertGuildSkill = "insert into %s(guildid,skillid,skillindex,skilllevel,skilllayer) values(%d,%d,%d,%d,%d)";

char	*LoadWorldCountryInfo = "select countryid,kingguid,kingguildid,kingname,challenger,challengerguildid,challengername,guid_queen,guid_general,guid_lminister,guid_rminister,guid_lguard,guid_rguard,name_queen,name_general,name_lminister,name_rminister,name_lguard,name_rguard,kingsuit,kingqicheng,resource0,resource1,resource2,resource3,notice, intervalcontrol, freeparam, crc, qinglongbang, qinglongbangzhuguid, qinglongbangzhuname, zhuquebang, zhuquebangzhuguid, zhuquebangzhuname, qinglongbang_challenge, qinglongbangzhuguid_challenge, qinglongbangzhuname_challenge, zhuquebang_challenge, zhuquebangzhuguid_challenge, zhuquebangzhuname_challenge, empire_data, empire_notice from %s";

char	*SaveWorldCountryInfo = "update t_country set kingguid=%u, kingguildid=%d, kingname=\'%s\',challenger=%u, challengerguildid=%d, challengername=\'%s\',guid_queen=%u,guid_general=%u,guid_lminister=%u,guid_rminister=%u,guid_lguard=%u,guid_rguard=%u,name_queen=\'%s\',name_general=\'%s\',name_lminister=\'%s\',name_rminister=\'%s\',name_lguard=\'%s\',name_rguard=\'%s\',kingsuit=%d,kingqicheng=%d,resource0=%d,resource1=%d,resource2=%d,resource3=%d,notice=\'%s\', intervalcontrol=\'%s\', freeparam=\'%s\', crc=%u, qinglongbang=%d, qinglongbangzhuguid=%u, qinglongbangzhuname=\'%s\', zhuquebang=%d, zhuquebangzhuguid=%u, zhuquebangzhuname=\'%s\', qinglongbang_challenge=%d, qinglongbangzhuguid_challenge=%u, qinglongbangzhuname_challenge=\'%s\', zhuquebang_challenge=%d, zhuquebangzhuguid_challenge=%u, zhuquebangzhuname_challenge=\'%s\', empire_data=\'%s\', empire_notice=\'%s\' where countryid=%u";

char	*DeleteWorldCountryInfo = "delete from t_country";

char	*LoadMapVarInfo = "select sid,data from %s order by sid asc";

char	*SaveMapVarInfo = "update %s set data =\'%s\' where sid = %d";

char	*LoadTopList = "select aid,toplisttype,dataversion,toplistkey,toplistdata,guildbossdata,forbidsort,fightval from t_toplist";

char	*SaveTopListInfo = "call proc_add_toplist(%d,%d,%d,\'%s\',\'%s\',\'%s\',%d,%d,%llu)";

char	*DelTopListInfo = "truncate %s";

char	*LoadArenaTopList = "select place,guid,level,playername,fightappraise,headid,herograde,point,team,lastweekplace from %s";

char	*InquireArenaRank = "select place from %s where guid = %u";

char	*SaveArenaTopList = "call proc_save_arenatoplist(%d,%llu,%d,\'%s\',%d,%d,%d,%d,%d,%d)";

char	*DelArenaTopListInfo = "truncate t_arenatoplist";

char	*DelPeakArenaTopListInfo = "truncate t_peakarenatoplist";

char	*SavePeakArenaTopList = "call proc_save_peakarenatoplist(%d,%llu,%d,\'%s\',%d,%d,%d,\'%s\',%f,%d)";

char	*InquirePeakArenaRank = "select place from %s where guid = %u";

char	*LoadPeakArenaTopList = "select place,guid,level,playername,fightappraise,heroindex,herograde,team,accumulateaward,wincount from %s";

char	*CheckDBConnection = "select CAST(globalvalue as UNSIGNED) from t_globalval where globaltype = 0";

char	*CheckDBVersion="select verNum from t_version";

char	*ChangeCharName = "call proc_change_player_name(%llu,\'%s\')";

char	*ChangeGuildName = "call proc_change_guild_name(%u,\'%s\')";

char	*InsertShopData = "insert into %s(serialid,shopid,playerguid,playername,gold,charge,price,createTime,deleteTime,buyerguid) values(%u,%d,%llu,\'%s\',%d,%d,%d,%u,%u,%u)";

char	*g_StrLoadShopInfo = "select serialid,shopid,playerguid,playername,gold,charge,price,createTime,deleteTime, buyerguid from %s where valid=1";

char	*DeleteShopData = "update %s set serialid = -1,valid = 0,deleteTime=%d,buyerguid=%u where serialid = %d";

char	*InsertBankData = "insert into %s(serialid,shopid,playerguid,gold,siller,charge,createTime,deleteTime,buyerguid) values(%u,%d,%llu,%d,%d,%d,%u,%u,%u)";

char	*LoadBankInfo = "select serialid,shopid,playerguid,gold,siller,charge,createTime,deleteTime, buyerguid from %s where valid=1";

char	*DeleteBankData = "update %s set serialid=-1, valid = 0,deleteTime=%d where serialid=%d and valid=1";

char	*LoadMasterCandidate = "select playerguid,playername from %s";

char	*SaveMasterCandidate = "insert into %s(aid,playerguid,playername) values(%u,%llu,\'%s\')";

char	*DeleteMasterCandidate = "truncate %s";

char	*LoadPrenticeCandidate = "select playerguid,playername from %s";

char	*SavePrenticeCandidate = "insert into %s(aid,playerguid,playername) values(%u,%llu,\'%s\')";

char	*DeletePrenticeCandidate = "truncate %s";

//当前正在交易的股票信息
char	*InsertStockTradeData = "insert into %s (serialId,playerguid,playername,price,gold,charge,tradeType,createTime,deleteTime) values(%u,%llu,\'%s\',%d,%u,%d,%d,%u,%u)";

char	*LoadStockTradeInfo = "select serialId,playerguid,playername,price,gold,charge,tradeType,createTime,deleteTime from %s where valid=1";

char	*SaveStockTradeData = "call proc_update_stocktrade(%u,%u,\'%s\',%d,%u,%d,%d,%u,%u)";

char	*DeleteStockTradeData = "update %s set valid = 0,deleteTime=%u where serialid=%u and valid=1";

//用户股票账户信息
char	*InsertStockAccData = "insert into %s (playerguid,playername,gold,siller,lasttradetime,createtime,lockstate) values(%llu,\'%s\',%d,%d,%u,%u,%d)";

char	*LoadStockAccInfo = "select playerguid,playername,gold,siller,lasttradetime,createtime,lockstate,locktime from %s where valid=1";

char	*UpdateStockAccData = "call proc_update_stockaccount(%u,\'%s\',%d,%d,%u,%u,%d,%u)";

char	*DeleteStockAccData = "update %s set gold=%d,siller=%d,valid=0 where playerguid=%llu and valid=1";

//股票交易的历史记录
char	*InsertStockRecordData = "insert into %s (serialid,playerguid,tradetime,tradetype,tradegold,tradeprice) values(%u,%llu,%u,%d,%u,%d)";

char	*LoadStockRecordInfo = "select serialid,playerguid,tradetime,tradetype,tradegold,tradeprice from %s where valid=1";

char	*DeleteStockRecordData = "update %s set serialid=%u,valid=0 where serialid=%u and valid=1";

char    *LoadCharExtInfo = "select shopfavorite,shopbuyhistory,battlehonour,physicalforce, maxphysicalforce,critRate_balance,toughness_balance,hit_balance,miss_balance,critDamage_balance,critDamageDerate_balance,ctrlHit_balance,ctrlMiss_balance,wushi_zhangong_level,jianxia_zhangong_level,qishe_zhangong_level,huoqiang_zhangong_level,xianzhi_zhangong_level,saman_zhangong_level,shizhijun_zhangong_level,shenghuoshi_zhangong_level,cike_zhangong_level,jinweijun_zhangong_level,yinxiushi_zhangong_level,lama_zhangong_level,wushi_zhangong_point,jianxia_zhangong_point,qishe_zhangong_point,huoqiang_zhangong_point,xianzhi_zhangong_point,saman_zhangong_point,shizhijun_zhangong_point,shenghuoshi_zhangong_point,cike_zhangong_point,jinweijun_zhangong_point,yinxiushi_zhangong_point,lama_zhangong_point,total_zhangong_coefficient,wushi_zhangong_coefficient,jianxia_zhangong_coefficient,qishe_zhangong_coefficient,huoqiang_zhangong_coefficient,xianzhi_zhangong_coefficient,saman_zhangong_coefficient,shizhijun_zhangong_coefficient,shenghuoshi_zhangong_coefficient,cike_zhangong_coefficient,jinweijun_zhangong_coefficient,yinxiushi_zhangong_coefficient,lama_zhangong_coefficient,tatol_zhanfang_point,wushi_zhanfang_level,jianxia_zhanfang_level,qishe_zhanfang_level,huoqiang_zhanfang_level,xianzhi_zhanfang_level,saman_zhanfang_level,shizhijun_zhanfang_level,shenghuoshi_zhanfang_level,cike_zhanfang_level,jinweijun_zhanfang_level,yinxiushi_zhanfang_level,lama_zhanfang_level,militaryrank_level,militaryrank_onlinetime,militaryrank_todayonlinetime,militaryrank_todaydate,hellsubskill,chariot_honour,chariot_honour_data,hpetguid,lpetguid,consummation,strvalue_up_total,convalue_up_total,iprvalue_up_total,dexvalue_up_total from %s where playerguid=%llu and valid=1";
char	*DeleteCharExtInfo = "update %s set valid=0 where playerguid=%llu";

char	*InsertCharExtInfo = "call proc_save_playerextinfo(%u,\'%s\',\'%s\',%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%u,%u,\'%s\',%u,\'%s\',%u,%u,\'%s\',%d,%d,%d,%d)";

//动态价格商店
#if 0
	char	*LoadDynamicShopInfo = "select shopid,shopindex,count,price,tick from %s";
	char	*SaveDynamicShopInfo = "insert into %s (shopid,shopindex,count,price,tick) values(%d,%d,%d,%d,%d)";
	char	*DeleteDynamicShopInfo = "delete from %s";
#else
	// add by dragon 动态随机商店
	char	*LoadDynamicShopInfo = "select shopid,shopindex,price from %s order by shopid";
	char	*SaveDynamicShopInfo = "insert into %s (shopid,shopindex,price) values(%d,%d,%d)";
	char	*DeleteDynamicShopInfo = "truncate %s";
#endif

// 2011-4-14 by rj 战车数据表操作 sql
char	*g_StrLoadPlayerChariotList = "select playerguid, hguid, lguid, dataid, name, level, hp, mp, element, growrate, skilldata, equipdata from %s where playerguid = %llu and version = %d";
char	*g_StrDeletePlayerChariotList = "update %s set version=-1 where playerguid = %llu";
char	*g_StrNewPlayerChariot = "call proc_save_chariotinfo(%u,%u,%u,%d,\'%s\',%d,%d,%d,%d,%d,\'%s\',\'%s\',%d)";

char	*g_LoadLostItemInfo = "select aid,playerguid,playername,guid,world,server,itemclass,creator,dataproperty,data,count,optype,isnormal from %s where aid>%u and valid =1 order by aid asc" ;
char	*g_DelLostItemInfo = "update %s set valid = 0 where aid=%u";
char	*g_UpdateLostItemInfo = "update %s set optype = %d where aid=%u";

// 跨服战积分相关处理
char	*LoadMPVPPointInfo = "select unitid, unitname, world, currentpoint, totalpoint, currentwintimes, currentjointimes, totalwintimes, totaljointimes, mpvptypename, currentweek, startweek, params from %s where valid = 1";
char	*SaveMPVPPointInfo = "call proc_save_mpvppoint(%d,\'%s\', %d, %d, %d, %d, %d, %d, %d, \'%s\', %d, %d, %d, 1)";
char	*DelMPVPPointInfo = "update %s set valid=0 where (unitid = %d and world = %d) or (world = -1)";

// 坐骑装备
char	*g_StrLoadHorseItemList = "select playerguid,hhorseguid,lhorseguid,world,server,guid,itemclass,slot,dataproperty,creator,termendtime,data,islock,unlocktime from %s where playerguid = %llu and hhorseguid = %u and lhorseguid = %u and valid=1 and version=%d";

char	*SaveHorseItemInfo = "call proc_save_horseequip(%u,%u,%u,%d,%d,%d,%d,%d,\'%s\',%d,\'%s\',%d,\'%s\',%d,%d,%u)";

char	*g_StrDeleteHorseItemList = "update %s set valid=0 where playerguid = %llu";

//	替身
char	*LoadSubstituteInfo = "select playerguid, playername, mapid, xpos, zpos, profession, extexp, offlinetime, camp, sex, level, portraitid, battlevalue, onevsonegrade, onevsoneweek, onevsonecurgrade, maxhp, attacknear, attackfar, defencenear, defencefar, attackmagic, defencemagic, toughness, miss, hit, critical, reducenear, reducefar, reducemagic, crithurt, pettype, petattacknear, petattackfar, petattackmagic from t_substitute where valid = 1 order by onevsonecurgrade asc";

char	*SaveSubstituteInfo	= "call proc_save_substitute(%u, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)";

char	*DelSubstituteInfo	= "update %s set valid=0";

//	家园
char	*LoadGardenInfo = "select gardenid, playerguid, playername, level, viewgroup, flag, monster, treelevel, treeenergy, treehealth, treesupplyfriend, treesupplyself, fruitlevel, fruitnum, version, treeinfo from %s where valid = 1 and playerguid = %llu";
char	*SaveGardenInfo = "call proc_save_garden(%u, %u,\'%s\', %u, %u, %d, \'%s\', %d, %d, %d, %d, %d, %d, \'%s\', %d, \'%s\')";

//家园资源
char	*LoadGardenResInfo = "select gardenid, resourceid, resourcetype, resourcepos, resourcenum, resourcestate, resourcelevel, maturetime from %s where valid = 1 and gardenid = %u";
char	*SaveGardenResInfo	= "call proc_save_gardenres(%u, %u, %d, %d, %d, %d, %d, %u)";

//家园子资源
char	*LoadGardenSubResInfo = "select gardenid, respos, subpos, substate from %s where valid = 1 and gardenid = %u and respos = %d";
char	*SaveGardenSubResInfo	= "call proc_save_gardensubres(%u, %d, %d, %d)";

//家园事件
char	*LoadGardenEventInfo = "select snid, eventtype, eventid, eventtitle, eventtime, nindex from %s where snid = %u order by nindex";
char	*SaveGardenEventInfo = "call proc_save_gardenevent(%u,%d,%d,\'%s\',%u,%d)";

// 宠物
char	*g_StrDeletePlayerPetList = "update %s set version=-1 where playerguid = %llu";
char	*g_StrNewPlayerPet = "call proc_save_petinfo(%u,%u,%u,%d,\'%s\',\'%s\',%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%d,%d,%d,%u,%u,\'%s\')";
char	*g_StrLoadPlayerPetList = "select playerguid ,hpetguid,lpetguid,dataid,petname,petnick,level,takelevel,propertytype,behaviortype,campdata,genera,enjoyvalue,strpervalue,conpervalue,dexpervalue,iprpervalue,growratevalue,repointvalue,exp,strvalue,convalue,dexvalue,iprvalue,skilldata,strpoint,smartpoint,mindpoint,conpoint,modelid,mat,happinesslefttime,stallorder,islock,unlocktime,nearattack,farattack,magicattack,cultivatecount,cultivatetime,mustskilldata from %s where playerguid =%llu and version=%d order by stallorder asc";
//经脉
char	*LoadChannelMeridian = "select playersnid,modetype,flagtype,protype1,provalue1,protype2,provalue2,protype3,provalue3,protype4,provalue4,color1,color2,valid from %s where playersnid = %u and valid=1";
char	*DelChannelMeridian = "update %s set valid=0 where playersnid = %u";
char	*SaveChannelMeridian = "call proc_save_channelmeridian(%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)";

//其他人物数据
char	*LoadPlayerNumericalInfo = "select snid, currencyvalue, currencytype, valid from %s where snid = %u and valid=1";
char	*DeletePlayerNumericalInfo = "update %s set valid=0 where snid = %u";
char	*SavePlayerNumericalInfo = "call proc_save_playernumerical(%u,%d,%d,%d)";

//1v1全局变量 type=1,2
char	*g_StrLoadGlobal_1v1Data = "select globaltype, CAST(`globalvalue` AS CHAR) from %s where globaltype = 1 or globaltype = 2";
char	*g_StrSaveGlobal_1v1Data = "call proc_`save_globalval(%d, \'%s\')";

//面具
char	*LoadMaskInfo	= "select playersnid,itemid,masktype,maskindex,maskdata,valid from %s where playersnid = %u and valid=1";
char	*DelMaskInfo	= "update %s set valid=0 where playersnid = %u";
char	*SaveMaskInfo	= "call proc_save_maskdata(%u,%u,%d,%d,\'%s\',%d)";

// 跨服联赛纪录
char	*LoadMPVPLeaguePointInfo = "select worldid, countryid, guildid, guildname, signindex, wintimes from %s where valid = 1";
char	*SaveMPVPLeaguePointInfo = "call proc_save_mpvpleague_result(%d,%d,%d,\'%s\', %d, %d)";
char	*DelMPVPLeaguePointInfo = "update %s set valid=0 where (guildid = %d and worldid = %d) ";

//slg 建筑数据
char	*LoadSlgBuildInfo = "select playerguid,buildtype,buildlv,extdata,buildoutputtime,buildoutputstacknum, buildoutputvalue,buildoutputitemnum,valid from %s where playerguid = %llu and valid=1";
char	*DelSlgBuildInfo = "update %s set valid=0 where playerguid = %llu";
char	*SaveSlgBuildInfo = "call proc_save_slgbuildinfo(%u,%u,%u,\'%s\',%u,%u,%u,%u)";

//兵种、士兵
char	*g_LoadSoldierInfo = "select playersnid,armyindex,soldiertype,soldiercount,valid from %s where playersnid = %u and valid = 1";
char	*g_DelSoldierInfo = "update %s set valid=0 where playersnid = %u";
char	*g_SaveSoldierInfo = "call proc_save_soldierinfo(%u,%u,%d,%u)";


// 宠物孵化
char	*g_LoadPetHatchAll = "select playerguid, petindex, pethatchinfo from %s where playerguid = %llu and valid = 1";
char	*g_SavePetHatchInfo = "call proc_save_pethatchinfo(%u,%d,\'%s\')";
char	*g_DelPetHatchInfo = "update %s set valid = 0  where playerguid = %llu";


//slg battle (远东争霸)
char	*g_StrLoadSlgBattle = "select siteindex, camp from %s order by siteindex";
char	*g_StrSaveSlgBattle = "call proc_save_slgbattle(%d, %d)";

// GM命令添加离线奖励 [wangyiran]
char	*g_StrAddLostItemByGM = "call proc_add_lostitem(\'%s\',%u,%d,%d)";

char	*g_StrLoadWorldWeChatRedInfo = "select aid,wechatredid,money,circle,playerguid from t_wechatred";
char	*g_StrSaveGLWeChatRedInfo = "call proc_save_wechatredinfo(%d, \'%s\', %d, %d, %u)";

char	*g_StrLoadPlayerWeChatRedInfo = "select aid,playerguid,redpackdata,itempackdata from t_playerwechatred";
char	*g_StrSavePlayerWeChatRedInfo = "call proc_save_playerwechatredinfo(%d, %d, \'%s\', \'%s\')";

char	*g_StrLoadWorldFriendInfo = "select playerguid,getphysicalcount,givephysicalcount,lastresetday,byzancount,friendlist,applylist,zandata,minejiyang,friendjiyang,achievementlist from t_friend";
char	*g_StrSaveWorldFriendInfo = "call proc_save_friendinfo(%llu, %u, %d, %d, %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')";

//挖矿玩法[10/20/2015 TianShuai]
char	*g_StrLoadMiningInfo = "select playerguid, minesearchcount, robsearchcount, robcount, buyrobcount, robreftime, lastreftime, defendprompt, inviteprompte,\
						buffendtime,buffcount,robinfo,inviteinfo,defendinfo, mineinfo1, mineinfo2, mineinfo3 ,mineinfo4, mineinfo5, robinfover from t_mining";
char	*g_StrSaveMiningInfo = "call proc_save_miningInfo(%u, %d, %d, %d, %d, %u, %u, %u, %u, %u, %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', %d)";

//查询所有机器人的SNID
char	*g_StrLoadFakeSnid = "SELECT playerguid FROM t_player WHERE accountname LIKE \'Fake@FakePlayer_%%\'";

// 建筑信息 [2/24/2016 superrobert]
char	*g_StrLoadBuildingInfo = "select playerguid, lastrandomtime, buildinfo from t_building";
char	*g_StrSaveBuildingInfo = "call proc_save_buildinginfo(%llu, %u, \'%s\')";

char	*g_StrUpdatePlayerFullDataExt2 = "call proc_updata_playerext2(%llu,\'%s\',\'%s\',\'%s\',\'%d\', \'%d\' ,\'%d\', \'%d\', \'%d\', \'%s\', \'%s\', \'%s\')";
char	*g_StrLoadPlayerFullDataExt2 = "select effectupdata,zhanyichapteraward,dailytask7,drawheroid1,drawheroid2,drawheroid3,drawheroid4,drawheroid5,dailyAbattoir, limitGift, equipTreasure from t_playerext2 where playerguid = %llu";

char	*g_StrLoadActivityInfo = "SELECT act, info FROM t_activity WHERE valid = 1";
char	*g_StrSaveActivityInfo = "CALL proc_save_activity(\'%s\', \'%s\')";
char	*g_StrFindActivityInfo = "SELECT info FROM t_activity WHERE valid = 1 and act = \'%s\'";

char	*LoadAbattoirTopList = "select place,guid from %s";

char	*SaveAbattoirTopList = "call proc_save_abattoirtoplist(%d,%llu)";

char	*DelAbattoirTopListInfo = "truncate t_abattoirtoplist";