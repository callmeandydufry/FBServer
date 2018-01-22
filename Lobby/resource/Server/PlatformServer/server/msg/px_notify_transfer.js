var TOP = '..';

var types 		= require('../base_types');
var router		= require('./msg_handler');
var baseDefine  = require('../base_define');

var ByteBuffer 	= types.ByteBuffer;

function PXNotifyTransfer()
{
	this.szAccount = "";
	this.szOrderID = "";
	this.GUID = -1;
	this.WorldID = -1;
	this.CoreMoney = 0;
	this.szProductID = "";
	this.Status = -1;
}

PXNotifyTransfer.prototype.write = function(buf)
{
	buf.writeUInt8(this.szAccount.length);
	buf.writeStr(this.szAccount);
	buf.writeUInt8(this.szOrderID.length);
	buf.writeStr(this.szOrderID);
	buf.writeInt64(this.GUID);
	buf.writeInt16(this.WorldID);
	buf.writeUInt32(this.CoreMoney);
	buf.writeInt8(this.szProductID.length);
	buf.writeStr(this.szProductID);
	buf.writeInt32(this.Status);
}


PXNotifyTransfer.prototype.id = function()
{
	return router.msgIDs.PX_NOTIFY_TRANSFER;
}

exports.msg = PXNotifyTransfer;
