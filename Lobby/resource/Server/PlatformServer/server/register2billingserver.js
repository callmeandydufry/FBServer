
var http 		= require('http');
var util 		= require('./util');
var url 		= require('url');
var LOG 		= util.LOG;
var ERROR 		= util.ERROR;
var CRASH		= util.CRASH;
var baseDefine  = require('./base_define');
var crypto		= require('crypto');
var querystring = require('querystring');

var billingServerKey = "e47298bb1c843172a79d0df6fsdf0987";
//var billing_server_url = "http://127.0.0.1/YytxPayOrderServer/ServletRegisterServer";
var billing_server_url = "http://localhost:8080/YytxGMPlatform/cdkeyserverregister";
function getPostData(serverId, serverName)
{
	var sign = "serverId="+serverId + "serverName=" + serverName + billingServerKey;
	//var signMD5 = crypto.createHmac('sha1', uc_apikey).update(sign).digest('hex');
	var signMD5 = crypto.createHash('md5').update(sign).digest('hex');
	//LOG("sign="+sign+"signMD5="+signMD5);
	var timeStamp1 = parseInt(new Date().getTime()/1000);
	var post_data ={
		"serverId":serverId,
		"serverName":serverName,
		"sign":signMD5
	};
	
	return post_data;
}

function register2BillingServer(serverId, serverName)
{

	var post_data = getPostData(serverId, serverName);
//	LOG('start http.get '+uc_test_server);
	UrlPost(
		billing_server_url,
		post_data, 
		function(err, res)
		{
			if(err){
				LOG("register2BillingServer result: error=" +err);
			}
			else{
				var retcode = res.code;
				if(retcode == 1){
					LOG("register2BillingServer : SUCCESS");
				}
				else{
					LOG("register2BillingServer : FAILED");
				}
			}
		}
	)

}

function UrlPost(addr, data, cb)
{
//	LOG("UrlPost="+addr+" "+data);
	var parse_u = url.parse(addr, true);
	var protocol = (parse_u.protocol == 'http:'?http:https);

	var post_data = JSON.stringify(data);

	var options = {
		hostname:parse_u.hostname,
		port:(parse_u.protocol=='http:'?8080:443),   //这里的http请求是发给tomcat服务器，tomcat服务器的端口是8080
		path:parse_u.path,
		method:'POST',
		headers:{
			"Content-Type":'application/json',
			"Content-Length":post_data.length
		}
	};
	

	var req = protocol.request(options, function(res){
		try{
			var data = "";
			res.on('data', function(chunk){
				data += chunk;
			});	
			res.on('end', function(){
				try{
					var obj = JSON.parse(data);
					cb(null, obj);
				}
				catch(e){
					cb(e.stack, null);
				}
			});
		}
		catch(e){
			CRASH(e.stack);
			cb(e.stack, null);
		}
	}).on("error", function(e){
		ERROR("UrlPost error:" + e.message);
		cb(e.message, null);
	});

	req.write(post_data);
	req.end();
}

exports.register2BillingServer = register2BillingServer;