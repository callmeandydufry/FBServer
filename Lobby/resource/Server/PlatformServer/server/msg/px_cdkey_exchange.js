var TOP = '..';

var types 		= require('../base_types');
var router		= require('./msg_handler');
var baseDefine  = require('../base_define');
var util 		= require('../util');
var LOGCDKey 		= util.LOGCDKey;

var ByteBuffer 	= types.ByteBuffer;

function PXCDKeyExchange()
{
	this.Code = 0;
	this.Snid = -1;
	this.Count = 0;
	this.ActivityInfo = "";
	this.AwardStr = new Array(10);
}

PXCDKeyExchange.prototype.write = function(buf)
{
	buf.writeInt32(this.Code);
	buf.writeInt64(this.Snid);
	buf.writeInt32(this.Count);
	buf.writeInt32(this.ActivityInfo.length);
	buf.writeStr(this.ActivityInfo);
	
	for(var i= 0; i < this.Count; i++)
	{
		buf.writeInt32(this.AwardStr[i].length);		
		buf.writeStr(this.AwardStr[i]);
	}
}

PXCDKeyExchange.prototype.id = function()
{
	return router.msgIDs.PX_CDKEY_EXCHANGE;
}

exports.msg = PXCDKeyExchange;