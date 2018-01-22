/*$T Common/Version.h GC 1.140 10/10/07 10:06:57 */
#include "kwNetBasics.h"
/*
 * created: 2008/01/16 created: 16:1:2008 15:59 filename:
 * d:\project\project_11_30\Common\Version.h file path:
 * d:\project\project_11_30\Common file base: Version file ext: h author: Richard
 * purpose: 同步项目版本号，包括server,client的所有solution,都应包含此头文件
 */
#ifndef Version_h__
#define Version_h__

#define VERSION_FIRST_VALUE 0
#define VERSION_SECOND_VALUE 0
#define VERSION_FOURTH_VALUE 0

// 临时更新号，当不能修改客户端版本号的时候用来表示代码修改 [yangzhen]
#define VERSION_CLIENT_UPDATE_NUMBER 0

#define MSGHEAD_MAGIC_NUM 0xD13DEA9B
#define MSG_MAGIC_NUM 0x9326662B
#define STREAM_MAGIC_NUM 0xD5C0A6E3
#define STREAM_ENCRPTOR_KEY 0xD7AC0001

#define VERSION_MASK_1ST_SEC 0xf0000000
#define VERSION_MASK_2ND_SEC 0x0ffc0000
#define VERSION_MASK_3RD_SEC 0x0003fe00
#define VERSION_MASK_4TH_SEC 0x000001ff

// 主版本，一般不变 [yangzhen]
#define VERSION_GET_1ST_SECTION(VER) ((VER & VERSION_MASK_1ST_SEC) >> 28)
// 客户端程序更新版本 [yangzhen]
#define VERSION_GET_2ND_SECTION(VER) ((VER & VERSION_MASK_2ND_SEC) >> 18)
// 客户端资源更新版本 [yangzhen]
#define VERSION_GET_3RD_SECTION(VER) ((VER & VERSION_MASK_3RD_SEC) >> 9)
// 纯服务器更新版本，包括代码与资源 [yangzhen]
#define VERSION_GET_4TH_SECTION(VER) (VER & VERSION_MASK_4TH_SEC)

// 组合版本号 [yangzhen]
#define MAKE_VERSION_NUM(FIRST, SECOND, THIRD, FOURTH) \
	(((FIRST & 0xf) << 28) | ((SECOND & 0x3ff) << 18) | ((THIRD & 0x1ff) << 9) | (FOURTH & 0x1ff))

// 严格检测的版本 [yangzhen]
#define STRICT_VERSION_CHECK_MASK (VERSION_MASK_1ST_SEC | VERSION_MASK_2ND_SEC)

// 严格版本号的后面两位始终为0，后面两位只影响更新，不限制tcp链接 [yangzhen]
#define CURRENT_STRICT_VERSION \
	MAKE_VERSION_NUM(VERSION_FIRST_VALUE, VERSION_SECOND_VALUE, 0, 0)

// 设置客户端资源版本 [yangzhen]
#define SET_VERSION_3RD_SECTION(VER) \
	_CURRENT_VERSION = (_CURRENT_VERSION & ~VERSION_MASK_3RD_SEC) | ((VER & 0x1ff) << 9)

// 纯服务器版本更新 [yangzhen]
#define SET_VERSION_4TH_SECTION(VER) \
	_CURRENT_VERSION = (_CURRENT_VERSION & ~VERSION_MASK_4TH_SEC) | (VER & 0x1ff)

extern uint32 _CURRENT_VERSION;
extern uint32 GET_CURRENT_VERSION();

#endif /* Version_h__ */
