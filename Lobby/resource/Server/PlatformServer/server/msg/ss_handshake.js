var TOP = '..';
var LOG = require(TOP + '/util').LOG;

var router		= require('./msg_handler');
var register2BillingServer = require('../register2billingserver').register2BillingServer;

function msg(buffer)
{
	var tmp = buffer;
	this.worldID = tmp.readInt16();
	this.mainVersion = tmp.readInt16();
	this.subVersion = tmp.readInt16();
	this.innerVersion = tmp.readInt16();
	this.revision = tmp.readInt16();
}

msg.prototype.write = function(buf)
{
	buf.writeInt16(this.worldID);
	buf.writeInt16(this.mainVersion);
	buf.writeInt16(this.subVersion);
	buf.writeInt16(this.innerVersion);
	buf.writeInt16(this.revision);
}

msg.prototype.id = function()
{
	return router.msgIDs.SS_HAND_SHAKE;	
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
	LOG(sock.sockID + ': handle ss handshake , ' + msg.worldID);
	sock.worldID = msg.worldID;
	sock.sendMsg(msg);
	
	//当游戏服务器连入Node服务器的时候会将ServerId传过来，这时向支付订单服务器发起一个消息将当前服务器的ip和serverid进行一次注册
	register2BillingServer(sock.worldID, "ServerName");
	
	return true;
}

exports.handler = handler;