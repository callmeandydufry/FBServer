var util = require("../util");
var LOG 	= util.LOG;
var ERROR 	= util.ERROR;
var CRASH	= util.CRASH;

//------------------------------------------------------------------
var platChannelNames = [];
var platauthRoute = [];

//------------------------------------------------------------------
//Appstore
var CHANNEL_ID_APPSTORE 					= 20000;
var plat_appstore 						= require('./appstore/plat_appstore');
platChannelNames[CHANNEL_ID_APPSTORE] 	= "appstore";
platauthRoute[CHANNEL_ID_APPSTORE] 		= plat_appstore.platAuth;

//------------------------------------------------------------------
//GooglePlay
var CHANNEL_ID_GOOGLEPLAY 					= 10000;
var plat_googleplay 						= require('./googleplay/plat_googleplay');
platChannelNames[CHANNEL_ID_GOOGLEPLAY] 	= "googleplay";
platauthRoute[CHANNEL_ID_GOOGLEPLAY] 		= plat_googleplay.platAuth;


//------------------------------------------------------------------
function platAuthRouteFunc(sock, channelID, msg, authSuccessCallback, authErrorCallback)
{
	try
	{
		if(platauthRoute[channelID] != undefined)
		{
			platauthRoute[channelID](sock, msg, authSuccessCallback, authErrorCallback);
		}
		else
		{
			ERROR("platAuthRouteFunc channelID=" + channelID + " not defined!");
		}
	}
	catch(e)
	{
		CRASH(e.stack);
		return;
	}
}

exports.platAuthRouteFunc = platAuthRouteFunc;