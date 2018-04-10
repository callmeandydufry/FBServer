
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
-- Table structure for table `t_player` ��ɫ��
--
DROP TABLE IF EXISTS `t_player`;
CREATE TABLE `t_player` (
  `aid` bigint(20) NOT NULL auto_increment,
  `AccountName` varchar(100) NOT NULL COMMENT '�ʺ���',
  `Snid` BIGINT(20) unsigned NOT NULL COMMENT '��ɫGUID',
  `CharacterName` varchar(50) NOT NULL COMMENT '��ɫ��',
  `OnlineTime` int(11) NOT NULL default 0 COMMENT '�ϴε�¼����ʱ��',
  `OnlineAllTime` int(11) unsigned NOT NULL default 0 COMMENT '������ʱ��',
  `OnlineAllTimeCurrDay` int(11) unsigned NOT NULL default 0 COMMENT '��������ʱ��',
  `LastLoginTime` int(11) NOT NULL COMMENT '����ʱ��',
  `LastLogoutTime` int(11) NOT NULL COMMENT '����ʱ��',
  `LastLoginIP` varchar(30) NOT NULL default '' COMMENT '����½��ip��ַ',
  `Gold` int(11) NOT NULL COMMENT '���',
  `Diamond` int(11) NOT NULL COMMENT '��ʯ',
  `GiftDiamond` int(11) NOT NULL COMMENT '��ʯ',
  `BuyDiamond` int(11) NOT NULL COMMENT '��ʯ',
  `Level` int(11) NOT NULL COMMENT '�ȼ�',
  `Exp` bigint(20) NOT NULL COMMENT '����',
  `MailIDList` varchar(500) NOT NULL COMMENT '�ʼ�list',
  `MailStatus` BIGINT(20) unsigned NOT NULL COMMENT '�ʼ�״̬',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_char_playerguid` (`Snid`),
  UNIQUE KEY `Index_char_playername` USING HASH (`CharacterName`),
  KEY `Index_char_accname` (`AccountName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `t_snidalloc` snid <-> account ӳ���
--
DROP TABLE IF EXISTS `t_snidalloc`;
CREATE TABLE `t_snidalloc` (
  `aid` bigint(20) NOT NULL auto_increment,
  `Snid` BIGINT(20) unsigned NOT NULL COMMENT '��ɫGUID',
  `AccountName` varchar(100) NOT NULL COMMENT '�ʺ���',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_char_playerguid` (`Snid`),
  KEY `Index_char_accname` (`AccountName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `t_version`  �汾��
--
DROP TABLE IF EXISTS `t_version`;
CREATE TABLE `t_version` (
  `aid` bigint(20) NOT NULL auto_increment,
  `verNum` int(11) NOT NULL COMMENT '�汾��',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_base` 
--
DROP TABLE IF EXISTS `t_mail_base`;
CREATE TABLE `t_mail_base` (
  `MailID` int(11) NOT NULL default -1 COMMENT '�ʼ�Ψһid',
  `ServerID` smallint(5) NOT NULL COMMENT '������ID',
  `MailType` int(11) NOT NULL default 0 COMMENT '�ʼ�����',
  `CreateTime` int(11) NOT NULL default 0 COMMENT '����ʱ��',
  `OverTime` int(11) NOT NULL default 0 COMMENT '����ʱ��',
  PRIMARY KEY  (`MailID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_attach` 
--
DROP TABLE IF EXISTS `t_mail_attach`;
CREATE TABLE `t_mail_attach` (
  `MailID` int(11) NOT NULL default -1 COMMENT '�ʼ�Ψһid',
  `TypeMisc1` int(11) NOT NULL default 0 COMMENT '����id',
  `TypeMisc2` int(11) NOT NULL default 0 COMMENT '����id',
  `TypeMisc3` int(11) NOT NULL default 0 COMMENT '����id',
  `Num1` int(11) NOT NULL default 0 COMMENT '��������',
  `Num2` int(11) NOT NULL default 0 COMMENT '��������',
  `Num3` int(11) NOT NULL default 0 COMMENT '��������',
  `Valid` TINYINT(3) NOT NULL DEFAULT '0' COMMENT '�Ƿ���Ч',
  PRIMARY KEY  (`MailID`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_content ` 
--
DROP TABLE IF EXISTS `t_mail_content`;
CREATE TABLE `t_mail_content` (
  `MailID` int(11) NOT NULL default -1 COMMENT '�ʼ�Ψһid',
  `Title` varchar(50) NOT NULL COMMENT '�ʼ�title',
  `Content` varchar(50) NOT NULL COMMENT '�ʼ�����',
  `Valid` TINYINT(3) NOT NULL DEFAULT '0' COMMENT '�Ƿ���Ч',
  PRIMARY KEY  (`MailID`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_system ` 
--
DROP TABLE IF EXISTS `t_mail_system`;
CREATE TABLE `t_mail_system` (
  `MailID` int(11) NOT NULL default -1 COMMENT '�ʼ�Ψһid',
  `MinLv` smallint(5) NOT NULL COMMENT '��С�ȼ�',
  `MaxLv` smallint(5) NOT NULL COMMENT '���ȼ�',
  `Valid` TINYINT(3) NOT NULL DEFAULT '0' COMMENT '�Ƿ���Ч',
  PRIMARY KEY  (`MailID`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_globalval ` 
--
DROP TABLE IF EXISTS `t_globalval`;
CREATE TABLE `t_globalval` (
  `GlobalIndex` int(11) NOT NULL default 0 COMMENT 'globalid',
  `GlobalVal` int(11) NOT NULL default 0 COMMENT 'globalVal',
  PRIMARY KEY  (`GlobalIndex`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;
INSERT INTO t_globalval(globaltype, globalvalue) VALUES(0, '1');



-- INSERT INTO `t_version` VALUES(1); ÿ�ΰ汾����Ҫ+1
INSERT INTO `t_version`(verNum) VALUES(2);

DELIMITER ;;