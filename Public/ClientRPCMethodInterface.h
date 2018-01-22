#ifndef __CLIENT_RPC_METHOD_INTERFACE_H__
#define __CLIENT_RPC_METHOD_INTERFACE_H__


#include "SharedTypes.h"
#include "PacketDataStream.h"
#include "ClientServerRPCSupport.h"
#include "ShareStruct.h"

NAMESPACE_LOBBY_BEGIN

/*
---------------------------------------------------------------------------------------------------
在 IClientRPCInterfaceMethods 这个宏里面定义所有服务端到客户端远程调用的接口
---------------------------------------------------------------------------------------------------
*/
#define IClientRPCInterfaceMethods(OPERATION)	\
RPCPROXY_METHOD_PARAM1(OPERATION, showLog, const char*, log)\
RPCPROXY_METHOD_PARAM1(OPERATION, loginSucess, SNID_t, u64PlayerID)\
RPCPROXY_METHOD_PARAM1(OPERATION, SynPlayerExtData, SPlayerExtData& , stPlayerState)\
RPCPROXY_METHOD_PARAM1(OPERATION, SynPlayerChatData, tagSendChatMsgData& , stChat)\

//---------------------------------------------------------------------------------------------------
// 远程调用基本接口声明 [12/25/2017 yz]
class ClientRPCMethodInterface
{
public:
	virtual ~ClientRPCMethodInterface(){}
	enum EMethodID
	{
		IClientRPCInterfaceMethods(CREATEENUM)
		EMethodID_MAX,
	};

	IClientRPCInterfaceMethods(CREATEVIRTUALFUNC)
};

//---------------------------------------------------------------------------------------------------
// 客户端用来向服务端打包消息的接口 [12/25/2017 yz]
class ClientRPCMethodProxy : public ClientRPCMethodInterface
{
public:
	ClientRPCMethodProxy(RPCPacketDataSender* dataSender)
		: mPacketSender(dataSender)
	{}

	IClientRPCInterfaceMethods(CREATEPROXYFUNC)

protected:
	RPCPacketDataSender* mPacketSender;
};

//---------------------------------------------------------------------------------------------------
// 服务端用来解包参数并调用真实处理函数的接口 [12/25/2017 yz]
class ClientRPCMethodHandler : public ClientRPCMethodInterface
{
public:
	typedef ClientRPCMethodInterface THISCLS;
	ClientRPCMethodHandler(RPCPacketDataReceiver* dataReceiver)
		: mPacketReceiver(dataReceiver)
	{
		IClientRPCInterfaceMethods(REGISTERINVOKERFUNC)
	}

	virtual void processPacket()
	{
		DataStream stream;
		if (mPacketReceiver->recvPacketStream(stream) && stream.getDataSize() > 0)
		{
			while (!stream.isCompleted())
			{
				int32 methodID = -1;
				stream.getValue(methodID);
				if (methodID >= 0 && methodID < EMethodID_MAX)
				{
					PacketUnpackFunction* func = getRPCUnpackFunction<THISCLS>(methodID);
					if (func)
					{
						func->processPacket(this, stream);
					}
				}
				else
				{
					break;
				}
			}
		}
	}

protected:
	RPCPacketDataReceiver* mPacketReceiver;
};

NAMESPACE_LOBBY_END

#endif