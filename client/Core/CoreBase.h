// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef __CORE_BASE__
#define __CORE_BASE__

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
//#include "CoreBase.generated.h"


#if !defined(GAME_DEBUG)
#define GAME_DEBUG 0
#endif

#if !defined(GAME_DEVELOPMENT)
#define GAME_DEVELOPMENT 0
#endif

#if !defined(GAME_TEST)
#define GAME_TEST 0
#endif

#if !defined(GAME_SHIPPING)
#define GAME_SHIPPING 0
#endif

#define GAME_UNKNOWN 0

// GAME_GM_OPTION 用于包含gm功能代码的宏，可以防止发布的时候，这些功能代码被错误的打包出去。
// GAME_DEBUG_OPTION 调试日志开关宏
// GAME_DEBUG_DATA_OPTION 调试数据开关宏

#if GAME_DEBUG //编译的是调试模式的dll
//#	pragma message("GAME_DEBUG mode.")
#	define GAME_GM_OPTION 1
#	define GAME_DEBUG_OPTION 1
#	define  GAME_DEBUG_DATA_OPTION 1

#elif GAME_DEVELOPMENT //编译的是开发模式的dll
//#	pragma message("GAME_DEVELOPMENT mode.")
#	define GAME_GM_OPTION 1
#	define GAME_DEBUG_OPTION 1
#	define  GAME_DEBUG_DATA_OPTION 1

#elif GAME_TEST	//编译的是测试模式的dll
//#	pragma message("GAME_TEST mode.")
#	define GAME_GM_OPTION 0
#	define GAME_DEBUG_OPTION 1
#	define  GAME_DEBUG_DATA_OPTION 1

#elif GAME_SHIPPING //编译的是发布模式的dll
//#	pragma message("GAME_SHIPPING mode.")
#	define GAME_GM_OPTION 0
#	define GAME_DEBUG_OPTION 0
#	define  GAME_DEBUG_DATA_OPTION 0

#else	//未知的编译模式
//#	pragma message("unknown build mode.")
#	define GAME_UNKNOWN 1
#	define GAME_GM_OPTION 0
#	define GAME_DEBUG_OPTION 0
#	define  GAME_DEBUG_DATA_OPTION 0

#endif


#if PLATFORM_ANDROID
#	ifdef GAME_DEBUG_OPTION
#		undef GAME_DEBUG_OPTION
#		define GAME_DEBUG_OPTION 0
#	endif
#endif

#if !defined(GAME_GM_OPTION)
#	define GAME_GM_OPTION 0
#endif

#if !defined(GAME_DEBUG_OPTION)
#	define GAME_DEBUG_OPTION 0
#endif

#if !defined(GAME_DEBUG_DATA_OPTION)
#	define GAME_DEBUG_DATA_OPTION 0
#endif


#if GAME_UNKNOWN
#	pragma warning("unknown build mode.")
#endif
//
//#if GAME_DEBUG_OPTION
//#	pragma message("GAME_DEBUG_OPTION 1.")
//#else 
//#	pragma message("GAME_DEBUG_OPTION 0.")
//#endif

#endif //__CORE_BASE__