#ifndef __SERVER_RPC_METHOD_INTERFACE_H__
#define __SERVER_RPC_METHOD_INTERFACE_H__

#include "SharedTypes.h"
#include "PacketDataStream.h"
#include "ClientServerRPCSupport.h"


NAMESPACE_LOBBY_BEGIN

class ClientSession;
/*
---------------------------------------------------------------------------------------------------
�� IServerRPCInterfaceMethods ��������涨�����пͻ��˵������Զ�̵��õĽӿ�
---------------------------------------------------------------------------------------------------
*/
#define IServerRPCInterfaceMethods(OPERATION)	\
RPCPROXY_METHOD_PARAM2(OPERATION, sendChatMsg2OtherPlayer, const char*, chatContent, SNID_t, u64ReceiverID) \
RPCPROXY_METHOD_PARAM1(OPERATION, requestPlayerData, SNID_t, snid) \

//---------------------------------------------------------------------------------------------------
// Զ�̵��û����ӿ����� [12/25/2017 yz]
class ServerRPCMethodInterface
{
public:
	virtual ~ServerRPCMethodInterface() {}

	enum EMethodID
	{
		IServerRPCInterfaceMethods(CREATEENUM)
		EMethodID_MAX,
	};

	IServerRPCInterfaceMethods(CREATEVIRTUALFUNC)
};

//---------------------------------------------------------------------------------------------------
// �ͻ������������˴����Ϣ�Ľӿ� [12/25/2017 yz]
class ServerRPCMethodProxy : public ServerRPCMethodInterface
{
public:
	ServerRPCMethodProxy(RPCPacketDataSender* dataSender)
		: mPacketSender(dataSender)
	{}

	IServerRPCInterfaceMethods(CREATEPROXYFUNC)

protected:
	RPCPacketDataSender* mPacketSender;
};

//---------------------------------------------------------------------------------------------------
// ������������������������ʵ�������Ľӿ� [12/25/2017 yz]
class ServerRPCMethodHandler : public ServerRPCMethodInterface
{
public:
	typedef ServerRPCMethodInterface THISCLS;
	ServerRPCMethodHandler(RPCPacketDataReceiver* dataReceiver)
		: mPacketReceiver(dataReceiver)
	{
		IServerRPCInterfaceMethods(REGISTERINVOKERFUNC)
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
	RPCPacketDataReceiver*	mPacketReceiver;
};

NAMESPACE_LOBBY_END

#endif