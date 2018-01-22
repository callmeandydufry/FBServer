var fs 		= require('fs');
var http 	= require('http');
var https 	= require('https');
var url 	= require('url');
var querystring = require('querystring');
var path = require('path');

var util 	= require('./util');

var LOGS = new Array();
var ORDERLOG = new Array();
var ERRORS = new Array();
var TDORDERLOGS = new Array();
var ArenaSprintMatchLOGS = new Array();
var CDKEYLOGS = new Array();

var CRASHS = new Array();

var LOG_PATH = './Server/Log';

function LOG(cxt)
{
	var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+" "+cxt+"\r\n";
	LOGS.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/platauth-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'
	if(LOGS.length <= 1){
		writeLOG(filename, LOGS);
	}
}

function LOGCDKey(cxt)
{
	var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+" "+cxt+"\r\n";
	CDKEYLOGS.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/platcdkey-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'
	if(CDKEYLOGS.length <= 1){
		writeLOG(filename, CDKEYLOGS);
	}
}

function LOGVerifyReceipt(cxt)
{
	var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+" "+cxt+"\r\n";
	LOGS.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/arenesprintmatch-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'
	if(LOGS.length <= 1){
		writeLOG(filename, LOGS);
	}
}

function LOGArenaSprintMatch(cxt)
{
	var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+" "+cxt+"\r\n";
	ArenaSprintMatchLOGS.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/arenesprintmatch-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'
	if(ArenaSprintMatchLOGS.length <= 1){
		writeLOG(filename, ArenaSprintMatchLOGS);
	}
}

function LOGTDOrderState(cxt)
{
	var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+" "+cxt+"\r\n";
	TDORDERLOGS.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/plattdorderstate-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'
	if(TDORDERLOGS.length <= 1){
		writeLOG(filename, TDORDERLOGS);
	}
}

function LOGOrder(cxt)
{
		var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+" "+cxt+"\r\n";
	ORDERLOG.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/platorder-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'

	if(ORDERLOG.length <= 1){
		writeLOG(filename, ORDERLOG);
	}
}

function writeLOG(filename, arr)
{
	if(arr.length > 0){
		var cnt = arr[0];
		fs.appendFile(filename, cnt, function(err){
			if(err) throw err;
			arr.shift();
			process.nextTick(function(){
				writeLOG(filename, arr);
			});
		});
	}
}

function ERROR(cxt)
{
	var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+cxt+"\r\n";
	ERRORS.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/error-platauth-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'
	if(ERRORS.length <= 1){
		writeLOG(filename, ERRORS);
	}
}

function CRASH(cxt)
{
	var d = new Date();
	var cnt = "("+d.getHours()+":"+d.getMinutes()+":"+d.getSeconds()+" "+d.getMilliseconds()+") "+cxt+"\r\n";
	CRASHS.push(cnt);
	console.log(cnt);
	
	var filename = LOG_PATH + '/crash-platauth-'+(d.getMonth()+1)+'-'+d.getDate()+'.log'
	if(CRASHS.length <= 1){
		writeLOG(filename, CRASHS);
	}
}
/*
  cb:
  function(err, res)
 */
function UrlGet(addr, cb)
{
	var parse_u = url.parse(addr, true);	
	var GET = parse_u.protocol == 'http:'?http.get:https.get;

	GET(addr, function(res){
		var data = "";
		res.on('data', function(chunk){
			data += chunk;
		});
		
		res.on('end', function(){
			try{
				var obj = JSON.parse(data);
				LOG("UrlGet addr="+addr+", result="+data);
				cb(null, obj);
			}
			catch(e){
				CRASH(e.stack);
				cb("error", null);
			}
		});
	}).on("error", function(e){
		ERROR("UrlGet error addr="+addr+", "+e.message);
		cb(e.message, null);
	});
}

/*
  cb:
  function(err, res)
 */
function UrlPost(addr, data, cb)
{
	var parse_u = url.parse(addr, true);
	var protocol = (parse_u.protocol == 'http:'?http:https);

//	var post_data = JSON.stringify(data);
//	var post_data = data;
		var querystring = require('querystring');
		var post_data = querystring.stringify(data);
	
/*	var options = {
		host:parse_u.hostname,
		port:(parse_u.protocol=='http:'?80:443),
		path:parse_u.path,
		method:'POST',
		headers:{
			"Content-Type":'application/json',
			"Content-Length":post_data.length
		}
	};*/
	
	
	var options = {
		host:parse_u.hostname,
		port:(parse_u.protocol=='http:'?80:443),
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
				var obj = JSON.parse(data);
				LOG("UrlPost addr="+addr+", result="+data);
				cb(null, obj);
			}
			catch(e){
				CRASH(e.stack);
				cb("error", null);
			}
		});
	}).on("error", function(e){
		LOG("UrlPost error:" + e.message);
		cb(e.message, null);
	});

	req.write(post_data);
	req.end();
}

function map_length(obj, cb)
{
	for(var i = 0; i < obj.length; ++i){
		cb(obj[i]);
	}
}

function map_length_disturb(obj, cb)
{
	for(var i = 0; i < obj.length; ++i){
		if(false == cb(obj[i]))
			return false;
	}
	return true;
}

function DEBUG_BYTE(buf)
{
	var bytesString = "";
	map_length(buf, function(n){
		bytesString += " ";
		bytesString += n.toString(16);
	});
//	LOG("bytes: "+bytesString);
}

function FilterAlphaNumberLine(str)
{
	return true;
	var result = /[0-9]*[a-z]*[A-Z]*\-*/.exec(str);
	if(result){
		result = result[0];
	}
	else{
		return false;
	}

	if(result.length != str.length){
		return false;
	}
	else{
		return true;
	}
}

exports.LOG 	= LOG;
exports.LOGOrder = LOGOrder;
exports.LOGCDKey = LOGCDKey;
exports.LOGTDOrderState = LOGTDOrderState;
exports.ERROR 	= ERROR;
exports.CRASH	= CRASH;
exports.UrlGet  = UrlGet;
exports.UrlPost = UrlPost;
exports.map_length = map_length;
exports.DEBUG_BYTE = DEBUG_BYTE;
exports.FilterAlphaNumberLine = FilterAlphaNumberLine;
exports.LOGArenaSprintMatch = LOGArenaSprintMatch;