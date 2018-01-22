var ByteBuffer  	= require('./base_types').ByteBuffer;
var MSG_HEADER_SIZE = require('./base_define').MSG_HEADER_SIZE;
var util			= require('./util');
var LOG				= util.LOG;
var DEBUG_BYTE		= util.DEBUG_BYTE;

function ClientSock()
{
	this.active 	= false;
	this.inData 	= null;
	this.outData 	= null;
	this.sock		= null;
	this.player		= null;
	this.sockID		= '';
	this.worldID	= 0;
	
	this.comIndex	= 0;
	
	this.writeBuffer = new ByteBuffer(1024);
	
	this.uniqueCode = '';
	this.account	= '';
	this.char_guid	= 0;
}

ClientSock.prototype.sendMsg = function(msg){
	if(this.active == false){
		return;
	}
	// reset pos
	this.writeBuffer.pos = 0;
	msg.write(this.writeBuffer);
	// get length
	var len = this.writeBuffer.pos;
	var body = new ByteBuffer(len);
	this.writeBuffer.buffer.copy(body.buffer, 0, 0, this.writeBuffer.pos);
	// head
	var head = new ByteBuffer(MSG_HEADER_SIZE);
	head.writeUInt16(msg.id());
	var pack = (this.comIndex << 24) + body.buffer.length;
	//var pack = (body.buffer.length << 8) + this.comIndex;
	head.writeUInt32(pack);
	this.outData.push(head.buffer);
	
	// body
	this.outData.push(body.buffer);
	LOG(this.sockID + ': send msg ' + msg.id() + ' size ' + body.buffer.length);
	DEBUG_BYTE(head.buffer);
	DEBUG_BYTE(body.buffer);
	
	this.flushOutData();
}

ClientSock.prototype.close	= function(){
	this.active 	= false;
	this.inData 	= null;
	this.outData 	= null;
	this.sock.destroy();
	this.sock		= null;
	this.player		= null;
	this.sockID		= '';

	this.uniqueCode = '';
	this.account	= '';
	this.char_guid	= 0;
}

ClientSock.prototype.flushOutData = function(){
	var sockObj = this;
	var outBuf = sockObj.outData;
	if(outBuf && outBuf.length > 0){
		var buf = outBuf[0];
		var ret = this.writeData(buf);

		// send all ok
		if(true == ret){
			LOG(sockObj.sockID + ': flush data full ' + buf.length);
			outBuf.shift();
			process.nextTick(function(){
				sockObj.flushOutData();
			});
		}
		// wait
		else if(false == ret){
			process.nextTick(function(){
				sockObj.flushOutData();
			});
		}
		// send partial
		else{
			LOG(sockObj.sockID + ': flush data partial ' + ret);
			outBuf[0] = buf.slice(ret);
			process.nextTick(function(){
				sockObj.flushOutData();
			});
		}
	}
}

// check if there any data pending
ClientSock.prototype.writeData = function(data){
	var sock = this.sock;
	if(sock.bufferSize > 0){
		return false;
	}
	else{
		// return ok
		if(true == sock.write(data)){
			return true;
		}
		// or queue buffer size
		else{
			return sock.bufferSize;
		}
	}
}

exports.ClientSock = ClientSock;