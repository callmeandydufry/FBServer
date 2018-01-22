var TOP = '..';

var types 		= require('../base_types');
var router		= require('./msg_handler');
var baseDefine  = require('../base_define');
var util 		= require('../util');
var LOG 		= util.LOG;

var ByteBuffer 	= types.ByteBuffer;

function PWRequestPlayerInfo()
{
	this.opIndex = 0;
	this.uid = "";
	this.serverID = "";
}

PWRequestPlayerInfo.prototype.write = function(buf)
{
	buf.writeUInt32(this.opIndex);
	buf.writeInt32(this.uid.length);
	buf.writeStr(this.uid);	
	buf.writeInt32(this.serverID.length);
	buf.writeStr(this.serverID);	
}

PWRequestPlayerInfo.prototype.id = function()
{
	return router.msgIDs.PW_REQUESTPLAYERINFO;
}

exports.msg = PWRequestPlayerInfo;