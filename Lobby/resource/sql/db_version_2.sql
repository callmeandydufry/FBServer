
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

-- INSERT INTO `t_version` VALUES(1); 每次版本号需要+1
INSERT INTO `t_version`(verNum) VALUES(2);

DELIMITER ;;