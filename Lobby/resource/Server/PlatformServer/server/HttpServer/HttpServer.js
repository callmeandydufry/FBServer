var http = require("http");
var url = require("url");
var requestHandlers = require("./HttpHandlers");

function handlerRoute(handle, pathname, response, request, queryData) {
  if (typeof handle[pathname] === 'function') {
    handle[pathname](response, request, queryData);
  } else {
    console.log("No request handler found for " + pathname);
    response.writeHead(404, {"Content-Type": "text/html"});
    response.write("404 Not found");
    response.end();
  }
}

function start() {
	var handle = {}
	handle["/add_game_money"] = requestHandlers.add_game_money;
	handle["/get_role_info"] = requestHandlers.get_role_info;
	
  function onRequest(request, response) {
    var postData = "";
    var urlObj = url.parse(request.url);
    var pathname = urlObj.pathname;
    var queryString = urlObj.query;
    
    request.setEncoding("utf8");

    request.addListener("data", function(postDataChunk) {
      postData += postDataChunk;
    });

    request.addListener("end", function() {
      handlerRoute(handle, pathname, response, postData, queryString);
    });
  }

  http.createServer(onRequest).listen(8888);
  console.log("HttpServer has started.");
}

exports.start = start;