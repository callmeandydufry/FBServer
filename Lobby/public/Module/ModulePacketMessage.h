
#ifndef _XX_MODULE_PACKET_H_
#define _XX_MODULE_PACKET_H_

#include "TypeDefine.h"
#include "KwNetMessage.h"
#include "MessageFactory.h"
#include "StructPlatform.h"
#include "ModulePacket.h"

namespace Messages
{
	class XXModulePacket : public IMessage
	{
	public:
		XXModulePacket()
			: mPacketData(NULL)
		{
			
		};

		virtual~XXModulePacket()
		{
			KSafeDelete(mPacketData);
		};

		virtual BOOL	Recieve(RecieveStream &iStream);
		virtual BOOL	Send(SendStream &oStream) const;
		virtual uint32	Process(Connector *pPlayer);

		virtual MSG_ID GetMsgID() const
		{
			return MESSAGE_SS_MODULEPACKET;
		}

		virtual uint32 GetMsgSize() const
		{
			return NULL != mPacketData ? mPacketData->getSize() : 0;
		}

	public:
		DataPacket* getDataPacket()
		{
			return mPacketData;
		}

		void exchangeDataPacket(DataPacket* packet)
		{
			KSafeDelete(mPacketData);
			mPacketData = packet;
		}

		DataPacket* exchangeDataPacket()
		{
			DataPacket* out = mPacketData;
			mPacketData = NULL;
			return out;
		}

	protected:
		DataPacket* createDataPacket() const;

	private:
		mutable DataPacket* mPacketData;
	};

	class XXModulePacketFactory :
		public MessageFactory
	{
	public:

		IMessage *CreateMessage()
		{
			return new XXModulePacket();
		}

		MSG_ID GetMsgID() const
		{
			return MESSAGE_SS_MODULEPACKET;
		}

		uint32 GetMessageMaxSize() const
		{
			return CM_IN_SIZE;
		}
	};

	class	XXModulePacketDispatch
	{
	public:
		static uint32	Process(XXModulePacket *pPacket, Connector *pPlayer);
	};
};
using namespace Messages;
#endif
