var PWVerifyReceipt = require('./msg/pw_verify_receipt').msg;

var https = require('https');
var http 		= require('http');
var util 		= require('./util');

var url 		= require('url');
var LOGOrder = util.LOGOrder;
var ERROR 		= util.ERROR;
var CRASH		= util.CRASH;
var baseDefine  = require('./base_define');
var GetServerName = baseDefine.GetServerName;
var GetSystemTypeByServerID = baseDefine.GetSystemTypeByServerID;

//是否开启google play服务端的公钥验证
var WAYI_GOOGLE_PUBLICKEY_CHECK = 1;

var crypto		= require('crypto');
var querystring = require('querystring');

var  appStore_test_url = "https://sandbox.itunes.apple.com/verifyReceipt"
var  appStore_url = "https://buy.itunes.apple.com/verifyReceipt"

var ChannelId_Android = 300000001;
var ChannelId_IOS = 20000;

//nodejs只能识别PEM格式的字符串，所以必须整理成如下格式
var GooglePlayReceiptPublicKey = '-----BEGIN PUBLIC KEY-----\n'+
'MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAj3WjEmX300xJGrnnfz1o\n' +
'ahY8mYqCJuWkhJN8oNrqWoVgQRkNTZcQgalpoD/f6XFWzzkLE+4OEkswpz2puJdk\n' +
'K/NKGWVqA+6cwaD+D5lwduxH3/jGhMkJKm1BK+o3IDjoUodqDvCeRzeZ4IqIcyMA\n' +
'VH0dfMASilRlwQ7vuoMAloXuIj+oiFGZO30urb/aYCrdgNtZb5KAeslJDf7uAzY2\n' +
'OEd/971GKoRICl5jCMOTUMawvw6TDdQfMMmmsvRDIbVjyIc55hwBpVyDPSrbaGQe\n' +
'RzoijbkFsgPte8+ghJsH06ivgCnbzqAp2vu39YP32rObdmGxfHprM4LdpiOFaFHh\n' +
'2QIDAQAB\n' +
'-----END PUBLIC KEY-----';

function getLocoPostData(msg)
{	
	if (msg.ChannelId == ChannelId_IOS)
	{
		var post_data={
		"receipt-data":msg.Token,
		}
		return post_data;
	}
	
	if (msg.ChannelId == ChannelId_Android)
	{
		var post_data={
		"receipt-data":msg.Token,
		}
		return post_data;
	}
}

function VerifyReceiptPlatResult(sock, msg, res)
{
	if(msg.ChannelId == ChannelId_IOS)
	{
		try
		{
			//status代表正确，这时才存在receipt这个参数
			if(res.status == 0)
			{
				if(res.receipt.bid != "com.leyinghudong.ppmr.yh")
				{
					LOGOrder("Appstore verify receipt error: PlayerSnid = " + msg.Snid + ", send cheat receipt!");
					return;
				}
			
				var px = new PWVerifyReceipt();
				px.Snid = msg.Snid;
				px.ChannelId = msg.ChannelId;	
				px.Result = res.status;
				px.ProductIdSize = res.receipt.product_id.length;
				px.ProductId = res.receipt.product_id;
				px.OrderIdSize = res.receipt.transaction_id.length;
				px.OrderId = res.receipt.transaction_id;
				sock.sendMsg(px);
			}
			else if(res.status == 21007)
			{
				var post_data = getLocoPostData(msg);
				//如果正式地址验证不过，则去sandbox地址上验证
				var url = appStore_test_url;
				UrlPost(url, post_data, function(err, res){
					if(err){
						LOGOrder("UrlPost error: " + err);
					}
					else{
						AppVerifyReceiptAgainResult(sock, msg, res);
					}
				})
			}
			else
			{
				LOGOrder("Appstore verify receipt error: status = " + res.status);
			}
		}
		catch(e)
		{
		  LOGOrder(e.stack);
			return;
		}
	}
	else
	{		
		var verResult = true;
		if(WAYI_GOOGLE_PUBLICKEY_CHECK == 1)
		{
			//此处的代码参考客户端java代码,IabHelper中的公钥验证
			//在此处加入google play收据的校验
			var verifier = crypto.createVerify("RSA-SHA1");
			verifier.update(msg.PurchaseData, 'utf8');
			verResult = verifier.verify(GooglePlayReceiptPublicKey, msg.PurchaseSign, 'base64');   //签名也是经过base64加密的
		}
		
		LOGOrder("GooglePlay receipt info: PurchaseData = " + msg.PurchaseData + ", PurchaseSign = " + msg.PurchaseSign + ", OrderId = " + msg.OrderId + ", ProductId = " + msg.ProductId);
		
		if(verResult)
		{
			var px = new PWVerifyReceipt();
			px.Snid = msg.Snid;
			px.ChannelId = msg.ChannelId;	
			px.Result = 0;
			px.ProductIdSize = msg.ProductIdSize;
			px.ProductId = msg.ProductId;
			px.OrderIdSize = msg.OrderIdSize;
			px.OrderId = msg.OrderId;
			sock.sendMsg(px);
		}
		else
		{
			LOGOrder("Google play verify receipt error: PlayerSnid = " + msg.Snid + ", send cheat receipt!");
			
			var px = new PWVerifyReceipt();
			px.Snid = msg.Snid;
			px.ChannelId = msg.ChannelId;	
			px.Result = 1;
			px.ProductIdSize = msg.ProductIdSize;
			px.ProductId = msg.ProductId;
			px.OrderIdSize = msg.OrderIdSize;
			px.OrderId = msg.OrderId;
			sock.sendMsg(px);
		}
	}
}


