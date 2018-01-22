var sock = require("./sock").serverSock;
var fs = require('fs');
var util = require("./util");
var httpServerStart = require("./HttpServer/HttpServer").start;

var LOG = util.LOG;
var CHECKLOGPATH = util.CHECKLOGPATH;

var port = 2234;

function init()
{
    
	LOG('server init');
	
	sock.init(port);
	
	//国内乐动服务器不能开http服务器，否则无法在一台物理机上启动多个服务器
	//httpServerStart();
	
	LOG('server init end');

	var pid = "" + process.pid;
	fs.writeFile("./Server/NodeJs.pid", pid, function(err){  
        if(err)
            console.log("generate pid file fail: " + err);  
        else  
            console.log("generate pid file ok");  
    	});
}

// init
init();
