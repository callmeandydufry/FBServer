var TOP = '..';

var types 		= require('../base_types');
var router		= require('./msg_handler');
var baseDefine  = require('../base_define');
var util 		= require('../util');
var LOG 		= util.LOG;

var ByteBuffer 	= types.ByteBuffer;

function PWRequestOrder()
{
	this.plat = 0;
	this.playerID = 0;
	this.result = 0;
	
	//Loco
	this.orderid = "";
	this.productid = "";
	
	//Tencent
	this.Tencent_reason = 0;
	this.Tencent_ProductIndex = -1;
	this.Tencent_ProductPrice = 0;
	this.Tencent_Billno = "";
	
	//Wayi
	this.Wayi_uid = "";
	this.Wayi_ProductIndex = -1;
	this.Wayi_ChargeDiamond = 0;
	this.Wayi_DonateDiamond= 0;
	this.Wayi_ProductPrice = 0;
	this.Wayi_Billno = "";
}

PWRequestOrder.prototype.write = function(buf)
{
	buf.writeUInt8(this.plat);
	buf.writeUInt32(this.playerID);
	buf.writeUInt8(this.result);
	switch(this.plat){
	case baseDefine.PLAT_LOCO:
	case baseDefine.PLAT_LOCO:
	case baseDefine.PLAT_UNIFIEDANDROID:
		this.writeLoco(buf);
		break;
	case baseDefine.PLAT_TENCENT_QQ:
	case baseDefine.PLAT_TENCENT_WEIXIN:
		this.writeTencent(buf);
		break;
	case baseDefine.PLAT_WAYI:
		this.writeWayi(buf);
		break;
	}
}


PWRequestOrder.prototype.writeLoco = function(buf)
{
	buf.writeInt32(this.orderid.length);
	buf.writeStr(this.orderid);
	buf.writeInt32(this.productid.length);
	buf.writeStr(this.productid);
}

PWRequestOrder.prototype.writeTencent = function(buf)
{
	buf.writeInt32(this.Tencent_reason);
	buf.writeInt32(this.Tencent_ProductIndex);
	buf.writeInt32(this.Tencent_ProductPrice);
	buf.writeInt32(this.Tencent_Billno.length);
	buf.writeStr(this.Tencent_Billno);
}

PWRequestOrder.prototype.writeWayi = function(buf)
{
	buf.writeInt32(this.Wayi_uid.length);
	buf.writeStr(this.Wayi_uid);
	buf.writeInt32(this.Wayi_ProductIndex);
	buf.writeInt32(this.Wayi_ChargeDiamond);
	buf.writeInt32(this.Wayi_DonateDiamond);
	buf.writeInt32(this.Wayi_ProductPrice);
	buf.writeInt32(this.Wayi_Billno.length);
	buf.writeStr(this.Wayi_Billno);
}

PWRequestOrder.prototype.id = function()
{
	return router.msgIDs.PW_REQUEST_ORDER;
}

exports.msg = PWRequestOrder;