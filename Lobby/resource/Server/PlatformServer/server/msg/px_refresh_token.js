var TOP = '..';

var types 		= require('../base_types');
var router		= require('./msg_handler');
var baseDefine  = require('../base_define');

var ByteBuffer 	= types.ByteBuffer;

function PXRefreshToken()
{
	this.plat = 0;
	this.result = 0;
	this.playerID = 0;
		
	// 360
	this.accessToken = "";
	this.refreshToken = "";
	this.expireIn = 0;
	this.getTokenTime = 0;
}

PXRefreshToken.prototype.write = function(buf)
{
	buf.writeUInt8(this.plat);
	buf.writeInt16(this.playerID);
	buf.writeUInt8(this.result);
	
	switch(this.plat){
	case baseDefine.PLAT_360:
		this.write360(buf);
		break;
	}
}

PXRefreshToken.prototype.write360 = function(buf)
{
	buf.writeInt32(this.refreshToken.length);
	buf.writeStr(this.refreshToken);
	
	buf.writeInt32(this.accessToken.length);
	buf.writeStr(this.accessToken);
	
	buf.writeInt32(this.expireIn);
	buf.writeInt32(this.getTokenTime);
}

PXRefreshToken.prototype.id = function()
{
	return router.msgIDs.PX_REFRESH_TOKEN;
}

exports.msg = PXRefreshToken;