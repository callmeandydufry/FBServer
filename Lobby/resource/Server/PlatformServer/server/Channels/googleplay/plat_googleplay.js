
var http 		= require('http');
var https 		= require('https');
var util 		= require('../../util');
var url 		= require('url');
var LOG 		= util.LOG;
var ERROR 		= util.ERROR;
var CRASH		= util.CRASH;
var baseDefine  = require('../../base_define');
var crypto		= require('crypto');
var querystring = require('querystring');

//------------------------------------------------------------------------------------------
var request_auth_url = "http://api.icochat.com/server/account/userInfo"
var loopin_appkey = "com.baiduyn.indonesiamyth";
var loopin_appsecret = "803!8c557dd14c0*a5ed64b14b%16e2d";


function getHttpRequestGetData(msg)
{
	var signSrc = request_auth_url + "&GET&appkey="+loopin_appkey+"&sid="+msg.Token;
	signSrc = crypto.createHash('md5').update(signSrc).digest('hex');
	signSrc = signSrc + loopin_appsecret;
	var sign = crypto.createHash('md5').update(signSrc).digest('hex');
	var get_data ={
		"appkey" : loopin_appkey,
		"sid" : msg.Token,
		"signature": sign
	};
	
	return get_data;
}

function platAuth(sock, msg, successCallback, errorCallback)
{
	var get_data = getHttpRequestGetData(msg);
	UrlGet(
		request_auth_url, 
		get_data, 
		function(err, res)
		{
			if(err){
				errorCallback(sock, msg);
			}
			else{
				LOG("YNGooglePlay platAuth result: code=" + res.status);
				if(res.status == 0)
				{
					msg.platUserID = res.data.uid.toString();
					msg.platUserIDSize = msg.platUserID.length;
					
					successCallback(sock, msg);
				}
				else
				{
					errorCallback(sock, msg);
				}
			}
		}
	)
}

function UrlGet(addr, data, cb)
{
	var parse_u = url.parse(addr, true);
	var protocol = (parse_u.protocol == 'http:'?http:https);

	var get_data = querystring.stringify(data);

	var options = {
		host:parse_u.hostname,
		port:(parse_u.protocol=='http:'?80:443),
		path:parse_u.path + "?" + get_data,
		method:'GET',
		headers:{
			"Content-Type":'application/x-www-form-urlencoded',
			"Content-Length":0
		}
	};

	LOG("get_content="+get_data);

	var req = protocol.request(options, function(res){
		var data = "";
		res.on('data', function(chunk){
			data += chunk;
		});
		
		res.on('end', function(){
			try{
				var obj = JSON.parse(data);
				LOG("UrlGet data == " + data);
				cb(null, obj);
			}
			catch(e){
				CRASH(e.stack);
				cb("error", null);
			}
		});
	}).on("error", function(e){
		ERROR("UrlGet error:" + e.message);
		cb(e.message, null);
	});

	req.end();
}


exports.platAuth = platAuth;