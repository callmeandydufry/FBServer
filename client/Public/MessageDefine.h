/*$T Share/MessageDefine.h GC 1.140 10/10/07 10:06:30 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __MESSAGE_DEFINE_H__
#define __MESSAGE_DEFINE_H__

namespace Messages
{
enum MSG_DEFINE
{
	MESSAGE_NONE					= 0,

	MESSAGE_SS_CONNECT					= 1,///<
	MESSAGE_SS_MODULEPACKET				= 2,

	MESSAGE_CG_BEGIN					= 10000,
	MESSAGE_CG_DATAPACKET				= 10001,
	MESSAGE_CG_HANDSHAKE				= 10002,
	MESSAGE_CG_REQUEST_LOGIN			= 10003,
	MESSAGE_CG_RPC_CALL					= 10004,
	MESSAGE_CG_PING						= 10005,
	MESSAGE_CG_END,

	MESSAGE_GC_BEGIN					= 20000,
	MESSAGE_GC_DATAPACKET				= 20001,
	MESSAGE_GC_HANDSHAKE				= 20002,
	MESSAGE_GC_ENCRYPT_KEY				= 20003,
	MESSAGE_GC_RPC_CALL					= 20004,
	MESSAGE_GC_PING						= 20005,
	MESSAGE_GC_END,

	MESSAGE_MAX
};
};
#endif