// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreBase.h"
#include "Logging/LogMacros.h"
/**
*	调试封装
*/

MOBAGAME_API DECLARE_LOG_CATEGORY_EXTERN(GameDebugLog, Log, All);

#if GAME_DEBUG_OPTION

#define GAME_DEBUG_PRINT(Format, ...) UE_LOG(GameDebugLog, Display, Format, __VA_ARGS__)

#define GAME_DEBUG_WARNING(Format, ...) UE_LOG(GameDebugLog, Warning, Format, __VA_ARGS__)

#else

#define GAME_DEBUG_PRINT(Format, ...) 
#define GAME_DEBUG_WARNING(Format, ...) 

#endif

class MOBAGAME_API DebugUtilities : public FNoncopyable
{
public:
};
