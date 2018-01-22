var PXCDKeyExchange = require('./msg/px_cdkey_exchange').msg;
var https	    = require('https');
var http 		= require('http');
var util 		= require('./util');
var LOGCDKey 		= util.LOGCDKey;

var url 		= require('url');
var LOGOrder = util.LOGOrder;
var ERROR 		= util.ERROR;
var CRASH		= util.CRASH;
var baseDefine  = require('./base_define');
var crypto		= require('crypto');
var querystring = require('querystring');

//�ֶ���̨��cdkey
///////////////////////////////////////////////////////////////////////////////////////////////////
var LocoGameKey= "182f029e069395753af6f65dbc08ef8d"
//var loco_url = "http://ot.locojoy.com/api/GiftCode/TakeGiftCode.ashx"
var loco_url = "http://localhost:8080/YytxGMPlatform/cdkeyexchange" // GM兑换路径
function locoGetPostData(msg)
{	
	var usign = LocoGameKey + msg.GameID + msg.Snid + msg.ServerId + msg.ChannelId + msg.GiftCode + msg.Time;
	var signMD5 = crypto.createHash('md5').update(usign).digest('hex');
			
	var post_data={
		"GameId":msg.GameID,
		"RoleId":msg.Snid,
		"ServerId":msg.ServerId,
		"ChannelId":msg.ChannelId,
		"GiftCode":msg.GiftCode,
		"Time":msg.Time,
		"Sign":signMD5
	}
	
	return post_data;
}

function LocoCDKeyResult(sock, msg, res)
{
	var px = new PXCDKeyExchange();
	var itemID = res.itemID;
	var itemNum = res.itemNum;
	var ids = new Array();
	var nums = new Array();
	ids = itemID.split("|");
	nums = itemNum.split("|");
	px.Code = parseInt(res.Code);
	if(px.Code == 2)
	{
		px.Code = 1;
	}
	px.Count = ids.length;
	px.Snid = msg.Snid;
	
	var size = ids.length;
	if(size>nums.length)
		size = numbs.length;
		 
	for(var i = 0;i<size;i++)
	{
		ids[i] += "|";
		ids[i] += nums[i];
		if(ids[i].length >255)
		{
			px.AwardStr[i] = ids[i].substr(0,255);
		}
		else
		{
			px.AwardStr[i] = ids[i];
		}
	}
	
	
	sock.sendMsg(px);
}

function locoErrorCDKeyResult(sock, msg, res)
{
	LOGCDKey("locoErrorCDKeyResult");
	var px = new PXCDKeyExchange();
	if(res == null)
	{
		px.Code = 99;
	}
	else
	{
		px.Code = parseInt(res.Code);
	}
	px.Snid = msg.Snid;
	px.Count = 0
	px.ActivityInfo = "";
	sock.sendMsg(px);
}

function CDKeyPlat2GMServer(sock, msg)
{
	var post_data = locoGetPostData(msg);
	LocoUrlGet(loco_url, post_data, function(err, res){
		if(err){
			LOGCDKey("util.LocoUrlGet err");
			locoErrorCDKeyResult(sock, msg, null);
		}
		else{
			if(res.Code > 0)
			{
				LocoCDKeyResult(sock, msg, res);
			}
			else
			{
				locoErrorCDKeyResult(sock, msg, res);
			}
		}
	})
}

function LocoUrlGet(addr, data, cb)
{
	var parse_u = url.parse(addr, true);
	var protocol = (parse_u.protocol == 'http:'?http:https);

	var post_data = JSON.stringify(data);
	//LOGCDKey(get_data);
	
	var options = {
		host:parse_u.hostname,
		port:8080,
		path:parse_u.path,
		method:'POST',
		headers:{
			"Content-Type":'application/json',
			"Content-Length":post_data.length
		}
	};
	

	//LOG("post_content="+post_data);
	var req = protocol.request(options, function(res){
		var data = "";
		res.on('data', function(chunk){
			data += chunk;
		});
		
		res.on('end', function(){
			try{
				LOGCDKey("UrlPost addr="+addr+", result="+data);
				var obj = JSON.parse(data);
				
				LOGCDKey("UrlPost addr="+addr+", result=var obj = JSON.parse(data);");
				
				
				
				cb(null, obj);
			}
			catch(e){
				CRASH(e.stack);
				cb("error", null);
			}
		});
	}).on("error", function(e){
		LOGCDKey("UrlPost error:" + e.message);
		cb(e.message, null);
	});
	req.write(post_data);
	req.end();
}
exports.CDKeyPlat2GMServer = CDKeyPlat2GMServer;
