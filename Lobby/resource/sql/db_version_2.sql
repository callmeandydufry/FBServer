
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

-- INSERT INTO `t_version` VALUES(1); ÿ�ΰ汾����Ҫ+1
INSERT INTO `t_version`(verNum) VALUES(2);

DELIMITER ;;