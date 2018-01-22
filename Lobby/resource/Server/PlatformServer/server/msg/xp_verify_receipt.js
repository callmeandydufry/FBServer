var http = require('http');

var TOP = '..';
var LOGVerifyReceipt = require(TOP + '/util').LOGVerifyReceipt;

var baseDefine = require(TOP+'/base_define');

var VerifyReceiptPlat2Purchase = require('../platVerifyReceipt').VerifyReceiptPlat2Purchase;

function msg(buffer)
{
	var tmp = buffer;
	this.Snid = tmp.readInt32();
	this.ChannelId = tmp.readInt32();
	this.ProductIdSize = tmp.readInt32();
	this.ProductId = tmp.readStr(this.ProductIdSize);
	this.OrderIdSize = tmp.readInt32();
	this.OrderId = tmp.readStr(this.OrderIdSize);
	this.TokenSize = tmp.readInt32();
	this.Token = tmp.readStr(this.TokenSize);
	this.PurchaseDataSize = tmp.readInt32();
	this.PurchaseData = tmp.readStr(this.PurchaseDataSize);
	this.PurchaseSignSize = tmp.readInt32();
	this.PurchaseSign = tmp.readStr(this.PurchaseSignSize);
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
	VerifyReceiptPlat2Purchase(sock, msg);
	return true;
}

exports.handler = handler;
