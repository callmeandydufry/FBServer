
#ifndef __CG_RPC_INVOKE_MSG_H__
#define __CG_RPC_INVOKE_MSG_H__

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "PacketDataStream.h"
#include "MessageDefine.h"

namespace Messages
{
	class CGRpcInvokeMsg : public IMessage
	{
	public:
		CGRpcInvokeMsg()
		{

		};

		virtual~CGRpcInvokeMsg()
		{
			
		};

		virtual BOOL	Recieve(RecieveStream &iStream);
		virtual BOOL	Send(SendStream &oStream) const;
		virtual uint32	Process(Connector *pPlayer);

		virtual MSG_ID GetMsgID() const
		{
			return MESSAGE_CG_RPC_CALL;
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

	class CGRpcInvokeMsgFactory :
		public MessageFactory
	{
	public:

		IMessage *CreateMessage()
		{
			return new CGRpcInvokeMsg();
		}

		MSG_ID GetMsgID() const
		{
			return MESSAGE_CG_RPC_CALL;
		}

		uint32 GetMessageMaxSize() const
		{
			return CM_IN_SIZE;
		}
	};

	class CGRpcInvokeMsgDispatch
	{
	public:
		static uint32 Process(CGRpcInvokeMsg *pPacket, Connector *pPlayer);
	};
};
using namespace Messages;
#endif
