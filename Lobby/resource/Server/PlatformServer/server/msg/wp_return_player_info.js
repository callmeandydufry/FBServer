var http = require('http');

var TOP = '..';
var LOG = require(TOP + '/util').LOG;

var onGameServerReponse = require('../HttpServer/HttpHandlers').onGameServerReponse;

function msg(buffer)
{
	var tmp = buffer;
	this.opIndex = tmp.readUInt32();
	this.retcode = tmp.readUInt32();
	this.PlayerNameSize = tmp.readInt32();
	this.PlayerName = tmp.readStr(this.PlayerNameSize);
	this.monthCardBuy = tmp.readInt32();
	this.growFund1Buy = tmp.readInt32();
	this.growFund2Buy = tmp.readInt32();
	this.growFund3Buy = tmp.readInt32();
}

function handler()
{
}

handler.prototype.maxSize = function()
{
	return 1000000;
}

handler.prototype.create = function(buffer)
{
	return new msg(buffer);
}

handler.prototype.handle = function(sock, msg)
{
	onGameServerReponse(msg.opIndex, msg);
	return true;
}

exports.handler = handler;