function AppVerifyReceiptAgainResult(sock, msg, res)
{
	if(msg.ChannelId == ChannelId_IOS)
	{
		//status代表正确，这时才存在receipt这个参数
		if(res.status == 0)
		{
			if(res.receipt.bid != "com.leyinghudong.ppmr.yh")
			{
				LOGOrder("Appstore verify receipt error: PlayerSnid = " + msg.Snid + ", send cheat receipt!");
				return;
			}
				
			var px = new PWVerifyReceipt();
			px.Snid = msg.Snid;
			px.ChannelId = msg.ChannelId;	
			px.Result = res.status;
			px.ProductIdSize = res.receipt.product_id.length;
			px.ProductId = res.receipt.product_id;
			px.OrderIdSize = res.receipt.transaction_id.length;
			px.OrderId = res.receipt.transaction_id;
			sock.sendMsg(px);
		}
		else
		{
			LOGOrder("Appstore verify receipt error: status = " + res.status);
			
			var px = new PWVerifyReceipt();
			px.Snid = msg.Snid;
			px.ChannelId = msg.ChannelId;	
			px.Result = res.status;
			px.ProductIdSize = msg.ProductIdSize;
			px.ProductId = msg.ProductId;
			px.OrderIdSize = msg.OrderIdSize;
			px.OrderId = msg.OrderId;
			sock.sendMsg(px);
		}
	}
}


function VerifyReceiptPlat2Purchase(sock, msg)
{
	var post_data = getLocoPostData(msg);
	
	var url = "";
	if(msg.ChannelId == ChannelId_IOS)
	{
		url = appStore_url;
	}
	else //google play暂时未加入验证，直接返回验证成功
	{
		VerifyReceiptPlatResult(sock, msg, null);
		return;
	}
	
	UrlPost(url, post_data, function(err, res){
		if(err){
			LOGOrder("UrlPost error: " + err);
		}
		else{
			VerifyReceiptPlatResult(sock, msg, res);
		}
	})
}

function UrlPost(addr, data, cb)
{
	var parse_u = url.parse(addr, true);
	var protocol = (parse_u.protocol == 'http:'?http:https);
	var port = (parse_u.protocol == 'http:'?80:443);

	var post_data = JSON.stringify(data);
		
	var options = {
		host:parse_u.hostname,
		port:port,
		path:parse_u.path,
		method:'POST',
		headers:{
			"Content-Type":'application/x-www-form-urlencoded',
			"Content-Length":post_data.length
		}
	};
	
	var req = protocol.request(options, function(res){
		var data = "";
		res.on('data', function(chunk){
			data += chunk;
		});
		
		res.on('end', function(){
			try{
				LOGOrder("UrlPost addr="+addr+", result="+data);
				var obj = JSON.parse(data);
				cb(null, obj);
			}
			catch(e){
				CRASH(e.stack);
				cb("error", null);
			}
		});
	}).on("error", function(e){
		LOGOrder("UrlPost error:" + e.message);
		cb(e.message, null);
	});

	req.write(post_data);
	req.end();
}
exports.VerifyReceiptPlat2Purchase = VerifyReceiptPlat2Purchase;