
#ifndef __ModuleExportInterface_H__
#define __ModuleExportInterface_H__

#include "TypeDefine.h"
#include "ModulePacket.h"
#include "ModuleSupport.h"


#if defined(__OS64__) || defined(__arch64__) || defined(_LP64)
struct MemberFunctionPtr__
{
	uint64 low;
	uint64 high;
};
// 成员函数指针的数据表示形式
typedef MemberFunctionPtr__	 MemberFunctionPtr;
#else
// 成员函数指针的数据表示形式
typedef uint64				MemberFunctionPtr;
#endif

struct FunctionHasRet {};
struct FunctionNoRet {};

/////////////////////////////////////////////////////
//检测函数是否有返回类型的工具
template<typename R>
class CheckResultUtil
{
public:
	typedef FunctionHasRet RetType;
};

template<>
class CheckResultUtil<void>
{
public:
	typedef FunctionNoRet RetType;
};

/////////////////////////////////////////////////////
//消除引用类型的参数的引用标记
template<typename T>
class RemoveReferenceTypeUtil
{
public:
	typedef T Type;
};

template<typename T>
class RemoveReferenceTypeUtil<T&>
{
public:
	typedef T Type;
};

template<typename T>
class RemoveReferenceTypeUtil<const T&>
{
public:
	typedef T Type;
};

/////////////////////////////////////////////////////
//将char*类型转换为string类型
template<typename T>
class ConvertCharPtr2String
{
public:
	typedef T Type;
	static T& toValue(Type& val)
	{
		return val;
	}
};

template<>
class ConvertCharPtr2String<char*>
{
public:
	typedef std::string Type;
	static char* toValue(Type& val)
	{
		return const_cast<char*>(val.c_str());
	}
};

template<>
class ConvertCharPtr2String<const char*>
{
public:
	typedef std::string Type;
	static const char* toValue(Type& val)
	{
		return const_cast<char*>(val.c_str());
	}
};

class PacketProcessFunction
{
public:
	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream) { return FALSE; }
	MemberFunctionPtr mActualExportFunc;
};

// 这个函数的作用就是为每个类型分配一个全局的数组，保存消息数据处理函数 [11/23/2016 yz]
template<typename CLS>
PacketProcessFunction* getModulePacketProcessFunction(int funcIndex, PacketProcessFunction* newFunc = NULL)
{
	static PacketProcessFunction* sFunctions[CLS::ExportMethod_MAX];
	if (NULL != newFunc)
	{
		KSafeDelete(sFunctions[funcIndex]);
		sFunctions[funcIndex] = newFunc;
	}
	return sFunctions[funcIndex];
}

// 注册外部消息处理接口 [11/23/2016 yz]
#define GET_PARAM_FROM_STREAM(i)	\
	typedef ConvertCharPtr2String<typename RemoveReferenceTypeUtil<T##i>::Type> TraitT##i;\
	typename TraitT##i::Type p##i;\
	if(!stream.getValue(p##i))	\
	{	\
		return FALSE;	\
	}

#define USE_PARAM_FROM_STREAM(i) \
	TraitT##i::toValue(p##i)

	// 0个参数 [11/23/2016 yz]
template<typename Cls, typename R>
class ConcreteProcessFunctionParam0 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)();

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		if (!stream.isCompleted())
		{
			return FALSE;
		}
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);
		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)();
		if (NULL != retStream)
			retStream->pushValue(ret);

		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		if (!stream.isCompleted())
		{
			return FALSE;
		}
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);
		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)();
		return TRUE;
	}
};

template<typename Cls, typename R>
void registerExportMethod(int funcIndex, R(Cls::*functor)())
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam0<Cls, R>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 1个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1>
class ConcreteProcessFunctionParam1 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		if (!stream.isCompleted())
		{
			return FALSE;
		}
		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam1<Cls, R, T1>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 2个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1, typename T2>
