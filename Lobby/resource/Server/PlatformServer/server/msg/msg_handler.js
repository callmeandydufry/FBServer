var LOG = require('../util').LOG;

function MsgDefine()
{
	// begin xp
	this.SS_HAND_SHAKE				= 1;
	this.XP_REQUEST_AUTH			= 10;
	this.PX_REQUEST_AUTH  			= 11;

	this.XP_CDKEY_EXCHANGE  		= 20;
	this.PX_CDKEY_EXCHANGE			= 21;
	
	this.XP_TD_ORDER_STATE  		= 22;
	
	this.PW_REQUESTPLAYERINFO = 23;
	this.WP_RETURNPLAYERINFO = 24;
	
	this.XP_VERIFY_RECEIPT = 25;
	this.PW_VERIFY_RECEIPT = 26;
	
	//this.WP_REQUESTNEWPLAYER_REDPACK = 27;
	//this.PW_RETURNNEWPLAYER_REDPACK = 28;

	//this.PX_REFRESH_TOKEN			= 63;
	//this.PX_NOTIFY_TRANSFER			= 64;
	// end px
}

var msgIDs = new MsgDefine();

var msgHandler = {}

function getMsgHandler(msgID)
{
	return msgHandler[msgID];
}

var ssHandShake				= require('./ss_handshake').handler;
var pxRequestAuth 			= require('./xp_request_auth').handler;
var xpCDKeyExchange			= require('./xp_cdkey_exchange').handler;
var xpTDOrderState			= require('./xp_td_orderstate').handler;
var wpReturnPlayerInfo = require('./wp_return_player_info').handler;
var xpVerityReceipt = require('./xp_verify_receipt').handler;
//var wpRequestNewPlayerRedPack = require('./wp_requestnewplayer_redpack').handler;

function init()
{
	LOG('msg_handler init');
	msgHandler[msgIDs.SS_HAND_SHAKE]				= new ssHandShake();
	msgHandler[msgIDs.XP_REQUEST_AUTH] 			= new pxRequestAuth();
	msgHandler[msgIDs.XP_CDKEY_EXCHANGE]		= new xpCDKeyExchange();
	msgHandler[msgIDs.XP_TD_ORDER_STATE]		= new xpTDOrderState();
	msgHandler[msgIDs.WP_RETURNPLAYERINFO] = new wpReturnPlayerInfo();
	msgHandler[msgIDs.XP_VERIFY_RECEIPT] = new xpVerityReceipt();
	//msgHandler[msgIDs.WP_REQUESTNEWPLAYER_REDPACK] = new wpRequestNewPlayerRedPack();
	LOG('msg_handler init end');
}

exports.getMsgHandler 	= getMsgHandler;
exports.init	 		= init;
exports.msgIDs			= msgIDs;
