#ifndef __TEMPLATE_UTILS_H__
#define __TEMPLATE_UTILS_H__

#include "TypeDefine.h"

#if _MSC_VER
#define GET_STRUCT_OFFSET(InStruct,Member)	((PtrInt)&((InStruct*)NULL)->Member)
#else // GCC [JiangWei]
#define GET_STRUCT_OFFSET(InStruct,Member)	(((PtrInt)&((InStruct*)0x1)->Member) - 0x1)
#endif

//--------------------------------------------------------------------------------------
// 用来判断某个类型是否需要调用构造函数 [12/7/2017 yz]
template<typename T>
class TTypeInfoAtomicBase
{
public:
	typedef T ConstInitType;
	enum { NeedsConstructor = 0 };
	enum { NeedsDestructor = 0 };
};

template<typename T>
class TTypeInfoConstructedBase
{
public:
	typedef const T& ConstInitType;
	enum { NeedsConstructor = 1 };
	enum { NeedsDestructor = 1 };
};


template<typename T>
class TTypeInfo : public TTypeInfoConstructedBase<T> {};

// 原生的指针不需要构造函数
template<typename T>
class TTypeInfo<T*> : public TTypeInfoAtomicBase<T*> {};

// 定义不用构造函数的类型
template <> class TTypeInfo<int> : public TTypeInfoAtomicBase<int> {};
template <> class TTypeInfo<uint32> : public TTypeInfoAtomicBase<uint32> {};
template <> class TTypeInfo<char> : public TTypeInfoAtomicBase<char> {};
template <> class TTypeInfo<uchar> : public TTypeInfoAtomicBase<uchar> {};
template <> class TTypeInfo<long> : public TTypeInfoAtomicBase<long> {};
template <> class TTypeInfo<ulong> : public TTypeInfoAtomicBase<ulong> {};
template <> class TTypeInfo<short> : public TTypeInfoAtomicBase<short> {};
template <> class TTypeInfo<uint16> : public TTypeInfoAtomicBase<uint16> {};
#if defined(__GNUC__) && (defined(__arch64__) || defined(_LP64))
// gcc编译64位程序时uint64与ulong定义相同
#else
template <> class TTypeInfo<uint64> : public TTypeInfoAtomicBase<uint64> {};
#endif
template <> class TTypeInfo<float> : public TTypeInfoAtomicBase<float> {};
//--------------------------------------------------------------------------------------

#endif