class ConcreteProcessFunctionParam2 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1, T2 p2);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1, typename T2>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1, T2 p2))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam2<Cls, R, T1, T2>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 3个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1, typename T2, typename T3>
class ConcreteProcessFunctionParam3 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1, typename T2, typename T3>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1, T2 p2, T3 p3))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam3<Cls, R, T1, T2, T3>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 4个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4>
class ConcreteProcessFunctionParam4 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam4<Cls, R, T1, T2, T3, T4>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 5个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class ConcreteProcessFunctionParam5 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam5<Cls, R, T1, T2, T3, T4, T5>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 6个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class ConcreteProcessFunctionParam6 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		GET_PARAM_FROM_STREAM(6);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5),
			USE_PARAM_FROM_STREAM(6)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		GET_PARAM_FROM_STREAM(6);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5),
			USE_PARAM_FROM_STREAM(6)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam6<Cls, R, T1, T2, T3, T4, T5, T6>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 7个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class ConcreteProcessFunctionParam7 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		GET_PARAM_FROM_STREAM(6);
		GET_PARAM_FROM_STREAM(7);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5),
			USE_PARAM_FROM_STREAM(6),
			USE_PARAM_FROM_STREAM(7)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		GET_PARAM_FROM_STREAM(6);
		GET_PARAM_FROM_STREAM(7);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5),
			USE_PARAM_FROM_STREAM(6),
			USE_PARAM_FROM_STREAM(7)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam7<Cls, R, T1, T2, T3, T4, T5, T6, T7>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

// 8个参数 [11/23/2016 yz]
template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class ConcreteProcessFunctionParam8 : public PacketProcessFunction
{
public:
	typedef R(Cls::*FunctionType)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8);

	virtual BOOL processPacket(ModuleComponent* obj, DataStream& stream, DataStream* retStream)
	{
		typename CheckResultUtil<R>::RetType retType;
		return processInternal(obj, stream, retStream, retType);
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionHasRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		GET_PARAM_FROM_STREAM(6);
		GET_PARAM_FROM_STREAM(7);
		GET_PARAM_FROM_STREAM(8);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		R ret = (static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5),
			USE_PARAM_FROM_STREAM(6),
			USE_PARAM_FROM_STREAM(7),
			USE_PARAM_FROM_STREAM(8)
			);
		if (NULL != retStream)
			retStream->pushValue(ret);
		return TRUE;
	}

	BOOL processInternal(ModuleComponent* obj, DataStream& stream, DataStream* retStream, FunctionNoRet retTrait)
	{
		FunctionType functor = *(FunctionType*)(&mActualExportFunc);

		GET_PARAM_FROM_STREAM(1);
		GET_PARAM_FROM_STREAM(2);
		GET_PARAM_FROM_STREAM(3);
		GET_PARAM_FROM_STREAM(4);
		GET_PARAM_FROM_STREAM(5);
		GET_PARAM_FROM_STREAM(6);
		GET_PARAM_FROM_STREAM(7);
		GET_PARAM_FROM_STREAM(8);
		if (!stream.isCompleted())
		{
			return FALSE;
		}

		(static_cast<Cls*>((ModuleComponent*)obj)->*functor)(
			USE_PARAM_FROM_STREAM(1),
			USE_PARAM_FROM_STREAM(2),
			USE_PARAM_FROM_STREAM(3),
			USE_PARAM_FROM_STREAM(4),
			USE_PARAM_FROM_STREAM(5),
			USE_PARAM_FROM_STREAM(6),
			USE_PARAM_FROM_STREAM(7),
			USE_PARAM_FROM_STREAM(8)
			);
		return TRUE;
	}
};

template<typename Cls, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void registerExportMethod(int funcIndex, R(Cls::*functor)(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8))
{
	PacketProcessFunction* newFunc = new ConcreteProcessFunctionParam8<Cls, R, T1, T2, T3, T4, T5, T6, T7, T8>();
	newFunc->mActualExportFunc = *(MemberFunctionPtr*)&functor;
	getModulePacketProcessFunction<Cls>(funcIndex, newFunc);
}

template<typename CLS>
class ExportMethodExecutor : public ModuleComponent
{
public:
	ExportMethodExecutor()
	{}

	virtual BOOL executePacket(DataPacket* packet)
	{
		__GUARD__
			// 这里不执行返回值的消息 [11/24/2016 yz]
			if (packet->mIsReturn)
			{
				return ModuleComponent::executeReturnPacket(packet);
			}

		PacketProcessFunction* processFuncInfo = getModulePacketProcessFunction<CLS>(packet->mPackMethodId);
		KCheck(processFuncInfo);
		return executePacketFunction(packet, processFuncInfo);

		__UNGUARD__
			return FALSE;
	}
};

class PacketReturnCallable
{
public:
	PacketReturnCallable();
	virtual ~PacketReturnCallable() {}
	virtual void executeCallback(ModuleComponent* obj, DataStream& stream) { }
	virtual void executeOvertime(ModuleComponent* obj) { }
	virtual BOOL isValid() { return FALSE; }
	uint32 getOperateID() const
	{
		return mOperateID;
	}

