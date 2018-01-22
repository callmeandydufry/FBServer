var http 		= require('http');
var https = require('https');

var util 		= require('./util');
var LOGTDOrderState	= util.LOGTDOrderState;

var url 		= require('url');
var ERROR 		= util.ERROR;
var CRASH		= util.CRASH;
var baseDefine  = require('./base_define');
var zlib		= require('zlib');
var querystring = require('querystring');

var GameKey= ""

var td_url = "http://api.talkinggame.com/api/charge/4200F4331766492EB2AC1D8EB03587EF"
var currencyExchangeRate = 0.0005;

function getTDPostData(msg)
{	
	var timestamp=new Date().getTime();
	
	var paymentType = "";
	if(msg.ChannelId == 10000)
	{
		paymentType = "GooglePlay";
	}
	else if(msg.ChannelId == 20000)
	{
		paymentType = "Appstore";
	}
	
	var productName = msg.ProductName;
	if(msg.ProductNameSize == 0)
	{
		productName = "ProductID_" + msg.ProductId;
	}
	
	var post_data=[{
		"msgID": "" + msg.TDMsgId,
	  "gameVersion": "1.0",
	  "OS": msg.OSStr,
	  "accountID": msg.Account,
	  "level": 1,
	  "gameServer": "" + msg.ServerId,
	  "orderID": msg.Order,
	  "iapID": productName,
	  "currencyAmount": msg.Money * currencyExchangeRate,
	  "currencyType": "CNY",
	  "virtualCurrencyAmount": msg.Diamon,
	  "paymentType": paymentType,
	  "status": "success",
	  "chargeTime": timestamp,
	  "mission": "",
	  "partner":"baiduyn"
	}];
	
	return post_data;
}

function orderResultTdOrder(sock, msg, res)
{
	
}

function TDOrderState2TD(sock, msg)
{
	var post_data = getTDPostData(msg);
	UrlPost(td_url, post_data, function(err, res){
		if(err){
			LOGTDOrderState("res error = " + err);
		}
		else{
			try{
				var obj = JSON.parse(res);
				LOGTDOrderState("res.Result="+res);
			}
			catch(e){
				CRASH(e.stack);
				LOGTDOrderState("res error: result JOSN parsed error");
			}
		}
	})
}

function UrlPost(addr, data, cb)
{
	var parse_u = url.parse(addr, true);
	var protocol = (parse_u.protocol == 'http:'?http:https);
	var post_data = JSON.stringify(data);
	
	LOGTDOrderState("post_data="+post_data);
	
	zlib.gzip(post_data, function(err, zipedData){
		if(err)
		{
			cb("zip post data error", null);
		}
		else
		{
			var options = {
				host:parse_u.hostname,
				port:(parse_u.protocol=='http:'?80:443),
				path:parse_u.path,
				method:'POST',
				headers:{
					"Content-Type":'application/x-www-form-urlencoded',
					"Accept-Encoding":'gzip',
					"Content-Length":zipedData.length
				}
			};
	
			var req = protocol.request(options, function(res){
				var data = [];
				res.on('data', function(chunk){
					data.push(chunk);
				});
			
				res.on('end', function(){
					try{
						data = Buffer.concat(data);
						zlib.gunzip(data, function(err, unzipedResult){
							if(err)
							{
								cb("unzip result data error : " + err, null);
							}
							else
							{
								cb(null, unzipedResult);
							}
						});
					}
					catch(e){
						CRASH(e.stack);
						cb("error", null);
					}
				});
			}).on("error", function(e){
				LOGTDOrderState("UrlPost error:" + e.message);
				cb(e.message, null);
			});
	
			//压缩成功之后，开始写入post数据
			req.write(zipedData);
			req.end();
		}	
	})
}
exports.TDOrderState2TD = TDOrderState2TD;