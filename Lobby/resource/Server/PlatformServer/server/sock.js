var net 		= require('net');
var util 		= require('./util');
var LOG 		= util.LOG;
var ERROR 		= util.ERROR;
var CRASH		= util.CRASH;
var map_length  = util.map_length;
var DEBUG_BYTE  = util.DEBUG_BYTE;

var router 		= require('./msg/msg_handler');

var ByteBuffer  = require('./base_types').ByteBuffer;
var SockObj		= require('./client_sock').ClientSock;

var headerSize  = require('./base_define').MSG_HEADER_SIZE;

var sockMgr = {};
var cliSock = null;

function sockStr(sock)
{
	return sock.remoteAddress+'('+sock.remotePort+')';
}

function getClientSock()
{
	return cliSock;
}

function removeSockObj(sockObj)
{
	if(sockObj){
		delete sockMgr[sockObj.sockID];
		sockObj.close();
	}
}

function checkLenData(bufArray, len)
{
	for(var i = 0; i < bufArray.length; ++i){
		var buf = bufArray[i];
		// ok
		if(buf.length >= len){
			return true;
		}
		// len -= buf.length
		else{
			len -= buf.length;
		}
	}

	return false;
}

// read and remove data from buffer array
function readLenData(bufArray, len)
{
	var ret = new Buffer(len);
	var offset = 0;
	while(bufArray.length > 0){
		var buf = bufArray[0];
		if(buf.length == len){
			buf.copy(ret, offset, 0, buf.length);
			bufArray.shift();

			return new ByteBuffer(ret);
		}
		else if(buf.length > len){
			buf.copy(ret, offset, 0, len);
			// slice source data
			bufArray[0] = buf.slice(len);

			return new ByteBuffer(ret);
		}
		else{
			buf.copy(ret, offset, 0, buf.length);
			offset += buf.length;
			len -= buf.length;
			bufArray.shift();
		}
	}
	return null;
}

// do not remove data from buffer array
function peekLenData(bufArray, len)
{
	var ret = new Buffer(len);
	var offset = 0;
	for(var i = 0; i < bufArray.length; ++i){
		var buf = bufArray[i];
		if(buf.length == len){
			buf.copy(ret, offset, 0, buf.length);
			
			return new ByteBuffer(ret);
		}
		else if(buf.length > len){
			buf.copy(ret, offset, 0, len);
			
			return new ByteBuffer(ret);
		}
		else{
			buf.copy(ret, offset, 0, buf.length);
			offset += buf.length;
			len -= buf.length;
		}
	}
	return null;
}

// read and remove data from buffer array
function skipLenData(bufArray, len)
{
	while(bufArray.length > 0){
		var buf = bufArray[0];
		if(buf.length == len){
			bufArray.shift();
			return;
		}
		else if(buf.length > len){
			// slice source data
			bufArray[0] = buf.slice(len);
			return;
		}
		else{
			len -= buf.length;
			bufArray.shift();
		}
	}
}

// handle msg
function handleInData(sockObj)
{
try{
	if(!sockObj) return;
	var inBufArray = sockObj.inData;
	if(false == checkLenData(inBufArray, headerSize)){
		return;
	}
	var header = peekLenData(inBufArray, headerSize);
	var msgID = header.readUInt16();
	var packUInt = header.readUInt32();
	var msgSize  = packUInt & 0xFFFFFF;
	var packIndex= packUInt >>> 24;
//	var packIndex = packUInt & 0xFF;
//	var msgSize = packUInt >>> 8;

	var msgHandler = router.getMsgHandler(msgID);
	if(null == msgHandler){
		ERROR(sockObj.sockID + ' no such handler for msg ' + msgID);
		removeSockObj(sockObj);
		return;
	}

	if(msgSize > msgHandler.maxSize()){
		ERROR(sockObj.sockID + ' recv size too large msgid ' + msgID + ' msgSize='+msgSize+' maxsize='+msgHandler.maxSize());
		
		removeSockObj(sockObj);
		return;
	}

	if(false == checkLenData(inBufArray, msgSize+headerSize)){
		LOG(sockObj.sockID + ' not enough buffer msgsize='+msgSize+', id='+msgID);	
		return;
	}
	
	var msgObj = msgHandler;

	LOG(sockObj.sockID + ' get msg ' + msgID + ' size ' + msgSize + ' index ' + packIndex);	
	skipLenData(inBufArray, headerSize);
	var msgBody = readLenData(inBufArray, msgSize);
	
	var msg = msgObj.create(msgBody);
	if(false == msgObj.handle(sockObj, msg)){
		ERROR('handle false msg ' + msgID);
	}
	
	process.nextTick(function(){
		handleInData(sockObj);
	});
}
catch(e){
	CRASH(e.stack);
	removeSockObj(sockObj);
}
}

function newServerSock(sock)
{
	var sockID = sockStr(sock);
	LOG("new sock " + sockID);
	var sockObj = new SockObj();

	sockObj.active 	= true;
	sockObj.sock 	= sock;
	sockObj.inData 	= new Array();
	sockObj.outData	= new Array();
	sockObj.sockID	= sockID;

	sockMgr[sockID] = sockObj;
	cliSock = sockObj;
	
	// data is a Buffer, not a string
	sock.on('data', function (data){
		LOG(sockID + ' data recv ' + data.length);
		DEBUG_BYTE(data);
		sockObj.inData.push(data);
		// 及时处理消息
		handleInData(sockObj);
	});
	
	sock.on('end', function (data){
		LOG(sockID + ' sock end');
		removeSockObj(sockObj);
	});
	
	sock.on('error', function (data){
		LOG(sockID + ' sock error');
		removeSockObj(sockObj);
	});
}

function init(port)
{
	LOG('router init');
	router.init();
	
	var server = net.createServer(newServerSock);
	server.listen(port);
	
	LOG('listen port ' + port);
	LOG('sock init end');
	
}

function ServerSock()
{
	this.init = init;
}

exports.serverSock = new ServerSock();
exports.getClientSock = getClientSock;