	void setOperateID(uint32 opID)
	{
		mOperateID = opID;
	}

	void setOvertimeLength(uint32 overtimeLen)
	{
		mOvertimeLength = overtimeLen;
	}

	BOOL checkOvertime(uint32 uTick)
	{
		if (uTick > mTimeStamp + mOvertimeLength)
			return TRUE;
		return FALSE;
	}

protected:
	uint32 mOperateID;
	uint32 mTimeStamp;
	uint32 mOvertimeLength;
};

// 接收远端调用者的返回，并在本地执行回调 [11/24/2016 yz]
template<typename CLS, typename R, typename RetTrait>
struct CallbackFunctionTrait {};

template<typename CLS, typename R>
struct CallbackFunctionTrait<CLS, R, FunctionHasRet>
{
	// 如果函数定义的返回值本身带有引用，这里先消除引用，再统一加上引用 [12/16/2017 yz]
	typedef typename RemoveReferenceTypeUtil<R>::Type RetType;
	typedef void(CLS::*CallbackType)(RetType& param);
	typedef void(CLS::*CallbackTypeUserData)(RetType& param, DataStream& userData);
	typedef void(CLS::*OvertimeCallbackType)();
	typedef void(CLS::*OvertimeCallbackTypeUserData)(DataStream& userData);
};

template<typename CLS, typename R>
struct CallbackFunctionTrait<CLS, R, FunctionNoRet>
{
	typedef void(CLS::*CallbackType)();
	typedef void(CLS::*CallbackTypeUserData)(DataStream& userData);
	typedef void(CLS::*OvertimeCallbackType)();
	typedef void(CLS::*OvertimeCallbackTypeUserData)(DataStream& userData);
};

template<typename CLS, typename R>
class PacketReturnCallableInstance : public PacketReturnCallable
{
public:
	typedef typename CheckResultUtil<R>::RetType RetTypeTrait;
	typedef typename CallbackFunctionTrait<CLS, R, RetTypeTrait>::CallbackType CallDeclaration;
	typedef typename CallbackFunctionTrait<CLS, R, RetTypeTrait>::CallbackTypeUserData CallDeclarationWithUserData;
	typedef typename CallbackFunctionTrait<CLS, R, RetTypeTrait>::OvertimeCallbackType OvertimeCallDeclaration;
	typedef typename CallbackFunctionTrait<CLS, R, RetTypeTrait>::OvertimeCallbackTypeUserData OvertimeCallDeclarationWithUserData;

	PacketReturnCallableInstance()
		: mCallback(NULL)
		, mCallbackUserData(NULL)
		, mOvertimeCallback(NULL)
		, mOvertimeCallbackUserData(NULL)
	{}

	void bindCallback(CallDeclaration callback, OvertimeCallDeclaration overtimeCallback, uint32 overtime = 1000)
	{
		mCallback = callback;
		mOvertimeCallback = overtimeCallback;
		mCallbackUserData = NULL;
		mOvertimeCallbackUserData = NULL;
		setOvertimeLength(overtime);
	}

	void bindCallback(CallDeclarationWithUserData callback, OvertimeCallDeclarationWithUserData overtimeCallback, DataStream& userData, uint32 overtime = 1000)
	{
		mCallback = NULL;
		mOvertimeCallback = NULL;
		mCallbackUserData = callback;
		mOvertimeCallbackUserData = overtimeCallback;
		mUserData.swapData(userData);
		setOvertimeLength(overtime);
	}

	virtual BOOL isValid()
	{
		return (NULL != mCallback || NULL != mCallbackUserData) ? TRUE : FALSE;
	}

	virtual void executeCallback(ModuleComponent* obj, DataStream& stream)
	{
		RetTypeTrait typeTrait;
		executeCallbackImpl(obj, stream, typeTrait);
	}

	virtual void executeOvertime(ModuleComponent* obj)
	{
		if (NULL != mOvertimeCallbackUserData)
		{
			(static_cast<CLS*>((ModuleComponent*)obj)->*mOvertimeCallbackUserData)(mUserData);
		}
		else if (NULL != mOvertimeCallback)
		{
			(static_cast<CLS*>((ModuleComponent*)obj)->*mOvertimeCallback)();
		}
	}

	void executeCallbackImpl(ModuleComponent* obj, DataStream& stream, FunctionHasRet)
	{
		R retVal;
		stream.getValue(retVal);

		if (NULL != mCallbackUserData)
		{
			(static_cast<CLS*>((ModuleComponent*)obj)->*mCallbackUserData)(retVal, mUserData);
		}
		else if (NULL != mCallback)
		{
			(static_cast<CLS*>((ModuleComponent*)obj)->*mCallback)(retVal);
		}
	}

