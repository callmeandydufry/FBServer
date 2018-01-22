var querystring = require("querystring");
var sock = require('../sock');
var baseDefine  = require('../base_define');
var util 		= require('../util');
var LOGOrder = util.LOGOrder;
var crypto		= require('crypto');

var PWRequestOrder = require('../msg/pw_request_order').msg;
var PWRequestPlayerInfo = require('../msg/pw_request_player_info').msg;

var WAYI_SECRET = "8D4bP7s3h46MlnrFVDntwKFRJh500CXa";

//开启md5签名校验的控制变量
var WAYI_MD5_CHECK = 1;

var waitingResponseList = {};
var currResponseIndex = 1;

function onGameServerReponse(opIndex, svrMsg)
{
	if (typeof waitingResponseList[opIndex] == "function")
	{
		waitingResponseList[opIndex](svrMsg);
		delete waitingResponseList[opIndex];
	}
}

function add_game_money(response, postData, queryData)
{
  LOGOrder("add_game_money handler was called.");

	if(postData.length == 0 && queryData != null)
	{
		postData = queryData;
	}
	
	if(postData.length == 0)
	{
		var retParams = {
			"code":1,
			"msg":"Params is empty",
		};
		returnStr = JSON.stringify(retParams);
		response.writeHead(200, {"Content-Type": "text/html"});
	  response.write(returnStr);
	  response.end();
		return;
	}
	
	LOGOrder("add_game_money?postData=" + postData);
	
	try
	{
		var postObj = querystring.parse(postData);
		//做一次md5的签名校验
		if(WAYI_MD5_CHECK == 1)
		{
			var sign = "" + postObj.uid + postObj.money + postObj.orderid + postObj.unixtime + WAYI_SECRET;
			var signMD5 = crypto.createHash('md5').update(sign).digest('hex');
			var signmd5 = signMD5 + "";
			if(signmd5 != postObj.sign)
			{
				var retParams = {
					"code":1,
					"msg":"sign is not right",
				};
				returnStr = JSON.stringify(retParams);
				response.writeHead(200, {"Content-Type": "text/html"});
			  response.write(returnStr);
			  response.end();
				return;
			}
		}
		
		var productId = 12;
		if(postObj.itemid == "com.wayi.sg.pay_11")
		{
			productId = 8;
		}
		else if(postObj.itemid == "com.wayi.sg.pay_21")
		{
			productId = 9;
		}
		else if(postObj.itemid == "com.wayi.sg.pay_22")
		{
			productId = 10;
		}
		else if(postObj.itemid == "com.wayi.sg.pay_23")
		{
			productId = 11;
		}
		
	  var px = new PWRequestOrder();
		px.plat = baseDefine.PLAT_WAYI;
		px.playerID = 0;
		px.result = 1;
		px.Wayi_uid = postObj.uid;
		px.Wayi_ProductIndex = productId;
		px.Wayi_ChargeDiamond = parseInt(postObj.money);
		px.Wayi_DonateDiamond = parseInt(postObj.bonus);
		px.Wayi_ProductPrice = parseInt(postObj.price);
		px.Wayi_Billno = postObj.orderid;
		sock.getClientSock().sendMsg(px);
		
		var retParams = {
			"code":0,
			"msg":"SUCCESS",
		};
		returnStr = JSON.stringify(retParams);
	  response.writeHead(200, {"Content-Type": "text/html"});
	  response.write(returnStr);
	  response.end();
	}
	catch(e)
	{
		var retParams = {
			"code":1,
			"msg":"add money failed, param error",
		};
		returnStr = JSON.stringify(retParams);
		response.writeHead(200, {"Content-Type": "text/html"});
	  response.write(returnStr);
	  response.end();
	  
	  LOGOrder(e.stack);
	  
		return;
	}
}

function get_role_info(response, postData, queryData)
{
	LOGOrder("get_role_info handler was called.");
	
	if(postData.length == 0 && queryData != null)
	{
		postData = queryData;
	}
	if(postData.length == 0)
	{
		var retParams = {
			"code":1,
			"msg":"Empty params",
		};
		var returnStr = JSON.stringify(retParams);
		response.writeHead(200, {"Content-Type": "text/html"});
	  response.write(returnStr);
	  response.end();
		return;
	}
	
	try
	{
		LOGOrder("get_role_info?postData=" + postData);
		
		var postObj = querystring.parse(postData);
		
		if(WAYI_MD5_CHECK == 1)
		{
			//做一次md5的签名校验
			var sign = "" + postObj.uid + WAYI_SECRET;
			var signMD5 = crypto.createHash('md5').update(sign).digest('hex');
			var signmd5 = signMD5 + "";
			if(signmd5 != postObj.sign)
			{
				var retParams = {
					"code":1,
					"msg":"sign is not right",
				};
				returnStr = JSON.stringify(retParams);
				response.writeHead(200, {"Content-Type": "text/html"});
			  response.write(returnStr);
			  response.end();
				return;
			}
		}
	
		var px = new PWRequestPlayerInfo();
		px.opIndex = currResponseIndex;
		px.uid = postObj.uid;
		sock.getClientSock().sendMsg(px);
	}
	catch(e)
	{
		var retParams = {
			"code":1,
			"msg":"json parse failed, uid param not exist",
		};
		var returnStr = JSON.stringify(retParams);
		response.writeHead(200, {"Content-Type": "text/html"});
	  response.write(returnStr);
	  response.end();
		return;
	}
	
	waitingResponseList[currResponseIndex] = function(wpReturnPlayerInfoMsg){
		if(wpReturnPlayerInfoMsg == null){
			return;
		}
		
		var retParams = {
			"code":wpReturnPlayerInfoMsg.retcode,
			"msg":wpReturnPlayerInfoMsg.PlayerName,
			"monthcard":wpReturnPlayerInfoMsg.monthCardBuy,
			"growfund1":wpReturnPlayerInfoMsg.growFund1Buy,
			"growfund2":wpReturnPlayerInfoMsg.growFund2Buy,
			"growfund3":wpReturnPlayerInfoMsg.growFund3Buy,
		};
		returnStr = JSON.stringify(retParams);
		response.writeHead(200, {"Content-Type": "text/html"});
	  response.write(returnStr);
	  response.end();
	}
	
	//防止这个缓存的reponse队列越来越大
	setTimeout(function(){
		if(typeof waitingResponseList[currResponseIndex] != "undefined")
		{
			delete waitingResponseList[currResponseIndex];
		}
	} ,2000); 
	
	currResponseIndex = currResponseIndex + 1;
}

exports.add_game_money = add_game_money;
exports.get_role_info = get_role_info;
exports.onGameServerReponse = onGameServerReponse;