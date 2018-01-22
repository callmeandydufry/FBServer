// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMisc.h"


FString GameMisc::GetEnumName(const TCHAR* EnumType, int32 EnumValue)
{
	const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, EnumType);
	if (Enum)
	{
		return Enum->GetNameStringByIndex(int32(EnumValue));
	}
	return FString::FromInt(EnumValue);
}
