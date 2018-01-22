var http = require('http');

var TOP = '..';
var LOG = require(TOP + '/util').LOG;
var baseDefine = require(TOP+'/base_define');

var platAuthMain	= require('../platauthMain').platAuthMain;

function msg(buffer)
{
	var tmp = buffer;
	this.plat = tmp.readUInt8();
	this.playerID = tmp.readUInt16();

	this.memberIDSize = tmp.readInt32();
	this.memberID     = tmp.readStr(this.memberIDSize);
	this.platUserIDSize = tmp.readInt32();
	this.platUserID     = tmp.readStr(this.platUserIDSize);
	this.TokenSize		= tmp.readInt32();
	this.Token				= tmp.readStr(this.TokenSize);
	this.MacAddressSize	= tmp.readInt32();
	this.MacAddress			= tmp.readStr(this.MacAddressSize);
	this.IpSize				= tmp.readInt32();
	this.Ip						= tmp.readStr(this.IpSize);
	this.DeviceTypeSize	= tmp.readInt32();
	this.DeviceType			= tmp.readStr(this.DeviceTypeSize);
	this.DeviceIDFASize		= tmp.readInt32();
	this.DeviceIDFA			= tmp.readStr(this.DeviceIDFASize);
	this.DeviceIDFVSize		= tmp.readInt32();
	this.DeviceIDFV			= tmp.readStr(this.DeviceIDFVSize);
	this.DeviceIDSize		= tmp.readInt32();
	this.DeviceID				= tmp.readStr(this.DeviceIDSize);
	
	this.Version			= tmp.readInt32();
	this.DeviceOSType	= tmp.readInt32();
	this.UserType			= tmp.readInt32();
	this.AppId				= tmp.readInt32();
	this.ChannelId		= tmp.readInt32();
	this.ChildChannel	= tmp.readInt32();
	this.GameId				= tmp.readInt32();
	this.GameServerId	= tmp.readInt32();
	this.ClientId			= tmp.readInt32();
	this.UserId				= tmp.readInt64();

	this.ChannelExtendDataSize	= tmp.readInt32();
	this.ChannelExtendData		= tmp.readStr(this.ChannelExtendDataSize);
	
	//防止设备id为空
	if(this.DeviceIDSize == 0 || this.DeviceID.length == 0)
	{
		this.DeviceID = this.memberID;
		this.DeviceIDSize = this.memberIDSize;
	}
	
	if(this.DeviceID.length == 0)
	{
		this.DeviceID = "invalid";
		this.DeviceIDSize = this.DeviceID.length;
	}
}

function handler()
{
}

handler.prototype.maxSize = function()
{
	//return 256;
	return 1000000;
}

handler.prototype.create = function(buffer)
{
	return new msg(buffer);
}

handler.prototype.handle = function(sock, msg)
{
	LOG(sock.sockID + ': handle xp request auth msg, plat:' + msg.plat+', playerid='+msg.playerID);

	platAuthMain(sock, msg);

	return true;
}

exports.handler = handler;
