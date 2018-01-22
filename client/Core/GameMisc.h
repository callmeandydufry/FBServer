// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreBase.h"
/**
 * 混杂的底层API
 */
class MOBAGAME_API GameMisc : public FNoncopyable
{
public:

	template<typename TMark>
	FORCEINLINE static bool BitFlag(int32 _Val, TMark _Mark)
	{
		int32 Mark = _Mark;
		return ((_Val & Mark) != 0);
	}
	//template <typename T, typename TMark>
	//static bool BitFlag(T _Val, TMark _Mark) = delete; // C++11 ，禁止生成其他参数的func函数

	FORCEINLINE static int32 MakeInt32(int16 hight16, int16 low16)
	{
		int32 low32 = (int32)low16;
		int32 hight32 = ((int32)hight16) << 16;
		int32 Final = low32 | hight32;
		return Final;
	}

	FORCEINLINE static int16 HightInt16(const int32& val)
	{
		int32 hight32 = val & 0xffff0000;
		int16 hight16 = (int16)(hight32 >> 16);
		return hight16;
	}

	FORCEINLINE static int16 LowInt16(const int32& val)
	{
		int32 low32 = val & 0x0000ffff;
		int16 low16 = (int16)(low32);
		return low16;
	}

	static FString GetEnumName(const TCHAR* EnumType, int32 EnumValue);

	template<typename TInterface, typename TFrom>
	static TScriptInterface<TInterface> CastScriptInterface(TFrom* From)
	{
		TScriptInterface<TInterface> ScriptInterface;
		TInterface* face = Cast<TInterface>(From);
		if (face)
		{
			ScriptInterface.SetObject(From);
			ScriptInterface.SetInterface(face);
		}
		return ScriptInterface;
	}

	template<typename InterfaceType>
	static TScriptInterface<InterfaceType> MakeScriptInterface(InterfaceType* Interface)
	{
		TScriptInterface<InterfaceType> Face;
		UObject* Obj = Cast<UObject>(Interface);
		if (Obj)
		{
			Face.SetObject(Obj);

			InterfaceType* SourceInterface = Interface;
			Face.SetInterface(SourceInterface);
		}
		return Face;
	}
	template<typename InterfaceType>
	FORCEINLINE static InterfaceType* GetScriptInterface(const TScriptInterface<InterfaceType>& Interface)
	{
		return (InterfaceType*)Interface.GetInterface();
	}

};

template<typename TInterface, typename TFrom>
TScriptInterface<TInterface> CastFace(TFrom* From)
{
	return GameMisc::CastScriptInterface<TInterface>(From);
}

template<typename InterfaceType>
FORCEINLINE static InterfaceType* GetFace(const TScriptInterface<InterfaceType>& Interface)
{
	return GameMisc::GetScriptInterface(Interface);
}

template<typename InterfaceType>
FORCEINLINE static TScriptInterface<InterfaceType> MakeFace(InterfaceType* Interface) 
{
	return GameMisc::MakeScriptInterface(Interface);
}

#define BITFLAG_TEST(_Val, _Mark) GameMisc::BitFlag(_Val, _Mark) 

#define ENUM_NAME(_Type, _Value) GameMisc::GetEnumName(TEXT(#_Type), _Value)
//( (void)sizeof(UE4Asserts_Private::GetMemberNameCheckedJunk(_Type::_Value)), GameMisc::GetEnumName(TEXT(#_Type), _Value) )


#define CASE_EQUAL(_Val, _Case, _Equal) case _Case: { _Val = _Equal } break
