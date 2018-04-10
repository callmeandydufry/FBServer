
--
-- Table structure for table `t_globalval` 全局变量表
--
DROP TABLE IF EXISTS `t_globalval`;
CREATE TABLE `t_globalval` (
  `aid` int(11) NOT NULL auto_increment,
  `globaltype` int(11) NOT NULL COMMENT '类型',
  `globalvalue` varchar(128) NOT NULL COMMENT '值',
  `valid` smallint(6) NOT NULL default '1' COMMENT '有效标记位',
  PRIMARY KEY  (`aid`),
  UNIQUE INDEX `globaltype` (`globaltype`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_player` 角色表
--
DROP TABLE IF EXISTS `t_player`;
CREATE TABLE `t_player` (
  `aid` bigint(20) NOT NULL auto_increment,
  `AccountName` varchar(100) NOT NULL COMMENT '帐号名',
  `Snid` BIGINT(20) unsigned NOT NULL COMMENT '角色GUID',
  `CharacterName` varchar(50) NOT NULL COMMENT '角色名',
  `OnlineTime` int(11) NOT NULL default 0 COMMENT '上次登录在线时间',
  `OnlineAllTime` int(11) unsigned NOT NULL default 0 COMMENT '总在线时长',
  `OnlineAllTimeCurrDay` int(11) unsigned NOT NULL default 0 COMMENT '当天在线时长',
  `LastLoginTime` int(11) NOT NULL COMMENT '上线时间',
  `LastLogoutTime` int(11) NOT NULL COMMENT '下线时间',
  `LastLoginIP` varchar(30) NOT NULL default '' COMMENT '最后登陆的ip地址',
  `Gold` int(11) NOT NULL COMMENT '金币',
  `Diamond` int(11) NOT NULL COMMENT '钻石',
  `GiftDiamond` int(11) NOT NULL COMMENT '钻石',
  `BuyDiamond` int(11) NOT NULL COMMENT '钻石',
  `Level` int(11) NOT NULL COMMENT '等级',
  `Exp` bigint(20) NOT NULL COMMENT '经验',
  `MailIDList` varchar(500) NOT NULL COMMENT '邮件list',
  `MailStatus` BIGINT(20) unsigned NOT NULL COMMENT '邮件状态',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_char_playerguid` (`Snid`),
  UNIQUE KEY `Index_char_playername` USING HASH (`CharacterName`),
  KEY `Index_char_accname` (`AccountName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `t_snidalloc` snid <-> account 映射表
--
DROP TABLE IF EXISTS `t_snidalloc`;
CREATE TABLE `t_snidalloc` (
  `aid` bigint(20) NOT NULL auto_increment,
  `Snid` BIGINT(20) unsigned NOT NULL COMMENT '角色GUID',
  `AccountName` varchar(100) NOT NULL COMMENT '帐号名',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_char_playerguid` (`Snid`),
  KEY `Index_char_accname` (`AccountName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `t_version`  版本表
--
DROP TABLE IF EXISTS `t_version`;
CREATE TABLE `t_version` (
  `aid` bigint(20) NOT NULL auto_increment,
  `verNum` int(11) NOT NULL COMMENT '版本号',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_base` 
--
DROP TABLE IF EXISTS `t_mail_base`;
CREATE TABLE `t_mail_base` (
  `MailID` int(11) NOT NULL default -1 COMMENT '邮件唯一id',
  `ServerID` smallint(5) NOT NULL COMMENT '服务器ID',
  `MailType` int(11) NOT NULL default 0 COMMENT '邮件类型',
  `CreateTime` int(11) NOT NULL default 0 COMMENT '创建时间',
  `OverTime` int(11) NOT NULL default 0 COMMENT '过期时间',
  PRIMARY KEY  (`MailID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_attach` 
--
DROP TABLE IF EXISTS `t_mail_attach`;
CREATE TABLE `t_mail_attach` (
  `MailID` int(11) NOT NULL default -1 COMMENT '邮件唯一id',
  `TypeMisc1` int(11) NOT NULL default 0 COMMENT '奖励id',
  `TypeMisc2` int(11) NOT NULL default 0 COMMENT '奖励id',
  `TypeMisc3` int(11) NOT NULL default 0 COMMENT '奖励id',
  `Num1` int(11) NOT NULL default 0 COMMENT '奖励数量',
  `Num2` int(11) NOT NULL default 0 COMMENT '奖励数量',
  `Num3` int(11) NOT NULL default 0 COMMENT '奖励数量',
  `Valid` TINYINT(3) NOT NULL DEFAULT '0' COMMENT '是否有效',
  PRIMARY KEY  (`MailID`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_content ` 
--
DROP TABLE IF EXISTS `t_mail_content`;
CREATE TABLE `t_mail_content` (
  `MailID` int(11) NOT NULL default -1 COMMENT '邮件唯一id',
  `Title` varchar(50) NOT NULL COMMENT '邮件title',
  `Content` varchar(50) NOT NULL COMMENT '邮件内容',
  `Valid` TINYINT(3) NOT NULL DEFAULT '0' COMMENT '是否有效',
  PRIMARY KEY  (`MailID`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail_system ` 
--
DROP TABLE IF EXISTS `t_mail_system`;
CREATE TABLE `t_mail_system` (
  `MailID` int(11) NOT NULL default -1 COMMENT '邮件唯一id',
  `MinLv` smallint(5) NOT NULL COMMENT '最小等级',
  `MaxLv` smallint(5) NOT NULL COMMENT '最大等级',
  `Valid` TINYINT(3) NOT NULL DEFAULT '0' COMMENT '是否有效',
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



-- INSERT INTO `t_version` VALUES(1); 每次版本号需要+1
INSERT INTO `t_version`(verNum) VALUES(2);

DELIMITER ;;