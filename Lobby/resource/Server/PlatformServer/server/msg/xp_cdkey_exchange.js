var http = require('http');

var TOP = '..';
var LOGCDKey = require(TOP + '/util').LOGCDKey;

var baseDefine = require(TOP+'/base_define');

var CDKeyPlat2GMServer = require('../platcdkey').CDKeyPlat2GMServer;

function msg(buffer)
{
	var tmp = buffer;
	this.GameID = tmp.readInt32();
	this.Snid = tmp.readInt64();
	this.ServerId = tmp.readInt32();
	this.ChannelId = tmp.readInt32();
	this.GiftCodeSize = tmp.readInt32();
	this.GiftCode	= tmp.readStr(this.GiftCodeSize);
	this.Time =  tmp.readInt32();
	this.PlatUIDSize = tmp.readInt32();
	this.PlatUID	= tmp.readStr(this.PlatUIDSize);
}

function handler()
{
}

handler.prototype.maxSize = function()
{
	//return 256;
	return 1000000;
}

handler.prototype.create = function(buffer)
{
	return new msg(buffer);
}

handler.prototype.handle = function(sock, msg)
{
	LOGCDKey('Recieve Msg xp_cdkey_exchange: ' + sock.sockID + 'Snid='+msg.Snid + "GiftCode=" +msg.GiftCode + "ChannelId=" + msg.ChannelId);

	CDKeyPlat2GMServer(sock, msg);

	return true;
}

exports.handler = handler;
