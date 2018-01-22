#ifndef __CLIENT_SERVER_RPC_SUPPORT_H__
#define __CLIENT_SERVER_RPC_SUPPORT_H__

#include "SharedTypes.h"
#include "PacketDataStream.h"
#include "DynaString.h"

NAMESPACE_LOBBY_BEGIN


/////////////////////////////////////////////////////
//消除引用类型的参数的引用标记
template<typename T>
class RemoveReferenceType
{
public:
	typedef T Type;
};

template<typename T>
class RemoveReferenceType<T&>
{
public:
	typedef T Type;
};

template<typename T>
class RemoveReferenceType<const T&>
{
public:
	typedef T Type;
};

/////////////////////////////////////////////////////
//将char*类型转换为string类型
template<typename T>
class Convert2ValidInvokerType
{
public:
	typedef T Type;
	static T& toValue(Type& val)
	{
		return val;
	}
};

template<>
class Convert2ValidInvokerType<char*>
{
public:
	typedef DynaString Type;
	static char* toValue(Type& val)
	{
		return const_cast<char*>(val.c_str());
	}
};

template<>
class Convert2ValidInvokerType<const char*>
{
public:
	typedef DynaString Type;
	static const char* toValue(Type& val)
	{
		return val.c_str();
	}
};

#if defined(__OS64__) || defined(__arch64__) || defined(_LP64)
struct ObjMemberFunctionPtr__
{
	uint64 low;
	uint64 high;
};
// 成员函数指针的数据表示形式
typedef ObjMemberFunctionPtr__	 ObjMemberFunctionPtr;
#else
// 成员函数指针的数据表示形式
typedef uint64					ObjMemberFunctionPtr;
#endif

class PacketUnpackFunction
{
public:
	virtual ~PacketUnpackFunction() {}
	virtual BOOL processPacket(void* obj, DataStream& stream) { return FALSE; }
	ObjMemberFunctionPtr mActualProcessFunc;
};

// 这个函数的作用就是为每个类型分配一个全局的数组，保存消息数据处理函数
template<typename CLS>
PacketUnpackFunction* getRPCUnpackFunction(int funcIndex, PacketUnpackFunction* newFunc = NULL)
{
	static PacketUnpackFunction* sFunctions[CLS::EMethodID_MAX];
	if (NULL != newFunc)
	{
		if (sFunctions[funcIndex])
		{
			delete sFunctions[funcIndex];
			sFunctions[funcIndex] = NULL;
		}
		sFunctions[funcIndex] = newFunc;
	}
	return sFunctions[funcIndex];
}

// 注册外部消息处理接口 [11/23/2016 yz]
#define GET_STREAM_PARAM(i)	\
	typedef Convert2ValidInvokerType<typename RemoveReferenceType<T##i>::Type> TraitT##i;\
	typename TraitT##i::Type p##i;\
	if(!stream.getValue(p##i))	\
	{	\
		return FALSE;	\
	}

#define USE_STREAM_PARAM(i) \
	TraitT##i::toValue(p##i)

// 0个参数 [11/23/2016 yz]
template<typename Cls>
class ConcreteUnpackFunctionParam0 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)();

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		if (!stream.isCompleted())
		{
			return FALSE;
		}
		FunctionType functor = *(FunctionType*)(&this->mActualInvokerFunc);
		(((Cls*)obj)->*functor)();
		return TRUE;
	}
};

template<typename Cls>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)())
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam0<Cls>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}

// 1个参数 [11/23/2016 yz]
template<typename Cls, typename T1>
class ConcreteUnpackFunctionParam1 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam1<Cls, T1>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}

// 2个参数 [11/23/2016 yz]
template<typename Cls, typename T1, typename T2>
class ConcreteUnpackFunctionParam2 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1, T2 p2);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		GET_STREAM_PARAM(2);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1),
			USE_STREAM_PARAM(2)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1, typename T2>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1, T2 p2))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam2<Cls, T1, T2>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}


// 3个参数 [11/23/2016 yz]
template<typename Cls, typename T1, typename T2, typename T3>
class ConcreteUnpackFunctionParam3 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		GET_STREAM_PARAM(2);
		GET_STREAM_PARAM(3);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1),
			USE_STREAM_PARAM(2),
			USE_STREAM_PARAM(3)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1, typename T2, typename T3>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1, T2 p2, T3 p3))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam3<Cls, T1, T2, T3>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}


