var PXRequestAuth = require('./msg/px_request_auth').msg;
var https = require('https');
var http 		= require('http');
var util 		= require('./util');
var url 		= require('url');
var LOG 		= util.LOG;
var ERROR 		= util.ERROR;
var CRASH		= util.CRASH;
var baseDefine  = require('./base_define');
var platAuthRouteFunc = require('./Channels/platRoute').platAuthRouteFunc;

function authMainSuccess(sock, msg)
{
	var px = new PXRequestAuth();
	px.plat = baseDefine.PLAT_DEFAULT;
	px.result = 1;
	px.playerID = msg.playerID;
	px.memberID = msg.platUserID;
	
	sock.sendMsg(px);
}

function authMainError(sock, msg)
{
	var px = new PXRequestAuth();
	px.plat = baseDefine.PLAT_DEFAULT;
	px.playerID = msg.playerID;
	px.result = 0;
	sock.sendMsg(px);
}

function platAuthMain(sock, msg)
{
	platAuthRouteFunc(sock, msg.ChannelId, msg, authMainSuccess, authMainError);
}

exports.platAuthMain = platAuthMain;