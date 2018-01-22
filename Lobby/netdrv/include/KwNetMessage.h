/*$T Common/KwNetMessage.h GC 1.140 10/10/07 10:06:30 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __KwNetMessage_H__
#define __KwNetMessage_H__

#include "KwNetBasics.h"
#include "KwNetStreamSystem.h"

class	Socket;
class	Connector;
uint32	GetMsgIndex(uint32 a);
void	SetMsgIndex(uint32 &a, uchar index);
uint32	GetMsgLen(uint32 a);
void	SetMsgLen(uint32 &a, uint32 Len);
#define GET_MSG_INDEX(a)		GetMsgIndex(a)
#define SET_MSG_INDEX(a, index)		SetMsgIndex(a, index)
#define GET_MSG_LEN(a)			GetMsgLen(a)
#define SET_MSG_LEN(a, len)		SetMsgLen(a, len)
#define BILL_GET_MSG_INDEX(a)		((a) >> 24)
#define BILL_SET_MSG_INDEX(a, index)	((a) = (((a) & 0xffffff) + ((index) << 24)))
#define BILL_GET_MSG_LEN(a)		((a) & 0xffffff)
#define BILL_SET_MSG_LEN(a, len)	((a) = ((a) & 0xff000000) + (len))
#define MSG_HEADER_SIZE			(sizeof(MSG_ID) + sizeof(uint32))

enum MESSAGE_PROCESS { MP_ERROR = 0, MP_BREAK, MP_CONTINUE, MP_NOTREMOVE, MP_NOTREMOVE_ERROR, };

#if defined(_70_CLIENT) && defined(_70_SERVER)
#define DECLARE_NET_MESSAGE(MESSAGE_NAME, MESSAGE_ID) \
	namespace Messages \
{ \
	class MESSAGE_NAME : public IMessage \
	{ \
	public: \
		virtual BOOL	Recieve(RecieveStream &iStream); \
		virtual BOOL	Send(SendStream &oStream) const; \
		virtual uint32	Process(Connector *pConnector); \
 \
		/*
		 =======================================================================================================
		 =======================================================================================================
		 */ \
		virtual MSG_ID GetMsgID() const \
		{ \
			return MESSAGE_ID;\
		} \
 \
	public:
#else
#define DECLARE_NET_MESSAGE(MESSAGE_NAME, MESSAGE_ID) \
	namespace Messages \
{ \
	class MESSAGE_NAME : public IMessage \
	{ \
	public: \
		virtual BOOL	Recieve(RecieveStream &iStream); \
		virtual BOOL	Send(SendStream &oStream) const; \
		virtual uint32	Process(Connector *pConnector); \
 \
		/*
		 =======================================================================================================
		 =======================================================================================================
		 */ \
		virtual MSG_ID GetMsgID() const \
		{ \
			return MESSAGE_ID; \
		} \
 \
	public:
#endif

#if defined(_70_CLIENT) && defined(_70_SERVER)
#define DECLARE_NET_MESSAGE_BEGIN(MESSAGE_NAME, MESSAGE_ID) \
	namespace Messages \
		{ \
		class MESSAGE_NAME : public IMessage \
		{ \
		public: \
		MESSAGE_NAME();	\
		virtual ~MESSAGE_NAME();\
		virtual BOOL	Recieve(RecieveStream &iStream); \
		virtual BOOL	Send(SendStream &oStream) const; \
		virtual uint32	Process(Connector *pConnector); \
		virtual uint32 GetMsgSize() const;\
		\
		/*
	=======================================================================================================
	=======================================================================================================
	*/ \
		virtual MSG_ID GetMsgID() const \
		{ \
			return MESSAGE_ID;\
		} \
		\
		public:
#else
#define DECLARE_NET_MESSAGE_BEGIN(MESSAGE_NAME, MESSAGE_ID) \
	namespace Messages \
		{ \
		class MESSAGE_NAME : public IMessage \
		{ \
		public: \
		MESSAGE_NAME();	\
		virtual ~MESSAGE_NAME();\
		virtual BOOL	Recieve(RecieveStream &iStream); \
		virtual BOOL	Send(SendStream &oStream) const; \
		virtual uint32	Process(Connector *pConnector); \
		virtual uint32 GetMsgSize() const;\
		\
		/*
	=======================================================================================================
	=======================================================================================================
	*/ \
		virtual MSG_ID GetMsgID() const \
		{ \
			return MESSAGE_ID; \
		} \
		\
		public:
#endif

#define DECLARE_NET_MESSAGE_FACTORY(MESSAGE_NAME, MESSAGE_ID) \
}; \
class MESSAGE_NAME##Factory : public MessageFactory \
{ \
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */ \
public: \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	IMessage *CreateMessage() \
	{ \
		return new MESSAGE_NAME(); \
	} \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	MSG_ID GetMsgID() const \
	{ \
		return MESSAGE_ID; \
	}

#define DECLARE_NET_MESSAGE_END(MESSAGE_NAME, MESSAGE_ID) \
}; \
		class MESSAGE_NAME##Factory : public MessageFactory \
		{ \
		/*
	-----------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------
	*/ \
		public: \
		\
		/*
		===============================================================================================================
		===============================================================================================================
		*/ \
		IMessage *CreateMessage() \
		{ \
			return new MESSAGE_NAME(); \
		} \
		\
		/*
		===============================================================================================================
		===============================================================================================================
		*/ \
		MSG_ID GetMsgID() const \
		{ \
			return MESSAGE_ID; \
		}\
		uint32 GetMessageMaxSize() const;\
		\
		}; \
		class	MESSAGE_NAME##Dispatch \
		{ \
		/*
		-----------------------------------------------------------------------------------------------------------------------
		-----------------------------------------------------------------------------------------------------------------------
		*/ \
		public: \
				static uint32	Process(MESSAGE_NAME *pMessage, Connector *pConnector); \
		}; \
		}; \
		using namespace Messages;

#define DECLARE_NET_MESSAGE_HANDLE(MESSAGE_NAME, MESSAGE_ID) \
}; \
class	MESSAGE_NAME##Dispatch \
{ \
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */ \
public: \
	static uint32	Process(MESSAGE_NAME *pMessage, Connector *pConnector); \
}; \
}; \
using namespace Messages;

#define DECLARE_NET_MESSAGE_ATOM_VAR(type, x) \
private: \
	type	m_##x; \
public: \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	const type Get##x() const \
	{ \
		return m_##x; \
	} \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	void Set##x(const type val) \
	{ \
		m_##x = val; \
	}

#define DECLARE_NET_MESSAGE_ATOM_VAR_ARR(type, num, x) \
private: \
	type	m_##x[num]; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	const type Get##x(const uint32 index) const \
	{ \
		KCheck(index < num); \
		return m_##x[index]; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	const type* Get##x() const \
	{ \
		return m_##x; \
	} \
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const uint32 index,const type val) \
	{ \
		KCheck(index < num); \
		m_##x[index] = val; \
	}\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const uint32 maxNum,const type *pVal) \
	{ \
		KCheck(maxNum <= num); \
		memcpy(m_##x,pVal,sizeof(type)*maxNum); \
	}

#define DECLARE_NET_MESSAGE_ATOM_VAR_FLAG(type, x, flag, eBit) \
private: \
	type	m_##x; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	const type Get##x() const \
	{ \
		return m_##x; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const type val) \
	{ \
		m_##x = val; \
		m_##flag.UpdateBits((int32) eBit, TRUE);\
	}

#define DECLARE_NET_MESSAGE_ATOM_VAR_ARR_FLAG(type, num, x, flag, eBit) \
private: \
	type	m_##x[num]; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	const type Get##x(const uint32 index) const \
	{ \
		KCheck(index < num); \
		return m_##x[index]; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const uint32 index,const type val) \
	{ \
		KCheck(index < num); \
		m_##x[index] = val; \
		m_##flag.UpdateBits((int32) eBit, TRUE);\
	}

#define DECLARE_NET_MESSAGE_STRUCT_VAR(type, x) \
private: \
	type	m_##x; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	type* Get##x() \
	{ \
		return &m_##x; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const type *pVal) \
	{ \
		KCheck(pVal); \
		memcpy(&m_##x, pVal, sizeof(type));\
	}

#define DECLARE_NET_MESSAGE_STRUCT_VAR_ARR(type,num, x) \
private: \
	type	m_##x[num]; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	type* Get##x(const uint32 index) \
	{ \
		if (index >= num)	\
		{	\
			KCheck(index < num); \
			return NULL;	\
		}		\
		return &m_##x[index]; \
	} \
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	type* Get##x() \
	{ \
		return m_##x; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const uint32 index,const type *pVal) \
	{ \
		KCheck(index < num); \
		KCheck(pVal); \
		memcpy(&m_##x[index], pVal, sizeof(type));\
	}

#define DECLARE_NET_MESSAGE_STRUCT_VAR_FLAG(type, x,flag, eBit) \
private: \
	type	m_##x; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	type* Get##x() \
	{ \
		return &m_##x; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const type *pVal) \
	{ \
		KCheck(pVal); \
		memcpy(&m_##x, pVal, sizeof(type));\
		m_##flag.UpdateBits((int32) eBit, TRUE);\
	}

#define DECLARE_NET_MESSAGE_STRUCT_VAR_ARR_FLAG(type,num, x,flag, eBit) \
private: \
	type	m_##x[num]; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	type* Get##x(const uint32 index) \
	{ \
		KCheck(index < num); \
		return &m_##x[index]; \
	} \
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	type* Get##x() \
	{ \
		return m_##x; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const uint32 index,const type *pVal) \
	{ \
		KCheck(index < num); \
		KCheck(pVal); \
		memcpy(&m_##x[index], pVal, sizeof(type));\
		m_##flag.UpdateBits((int32) eBit, TRUE);\
	}

#define DECLARE_NET_MESSAGE_ANY_VAR_ARR_WITH_FLAG(type, num, x, FlagType, flag) \
private: \
	FlagType m_##flag;	\
	type	m_##x[num]; \
public: \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	const type* Get##x() const \
	{ \
		return m_##x; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	const type* Get##x(const uint32 index) const \
	{ \
		KCheck(index < num); \
		return &m_##x[index]; \
	} \
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	void Set##x(const uint32 index,const type val) \
	{ \
		KCheck(index < num); \
		m_##x[index] = val; \
		m_##flag.UpdateBit((int32) index, TRUE);\
	}\
	\
	/*
	===============================================================================================================
	===============================================================================================================
	*/ \
	const FlagType* Get##flag(void) const \
	{ \
		return &m_##flag; \
	} 



#define SEND_NET_MESSAGE_ATOM_VAR(x)	oStream.Send((char *) (&m_##x), sizeof(m_##x));
#define RECV_NET_MESSAGE_ATOM_VAR(x)	iStream.Reci((char *) (&m_##x), sizeof(m_##x));
#define SEND_NET_MESSAGE_ATOM_VAR_ARR(num,x)	for (int32 _Idx=0;_Idx<num;_Idx++) oStream.Send((char *) (&m_##x[_Idx]), sizeof(m_##x[_Idx]));
#define RECV_NET_MESSAGE_ATOM_VAR_ARR(num,x)	for (int32 _Idx=0;_Idx<num;_Idx++) iStream.Reci((char *) (&m_##x[_Idx]), sizeof(m_##x[_Idx]));
#define SEND_NET_MESSAGE_ATOM_VAR_FLAG(x,flag, eBit)	if(m_##flag.isSetBit(eBit)) oStream.Send((char *) (&m_##x), sizeof(m_##x));
#define RECV_NET_MESSAGE_ATOM_VAR_FLAG(x,flag, eBit)	if(m_##flag.isSetBit(eBit)) iStream.Reci((char *) (&m_##x), sizeof(m_##x));
#define SEND_NET_MESSAGE_ATOM_VAR_ARR_FLAG(num,x,flag, eBit)	if(m_##flag.isSetBit(eBit)) for (int32 _Idx=0;_Idx<num;_Idx++) oStream.Send((char *) (&m_##x[_Idx]), sizeof(m_##x[_Idx]));
#define RECV_NET_MESSAGE_ATOM_VAR_ARR_FLAG(num,x,flag, eBit)	if(m_##flag.isSetBit(eBit)) for (int32 _Idx=0;_Idx<num;_Idx++) iStream.Reci((char *) (&m_##x[_Idx]), sizeof(m_##x[_Idx]));
#define SEND_NET_MESSAGE_STRUCT_VAR(x)	m_##x.send(oStream);
#define RECV_NET_MESSAGE_STRUCT_VAR(x)	m_##x.reci(iStream);
#define SEND_NET_MESSAGE_STRUCT_VAR_FLAG(x,flag, eBit)	if(m_##flag.isSetBit(eBit)) m_##x.Send(oStream);
#define RECV_NET_MESSAGE_STRUCT_VAR_FLAG(x,flag, eBit)	if(m_##flag.isSetBit(eBit)) m_##x.Reci(iStream);
#define SEND_NET_MESSAGE_STRUCT_VAR_ARR(num,x)	for (int32 _Idx=0;_Idx<num;_Idx++) m_##x[_Idx].Send(oStream);
#define RECV_NET_MESSAGE_STRUCT_VAR_ARR(num,x)	for (int32 _Idx=0;_Idx<num;_Idx++) m_##x[_Idx].Reci(iStream);
#define SEND_NET_MESSAGE_STRUCT_VAR_ARR_FLAG(num,x,flag, eBit)	if(m_##flag.isSetBit(eBit)) for (int32 _Idx=0;_Idx<num;_Idx++) m_##x[_Idx].Send(oStream);
#define RECV_NET_MESSAGE_STRUCT_VAR_ARR_FLAG(num,x,flag, eBit)	if(m_##flag.isSetBit(eBit)) for (int32 _Idx=0;_Idx<num;_Idx++) m_##x[_Idx].Reci(iStream);

/*
===============================================================================================================
===============================================================================================================
*/
#define init_netmessage_string(x)		m_##x##Size = 0; memset(m_sz##x,0,sizeof(m_sz##x));
#define calc_netmessage_string_size(x) ( sizeof(m_##x##Size) + m_##x##Size*sizeof(char) )

#define SIZEOF_VAR(x)			sizeof(x)
#define DECLARE_NET_MESSAGE_STRING_VAR(type, maxlen, x,isMemcpy) \
private : \
	type	m_##x##Size; \
	char	m_sz##x[maxlen]; \
public: \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	const type Get##x##Size() const \
	{ \
		return m_##x##Size; \
	} \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	const char *Get##x() const \
	{ \
		return m_sz##x; \
	} \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	void Set##x(const char *p) \
	{ \
		m_##x##Size = 0; \
		if(p) \
		{ \
			if(TRUE==isMemcpy ) \
			{\
				memcpy(m_sz##x, p, maxlen - 1); \
				m_sz##x[maxlen - 1] = 0; \
				m_##x##Size = maxlen-1; \
			}\
			else\
			{\
				tsnprintf_s(m_sz##x, maxlen, "%s", p);\
				m_##x##Size = (type) strlen(m_sz##x); \
			}\
		} \
	}

#define DECLARE_NET_MESSAGE_STRING_VAR_FLAG(type, maxlen, x, flag, eBit,isMemcpy) \
private : \
	type	m_##x##Size; \
	char	m_sz##x[maxlen]; \
public: \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	const type Get##x##Size() const \
	{ \
		return m_##x##Size; \
	} \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	const char *Get##x() const \
	{ \
		return m_sz##x; \
	} \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	void Set##x(const char *p) \
	{ \
		m_##x##Size = 0; \
		if(p) \
		{ \
			if(TRUE==isMemcpy ) \
			{\
				memcpy(m_sz##x, p, maxlen - 1); \
				m_sz##x[maxlen - 1] = 0; \
				m_##x##Size = maxlen-1; \
			}\
			else\
			{\
				tsnprintf_s(m_sz##x, maxlen, "%s", p);\
				m_##x##Size = (type) strlen(m_sz##x); \
			}\
			m_##flag.UpdateBits((int32) eBit, TRUE);\
		} \
	}

#define SEND_NET_MESSAGE_STRING_VAR(x) \
	oStream.Send((char *) (&m_##x##Size), sizeof(m_##x##Size)); \
	if(m_##x##Size > 0 && m_##x##Size < sizeof(m_sz##x)) \
		oStream.Send((char *) (&m_sz##x), sizeof(char) * m_##x##Size);

#define RECV_NET_MESSAGE_STRING_VAR(x) \
	iStream.Reci((char *) (&m_##x##Size), sizeof(m_##x##Size)); \
	if(m_##x##Size > 0 && m_##x##Size < sizeof(m_sz##x)) \
	iStream.Reci((char *) (&m_sz##x), sizeof(char) * m_##x##Size);

#define SEND_NET_MESSAGE_STRING_VAR_FLAG(x,flag, eBit) \
	if(m_##flag.isSetBit(eBit)) \
	{\
		oStream.Send((char *) (&m_##x##Size), sizeof(m_##x##Size));	\
		if(m_##x##Size > 0 && m_##x##Size < sizeof(m_sz##x))	\
			oStream.Send((char *) (&m_sz##x), sizeof(char) * m_##x##Size);	\
	}

#define RECV_NET_MESSAGE_STRING_VAR_FLAG(x,flag, eBit) \
	if(m_##flag.isSetBit(eBit)) \
	{\
		iStream.Reci((char *) (&m_##x##Size), sizeof(m_##x##Size)); \
		if(m_##x##Size > 0 && m_##x##Size < sizeof(m_sz##x)) \
			iStream.Reci((char *) (&m_sz##x), sizeof(char) * m_##x##Size); \
	}

#define SIZEOF_STR(type, x)		sizeof(type) + sizeof(char) * m_##x##Size

#define MAXLEN_STR(type, maxlen)	sizeof(type) + maxlen

#define IMPLEMENT_NET_MESSAGE_PROCESS(MESSAGE_NAME, MESSAGE_ID) \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	uint32 MESSAGE_NAME::Process(Connector *pConnector) \
	{ \
		__GUARD__ return MESSAGE_NAME##Dispatch::Process(this, pConnector); \
		__UNGUARD__ return FALSE; \
	}

#define IMPLEMENT_NET_MESSAGE_RECV_BEGIN(MESSAGE_NAME, MESSAGE_ID) \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	uint32 MESSAGE_NAME::Process(Connector *pConnector) \
	{ \
		__GUARD__ return MESSAGE_NAME##Dispatch::Process(this, pConnector); \
		__UNGUARD__ return FALSE; \
	} \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	BOOL MESSAGE_NAME::Recieve(RecieveStream &iStream) \
	{ \
		__GUARD__

#define IMPLEMENT_NET_MESSAGE_RECV_END(MESSAGE_NAME, MESSAGE_ID) \
	return TRUE; \
	__UNGUARD__; \
	return FALSE; \
}

#define IMPLEMENT_NET_MESSAGE_SEND_BEGIN(MESSAGE_NAME, MESSAGE_ID) \
 \
	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */ \
	BOOL MESSAGE_NAME::Send(SendStream &oStream) const \
	{ \
		__GUARD__

#define IMPLEMENT_NET_MESSAGE_SEND_END(MESSAGE_NAME, MESSAGE_ID) \
	return TRUE; \
	__UNGUARD__; \
	return FALSE; \
}

#define GETVAR(p, x)		p->Get##x()
#define SETVAR(p, x, val)	p->Set##x(val)
#define GETVAR(p, x)		p->Get##x()
#define SETVAR(p, x, val)	p->Set##x(val)
#define ADD_MSG_FACTORY(x)	AddFactory(new x##Factory());

#define BEGIN_MESSAGE_PROCESS_DEF() \
	GameConnector	    *pGameConnector = (GameConnector *) pConnector; \
	KCheck(pGameConnector); \
	Player	*pPlayer = pGameConnector->GetPlayer(); \
	KCheck(pPlayer); \
	Map	*pMap = pPlayer->GetMap(); \
	if(NULL == pMap) return MP_CONTINUE; \
	KCheck(KGetCurrentTID() == pMap->m_ThreadID); \
	if( pPlayer->GetConnector()->GetStatus() != PS_KHAN_SERVER_NORMAL ) \
		return MP_CONTINUE; \
	if( !pPlayer->IsActiveObj() ) \
		return MP_CONTINUE;

#define BEGIN_GX_MESSAGE_PROCESS_DEF() \
	ut_id_connector	PlayerID = pKMessage->GetPlayerID();\
	GameConnector	*pGameConnector = (GameConnector*)g_pConnectorPool->GetConnector(PlayerID); \
	KCheck(pGameConnector); \
	Player	*pPlayer = pGameConnector->GetPlayer(); \
	KCheck(pPlayer); \
	Map	*pMap = pPlayer->GetMap(); \
	if(NULL == pMap) return MP_CONTINUE;

#define BEGIN_GX_MESSAGE_PROCESS_DEF_RETVOID() \
	ut_id_connector	PlayerID = pKMessage->GetPlayerID();\
	GameConnector	*pGameConnector = (GameConnector*)g_pConnectorPool->GetConnector(PlayerID); \
	KCheck(pGameConnector); \
	Player	*pPlayer = pGameConnector->GetPlayer(); \
	KCheck(pPlayer); \
	Map	*pMap = pPlayer->GetMap(); \
	if(NULL == pMap) return ;

class	IMessage
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	uchar	m_Status;
	uchar	m_ComID;
	MSG_ID	m_MsgIdOffset;
	char	m_FillData[4];
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	IMessage();
	virtual ~IMessage();

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */

	virtual void Clear()
	{
	};
	virtual BOOL	Recieve(RecieveStream &iStream) = 0;
	virtual BOOL	Send(SendStream &oStream) const = 0;
	virtual uint32	Process(Connector *pConnector) = 0;

	virtual MSG_ID	GetMsgID() const = 0;
	virtual uint32	GetMsgSize() const = 0;

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	virtual BOOL CheckMessage()
	{
		return TRUE;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	uchar GetMsgStatus() const
	{
		return m_Status;
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void SetMsgStatus(uchar Status)
	{
		m_Status = Status;
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	uchar GetComID() const
	{
		return m_ComID;
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void SetComIndex(uchar Index)
	{
		m_ComID = Index;
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	virtual BOOL MessageInfo()
	{
		return TRUE;
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void ModifyMsgIdOffset(MSG_ID offset)
	{
		m_MsgIdOffset = offset;
	};

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	MSG_ID GetMsgIdOffset() const
	{
		return m_MsgIdOffset;
	};
};
#endif