// 4个参数 [11/23/2016 yz]
template<typename Cls, typename T1, typename T2, typename T3, typename T4>
class ConcreteUnpackFunctionParam4 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		GET_STREAM_PARAM(2);
		GET_STREAM_PARAM(3);
		GET_STREAM_PARAM(4);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1),
			USE_STREAM_PARAM(2),
			USE_STREAM_PARAM(3),
			USE_STREAM_PARAM(4)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1, typename T2, typename T3, typename T4>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam4<Cls, T1, T2, T3, T4>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}


// 5个参数 [11/23/2016 yz]
template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5>
class ConcreteUnpackFunctionParam5 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		GET_STREAM_PARAM(2);
		GET_STREAM_PARAM(3);
		GET_STREAM_PARAM(4);
		GET_STREAM_PARAM(5);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1),
			USE_STREAM_PARAM(2),
			USE_STREAM_PARAM(3),
			USE_STREAM_PARAM(4),
			USE_STREAM_PARAM(5)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam5<Cls, T1, T2, T3, T4, T5>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}


// 6个参数 [11/23/2016 yz]
template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class ConcreteUnpackFunctionParam6 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		GET_STREAM_PARAM(2);
		GET_STREAM_PARAM(3);
		GET_STREAM_PARAM(4);
		GET_STREAM_PARAM(5);
		GET_STREAM_PARAM(6);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1),
			USE_STREAM_PARAM(2),
			USE_STREAM_PARAM(3),
			USE_STREAM_PARAM(4),
			USE_STREAM_PARAM(5),
			USE_STREAM_PARAM(6)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam6<Cls, T1, T2, T3, T4, T5, T6>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}


// 7个参数 [11/23/2016 yz]
template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class ConcreteUnpackFunctionParam7 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		GET_STREAM_PARAM(2);
		GET_STREAM_PARAM(3);
		GET_STREAM_PARAM(4);
		GET_STREAM_PARAM(5);
		GET_STREAM_PARAM(6);
		GET_STREAM_PARAM(7);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1),
			USE_STREAM_PARAM(2),
			USE_STREAM_PARAM(3),
			USE_STREAM_PARAM(4),
			USE_STREAM_PARAM(5),
			USE_STREAM_PARAM(6),
			USE_STREAM_PARAM(7)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam7<Cls, T1, T2, T3, T4, T5, T6, T7>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}


// 8个参数 [11/23/2016 yz]
template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class ConcreteUnpackFunctionParam8 : public PacketUnpackFunction
{
public:
	typedef void(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8);

	virtual BOOL processPacket(void* obj, DataStream& stream)
	{
		FunctionType functor = *(FunctionType*)(&mActualProcessFunc);

		GET_STREAM_PARAM(1);
		GET_STREAM_PARAM(2);
		GET_STREAM_PARAM(3);
		GET_STREAM_PARAM(4);
		GET_STREAM_PARAM(5);
		GET_STREAM_PARAM(6);
		GET_STREAM_PARAM(7);
		GET_STREAM_PARAM(8);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(((Cls*)obj)->*functor)(
			USE_STREAM_PARAM(1),
			USE_STREAM_PARAM(2),
			USE_STREAM_PARAM(3),
			USE_STREAM_PARAM(4),
			USE_STREAM_PARAM(5),
			USE_STREAM_PARAM(6),
			USE_STREAM_PARAM(7),
			USE_STREAM_PARAM(8)
			);
		return TRUE;
	}
};

template<typename Cls, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
PacketUnpackFunction* generateRPCProcessMethod(int funcIndex, void(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8))
{
	PacketUnpackFunction* newFunc = new ConcreteUnpackFunctionParam8<Cls, T1, T2, T3, T4, T5, T6, T7, T8>();
	newFunc->mActualProcessFunc = *(ObjMemberFunctionPtr*)&functor;
	return getRPCUnpackFunction<Cls>(funcIndex, newFunc);
}

class RPCPacketDataSender
{
public:
	virtual void sendPacketStream(DataStream& stream) = 0;
};

class RPCPacketDataReceiver
{
public:
	virtual BOOL recvPacketStream(DataStream& stream) = 0;
};

class RPCMethodParamSerializer
{
public:
	static void serializeParams(DataStream& stream)
	{

	}

	template<typename T1>
	static void serializeParams(DataStream& stream, T1 p1)
	{
		stream.pushValue(p1);
	}

