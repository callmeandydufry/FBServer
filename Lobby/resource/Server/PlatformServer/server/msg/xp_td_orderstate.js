var http = require('http');

var TOP = '..';
var LOGTDOrderState = require(TOP + '/util').LOGTDOrderState;

var baseDefine = require(TOP+'/base_define');

var TDOrderState2TD = require('../plattdorderstate').TDOrderState2TD;

function msg(buffer)
{
	var tmp = buffer;
	this.TDMsgId = tmp.readInt32();
	this.OSType = tmp.readInt32();
	if(this.OSType == 0)
	{
		this.OSStr = "IOS";
	}
	else
	{
		this.OSStr = "Android";
	}
	this.Money = tmp.readFloat();
	this.Diamon = tmp.readFloat();
	this.ServerId = tmp.readInt32();
	this.ChannelId = tmp.readInt32();
	this.ProductId = tmp.readInt32();
	this.ProductNameSize = tmp.readInt32();
	this.ProductName	= tmp.readStr(this.ProductNameSize);
	this.MoneTypeSize = tmp.readInt32();
	this.MoneyType	= tmp.readStr(this.MoneTypeSize);
	this.AccountSize = tmp.readInt32();
	this.Account	= tmp.readStr(this.AccountSize);
	this.OrderSize = tmp.readInt32();
	this.Order	= tmp.readStr(this.OrderSize);
	this.PayTimeSize = tmp.readInt32();
	this.PayTime	= tmp.readStr(this.PayTimeSize);
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
	LOGTDOrderState('Recieve Msg xp_td_ordersate: ' + sock.sockID + "Money="+msg.Money);
	TDOrderState2TD(sock, msg);
	return true;
}

exports.handler = handler;

