var TOP = '..';

var types 		= require('../base_types');
var router		= require('./msg_handler');
var baseDefine  = require('../base_define');
var util 		= require('../util');
var LOG 		= util.LOG;

var ByteBuffer 	= types.ByteBuffer;

function PXRequestAuth()
{
	this.plat = 1;
	this.result = 0;
	this.playerID = 0;
	this.memberID = "";
}

PXRequestAuth.prototype.write = function(buf)
{
	buf.writeUInt8(this.plat);
	buf.writeInt16(this.playerID);
	buf.writeUInt8(this.result);
	buf.writeInt32(this.memberID.length);
	buf.writeStr(this.memberID);
}

PXRequestAuth.prototype.id = function()
{
	return router.msgIDs.PX_REQUEST_AUTH;
}

exports.msg = PXRequestAuth;