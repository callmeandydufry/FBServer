#ifndef __GC_RPC_INVOKE_MSG_H__
#define __GC_RPC_INVOKE_MSG_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "PacketDataStream.h"
#include "MessageDefine.h"

namespace Messages
{
	class GCRpcInvokeMsg : public IMessage
	{
	public:
		GCRpcInvokeMsg()
		{

		};

		virtual~GCRpcInvokeMsg()
		{

		};

		virtual BOOL	Recieve(RecieveStream &iStream);
		virtual BOOL	Send(SendStream &oStream) const;
		virtual uint32	Process(Connector *pPlayer);

		virtual MSG_ID GetMsgID() const
		{
			return MESSAGE_GC_RPC_CALL;
		}

		virtual uint32 GetMsgSize() const
		{
			return mData.getSize();
		}

		DataStream& getData()
		{
			return mData;
		}

	private:
		DataStream mData;
	};

	class GCRpcInvokeMsgFactory :
		public MessageFactory
	{
	public:

		IMessage *CreateMessage()
		{
			return new GCRpcInvokeMsg();
		}

		MSG_ID GetMsgID() const
		{
			return MESSAGE_GC_RPC_CALL;
		}

		uint32 GetMessageMaxSize() const
		{
			return CM_IN_SIZE;
		}
	};

	class GCRpcInvokeMsgDispatch
	{
	public:
		static uint32 Process(GCRpcInvokeMsg *pPacket, Connector *pPlayer);
	};
};
using namespace Messages;
#endif