	template<typename T1, typename T2>
	static void serializeParams(DataStream& stream, T1 p1, T2 p2)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
	}

	template<typename T1, typename T2, typename T3>
	static void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
	}

	template<typename T1, typename T2, typename T3, typename T4>
	static void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	static void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
		stream.pushValue(p5);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	static void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
		stream.pushValue(p5);
		stream.pushValue(p6);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	static void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
		stream.pushValue(p5);
		stream.pushValue(p6);
		stream.pushValue(p7);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	static void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
		stream.pushValue(p5);
		stream.pushValue(p6);
		stream.pushValue(p7);
		stream.pushValue(p8);
	}
};

//CREATEENUM---------------------------------
#define CREATEENUM_RPCPROXY_METHOD_NO_PARAM(funcName) \
	EMethodID_##funcName,
#define CREATEENUM_RPCPROXY_METHOD_PARAM_WITHTYPE(funcName, ...)
#define CREATEENUM_RPCPROXY_METHOD_PARAM_NOTYPE(funcName, ...)

//CREATEVIRTUALFUNC---------------------------------
#define CREATEVIRTUALFUNC_RPCPROXY_METHOD_NO_PARAM(funcName)
#define CREATEVIRTUALFUNC_RPCPROXY_METHOD_PARAM_WITHTYPE(funcName, ...) \
	virtual void funcName(__VA_ARGS__) = 0;
#define CREATEVIRTUALFUNC_RPCPROXY_METHOD_PARAM_NOTYPE(funcName, ...)

//Create RPC Proxy Function---------------------------------
#define CREATEPROXYFUNC_RPCPROXY_METHOD_NO_PARAM(funcName)
#define CREATEPROXYFUNC_RPCPROXY_METHOD_PARAM_WITHTYPE(funcName, ...) \
	virtual void funcName(__VA_ARGS__)\
	{\
		DataStream stream;\
		stream.pushValue((int32)EMethodID_##funcName);\

#define CREATEPROXYFUNC_RPCPROXY_METHOD_PARAM_NOTYPE(funcName, ...)	\
		RPCMethodParamSerializer::serializeParams(stream, ##__VA_ARGS__);	\
		mPacketSender->sendPacketStream(stream);\
	}

//Register invoker RPC function---------------------------------
#define REGISTERINVOKERFUNC_RPCPROXY_METHOD_NO_PARAM(funcName)	\
	generateRPCProcessMethod(EMethodID_##funcName, &THISCLS::funcName);
#define REGISTERINVOKERFUNC_RPCPROXY_METHOD_PARAM_WITHTYPE(funcName, ...)
#define REGISTERINVOKERFUNC_RPCPROXY_METHOD_PARAM_NOTYPE(funcName, ...)

	
#define RPCPROXY_METHOD_PARAM0(OPERATION, name) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(name)	\

#define RPCPROXY_METHOD_PARAM1(OPERATION, name, T1, P1) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(name, P1)	\

#define RPCPROXY_METHOD_PARAM2(OPERATION, name, T1, P1, T2, P2) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1, T2 P2)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(name, P1, P2)	\

#define RPCPROXY_METHOD_PARAM3(OPERATION, name, T1, P1, T2, P2, T3, P3) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1, T2 P2, T3 P3)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPEname, P1, P2, P3)	\

#define RPCPROXY_METHOD_PARAM4(OPERATION, name, T1, P1, T2, P2, T3, P3, T4, P4) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1, T2 P2, T3 P3, T4 P4)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(name, P1, P2, P3, P4)	\

#define RPCPROXY_METHOD_PARAM5(OPERATION, name, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(name, P1, P2, P3, P4, P5)	\

#define RPCPROXY_METHOD_PARAM6(OPERATION, name, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(ame, P1, P2, P3, P4, P5, P6)	\

#define RPCPROXY_METHOD_PARAM7(OPERATION, name, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(name, P1, P2, P3, P4, P5, P6, P7)	\

#define RPCPROXY_METHOD_PARAM8(OPERATION, name, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8) \
	OPERATION##_RPCPROXY_METHOD_NO_PARAM(name)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_WITHTYPE(name, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8)	\
	OPERATION##_RPCPROXY_METHOD_PARAM_NOTYPE(name, P1, P2, P3, P4, P5, P6, P7, P8)	\

NAMESPACE_LOBBY_END

#endif