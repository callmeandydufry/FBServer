
#ifndef __GAME_DEF_H__
#define __GAME_DEF_H__

#undef MAX_PATH

/************************************************************************/
/* 最大值最小值定义                                                     */
/************************************************************************/
#define MAX_TEAMINFOLIST_NUM				(20)																					//
#define MAX_SERVER							(24)																					//
#define MAX_WORLD							(255)																					//
#define MAX_KHAN_CHARACTER_INPUTNAME				(24)																					//
#define MAX_CHARACTER_NUM					(3)																						//
#ifdef __WINDOWS__
#ifdef __RELEASE_SERVER__
#define MAX_WORLD_PLAYER						(10000)
#else
#define MAX_WORLD_PLAYER						(10)
#endif
#else
#define MAX_WORLD_PLAYER						(10000)
#endif

//数据库相关
#define DATABASE_STR_LEN					(128)																					//
#define DB_USE_STR_LEN						(32)																					//
#define DB_PASSWORD_STR_LEN					(32)																					//
#define DB_OPERATION_TIME					(0)																					//
#define DB_CHAR_NUMBER						(5)																						//
#define DB_2_SM_TIME						(60000)																					//
#define DB_CONNECTION_COUNT					(20)	
#define DB_TITLE_LEN						(20)	
#define DB_CONTENT_LEN						(32)

#define DB_MAX_STR_LEN (128)
#define DB_MAX_USER_LEN	 (32)	
#define DB_MAX_PASSWD_LEN (32)	

#define MAX_ACCOUNT							(100)
#define MAX_CHARACTER_NAME					(30)
#define MAX_FILE_PATH						(260)																					//
#define MAX_TOKEN_LEN						(512)
#define MAX_CREATE_SNID_NUM					(1000)																					// 批量创建snid数量 [1/5/2018 Chief] 
#define MAX_MAILID_LIST						(500)

#ifdef __WINDOWS__
#ifdef __RELEASE_SERVER__
#define MAX_SNID_POOL_NUMS					(1000000)
#define MAX_MAIL_POOL_NUMS					(1000000)
#else
#define MAX_SNID_POOL_NUMS					(10000)
#define MAX_MAIL_POOL_NUMS					(10000)
#endif
#else
#define MAX_SNID_POOL_NUMS					(1000000)
#define MAX_MAIL_POOL_NUMS					(1000000)
#endif

#define MAX_ROBOT_NUM						(1000)
#define MAX_ROBOT_CMD_STR_LEN				(32)
/************************************************************************/
/* 网络线路类型定义（目前只定义网通电信两种）                           */
/************************************************************************/
enum NET_TYPE
{
	NET_TYPE_UNKNOW = -1,		// 未知
	NET_TYPE_CNC = 0,			// 网通
	NET_TYPE_CHINANET = 1,		// 电信
	NET_TYPE_MAX_NUM = 2,		// 类型数量
};

// GlobalVal ENUM [2/8/2018 Chief]
enum EProFlag
{
	EPF_INVALID = -1,

	EPF_MailID = 0,

	EPF_MAX,
};

// 邮件当前可使用的最大ID [2/8/2018 Chief]
#define MAX_MAIL_USE_ID					(1000000000)

#endif
