/*$T Common/Version.h GC 1.140 10/10/07 10:06:57 */
#include "kwNetBasics.h"
/*
 * created: 2008/01/16 created: 16:1:2008 15:59 filename:
 * d:\project\project_11_30\Common\Version.h file path:
 * d:\project\project_11_30\Common file base: Version file ext: h author: Richard
 * purpose: ͬ����Ŀ�汾�ţ�����server,client������solution,��Ӧ������ͷ�ļ�
 */
#ifndef Version_h__
#define Version_h__

#define VERSION_FIRST_VALUE 0
#define VERSION_SECOND_VALUE 0
#define VERSION_FOURTH_VALUE 0

// ��ʱ���ºţ��������޸Ŀͻ��˰汾�ŵ�ʱ��������ʾ�����޸� [yangzhen]
#define VERSION_CLIENT_UPDATE_NUMBER 0

#define MSGHEAD_MAGIC_NUM 0xD13DEA9B
#define MSG_MAGIC_NUM 0x9326662B
#define STREAM_MAGIC_NUM 0xD5C0A6E3
#define STREAM_ENCRPTOR_KEY 0xD7AC0001

#define VERSION_MASK_1ST_SEC 0xf0000000
#define VERSION_MASK_2ND_SEC 0x0ffc0000
#define VERSION_MASK_3RD_SEC 0x0003fe00
#define VERSION_MASK_4TH_SEC 0x000001ff

// ���汾��һ�㲻�� [yangzhen]
#define VERSION_GET_1ST_SECTION(VER) ((VER & VERSION_MASK_1ST_SEC) >> 28)
// �ͻ��˳�����°汾 [yangzhen]
#define VERSION_GET_2ND_SECTION(VER) ((VER & VERSION_MASK_2ND_SEC) >> 18)
// �ͻ�����Դ���°汾 [yangzhen]
#define VERSION_GET_3RD_SECTION(VER) ((VER & VERSION_MASK_3RD_SEC) >> 9)
// �����������°汾��������������Դ [yangzhen]
#define VERSION_GET_4TH_SECTION(VER) (VER & VERSION_MASK_4TH_SEC)

// ��ϰ汾�� [yangzhen]
#define MAKE_VERSION_NUM(FIRST, SECOND, THIRD, FOURTH) \
	(((FIRST & 0xf) << 28) | ((SECOND & 0x3ff) << 18) | ((THIRD & 0x1ff) << 9) | (FOURTH & 0x1ff))

// �ϸ���İ汾 [yangzhen]
#define STRICT_VERSION_CHECK_MASK (VERSION_MASK_1ST_SEC | VERSION_MASK_2ND_SEC)

// �ϸ�汾�ŵĺ�����λʼ��Ϊ0��������λֻӰ����£�������tcp���� [yangzhen]
#define CURRENT_STRICT_VERSION \
	MAKE_VERSION_NUM(VERSION_FIRST_VALUE, VERSION_SECOND_VALUE, 0, 0)

// ���ÿͻ�����Դ�汾 [yangzhen]
#define SET_VERSION_3RD_SECTION(VER) \
	_CURRENT_VERSION = (_CURRENT_VERSION & ~VERSION_MASK_3RD_SEC) | ((VER & 0x1ff) << 9)

// ���������汾���� [yangzhen]
#define SET_VERSION_4TH_SECTION(VER) \
	_CURRENT_VERSION = (_CURRENT_VERSION & ~VERSION_MASK_4TH_SEC) | (VER & 0x1ff)

extern uint32 _CURRENT_VERSION;
extern uint32 GET_CURRENT_VERSION();

#endif /* Version_h__ */