	void executeCallbackImpl(ModuleComponent* obj, DataStream& stream, FunctionNoRet)
	{
		if (NULL != mCallbackUserData)
		{
			(static_cast<CLS*>((ModuleComponent*)obj)->*mCallbackUserData)(mUserData);
		}
		else if (NULL != mCallback)
		{
			(static_cast<CLS*>((ModuleComponent*)obj)->*mCallback)();
		}
	}

protected:
	CallDeclaration mCallback;
	CallDeclarationWithUserData mCallbackUserData;
	OvertimeCallDeclaration mOvertimeCallback;
	OvertimeCallDeclarationWithUserData mOvertimeCallbackUserData;
	DataStream mUserData;
};

class PacketSendProxy
{
public:
	virtual BOOL overrideSendPacket(int32 targetModule, DataPacket* packet) = 0;
};

class PacketCallable
{
public:
	PacketCallable()
		: mPacket(NULL)
		, mTargetModuleName(INVALID_ID)
		, mSourceModuleName(INVALID_ID)
		, mTargetObjID(INVALID_ID)
		, mSourceObjID(INVALID_ID)
		, mInvoker(NULL)
		, mReturnCallback(NULL)
	{
	}

	virtual ~PacketCallable();

	DataPacket* getDataPacket();

	void setTarget(int32 moduleName, int64 objID);
	void setSource(int32 moduleName, int64 objID);
	void setInvoker(ModuleComponent* invoker);
	void setSendPacketProxy(PacketSendProxy* proxy);

	void serializeParams(DataStream& stream)
	{

	}

	template<typename T1>
	void serializeParams(DataStream& stream, T1 p1)
	{
		stream.pushValue(p1);
	}

	template<typename T1, typename T2>
	void serializeParams(DataStream& stream, T1 p1, T2 p2)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
	}

	template<typename T1, typename T2, typename T3>
	void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
	}

	template<typename T1, typename T2, typename T3, typename T4>
	void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
		stream.pushValue(p5);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		stream.pushValue(p1);
		stream.pushValue(p2);
		stream.pushValue(p3);
		stream.pushValue(p4);
		stream.pushValue(p5);
		stream.pushValue(p6);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
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
	void serializeParams(DataStream& stream, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
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

	template<typename CALLBACK_CLS, typename R>
	PacketReturnCallableInstance<CALLBACK_CLS, R>* createReturnCallback()
	{
		if (NULL != mReturnCallback)
		{
			KSafeDelete(mReturnCallback);
			mReturnCallback = NULL;
		}
		mReturnCallback = new PacketReturnCallableInstance<CALLBACK_CLS, R>();
		return (PacketReturnCallableInstance<CALLBACK_CLS, R>*)mReturnCallback;
	}

protected:
	virtual void sendPacket();

	int32 mTargetModuleName;
	int64 mTargetObjID;
	int32 mSourceModuleName;
	int64 mSourceObjID;
	DataPacket* mPacket;
	ModuleComponent* mInvoker;
	PacketSendProxy* mSendPacketProxy;

	PacketReturnCallable* mReturnCallback;
};

template<typename EXPORT_INTERFACE>
class ExportMethodCallable
{
public:
	ExportMethodCallable()
		: mCallInterface(NULL)
	{}

	ExportMethodCallable(EXPORT_INTERFACE* interf)
		: mCallInterface(interf)
	{}

	ExportMethodCallable(const ExportMethodCallable& callable)
	{
		mCallInterface = callable.getAndClear();
	}

	virtual ~ExportMethodCallable()
	{
		KSafeDelete(mCallInterface);
	}

	ExportMethodCallable& operator=(const ExportMethodCallable& rhs)
	{
		KSafeDelete(mCallInterface);
		mCallInterface = rhs.getAndClear();
	}

	EXPORT_INTERFACE& operator*() const
	{
		return *mCallInterface;
	}

	EXPORT_INTERFACE* operator->() const
	{
		return mCallInterface;
	}

	EXPORT_INTERFACE* getAndClear() const
	{
		EXPORT_INTERFACE* ret = mCallInterface;
		mCallInterface = NULL;
		return ret;
	}

	void clear()
	{
		KSafeDelete(mCallInterface);
	}

protected:
	mutable EXPORT_INTERFACE* mCallInterface;
};

#include "ModuleInlineMacros.h"

#endif
