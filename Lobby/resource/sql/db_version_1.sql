

--
-- Table structure for table `t_orderlist`  订单表
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
-- Table structure for table `t_postrelation`  邮件关系对应表
--
DROP TABLE IF EXISTS `t_postrelation`;
CREATE TABLE `t_postrelation` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '角色GUID',
  `relationdata` varchar(768) NOT NULL default '0' COMMENT '对应关系',
  `readflag` varchar(768) NOT NULL default '0' COMMENT '是否已经读了',
  `importflag` varchar(768) NOT NULL default '0' COMMENT '是否重要',
  PRIMARY KEY  (`aid`),
  KEY `Index_it_playerguid` (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_version`  奖励表
--
DROP TABLE IF EXISTS `t_version`;
CREATE TABLE `t_version` (
  `aid` bigint(20) NOT NULL auto_increment,
  `verNum` int(11) NOT NULL COMMENT '版本号',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Table structure for table `t_arenatoplist`  竞技场排行榜表
DROP TABLE IF EXISTS `t_cdkey`;
CREATE TABLE `t_cdkey` (
  `aid` bigint(20) NOT NULL auto_increment,
  `cdkey` varchar(30) default '' COMMENT 'cdkey',
  `title` varchar(100) default '' COMMENT '名称',
  `cdkeyclass` int(10)  NOT NULL COMMENT 'cdkey大类型',
  `cdkeytype` int(10)  NOT NULL COMMENT 'cdkey类型',
  `iscount` int(10)  NOT NULL COMMENT '是否计数',
  `begintime` int(10) unsigned NOT NULL COMMENT '开始时间',
  `endtime` int(10) unsigned NOT NULL COMMENT '结束时间',
  `guid` int(10) unsigned NOT NULL COMMENT '领取者数据标识GUID',
  `item1` int(11) default '-1' COMMENT '物品1',
  `itemcount1` int(11) default '0' COMMENT '物品1数量',
  `item2` int(11) default '-1' COMMENT '物品2',
  `itemcount2` int(11) default '0' COMMENT '物品2数量',
  `item3` int(11) default '-1' COMMENT '物品3',
  `itemcount3` int(11) default '0' COMMENT '物品3数量',
  `item4` int(11) default '-1' COMMENT '物品4',
  `itemcount4` int(11) default '0' COMMENT '物品4数量',
  `item5` int(11) default '-1' COMMENT '物品5',
  `itemcount5` int(11) default '0' COMMENT '物品5数量',
  `item6` int(11) default '-1' COMMENT '物品6',
  `itemcount6` int(11) default '0' COMMENT '物品6数量',
  `item7` int(11) default '-1' COMMENT '物品7',
  `itemcount7` int(11) default '0' COMMENT '物品7数量',
  `item8` int(11) default '-1' COMMENT '物品8',
  `itemcount8` int(11) default '0' COMMENT '物品8数量',
  `item9` int(11) default '-1' COMMENT '物品9',
  `itemcount9` int(11) default '0' COMMENT '物品9数量',
  `item10` int(11) default '-1' COMMENT '物品10',
  `itemcount10` int(11) default '0' COMMENT '物品10数量',
  `valid` int(11) NOT NULL default '0' COMMENT '有效标记位',
  PRIMARY KEY  (`aid`),
  KEY `Index_cdkey` (`cdkey`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Table structure for table `t_arenatoplist`  竞技场排行榜表
DROP TABLE IF EXISTS `t_arenatoplist`;
CREATE TABLE `t_arenatoplist` (
  `aid` bigint(20) NOT NULL auto_increment,
  `place` int(11) NOT NULL COMMENT '排序位置',
  `guid` int(10) unsigned NOT NULL COMMENT '数据标识GUID',
  `level` int(11) NOT NULL COMMENT '等级',
  `playername` varchar(60) default '' COMMENT '角色名',
  `fightappraise` int(11) NOT NULL COMMENT '战斗力',
  `heroindex` int(11) NOT NULL COMMENT '英雄索引',
  `herograde` int(11) NOT NULL COMMENT '英雄阶位',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_awardpost`  奖励表
--
DROP TABLE IF EXISTS `t_awardpost`;
CREATE TABLE `t_awardpost` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '被奖励GUID',
  `flag` int(10)  NOT NULL COMMENT '奖励类型',
  `title` varchar(50) NOT NULL COMMENT '奖励标题',
  `awarddata` varchar(200) NOT NULL COMMENT '奖励数据',
  `createtime` int(10) unsigned NOT NULL COMMENT '邮件创建时间',
  `valid` int(11) NOT NULL default '0' COMMENT '有效标记位',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
-- Table structure for table `t_herocard`  英雄表
--
DROP TABLE IF EXISTS `t_herocard`;
CREATE TABLE `t_herocard` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '角色GUID',
  `heroid` int(11) NOT NULL COMMENT '卡牌id',
  `slot` smallint(6) NOT NULL COMMENT '卡牌位置',
  `battleslot` smallint(6) NOT NULL COMMENT '出阵位置',
  `valid` smallint(6) NOT NULL default '1' COMMENT '有效标记位',
  `version` int(11) NOT NULL default '0' COMMENT '数据版本',
  `dataproperty` varchar(768) NOT NULL default '0' COMMENT '卡牌详细属性数据',
  `skillproperty` varchar(768) NOT NULL default '0' COMMENT '卡牌技能参数',
  PRIMARY KEY  (`aid`),
  KEY `Index_it_playerguid` (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_item`  物品表
--
DROP TABLE IF EXISTS `t_item`;
CREATE TABLE `t_item` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '角色GUID',
  `guid` int(11) NOT NULL COMMENT '物品ID guid位',
  `world` int(11) NOT NULL COMMENT '物品ID world位',
  `server` int(11) NOT NULL COMMENT '物品ID server位',
  `itemclass` int(11) NOT NULL COMMENT '物品类型',
  `bagtype` int(11) NOT NULL COMMENT '背包类型',
  `slot` smallint(6) NOT NULL COMMENT '物品位置',
  `creator` varchar(40) default '0' COMMENT '物品创建者',
  `valid` smallint(6) NOT NULL default '1' COMMENT '有效标记位',
  `version` int(11) default '0' COMMENT '数据版本',
  `dataproperty` varchar(768) NOT NULL default '0' COMMENT '物品详细属性数据',
  `data` varchar(40) NOT NULL COMMENT '物品参数数据',
  `count` int(10) unsigned NOT NULL COMMENT '物品数量',
  PRIMARY KEY  (`aid`),
  KEY `Index_it_playerguid` (`playerguid`),
  KEY `Index_it_itemguid` (`guid`,`world`,`server`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_itemserial`  物品GUID分配表
--
DROP TABLE IF EXISTS `t_itemserial`;
CREATE TABLE `t_itemserial` (
  `aid` bigint(20) NOT NULL auto_increment,
  `sid` int(11) default NULL COMMENT '服务器ID',
  `dbskey` int(11) default NULL COMMENT '未使用',
  `sn` int(11) default NULL COMMENT '物品GUID分配序号',
  PRIMARY KEY  (`aid`),
  KEY `Index_tik_sid` (`sid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_mail`  邮件表
--
DROP TABLE IF EXISTS `t_mail`;
CREATE TABLE `t_mail` (
  `aid` bigint(20) NOT NULL auto_increment,
  `mailid` int(11) unsigned NOT NULL COMMENT '邮件唯一id,自增序列',
  `sendername` varchar(50) NOT NULL COMMENT '发件人角色名',
  `receivername` varchar(50) NOT NULL COMMENT '收件人角色名',
  `mailtitle` varchar(50) NOT NULL COMMENT '邮件标题',
  `mailcontent` text NOT NULL COMMENT '邮件内容',
  `pindex` int(11) NOT NULL COMMENT '邮件编号',
  `isimport` int(11) NOT NULL default '0' COMMENT '邮件是否重要',
  `valid` int(11) NOT NULL default '0' COMMENT '有效标记位',
  `playerguid` int(10) unsigned default '4294967295' COMMENT '发件者GUID',
  `portrait` int(11) NOT NULL COMMENT '发件者头像',
  `flag` int(10) unsigned NOT NULL COMMENT '邮件类型:0普通邮件 1普通邮件 2脚本邮件 3系统邮件',
  `createtime` int(10) unsigned NOT NULL COMMENT '邮件创建时间',
  `isread` int(10) unsigned NOT NULL COMMENT '是否以读',
  `awarddata` varchar(300) NOT NULL COMMENT '奖励数据',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_mail_pIndex` (`pindex`),
  KEY `Index_recer` USING HASH (`receivername`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_player` 角色表
--
DROP TABLE IF EXISTS `t_player`;
CREATE TABLE `t_player` (
  `aid` bigint(20) NOT NULL auto_increment,
  `accountname` varchar(100) NOT NULL COMMENT '帐号名',
  `playerguid` int(10) unsigned NOT NULL COMMENT '角色GUID',
  `playername` varchar(50) NOT NULL COMMENT '角色名',
  `isfirstlogin` smallint(6) unsigned NOT NULL default '1' COMMENT '首次登录标记',
  `createtime` int(11) NOT NULL COMMENT '创建时间',
  `level` int(11) NOT NULL COMMENT '等级',
  `exp` bigint(20) NOT NULL COMMENT '经验',
  `silver` int(11) NOT NULL COMMENT '金币',
  `diamon` int(11) NOT NULL COMMENT '钻石',
  `rmbdiamon` int(11) NOT NULL COMMENT '人民币钻石',
  `magicstone` int(11) NOT NULL COMMENT '秘法石',
  `herosoul` int(11) NOT NULL COMMENT '将魂',
  `recallpaper` int(11) NOT NULL COMMENT '召唤符',
  `honor` int(11) NOT NULL COMMENT '勋章',
  `crystal` int(11) NOT NULL COMMENT '洗练石',
  `heroexp` int(11) NOT NULL COMMENT '圣光',
  `spirit` int(11) NOT NULL COMMENT '精力',
  `spiritbuycount` smallint(6) NOT NULL COMMENT '精力购买次数',
  `physicalpower` int(11) NOT NULL COMMENT '体力',
  `physicalbuycount` smallint(6) NOT NULL COMMENT '体力购买次数',
  `arenabuycount` smallint(6) NOT NULL COMMENT '竞技场购买次数',
  `arenaleftcount` smallint(6) NOT NULL COMMENT '竞技场挑战剩余次数',
  `arenawinstreak` smallint(6) NOT NULL COMMENT '竞技场连胜次数',
  `arenamaxrank` int(11) NOT NULL default 102400 COMMENT '竞技场最高名次',
  `arenachallengecd` int(11) NOT NULL default 0 COMMENT '竞技场挑战cd',
  `zhanyidetail` text not null default '' COMMENT '战役详细数据',
  `achievement` text not null default '' COMMENT '成就数据',
  `dailytask` text not null default '' COMMENT '日常数据',
  `orderlist`	text not null default '' default '' COMMENT	'订单数据',
  `shopdata` text not null default '' COMMENT '商店数据',
  `bankallmoney` bigint(20) NOT NULL COMMENT '银行兑换的总金额',
  `questdata` text not null default '' COMMENT '自定义扩展数据',
  `onlinetime` int(11) NOT NULL default 0 COMMENT '上次登录在线时间',
  `onlinetimetoday` int(11) unsigned NOT NULL default 0 COMMENT '当天在线时长',
  `totalonlinetime` int(11) unsigned NOT NULL default 0 COMMENT '总在线时长',
  `logintime` int(11) NOT NULL COMMENT '上线时间',
  `logouttime` int(11) NOT NULL COMMENT '下线时间',
  `lastloginip` varchar(30) NOT NULL default '' COMMENT '最后登陆的ip地址',
  `version` int(11) NOT NULL COMMENT '版本号',
  `map` int(11) NOT NULL COMMENT '场景ID',
  `x` int(11) NOT NULL COMMENT '坐标位置x',
  `z` int(11) NOT NULL COMMENT '坐标位置z',
  `hp` int(11) NOT NULL COMMENT '血量',
  `playerworld` smallint(6) unsigned NOT NULL,
  `isdelchar` smallint(6) NOT NULL COMMENT '玩家删除标记',
  `crc` int(10) unsigned zerofill NOT NULL,
  `valid` smallint(6) NOT NULL COMMENT '有效标记位',
  `lockdata` smallint(6) NOT NULL default '0' COMMENT '玩家是否被锁定，锁定状态只能写入不能读取',
  `forbidlogintime` int(11) unsigned NOT NULL default 0 COMMENT '禁止登录',
  `forbidchattime` int(11) unsigned NOT NULL default 0 COMMENT '禁止发言',
  `lastresetyearday` int(11) NOT NULL default '-1' COMMENT '上次系统重置日期',
  `macaddress` varchar(50) NOT NULL default ''  COMMENT '玩家创建角色的mac地址',
  `channelid` int(11) NOT NULL default '0' COMMENT '渠道id',
  `nwvipexp` int(11) unsigned NOT NULL default '0'  COMMENT 'vip经验',
  `nwviplevel` int(11) NOT NULL default '0' COMMENT 'vip等级',
  `playersimpledata` text not null default '' COMMENT '简单英雄数据',
  `rmb` int(11) NOT NULL default '0' COMMENT '充值的人民币',
  `chargesum` int(11) NOT NULL default '0' COMMENT '充值的总（真金）钻石数量不包括赠金和绑金',
  `donatediamond` int(11) NOT NULL default '0' COMMENT '赠送的钻石',
  `monthalldiamond` int(11) NOT NULL default '0' COMMENT '月卡领取的钻石数量',
  `arenadata` text not null default '' COMMENT '竞技场数据',
  `guildid` int(10) NOT NULL default '-1' COMMENT '帮会id',
  `leaveguildtime` int(11) unsigned default '0' NOT NULL COMMENT '离开帮会时间',
  `guildcontribute` int(10) unsigned NOT NULL default '0'  COMMENT '帮会贡献',
  `idfa` varchar(50) NOT NULL default '' COMMENT 'ios设备的标志',
  `deviceos` int(6) NOT NULL default '0' COMMENT '设备操作系统类型',
  `platuserid` bigint(20) NOT NULL default '0' COMMENT '三方平台返回的用户id',
  `brothercoin` int(11) default '0' NOT NULL COMMENT '兄弟币',
  `vipawardflag` int(10) not null default '0' COMMENT 'vip奖励领取标识',
  `monthcardlastdays` int(10) not null default '0' COMMENT '月卡的剩余天数',
  `monthcardgetdays` int(10) not null default '0' COMMENT '月卡的领取天数',
  `monthcardendtime` varchar(30) not null default '' COMMENT '月卡的失效日期',
  `chargeallsum` int(10) not null default '0' COMMENT '总共充值的真金数',
  `donateallsum` int(10) not null default '0' COMMENT '总共赠金',
  `chargeallsumcurrday` int(10) not null default '0' COMMENT '今天获得的真金',
  `donateallsumcurrday` int(10) not null default '0' COMMENT '今天获得的赠金',
	`gameid` int(10) not null default '0' COMMENT '分配给游戏的id',
	`gameserverid` int(10) not null default '0' COMMENT '游戏服务器的id',
	`imei` varchar(60) NOT NULL default '' COMMENT '设备唯一标识码',
	`accountid` varchar(100) NOT NULL default '' COMMENT '帐号id',
	`registerip` varchar(24) NOT NULL default '' COMMENT 'opengl render信息',
	`telecomoper` varchar(50) NOT NULL default '' COMMENT '用户网络运营商',
	`devicehardware` varchar(100) NOT NULL default '' COMMENT '手机型号，如”iPhone 6 (A1549/A1586)“，“iPad Air (A1474)”等等',
	`devicesoftware` varchar(100) NOT NULL default '' COMMENT '手机操作系统，如IOS 7.0 ,ANDROID 3.0等等',
	`screenwidth` int(10) not null default '0' COMMENT '显示屏宽度',
	`screenhight` int(10) not null default '0' COMMENT '显示屏高度',
	`density` float(20) not null default '0' COMMENT '像素密度',
	`cpuhardware` varchar(100) NOT NULL default '' COMMENT '字符串存储格式为（|分割）：cpu类型|频率|核数',
	`memory` int(10) not null default '0' COMMENT '内存容量（单位G）',
	`glrender` varchar(100) NOT NULL default '' COMMENT 'opengl render信息',
	`glversion` varchar(100) NOT NULL default '' COMMENT 'opengl render信息',
	`deviceid` varchar(201) NOT NULL default '' COMMENT '设备唯一id，如mac或者idfa',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_char_playerguid` (`playerguid`),
  UNIQUE KEY `Index_char_playername` USING HASH (`playername`),
  KEY `Index_char_accname` (`accountname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `t_playerext` 角色表扩展表
--
DROP TABLE IF EXISTS `t_playerext`;
CREATE TABLE `t_playerext` (
  `playerguid` int(10) unsigned NOT NULL COMMENT '角色GUID',
  `offlineaward` text not null default '' COMMENT '离线时间奖励玩法',
  `playercdkey` varchar(500) NOT NULL default '0'  COMMENT '玩家身上保持的cdkey数据',
  `assigngift` text NOT NULL default ''  COMMENT '日连续签到月签到等级礼包',
  `zhanyifinishdata` text NOT NULL default ''  COMMENT '副本完成次数',
  PRIMARY KEY  (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_playerrename`
--
DROP TABLE IF EXISTS `t_playerrename`;
CREATE TABLE `t_playerrename` (
  `aid` bigint(20) NOT NULL auto_increment,
  `playerguid` int(10) unsigned NOT NULL COMMENT '角色GUID',
  `oldname` varchar(50) NOT NULL COMMENT '旧角色名',
  `newname` varchar(50) NOT NULL default '@' COMMENT '新角色名',
  PRIMARY KEY  (`aid`),
  UNIQUE KEY `Index_newname` USING HASH (`newname`),
  KEY `Index_char_playerguid` (`playerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_toplist`  排行榜表
--
DROP TABLE IF EXISTS `t_toplist`;
CREATE TABLE `t_toplist` (
  `aid` bigint(20) NOT NULL auto_increment,
  `toplisttype` int(11) NOT NULL default '-1' COMMENT '数据种类',
  `dataversion` int(11) NOT NULL default 0 COMMENT '数据版本号',
  `toplistkey` text not null default '' COMMENT 'key',
  `toplistdata` text not null default '' COMMENT '数据',
  `guildbossdata` text not null default '' COMMENT 'guildbossdata',
  `forbidsort` int(11) NOT NULL default '0' COMMENT '当前项目是否进行排序',
  `fightval` int(11)  NOT NULL default '0' COMMENT '玩家战力',
  `guid` int(11) unsigned default NULL COMMENT 'GUID',
  PRIMARY KEY  (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_guild`  帮会表
--
DROP TABLE IF EXISTS `t_guild`;
CREATE TABLE `t_guild` (
  `aid` bigint(20) NOT NULL auto_increment,
  `guildid` int(10) NOT NULL default '-1' COMMENT '帮会ID',
  `guildname` varchar(30) NOT NULL COMMENT '帮会名',
  `guildnotice` varchar(256) NOT NULL default '' COMMENT '帮会公告',
  `chairmanid` int(10) unsigned NOT NULL COMMENT '帮主GUID',
  `chairmanname` varchar(32) NOT NULL default '' COMMENT '帮主角色名',
  `guildlvl` int(11) NOT NULL COMMENT '帮会等级',
  `guildicon` smallint(6) NOT NULL default '1' COMMENT '帮会图标',
  `memcount` int(11) NOT NULL COMMENT '帮会成员个数',
  `time` int(11) NOT NULL COMMENT '帮会成立时间',
  `joinlvl` int(11) NOT NULL COMMENT '进入帮会等级限制',
  `joinlimit` smallint(11) NOT NULL COMMENT '帮会权限',
  `lastresetyearday` int(11) NOT NULL default '-1' COMMENT '上次系统重置日期',
  `activity` int(11) NOT NULL default '0' COMMENT '活跃度',
  `valid` int(11) NOT NULL COMMENT '有效标记位',
  PRIMARY KEY  (`aid`),
  KEY `guildid` (`guildid`),
  KEY `guildname` USING HASH (`guildname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_guildmember`  帮会成员表
--
DROP TABLE IF EXISTS `t_guildmember`;
CREATE TABLE `t_guildmember` (
  `aid` bigint(20) NOT NULL auto_increment,
  `guildid` int(10) NOT NULL default '-1' COMMENT '帮会ID',
  `userguid` int(10) unsigned NOT NULL COMMENT '帮会成员GUID',
  `username` varchar(32) NOT NULL COMMENT '帮会成员角色名',
  `position` int(11) NOT NULL COMMENT '帮会职位',
  `lastlogintime` int(10) unsigned NOT NULL COMMENT '上一次登录时间',
  `level` int(10) unsigned NOT NULL COMMENT '等级',
  `heroid` int(10) NOT NULL COMMENT '英雄id',
  `herograde` smallint(6) NOT NULL default '0' COMMENT '英雄阶位',
  `herostar` smallint(6) NOT NULL default '1' COMMENT '英雄星级',
  PRIMARY KEY  (`aid`),
  KEY `guildid` (`guildid`),
  KEY `userguid` (`userguid`),
  KEY `username` USING HASH (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_friend` 角色表扩展表
--
DROP TABLE IF EXISTS `t_friend`;
CREATE TABLE `t_friend` (
  `playerguid` int(10) unsigned NOT NULL COMMENT '角色GUID',
  `getphysicalcount` smallint(6) NOT NULL default '0' COMMENT '领取体力次数',
  `lastresetday` int(11) default '-1' COMMENT '上次更新日期',
  `friendlist` text not null default '' COMMENT '好友列表',
  `applylist` text not null default ''  COMMENT '申请列表',
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
-- INSERT INTO `t_version` VALUES(1); 每次版本号需要+1
INSERT INTO `t_version`(verNum) VALUES(1);

call proc_init_data();
DROP PROCEDURE IF EXISTS `proc_init_data`;

DELIMITER ;;