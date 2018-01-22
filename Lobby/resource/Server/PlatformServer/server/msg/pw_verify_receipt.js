var TOP = '..';

var types 		= require('../base_types');
var router		= require('./msg_handler');
var baseDefine  = require('../base_define');
var util 		= require('../util');
var LOGVerifyReceipt 		= util.LOGVerifyReceipt;

var ByteBuffer 	= types.ByteBuffer;

function PWVerifyReceipt()
{
	
	this.Snid = -1;
	this.ChannelId = 0;
	this.Result = 0;
	this.ProductIdSize = 0;
	this.ProductId = "";
	this.OrderIdSize = 0;
	this.OrderId = "";
}

PWVerifyReceipt.prototype.write = function(buf)
{
	buf.writeInt32(this.Snid);
	buf.writeInt32(this.ChannelId);
	buf.writeInt32(this.Result);
	buf.writeInt32(this.ProductIdSize);
	buf.writeStr(this.ProductId);	
	buf.writeInt32(this.OrderIdSize);
	buf.writeStr(this.OrderId);	
}

PWVerifyReceipt.prototype.id = function()
{
	return router.msgIDs.PW_VERIFY_RECEIPT;
}

exports.msg = PWVerifyReceipt;