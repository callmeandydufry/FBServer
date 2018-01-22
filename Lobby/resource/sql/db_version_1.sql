

--
-- Table structure for table `t_orderlist`  ������
--
DROP TABLE IF EXISTS `t_orderlist`;
CREATE TABLE `t_orderlist` (
  `aid` bigint(20) NOT NULL auto_increment,
  `orderid` varchar(500) NOT NULL default '0' COMMENT 'orderid',
  `productid` int(10)  NOT NULL COMMENT 'productid',
  `buyoktime` varchar(50) NOT NULL default '0' COMMENT 'buyoktime',
	`chargediamond` int(10) unsigned NOT NULL COMMENT 'chargediamond',
	`donatediamond` int(10) unsigned NOT NULL COMMENT 'donatediamond',
	`price` int(10) unsigned NOT NULL COMMENT 'price',
  PRIMARY KEY  (`aid`),
  KEY `Index_it_orderid` (`orderid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `t_postrelation`  �ʼ���ϵ��Ӧ��
--
DROP TABLE IF EXISTS `t_postrelation`;
CREATE TABLE `t_postrelation` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '��ɫGUID',
  `relationdata` varchar(768) NOT NULL default '0' COMMENT '��Ӧ��ϵ',
  `readflag` varchar(768) NOT NULL default '0' COMMENT '�Ƿ��Ѿ�����',
  `importflag` varchar(768) NOT NULL default '0' COMMENT '�Ƿ���Ҫ',
  PRIMARY KEY  (`aid`),
  KEY `Index_it_playerguid` (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_version`  ������
--
DROP TABLE IF EXISTS `t_version`;
CREATE TABLE `t_version` (
  `aid` bigint(20) NOT NULL auto_increment,
  `verNum` int(11) NOT NULL COMMENT '�汾��',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Table structure for table `t_arenatoplist`  ���������а��
DROP TABLE IF EXISTS `t_cdkey`;
CREATE TABLE `t_cdkey` (
  `aid` bigint(20) NOT NULL auto_increment,
  `cdkey` varchar(30) default '' COMMENT 'cdkey',
  `title` varchar(100) default '' COMMENT '����',
  `cdkeyclass` int(10)  NOT NULL COMMENT 'cdkey������',
  `cdkeytype` int(10)  NOT NULL COMMENT 'cdkey����',
  `iscount` int(10)  NOT NULL COMMENT '�Ƿ����',
  `begintime` int(10) unsigned NOT NULL COMMENT '��ʼʱ��',
  `endtime` int(10) unsigned NOT NULL COMMENT '����ʱ��',
  `guid` int(10) unsigned NOT NULL COMMENT '��ȡ�����ݱ�ʶGUID',
  `item1` int(11) default '-1' COMMENT '��Ʒ1',
  `itemcount1` int(11) default '0' COMMENT '��Ʒ1����',
  `item2` int(11) default '-1' COMMENT '��Ʒ2',
  `itemcount2` int(11) default '0' COMMENT '��Ʒ2����',
  `item3` int(11) default '-1' COMMENT '��Ʒ3',
  `itemcount3` int(11) default '0' COMMENT '��Ʒ3����',
  `item4` int(11) default '-1' COMMENT '��Ʒ4',
  `itemcount4` int(11) default '0' COMMENT '��Ʒ4����',
  `item5` int(11) default '-1' COMMENT '��Ʒ5',
  `itemcount5` int(11) default '0' COMMENT '��Ʒ5����',
  `item6` int(11) default '-1' COMMENT '��Ʒ6',
  `itemcount6` int(11) default '0' COMMENT '��Ʒ6����',
  `item7` int(11) default '-1' COMMENT '��Ʒ7',
  `itemcount7` int(11) default '0' COMMENT '��Ʒ7����',
  `item8` int(11) default '-1' COMMENT '��Ʒ8',
  `itemcount8` int(11) default '0' COMMENT '��Ʒ8����',
  `item9` int(11) default '-1' COMMENT '��Ʒ9',
  `itemcount9` int(11) default '0' COMMENT '��Ʒ9����',
  `item10` int(11) default '-1' COMMENT '��Ʒ10',
  `itemcount10` int(11) default '0' COMMENT '��Ʒ10����',
  `valid` int(11) NOT NULL default '0' COMMENT '��Ч���λ',
  PRIMARY KEY  (`aid`),
  KEY `Index_cdkey` (`cdkey`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Table structure for table `t_arenatoplist`  ���������а��
DROP TABLE IF EXISTS `t_arenatoplist`;
CREATE TABLE `t_arenatoplist` (
  `aid` bigint(20) NOT NULL auto_increment,
  `place` int(11) NOT NULL COMMENT '����λ��',
  `guid` int(10) unsigned NOT NULL COMMENT '���ݱ�ʶGUID',
  `level` int(11) NOT NULL COMMENT '�ȼ�',
  `playername` varchar(60) default '' COMMENT '��ɫ��',
  `fightappraise` int(11) NOT NULL COMMENT 'ս����',
  `heroindex` int(11) NOT NULL COMMENT 'Ӣ������',
  `herograde` int(11) NOT NULL COMMENT 'Ӣ�۽�λ',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_awardpost`  ������
--
DROP TABLE IF EXISTS `t_awardpost`;
CREATE TABLE `t_awardpost` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '������GUID',
  `flag` int(10)  NOT NULL COMMENT '��������',
  `title` varchar(50) NOT NULL COMMENT '��������',
  `awarddata` varchar(200) NOT NULL COMMENT '��������',
  `createtime` int(10) unsigned NOT NULL COMMENT '�ʼ�����ʱ��',
  `valid` int(11) NOT NULL default '0' COMMENT '��Ч���λ',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_globalval` ȫ�ֱ�����
--
DROP TABLE IF EXISTS `t_globalval`;
CREATE TABLE `t_globalval` (
  `aid` int(11) NOT NULL auto_increment,
  `globaltype` int(11) NOT NULL COMMENT '����',
  `globalvalue` varchar(128) NOT NULL COMMENT 'ֵ',
  `valid` smallint(6) NOT NULL default '1' COMMENT '��Ч���λ',
  PRIMARY KEY  (`aid`),
  UNIQUE INDEX `globaltype` (`globaltype`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_herocard`  Ӣ�۱�
--
DROP TABLE IF EXISTS `t_herocard`;
CREATE TABLE `t_herocard` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '��ɫGUID',
  `heroid` int(11) NOT NULL COMMENT '����id',
  `slot` smallint(6) NOT NULL COMMENT '����λ��',
  `battleslot` smallint(6) NOT NULL COMMENT '����λ��',
  `valid` smallint(6) NOT NULL default '1' COMMENT '��Ч���λ',
  `version` int(11) NOT NULL default '0' COMMENT '���ݰ汾',
  `dataproperty` varchar(768) NOT NULL default '0' COMMENT '������ϸ��������',
  `skillproperty` varchar(768) NOT NULL default '0' COMMENT '���Ƽ��ܲ���',
  PRIMARY KEY  (`aid`),
  KEY `Index_it_playerguid` (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_item`  ��Ʒ��
--
DROP TABLE IF EXISTS `t_item`;
CREATE TABLE `t_item` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '��ɫGUID',
  `guid` int(11) NOT NULL COMMENT '��ƷID guidλ',
  `world` int(11) NOT NULL COMMENT '��ƷID worldλ',
  `server` int(11) NOT NULL COMMENT '��ƷID serverλ',
  `itemclass` int(11) NOT NULL COMMENT '��Ʒ����',
  `bagtype` int(11) NOT NULL COMMENT '��������',
  `slot` smallint(6) NOT NULL COMMENT '��Ʒλ��',
  `creator` varchar(40) default '0' COMMENT '��Ʒ������',
  `valid` smallint(6) NOT NULL default '1' COMMENT '��Ч���λ',
  `version` int(11) default '0' COMMENT '���ݰ汾',
  `dataproperty` varchar(768) NOT NULL default '0' COMMENT '��Ʒ��ϸ��������',
  `data` varchar(40) NOT NULL COMMENT '��Ʒ��������',
  `count` int(10) unsigned NOT NULL COMMENT '��Ʒ����',
  PRIMARY KEY  (`aid`),
  KEY `Index_it_playerguid` (`playerguid`),
  KEY `Index_it_itemguid` (`guid`,`world`,`server`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_itemserial`  ��ƷGUID�����
--
DROP TABLE IF EXISTS `t_itemserial`;
CREATE TABLE `t_itemserial` (
  `aid` bigint(20) NOT NULL auto_increment,
  `sid` int(11) default NULL COMMENT '������ID',
  `dbskey` int(11) default NULL COMMENT 'δʹ��',
  `sn` int(11) default NULL COMMENT '��ƷGUID�������',
  PRIMARY KEY  (`aid`),
  KEY `Index_tik_sid` (`sid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail`  �ʼ���
--
DROP TABLE IF EXISTS `t_mail`;
CREATE TABLE `t_mail` (
  `aid` bigint(20) NOT NULL auto_increment,
  `mailid` int(11) unsigned NOT NULL COMMENT '�ʼ�Ψһid,��������',
  `sendername` varchar(50) NOT NULL COMMENT '�����˽�ɫ��',
  `receivername` varchar(50) NOT NULL COMMENT '�ռ��˽�ɫ��',
  `mailtitle` varchar(50) NOT NULL COMMENT '�ʼ�����',
  `mailcontent` text NOT NULL COMMENT '�ʼ�����',
  `pindex` int(11) NOT NULL COMMENT '�ʼ����',
  `isimport` int(11) NOT NULL default '0' COMMENT '�ʼ��Ƿ���Ҫ',
  `valid` int(11) NOT NULL default '0' COMMENT '��Ч���λ',
  `playerguid` int(10) unsigned default '4294967295' COMMENT '������GUID',
  `portrait` int(11) NOT NULL COMMENT '������ͷ��',
  `flag` int(10) unsigned NOT NULL COMMENT '�ʼ�����:0��ͨ�ʼ� 1��ͨ�ʼ� 2�ű��ʼ� 3ϵͳ�ʼ�',
  `createtime` int(10) unsigned NOT NULL COMMENT '�ʼ�����ʱ��',
  `isread` int(10) unsigned NOT NULL COMMENT '�Ƿ��Զ�',
  `awarddata` varchar(300) NOT NULL COMMENT '��������',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_mail_pIndex` (`pindex`),
  KEY `Index_recer` USING HASH (`receivername`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_player` ��ɫ��
--
DROP TABLE IF EXISTS `t_player`;
CREATE TABLE `t_player` (
  `aid` bigint(20) NOT NULL auto_increment,
  `accountname` varchar(100) NOT NULL COMMENT '�ʺ���',
  `playerguid` int(10) unsigned NOT NULL COMMENT '��ɫGUID',
  `playername` varchar(50) NOT NULL COMMENT '��ɫ��',
  `isfirstlogin` smallint(6) unsigned NOT NULL default '1' COMMENT '�״ε�¼���',
  `createtime` int(11) NOT NULL COMMENT '����ʱ��',
  `level` int(11) NOT NULL COMMENT '�ȼ�',
  `exp` bigint(20) NOT NULL COMMENT '����',
  `silver` int(11) NOT NULL COMMENT '���',
  `diamon` int(11) NOT NULL COMMENT '��ʯ',
  `rmbdiamon` int(11) NOT NULL COMMENT '�������ʯ',
  `magicstone` int(11) NOT NULL COMMENT '�ط�ʯ',
  `herosoul` int(11) NOT NULL COMMENT '����',
  `recallpaper` int(11) NOT NULL COMMENT '�ٻ���',
  `honor` int(11) NOT NULL COMMENT 'ѫ��',
  `crystal` int(11) NOT NULL COMMENT 'ϴ��ʯ',
  `heroexp` int(11) NOT NULL COMMENT 'ʥ��',
  `spirit` int(11) NOT NULL COMMENT '����',
  `spiritbuycount` smallint(6) NOT NULL COMMENT '�����������',
  `physicalpower` int(11) NOT NULL COMMENT '����',
  `physicalbuycount` smallint(6) NOT NULL COMMENT '�����������',
  `arenabuycount` smallint(6) NOT NULL COMMENT '�������������',
  `arenaleftcount` smallint(6) NOT NULL COMMENT '��������սʣ�����',
  `arenawinstreak` smallint(6) NOT NULL COMMENT '��������ʤ����',
  `arenamaxrank` int(11) NOT NULL default 102400 COMMENT '�������������',
  `arenachallengecd` int(11) NOT NULL default 0 COMMENT '��������սcd',
  `zhanyidetail` text not null default '' COMMENT 'ս����ϸ����',
  `achievement` text not null default '' COMMENT '�ɾ�����',
  `dailytask` text not null default '' COMMENT '�ճ�����',
  `orderlist`	text not null default '' default '' COMMENT	'��������',
  `shopdata` text not null default '' COMMENT '�̵�����',
  `bankallmoney` bigint(20) NOT NULL COMMENT '���жһ����ܽ��',
  `questdata` text not null default '' COMMENT '�Զ�����չ����',
  `onlinetime` int(11) NOT NULL default 0 COMMENT '�ϴε�¼����ʱ��',
  `onlinetimetoday` int(11) unsigned NOT NULL default 0 COMMENT '��������ʱ��',
  `totalonlinetime` int(11) unsigned NOT NULL default 0 COMMENT '������ʱ��',
  `logintime` int(11) NOT NULL COMMENT '����ʱ��',
  `logouttime` int(11) NOT NULL COMMENT '����ʱ��',
  `lastloginip` varchar(30) NOT NULL default '' COMMENT '����½��ip��ַ',
  `version` int(11) NOT NULL COMMENT '�汾��',
  `map` int(11) NOT NULL COMMENT '����ID',
  `x` int(11) NOT NULL COMMENT '����λ��x',
  `z` int(11) NOT NULL COMMENT '����λ��z',
  `hp` int(11) NOT NULL COMMENT 'Ѫ��',
  `playerworld` smallint(6) unsigned NOT NULL,
  `isdelchar` smallint(6) NOT NULL COMMENT '���ɾ�����',
  `crc` int(10) unsigned zerofill NOT NULL,
  `valid` smallint(6) NOT NULL COMMENT '��Ч���λ',
  `lockdata` smallint(6) NOT NULL default '0' COMMENT '����Ƿ�����������״ֻ̬��д�벻�ܶ�ȡ',
  `forbidlogintime` int(11) unsigned NOT NULL default 0 COMMENT '��ֹ��¼',
  `forbidchattime` int(11) unsigned NOT NULL default 0 COMMENT '��ֹ����',
  `lastresetyearday` int(11) NOT NULL default '-1' COMMENT '�ϴ�ϵͳ��������',
  `macaddress` varchar(50) NOT NULL default ''  COMMENT '��Ҵ�����ɫ��mac��ַ',
  `channelid` int(11) NOT NULL default '0' COMMENT '����id',
  `nwvipexp` int(11) unsigned NOT NULL default '0'  COMMENT 'vip����',
  `nwviplevel` int(11) NOT NULL default '0' COMMENT 'vip�ȼ�',
  `playersimpledata` text not null default '' COMMENT '��Ӣ������',
  `rmb` int(11) NOT NULL default '0' COMMENT '��ֵ�������',
  `chargesum` int(11) NOT NULL default '0' COMMENT '��ֵ���ܣ������ʯ��������������Ͱ��',
  `donatediamond` int(11) NOT NULL default '0' COMMENT '���͵���ʯ',
  `monthalldiamond` int(11) NOT NULL default '0' COMMENT '�¿���ȡ����ʯ����',
  `arenadata` text not null default '' COMMENT '����������',
  `guildid` int(10) NOT NULL default '-1' COMMENT '���id',
  `leaveguildtime` int(11) unsigned default '0' NOT NULL COMMENT '�뿪���ʱ��',
  `guildcontribute` int(10) unsigned NOT NULL default '0'  COMMENT '��ṱ��',
  `idfa` varchar(50) NOT NULL default '' COMMENT 'ios�豸�ı�־',
  `deviceos` int(6) NOT NULL default '0' COMMENT '�豸����ϵͳ����',
  `platuserid` bigint(20) NOT NULL default '0' COMMENT '����ƽ̨���ص��û�id',
  `brothercoin` int(11) default '0' NOT NULL COMMENT '�ֵܱ�',
  `vipawardflag` int(10) not null default '0' COMMENT 'vip������ȡ��ʶ',
  `monthcardlastdays` int(10) not null default '0' COMMENT '�¿���ʣ������',
  `monthcardgetdays` int(10) not null default '0' COMMENT '�¿�����ȡ����',
  `monthcardendtime` varchar(30) not null default '' COMMENT '�¿���ʧЧ����',
  `chargeallsum` int(10) not null default '0' COMMENT '�ܹ���ֵ�������',
  `donateallsum` int(10) not null default '0' COMMENT '�ܹ�����',
  `chargeallsumcurrday` int(10) not null default '0' COMMENT '�����õ����',
  `donateallsumcurrday` int(10) not null default '0' COMMENT '�����õ�����',
	`gameid` int(10) not null default '0' COMMENT '�������Ϸ��id',
	`gameserverid` int(10) not null default '0' COMMENT '��Ϸ��������id',
	`imei` varchar(60) NOT NULL default '' COMMENT '�豸Ψһ��ʶ��',
	`accountid` varchar(100) NOT NULL default '' COMMENT '�ʺ�id',
	`registerip` varchar(24) NOT NULL default '' COMMENT 'opengl render��Ϣ',
	`telecomoper` varchar(50) NOT NULL default '' COMMENT '�û�������Ӫ��',
	`devicehardware` varchar(100) NOT NULL default '' COMMENT '�ֻ��ͺţ��硱iPhone 6 (A1549/A1586)������iPad Air (A1474)���ȵ�',
	`devicesoftware` varchar(100) NOT NULL default '' COMMENT '�ֻ�����ϵͳ����IOS 7.0 ,ANDROID 3.0�ȵ�',
	`screenwidth` int(10) not null default '0' COMMENT '��ʾ�����',
	`screenhight` int(10) not null default '0' COMMENT '��ʾ���߶�',
	`density` float(20) not null default '0' COMMENT '�����ܶ�',
	`cpuhardware` varchar(100) NOT NULL default '' COMMENT '�ַ����洢��ʽΪ��|�ָ��cpu����|Ƶ��|����',
	`memory` int(10) not null default '0' COMMENT '�ڴ���������λG��',
	`glrender` varchar(100) NOT NULL default '' COMMENT 'opengl render��Ϣ',
	`glversion` varchar(100) NOT NULL default '' COMMENT 'opengl render��Ϣ',
	`deviceid` varchar(201) NOT NULL default '' COMMENT '�豸Ψһid����mac����idfa',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_char_playerguid` (`playerguid`),
  UNIQUE KEY `Index_char_playername` USING HASH (`playername`),
  KEY `Index_char_accname` (`accountname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `t_playerext` ��ɫ����չ��
--
DROP TABLE IF EXISTS `t_playerext`;
CREATE TABLE `t_playerext` (
  `playerguid` int(10) unsigned NOT NULL COMMENT '��ɫGUID',
  `offlineaward` text not null default '' COMMENT '����ʱ�佱���淨',
  `playercdkey` varchar(500) NOT NULL default '0'  COMMENT '������ϱ��ֵ�cdkey����',
  `assigngift` text NOT NULL default ''  COMMENT '������ǩ����ǩ���ȼ����',
  `zhanyifinishdata` text NOT NULL default ''  COMMENT '������ɴ���',
  PRIMARY KEY  (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_playerrename`
--
DROP TABLE IF EXISTS `t_playerrename`;
CREATE TABLE `t_playerrename` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '��ɫGUID',
  `oldname` varchar(50) NOT NULL COMMENT '�ɽ�ɫ��',
  `newname` varchar(50) NOT NULL default '@' COMMENT '�½�ɫ��',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_newname` USING HASH (`newname`),
  KEY `Index_char_playerguid` (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_toplist`  ���а��
--
DROP TABLE IF EXISTS `t_toplist`;
CREATE TABLE `t_toplist` (
  `aid` bigint(20) NOT NULL auto_increment,
  `toplisttype` int(11) NOT NULL default '-1' COMMENT '��������',
  `dataversion` int(11) NOT NULL default 0 COMMENT '���ݰ汾��',
  `toplistkey` text not null default '' COMMENT 'key',
  `toplistdata` text not null default '' COMMENT '����',
  `guildbossdata` text not null default '' COMMENT 'guildbossdata',
  `forbidsort` int(11) NOT NULL default '0' COMMENT '��ǰ��Ŀ�Ƿ��������',
  `fightval` int(11)  NOT NULL default '0' COMMENT '���ս��',
  `guid` int(11) unsigned default NULL COMMENT 'GUID',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_guild`  ����
--
DROP TABLE IF EXISTS `t_guild`;
CREATE TABLE `t_guild` (
  `aid` bigint(20) NOT NULL auto_increment,
  `guildid` int(10) NOT NULL default '-1' COMMENT '���ID',
  `guildname` varchar(30) NOT NULL COMMENT '�����',
  `guildnotice` varchar(256) NOT NULL default '' COMMENT '��ṫ��',
  `chairmanid` int(10) unsigned NOT NULL COMMENT '����GUID',
  `chairmanname` varchar(32) NOT NULL default '' COMMENT '������ɫ��',
  `guildlvl` int(11) NOT NULL COMMENT '���ȼ�',
  `guildicon` smallint(6) NOT NULL default '1' COMMENT '���ͼ��',
  `memcount` int(11) NOT NULL COMMENT '����Ա����',
  `time` int(11) NOT NULL COMMENT '������ʱ��',
  `joinlvl` int(11) NOT NULL COMMENT '������ȼ�����',
  `joinlimit` smallint(11) NOT NULL COMMENT '���Ȩ��',
  `lastresetyearday` int(11) NOT NULL default '-1' COMMENT '�ϴ�ϵͳ��������',
  `activity` int(11) NOT NULL default '0' COMMENT '��Ծ��',
  `valid` int(11) NOT NULL COMMENT '��Ч���λ',
  PRIMARY KEY  (`aid`),
  KEY `guildid` (`guildid`),
  KEY `guildname` USING HASH (`guildname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_guildmember`  ����Ա��
--
DROP TABLE IF EXISTS `t_guildmember`;
CREATE TABLE `t_guildmember` (
  `aid` bigint(20) NOT NULL auto_increment,
  `guildid` int(10) NOT NULL default '-1' COMMENT '���ID',
  `userguid` int(10) unsigned NOT NULL COMMENT '����ԱGUID',
  `username` varchar(32) NOT NULL COMMENT '����Ա��ɫ��',
  `position` int(11) NOT NULL COMMENT '���ְλ',
  `lastlogintime` int(10) unsigned NOT NULL COMMENT '��һ�ε�¼ʱ��',
  `level` int(10) unsigned NOT NULL COMMENT '�ȼ�',
  `heroid` int(10) NOT NULL COMMENT 'Ӣ��id',
  `herograde` smallint(6) NOT NULL default '0' COMMENT 'Ӣ�۽�λ',
  `herostar` smallint(6) NOT NULL default '1' COMMENT 'Ӣ���Ǽ�',
  PRIMARY KEY  (`aid`),
  KEY `guildid` (`guildid`),
  KEY `userguid` (`userguid`),
  KEY `username` USING HASH (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_friend` ��ɫ����չ��
--
DROP TABLE IF EXISTS `t_friend`;
CREATE TABLE `t_friend` (
  `playerguid` int(10) unsigned NOT NULL COMMENT '��ɫGUID',
  `getphysicalcount` smallint(6) NOT NULL default '0' COMMENT '��ȡ��������',
  `lastresetday` int(11) default '-1' COMMENT '�ϴθ�������',
  `friendlist` text not null default '' COMMENT '�����б�',
  `applylist` text not null default ''  COMMENT '�����б�',
  PRIMARY KEY  (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DELIMITER ;;

--
-- procedure proc_updata_playerext
--

DROP PROCEDURE IF EXISTS `proc_updata_playerext`;;
CREATE PROCEDURE `proc_updata_playerext`(
psnid						int,
pofflineaward			text,
pplayercdkey			varchar(500),
passigninfo				text,
pzhanyidata				text
)
BEGIN
	declare rcount int;
	select count(*) into rcount from t_playerext where playerguid = psnid;
	if rcount >= 1 then
		update t_playerext set 
			offlineaward=pofflineaward,
			playercdkey=pplayercdkey,
			assigngift=passigninfo,
			zhanyifinishdata=pzhanyidata
		where playerguid=psnid;
	else
		insert into t_playerext
			(
			playerguid,
			offlineaward,
			playercdkey,
			assigngift,
			zhanyifinishdata)
		values
			(psnid,
			pofflineaward,
			pplayercdkey,
			passigninfo,
			pzhanyidata);
	end if;
END;;

--
-- procedure proc_save_guildinfo
--
DROP PROCEDURE IF EXISTS `proc_save_guildinfo`;;
CREATE PROCEDURE `proc_save_guildinfo`(
pguildid					int,
pguildname				varchar(30),
pguilddesc				varchar(256),
pchairguid				int unsigned,
pchairname				varchar(32),
pguildlvl					int,
pguildicon				int,
pusercount				int,
pcreatetime				int,
pjoinlvl					int,
pjoinlimit				int,
plastyeadday			int,
pactivity					int,
pvalid						int
)
BEGIN
	declare rcount int;
	select guildid into rcount from t_guild where guildid = pguildid;
	if rcount = pguildid then
		update t_guild set 
			guildname=pguildname,
			guildnotice=pguilddesc,
			chairmanid=pchairguid,
			chairmanname=pchairname,    
			guildlvl=pguildlvl,   
			guildicon=pguildicon,    
			memcount=pusercount,
			time=pcreatetime,      
			joinlvl=pjoinlvl,  
			joinlimit=pjoinlimit,
			lastresetyearday = plastyeadday,
			activity=pactivity,
			valid=pvalid
		where guildid=pguildid;
	else
		insert into t_guild
			(guildid,
			guildname,
			guildnotice,
			chairmanid,
			chairmanname,    
			guildlvl,   
			guildicon,    
			memcount,
			time,      
			joinlvl,  
			joinlimit,
			lastresetyearday,
			activity,
			valid)
		values
			(pguildid,       
			pguildname,
			pguilddesc,
			pchairguid,
			pchairname,    
			pguildlvl,   
			pguildicon,    
			pusercount,
			pcreatetime,      
			pjoinlvl,  
			pjoinlimit,
			plastyeadday,
			pactivity,
			pvalid);
	end if;
END;;

--
-- procedure proc_init_data
--

DROP PROCEDURE IF EXISTS `proc_init_data`;;
CREATE PROCEDURE `proc_init_data`()
BEGIN
declare awardid int;
set awardid=1;
while awardid<102400 do
	INSERT INTO t_awardpost(playerguid,flag,title,awarddata,createtime,valid) VALUES (4294967295,-1,0,0,0,0);
	set awardid=awardid+1;
end while;
END;;

--
-- procedure proc_add_toplist
--
DROP PROCEDURE IF EXISTS `proc_add_toplist`;;
CREATE PROCEDURE `proc_add_toplist`(
p_id		int,
p_toplisttype			int,
p_dataversion			int,
p_toplistkey			varchar(3000),
p_toplistdata			varchar(3000),
p_guildbossdata		varchar(3000),
p_forbidsort			int,
p_fight						int,
p_guid						int
)
BEGIN
DECLARE vcount INT DEFAULT 0;
SELECT COUNT(1) INTO vcount FROM t_toplist WHERE aid=p_id;
IF vcount >= 1 THEN
UPDATE t_toplist SET
toplisttype=p_toplisttype,
dataversion=p_dataversion,
toplistkey=p_toplistkey,
toplistdata=p_toplistdata,
guildbossdata=p_guildbossdata,
forbidsort=p_forbidsort,
fightval=p_fight,
guid=p_guid
WHERE aid=p_id;
ELSE
INSERT INTO t_toplist(toplisttype,dataversion,toplistkey,toplistdata,guildbossdata,forbidsort,fightval,guid)
	VALUES(p_toplisttype,p_dataversion,p_toplistkey,p_toplistdata,p_guildbossdata,p_forbidsort,p_fight,p_guid);
END IF;
END;;

--
-- procedure proc_save_arenatoplist
--

DROP PROCEDURE IF EXISTS `proc_save_arenatoplist`;;
CREATE PROCEDURE `proc_save_arenatoplist`(
varplace					INT,
varguid					INT,
varlevel					INT,
varplayername		varchar(128),
varfightappraise INT,
varheroindex			INT,
varherograde			INT)
BEGIN
DECLARE vcount INT DEFAULT 0;
SELECT COUNT(1) INTO vcount FROM t_arenatoplist WHERE place=varplace;
IF vcount >= 1 THEN
UPDATE t_arenatoplist SET
guid=varguid,
level=varlevel,
playername=varplayername,
fightappraise=varfightappraise,
heroindex=varheroindex,
herograde=varherograde
WHERE place=varplace;
ELSE
INSERT INTO t_arenatoplist(place, guid, level, playername, fightappraise, heroindex, herograde)
	VALUES(varplace, varguid, varlevel, varplayername, varfightappraise, varheroindex, varherograde);
END IF;
END;;

--
-- procedure proc_save_globalval
--

DROP PROCEDURE IF EXISTS `proc_save_globalval`;;
CREATE PROCEDURE `proc_save_globalval`(
pglobaltype					INT,
pglobalvalue				 varchar(128))
BEGIN
DECLARE vcount INT DEFAULT 0;
SELECT COUNT(1) INTO vcount FROM t_globalval WHERE globaltype=pglobaltype;
IF vcount >= 1 THEN
UPDATE t_globalval SET
globaltype=pglobaltype,
globalvalue=pglobalvalue,
valid=1
WHERE globaltype=pglobaltype;
ELSE
INSERT INTO t_globalval(globaltype, globalvalue, valid)
	VALUES(pglobaltype, pglobalvalue,1);
END IF;
END;;

--
-- procedure proc_create_newplayer
--
DROP PROCEDURE IF EXISTS `proc_create_newplayer`;;
CREATE PROCEDURE `proc_create_newplayer`(
paccountname	VARCHAR(100) BINARY,
pplayername		VARCHAR(50) BINARY,
pmac					VARCHAR(50) BINARY,
pidfa					VARCHAR(50) BINARY,
pcreatetime		INT,
pchannel			INT,
pdeviceos			SMALLINT,
platuserid		BIGINT UNSIGNED,
pgameid			INT,
pgameserverid			INT,
paccountid			VARCHAR(100) BINARY,
pimei		VARCHAR(60) BINARY,
pregisterip VARCHAR(24) BINARY,
ptelecomoper VARCHAR(50) BINARY,
pdevicehardware VARCHAR(100) BINARY,
pdevicesoftware VARCHAR(100) BINARY,
pscreenwidth  INT,
pscreenhight  INT,
pdensity			FLOAT,
pcpuhardware VARCHAR(100) BINARY,
pmemory   INT,
pglrender VARCHAR(100) BINARY, 
pglversion VARCHAR(100) BINARY,
pdeviceid VARCHAR(201) BINARY)
BEGIN
	DECLARE rguid      	  			INT UNSIGNED DEFAULT -1;
	DECLARE result		  				INT DEFAULT -1;
	DECLARE isinrenametable		  INT DEFAULT 0;
	DECLARE sameaccount					INT DEFAULT 0;
	DECLARE curguid							INT UNSIGNED DEFAULT -1;
	START TRANSACTION;
	SELECT COUNT(*) INTO sameaccount FROM t_player WHERE accountname=paccountname;
	IF sameaccount >= 1 THEN
		SET result = -3;
	END IF;
	
	IF result = -1 THEN
		SET rguid = -1;
		SELECT CAST(globalvalue AS UNSIGNED) INTO curguid FROM t_globalval WHERE globaltype = 0 LIMIT 1;
		SET curguid = curguid + 1;
		UPDATE t_globalval SET globalvalue = CAST(curguid AS CHAR)  WHERE globaltype = 0;

		SET rguid = curguid;
		IF rguid <> -1 THEN
			INSERT INTO t_player(accountname,playerguid,playername,createtime,level,exp,silver,diamon,magicstone,herosoul,
				recallpaper,honor,crystal,heroexp,spirit,spiritbuycount,physicalpower,physicalbuycount,arenabuycount,arenaleftcount,
				arenawinstreak,onlinetime,logintime,logouttime,lastloginip,map,x,z,hp,totalonlinetime,valid,macaddress,channelid,
				idfa,deviceos,platuserid,gameid,gameserverid,accountid,imei,registerip,telecomoper,devicehardware,devicesoftware,screenwidth,
				screenhight,density,cpuhardware,memory,glrender,glversion,deviceid)
			VALUES(paccountname,rguid,CONCAT(CAST(curguid AS CHAR),CAST(UNIX_TIMESTAMP() AS CHAR)),pcreatetime,1,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,10,
				0,0,0,0,'',850,0,0,100,0,1,pmac,pchannel,
				pidfa, pdeviceos, platuserid,pgameid,pgameserverid,paccountid,pimei,
				pregisterip,ptelecomoper,pdevicehardware,pdevicesoftware,pscreenwidth,pscreenhight,pdensity,pcpuhardware,
				pmemory,pglrender,pglversion,pdeviceid);
		 SELECT ROW_COUNT() INTO result;
		ELSE
		 SET result = -3;
		END IF;
		COMMIT;
	ELSE
		ROLLBACK;
	END IF;
	SELECT result,rguid;
END;;

--
-- procedure proc_delete_player_new
--
DROP PROCEDURE IF EXISTS `proc_delete_player_new`;;
CREATE PROCEDURE `proc_delete_player_new`(
paccount              varchar(100),
pplayerguid             int unsigned)
BEGIN
	declare rplayerguid   int unsigned;
	declare rlevel      int;
	declare rdeltime    int;
	declare rnext       int;
	declare rversion  int;
	set     rplayerguid = -1;
	set     rnext = 0;
	set     rversion = 0;
	start transaction;
	          update t_player set valid = 0 where accountname= paccount and playerguid = pplayerguid;
	commit;
	select rnext,rversion;
END;;

--
-- procedure proc_save_iteminfo
--
DROP PROCEDURE IF EXISTS `proc_save_iteminfo`;;
CREATE PROCEDURE `proc_save_iteminfo`(
pplayerguid	int unsigned,
pguid				int,
pworld			int,
pserver			int,
pitemclass	int,
pbagtype		int,
pslot				smallint,
pdataproperty	varchar(768),
pversion		int,
pcreator		varchar(40),
pdata				varchar(40),
pcount			int)
BEGIN
	declare vcount int default 0;
	select count(1) into vcount from t_item where playerguid=pplayerguid and slot=pslot and bagtype =pbagtype;
	if vcount >= 1 then
		update t_item set
			playerguid=pplayerguid,
			world=pworld,
			server=pserver,
			guid=pguid,
			itemclass=pitemclass,
			bagtype=pbagtype,
			slot=pslot,
			dataproperty=pdataproperty,
			version=pversion,
			creator=pcreator,
			data=pdata,
			count=pcount,
			valid=1
		where playerguid=pplayerguid and slot=pslot and bagtype =pbagtype limit 1;
	else
		insert into t_item(playerguid,world,server,guid,itemclass,bagtype,slot,dataproperty,version,creator,data,count,valid)
				values(pplayerguid,pworld,pserver,pguid,pitemclass,pbagtype,pslot,pdataproperty,pversion,pcreator,pdata,pcount,1);
	end if;
END;;

--
-- procedure proc_save_herocardinfo
--
DROP PROCEDURE IF EXISTS `proc_save_herocardinfo`;;
CREATE PROCEDURE `proc_save_herocardinfo`(
pplayerguid	int unsigned,
pheroid			int,
pslot				smallint,
pbattleslot				smallint,
pversion		int,
pdataproperty	varchar(768),
pskilldata		varchar(768))
BEGIN
	declare vcount int default 0;
	select count(1) into vcount from t_herocard where playerguid=pplayerguid and slot=pslot;
	if vcount >= 1 then
		update t_herocard set
			playerguid=pplayerguid,
			heroid=pheroid,
			slot=pslot,
			battleslot = pbattleslot,
			version=pversion,
			dataproperty=pdataproperty,
			skillproperty=pskilldata,
			valid=1
		where playerguid=pplayerguid and slot=pslot limit 1;
	else
		insert into t_herocard(playerguid,heroid,slot,battleslot,valid,version,dataproperty,skillproperty)
				values(pplayerguid,pheroid,pslot,pbattleslot,1,pversion,pdataproperty,pskilldata);
	end if;
END;;

--
-- procedure proc_save_awardpostinfo
--
DROP PROCEDURE IF EXISTS `proc_save_awardpostinfo`;;
CREATE PROCEDURE `proc_save_awardpostinfo`(
vIndex int,
pplayerguid int unsigned,
pflag int,
pawarddata varchar(200),
pcreatetime int unsigned,
pvalid int)
BEGIN
	declare rcount int;
	select pindex into rcount from t_awardpost where pindex = vIndex;
	if rcount = vIndex then
		update t_awardpost set pindex=vIndex,playerguid=pplayerguid,flag=pflag,awarddata=pawarddata,createtime=pcreatetime,valid=pvalid where pindex=vIndex;
	else
		insert into t_awardpost(pindex,playerguid,flag,awarddata,createtime,valid)
			  values(vIndex,pplayerguid,pflag,pawarddata,pcreatetime,pvalid);
	end if;
END;;

--
-- procedure proc_save_mailinfo
--
DROP PROCEDURE IF EXISTS `proc_save_mailinfo`;;
CREATE PROCEDURE `proc_save_mailinfo`(
psendername varchar(50),
preceivername varchar(50),
pmailtitle varchar(50),
pContext text,
pGuid int unsigned,
pMailid int unsigned,
pPortrait int,
pFlag int unsigned,
pCreateTime int unsigned,
pIsRead int,
pawarddata varchar(300),
vIndex int,
pimport int,
pValid int
)
BEGIN
	declare rcount int;
	select pindex into rcount from t_mail where pindex = vIndex;
	if rcount = vIndex then
		update t_mail set sendername=psendername,receivername=preceivername,mailtitle=pmailtitle, mailcontent=pContext,playerguid=pGuid,portrait=pPortrait,flag=pFlag,
							createtime=pCreateTime,isread=pIsRead,awarddata=pawarddata,valid=pValid,mailid=pMailid,isimport=pimport
				where pindex=vIndex;
	else
		insert into t_mail(pindex,sendername,receivername,mailtitle, mailcontent,playerguid,portrait,flag,createtime,isread,awarddata,valid,mailid,isimport)
			  values(vIndex,psendername,preceivername,pmailtitle,pContext,pGuid,pPortrait,pFlag,pCreateTime,pIsRead,pawarddata,pValid,pMailid,pimport);
	end if;
END;;


--
-- procedure proc_save_postrelation
--
DROP PROCEDURE IF EXISTS `proc_save_postrelation`;;
CREATE PROCEDURE `proc_save_postrelation`(
psnid int unsigned,
pdata varchar(768),
pread varchar(768),
pimport varchar(768)
)
BEGIN
	declare rcount int unsigned;
	select COUNT(*) into rcount from t_postrelation where playerguid = psnid;
	if rcount >= 1 then
		update t_postrelation set relationdata=pdata,readflag=pread,importflag=pimport
				where playerguid=psnid;
	else
		insert into t_postrelation(playerguid,relationdata,readflag,importflag) values(psnid, pdata, pread, pimport);
	end if;
END;;

DROP PROCEDURE IF EXISTS `proc_change_player_name`;;
CREATE PROCEDURE `proc_change_player_name`(
pplayerguid INT UNSIGNED,
pnewname VARCHAR(50) BINARY)
BEGIN
	DECLARE presult INT DEFAULT 0;
	DECLARE visneedrename INT DEFAULT 0;
	DECLARE viscanusenewname INT DEFAULT 0;
	SELECT count(*) INTO visneedrename from t_player where playerguid=pplayerguid and valid=1;
	IF visneedrename <= 0 then
		SET presult = -2;
	END IF;
	
	SELECT count(*) INTO viscanusenewname from t_player where playername=pnewname;
	IF viscanusenewname >= 1 then
		SET presult = -2;
	END IF;
	
	update t_player set playername=pnewname where playerguid=pplayerguid;
	COMMIT;
	
	SELECT presult;
END;;

DROP PROCEDURE IF EXISTS `proc_save_friendinfo`;;
CREATE PROCEDURE `proc_save_friendinfo`(
pplayerid					BIGINT UNSIGNED,
pgetphysicalcount SMALLINT,
plastresetday			int,
pfriendlist				varchar(6000),
papplylist				varchar(6000)
)
BEGIN
	DECLARE sameaccount					INT DEFAULT 0;
	SELECT COUNT(*) INTO sameaccount FROM t_friend WHERE playerguid=pplayerid;
	IF sameaccount >= 1 THEN
		update t_friend set 
			getphysicalcount=pgetphysicalcount,
			lastresetday=plastresetday,
			friendlist=pfriendlist,
			applylist=papplylist
		where playerguid=pplayerid;
	else
		insert into t_friend
			(playerguid,
			getphysicalcount,
			lastresetday,
			friendlist,
			applylist)
		values
			(pplayerid,       
			pgetphysicalcount,
			plastresetday,
			pfriendlist,
			papplylist);
	end if;
END;;

--
-- function func_Ascii2Value
--
DROP FUNCTION if EXISTS `func_Ascii2Value`;;
CREATE FUNCTION `func_Ascii2Value` (`invalue` char(1)) RETURNS INTEGER unsigned
BEGIN
	DECLARE vReturn int unsigned default 0;
	
	if 		STRCMP(invalue,'0') = 0 then set vReturn=0;
	elseif	STRCMP(invalue,'1') = 0 then set vReturn=1;
	elseif	STRCMP(invalue,'2') = 0 then set vReturn=2;
	elseif	STRCMP(invalue,'3') = 0 then set vReturn=3;
	elseif	STRCMP(invalue,'4') = 0 then set vReturn=4;
	elseif	STRCMP(invalue,'5') = 0 then set vReturn=5;
	elseif	STRCMP(invalue,'6') = 0 then set vReturn=6;
	elseif	STRCMP(invalue,'7') = 0 then set vReturn=7;
	elseif	STRCMP(invalue,'8') = 0 then set vReturn=8;
	elseif	STRCMP(invalue,'9') = 0 then set vReturn=9;
	elseif	STRCMP(invalue,'A') = 0 then set vReturn=10;
	elseif	STRCMP(invalue,'B') = 0 then set vReturn=11;
	elseif	STRCMP(invalue,'C') = 0 then set vReturn=12;
	elseif	STRCMP(invalue,'D') = 0 then set vReturn=13;
	elseif	STRCMP(invalue,'E') = 0 then set vReturn=14;
	elseif	STRCMP(invalue,'F') = 0 then set vReturn=15;
	else set vReturn=0;
	end if;
	
	return vReturn;
END;;

--
-- function func_get_uint8
--
DROP FUNCTION if EXISTS `func_get_uint8`;;
CREATE FUNCTION `func_get_uint8` (`str` char(2)) RETURNS TINYINT UNSIGNED
BEGIN
	DECLARE vReturn TINYINT unsigned default 0;
	DECLARE vTmp,vTmp1,vTmp2 TINYINT unsigned default 0;
	
	SET vTmp1 = func_Ascii2Value(SUBSTR(str,1,1));
	SET vTmp2 = func_Ascii2Value(SUBSTR(str,2,1));
	SET vTmp = (vTmp1<<4) + vTmp2;
	SET vReturn = vReturn + vTmp;
	
	return vReturn;
END;;

--
-- function func_get_uint16
--
DROP FUNCTION if EXISTS `func_get_uint16`;;
CREATE FUNCTION `func_get_uint16` (`str` char(4)) RETURNS SMALLINT UNSIGNED
BEGIN
	DECLARE vReturn SMALLINT unsigned default 0;
	DECLARE vTmp,vTmp1,vTmp2 SMALLINT unsigned default 0;
	
	SET vTmp1 = func_Ascii2Value(SUBSTR(str,1,1));
	SET vTmp2 = func_Ascii2Value(SUBSTR(str,2,1));
	SET vTmp = (vTmp1<<4) + vTmp2;
	SET vReturn = vReturn + vTmp;
	
	SET vTmp1 = func_Ascii2Value(SUBSTR(str,3,1));
	SET vTmp2 = func_Ascii2Value(SUBSTR(str,4,1));
	SET vTmp = (vTmp1<<4) + vTmp2;
	SET vReturn = vReturn + (vTmp<<8);
	
	return vReturn;
END;;

--
-- function func_get_uint32
--
DROP FUNCTION if EXISTS `func_get_uint32`;;
CREATE FUNCTION `func_get_uint32` (`str` char(8)) RETURNS INT UNSIGNED
BEGIN
	DECLARE vReturn int unsigned default 0;
	DECLARE vTmp,vTmp1,vTmp2 int unsigned default 0;
	
	SET vTmp1 = func_Ascii2Value(SUBSTR(str,1,1));
	SET vTmp2 = func_Ascii2Value(SUBSTR(str,2,1));
	SET vTmp = (vTmp1<<4) + vTmp2;
	SET vReturn = vReturn + vTmp;
	
	SET vTmp1 = func_Ascii2Value(SUBSTR(str,3,1));
	SET vTmp2 = func_Ascii2Value(SUBSTR(str,4,1));
	SET vTmp = (vTmp1<<4) + vTmp2;
	SET vReturn = vReturn + (vTmp<<8);

	SET vTmp1 = func_Ascii2Value(SUBSTR(str,5,1));
	SET vTmp2 = func_Ascii2Value(SUBSTR(str,6,1));
	SET vTmp = (vTmp1<<4) + vTmp2;
	SET vReturn = vReturn + (vTmp<<16);

	SET vTmp1 = func_Ascii2Value(SUBSTR(str,7,1));
	SET vTmp2 = func_Ascii2Value(SUBSTR(str,8,1));
	SET vTmp = (vTmp1<<4) + vTmp2;
	SET vReturn = vReturn + (vTmp<<24);
	
	return vReturn;
END;;
DELIMITER ;

INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(0,7001,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(1,7002,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(2,7003,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(3,7004,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(4,7005,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(5,7006,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(6,7007,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(7,7008,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(8,7009,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(9,7010,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(10,7011,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(11,7012,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(12,7013,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(13,7014,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(14,7015,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(15,7016,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(16,7017,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(17,7018,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(18,7019,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(19,7020,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(20,7021,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(21,7022,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(22,7023,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(23,7024,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(24,7025,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(25,7026,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(26,7027,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(27,7028,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(28,7029,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(29,7030,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(30,7031,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(31,7032,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(32,7033,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(33,7034,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(34,7035,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(35,7036,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(36,7037,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(37,7038,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(38,7039,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(39,7040,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(40,7041,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(41,7042,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(42,7043,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(43,7044,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(44,7045,1001);
INSERT INTO `t_itemserial`(sid,dbskey,sn) VALUES(45,7046,1001);
-- INSERT INTO `t_guidserial` VALUES(1010000001);
INSERT INTO t_globalval(globaltype, globalvalue) VALUES(0, '1010000001');
INSERT INTO t_globalval(globaltype, globalvalue) VALUES(1, '10100001');
INSERT INTO t_globalval(globaltype, globalvalue) VALUES(2, '1');

INSERT INTO `t_globalval`(globaltype,globalvalue,valid) VALUES(6,1,1);
INSERT INTO `t_globalval`(globaltype,globalvalue,valid) VALUES(8,1,1);
-- INSERT INTO `t_version` VALUES(1); ÿ�ΰ汾����Ҫ+1
INSERT INTO `t_version`(verNum) VALUES(1);

call proc_init_data();
DROP PROCEDURE IF EXISTS `proc_init_data`;

DELIMITER ;;