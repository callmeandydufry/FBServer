#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "Util.h"
#include "BaseTool.h"
#include "MessageFactoryManager.h"

#if __MAPSERVER__ 
#include "XRLRemoteLog.h"
#include "ServerConnectorManager_RemoteLog.h"
#endif


#if ( defined(__LINUX__) && (defined(__MAPSERVER__) || defined(__GLSERVER__)) )
#include "udpsender.h"
extern CUdpSender g_UdpSender;
#endif

#define LOG_DIR "Log"

namespace	QLogSystem
{
	FastLock	__logger_guard__;
	int32		__print_log__ = 1;
	int32		__active_log__ = 1;
	BOOL		__allinone_log__ = FALSE;

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QUICK_LOG(int32 eChannel, const char *cszFormat, ...)
	{
		__GUARD__;
		if(__active_log__ != 1) return;
		if(eChannel < 0 || eChannel >= ALL_LOG_NUMBER) return;

		int32 nDayElapsed = GET_TIME().GetDaysNumEplised();

		//  [yangzhen]
		char szBuff[ciLog_Temp_Buff] = { 0 };
		char szLogBuff[ciLog_Temp_Buff] = { 0 };
		va_list argptr;
		_NET_TRY
		{
			//组织日志字符串
			va_start(argptr, cszFormat);
			tvsnprintf(szBuff, ciLog_Temp_Buff - ciLog_Temp_Name - 1, cszFormat, argptr);
			va_end(argptr);

			char szTime[ciLog_Temp_Name] = { 0 };
			tsnprintf( szTime, ciLog_Temp_Name - 1, "(%d %.2d:%.2d:%.2d %.3f %lu)", 
				GET_TIME().GetCTime(),
				GET_TIME().hour(), 
				GET_TIME().minute(), 
				GET_TIME().second(), 
				(float) (GET_TIME().TickCount()) / 1000.0, 
				KGetCurrentTID() );

			tsnprintf_s(szLogBuff, ciLog_Temp_Buff, "%s %s\n", szTime, szBuff);
		}
		_NET_CATCH
		{
			if(__print_log__) 
				printf("ERROR: SLOW LOG UNKNOWN ERROR!\n");
			return;
		}


		if(QLogger::instance().IsSlowLog(eChannel))
		{
			QLogger::instance().write_log_realtime(eChannel, szLogBuff, nDayElapsed);
			//QLogger::instance().UploadingLog(eChannel, szLogBuff);
		}
		else
		{
			QLogger::instance().cache_log_notime(eChannel, szLogBuff, nDayElapsed);
			//QLogger::instance().UploadingLog(eChannel, szLogBuff);
		}

		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void SLOW_AUDIT_LOG(int32 eChannel, const char *cszFormat, ...)
	{
		__GUARD__;

		if(__active_log__ != 1) 
			return;

		char szBuff[ciLog_Temp_Buff] = { 0 };
		char szLogBuff[ciLog_Temp_Buff] = { 0 };

		int32 nDayElapsed = GET_TIME().GetDaysNumEplised();

		va_list argptr;
		_NET_TRY
		{
			//组织日志字符串
			va_start(argptr, cszFormat);
			tvsnprintf(szBuff, ciLog_Temp_Buff - ciLog_Temp_Name - 1, cszFormat, argptr);
			va_end(argptr);

			tsnprintf_s(szLogBuff, ciLog_Temp_Buff, "%s\n",szBuff);
		}
		_NET_CATCH
		{
			if(__print_log__) 
				printf("ERROR: SLOW LOG UNKNOWN ERROR!\n");
			return;
		}

		QLogger::instance().write_log_realtime(eChannel, szLogBuff, nDayElapsed);

		__UNGUARD__
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	QLogger &QLogger::instance(void)
	{
		/*~~~~~~~~~~~~~~~~~~~*/
		static QLogger	s_qlog;
		/*~~~~~~~~~~~~~~~~~~~*/

		return s_qlog;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	QLogger::QLogger()
	{
		__GUARD__;
		for(int32 _Idx = 0; _Idx < ALL_LOG_NUMBER; _Idx++)
		{
			m_pMemoryLog[_Idx] = NULL;
			m_WritePos[_Idx] = 0;
			m_szLogPath[_Idx] = NULL;
			m_isUploading[_Idx] = FALSE;
			m_isPrintToConsole[_Idx] = FALSE;
			m_isUseHoral[_Idx] = FALSE;
			m_pBackupCacheLog[_Idx] = NULL;
			m_BackupLogWritePos[_Idx] = 0;
		}

		m_pSaveLog = NULL;
		m_MemorySize = 0;
		m_nLastBackUpTime = 0;

		m_isUseBackUPBuff = FALSE; // 不备份日志 [11/3/2016 scw]

		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	QLogger::~QLogger(void)
	{
		__GUARD__;
		for(int32 _Idx = 0; _Idx < ALL_LOG_NUMBER; _Idx++)
		{
			KSafeDelete_ARRAY(m_pMemoryLog[_Idx]);
			m_WritePos[_Idx] = 0;
			KSafeDelete_ARRAY(m_szLogPath[_Idx]);

			KSafeDelete_ARRAY(m_pBackupCacheLog[_Idx]);
			m_BackupLogWritePos[_Idx] = 0;
		}

		KSafeDelete_ARRAY(m_pSaveLog);
		m_MemorySize = 0;
		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	BOOL QLogger::Init(int32 cachesize)
	{
		__GUARD__;
		if(0 < cachesize)
			m_MemorySize = cachesize;
		else
			m_MemorySize = ciDefault_Log_Cache_Size;
		
		RegisterAllLogPath();

		for(int32 _Idx = 0; _Idx < ALL_LOG_NUMBER; _Idx++)
		{
			if(m_LogType[_Idx] == eQUICK_LOG)
			{
				m_pMemoryLog[_Idx] = new char[m_MemorySize];
				if(m_pMemoryLog[_Idx] == NULL) return FALSE;
				memset(m_pMemoryLog[_Idx], 0, m_MemorySize);
				m_WritePos[_Idx] = 0;
			}
			else
			{
				m_pMemoryLog[_Idx] = NULL;
				m_WritePos[_Idx] = 0;
			}

			m_pBackupCacheLog[_Idx] = new char[m_MemorySize];
			if(m_pBackupCacheLog[_Idx] == NULL) return FALSE;
			memset(m_pBackupCacheLog[_Idx], 0, m_MemorySize);
			m_BackupLogWritePos[_Idx] = 0;
		}

		m_pSaveLog = new char[m_MemorySize];
		if(m_pSaveLog == NULL) return FALSE;
		memset(m_pSaveLog, 0, m_MemorySize);

		return TRUE;
		__UNGUARD__;
		return FALSE;
	}

	BOOL QLogger::isUploadingLog(int32 eChannel)
	{
		__GUARD__
			KCheck( eChannel>=0 && eChannel<ALL_LOG_NUMBER );
			return m_isUploading[eChannel];
		__UNGUARD__ return FALSE;
	}

	int8 QLogger::GetLogType(int32 eChannel)
	{
		if( eChannel>=0 && eChannel<ALL_LOG_NUMBER )
		{
			return m_LogType[eChannel];
		}
		return -1;
	}

	BOOL QLogger::IsSlowLog(int32 eChannel)
	{
		if( eChannel>=0 && eChannel<ALL_LOG_NUMBER )
		{
			if(eSLOW_LOG == m_LogType[eChannel])
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL QLogger::IsPrintToConsole(int32 eChannel)
	{
		if(eChannel >= 0 && eChannel < ALL_LOG_NUMBER)
		{
			return m_isPrintToConsole[eChannel];
		}
		return FALSE;
	}
	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::RegisterAllLogPath()
	{
		__GUARD__

		RegisterLogPath( QUICK_LOG_LOGIN,			"login",			eQUICK_LOG, TRUE);
		RegisterLogPath( QUICK_LOG_DEBUG,			"maptrace",			eQUICK_LOG, TRUE);
		RegisterLogPath( QUICK_LOG_ERROR,			"errors",			eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_LUA_ERROR,		"luaerrors",		eQUICK_LOG, TRUE );
		RegisterLogPath( QUICK_LOG_FUNCTION,		"luacalls",			eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_WORLD,			"world",			eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_ITEMCHANGE,		"db/itemchange",	eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_MONEYCHANGE,		"db/moneychange",	eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_CHAT,			"chat",				eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_GM,				"db/GM",			eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_IBMONEY,			"db/ibmoneylog",	eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_IBCOST,			"db/ibcostlog",		eQUICK_LOG );
		RegisterLogPath( QUICK_LOG_ENTERMAP,		"EnterMap",			eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_TICK_TIMER,		"mapticktimer",		eQUICK_LOG, TRUE);
		RegisterLogPath( QUICK_LOG_DBDEBUG,		    "db/dbdebug",		eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_TICK,		    "threadtick",		eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_PLAYERHERO,		"PlayerHero",		eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_POSTMAIL,		"PostMail",			eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_AWARDPOST,		"AwardPost",		eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_BATTLE,			"Battle",			eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_ARENA,			"Arena",			eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_ITEM_OPERATE,	"ItemOperate",		eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_MAPLAWLESSOP,	"MapLawlessOp",		eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_PLAYERUSESKILL,	"PlayerSkillCount",	eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_SKILL,			"db/skillchange",	eQUICK_LOG);
		RegisterLogPath( QUICK_LOG_PLAYERLOGINOUT,	"db/loginout",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_ORDER_DEBUG,		"orderdebug",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_ORDER_ERROR,		"ordererrors",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_ORDER_BILLING,	"db/orderbilling",	eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_DB_HEROGROW,		"db/herogrowinfo",	eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_EQUIP,			"db/equip",			eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_AWARDPOST_DB,		"db/awardpostdb",	eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_POST_DB,			"db/postdb",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_OBJ_INFO,			"ObjInfo",			eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_CDKEY,			"CDKey",			eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_DEBUGDUMP,		"DebugDump",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_CONNECTOR,		"connector_login",	eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_MAILMGR,			"mailmgr",			eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_ZUOBI,			"zuobi",			eQUICK_LOG, TRUE);
		RegisterLogPath(QUICK_LOG_ACTIVITY_STATUS,	"activitystatus",	eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_PLAYER_RENAME,	"playerRename",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_GUILD_RENAME,		"guildRename",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_PLAYER_PHYSICALPOWER_OVERFLOW,		"PhysicalPowerOverflow",		eQUICK_LOG);
		RegisterLogPath(QUICK_LOG_CITY_BUILDING_MAP, "CityBuildingMap", eQUICK_LOG);

		RegisterLogPath(SLOW_LOG_AIDEBUG,			"AIDebug",			eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_SERVER,			"Mapserver",		eSLOW_LOG, TRUE);
		RegisterLogPath(SLOW_LOG_RECONNECT,			"Reconnect",		eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_MSGERROR,			"MsgError",			eSLOW_LOG, TRUE);
		RegisterLogPath(SLOW_LOG_SERVERERROR,		"ServerError",		eSLOW_LOG, TRUE);
		RegisterLogPath(SLOW_LOG_WORLD,				"GLserver",			eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_RECYCLEPLAYER,		"SaveArchive",		eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_MAPSERVER_SHAREMEM,"DBShare",			eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_GL_DBCENTER,		"DBCenter",			eSLOW_LOG, TRUE);
		RegisterLogPath(SLOW_LOG_DEAMON,			"CheckDeadLock",	eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_CLIENT_ERROR_REPORT,"ClientErrorReport", eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_MAPSERVER_USAGE,	"MapUsage",			eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_MSG_RECORD,		"MsgRecord",		eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_RAW_DBSHARE,		"status_dbshare",	eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_RAW_BILLING,		"status_billing",	eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_RAW_MAPSERVER,		"status_map",		eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_RAW_PLATFORM,		"status_plat",		eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_NET_DEBUG,			"NetDebug",			eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_SMERROR,			"smError",			eSLOW_LOG);
		RegisterLogPath(SLOW_LOG_REMOTE_LOG_SERVER,	"RemoteLogServer",	eSLOW_LOG);

		RegisterLogPath(SLOW_LOG_GAME_LOGIC_ITEMFLOW,			"audit/ItemFlow",	eSLOW_LOG, FALSE);
		RegisterLogPath(SLOW_LOG_GAME_LOGIC_MONEYFLOW,			"audit/MoneyFlow",	eSLOW_LOG, FALSE);
		RegisterLogPath(SLOW_LOG_GAME_LOGIC_USERCHARGE,			"audit/UserChargeLog",	eSLOW_LOG, FALSE);
		RegisterLogPath(SLOW_LOG_GAME_LOGIC_USERLOGIN,			"audit/UserLoginLog",	eSLOW_LOG, FALSE);
		RegisterLogPath(SLOW_LOG_GAME_LOGIC_USERLOGOUT,			"audit/UserLogoutLog",	eSLOW_LOG, FALSE);
		RegisterLogPath(SLOW_LOG_GAME_LOGIC_USERREGISTER,		"audit/UserRegisterLog",	eSLOW_LOG, FALSE);
		RegisterLogPath(SLOW_LOG_GAME_LOGIC_BATTLEINFO,			"audit/BattleInfoLog",	eSLOW_LOG, FALSE);

		RegisterLogPath(SLOW_LOG_MERGE_SERVER, "MergeServer", eSLOW_LOG, TRUE);
		
		// 创建目录 [xingzhishan]
		RegisterDir();
		__UNGUARD__
	}

	void QLogger::RegisterDir()
	{
		if (access("./" LOG_DIR "/db", 0) == -1)
		{
			createDir("./" LOG_DIR "/db");
		}

		if (access("./" LOG_DIR "/audit", 0) == -1)
		{
			createDir("./" LOG_DIR "/audit");
		}
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::RegisterLogPath(int32 eChannel, char* szPath, int8 LogType, BOOL isPrintToConsole, BOOL isUseHoral)
	{
		__GUARD__
		KCheck( eChannel>=0 && eChannel<ALL_LOG_NUMBER );
		uint32 uSize = (uint32)strlen(szPath)+1;
		m_szLogPath[eChannel] = new char[uSize];
		KCheck(m_szLogPath[eChannel]);
		memset(m_szLogPath[eChannel],0,uSize);
		strncpy( m_szLogPath[eChannel], szPath, uSize-1 );
		m_isUploading[eChannel] = FALSE;
		m_LogType[eChannel] = LogType;
		m_isPrintToConsole[eChannel] = isPrintToConsole;
		m_isUseHoral[eChannel] = isUseHoral;

		__UNGUARD__
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::write_log_realtime(int32 eChannel, char *msg, int32 elapsedDay)
	{
		__GUARD__
		if(eChannel < 0 || eChannel >= ALL_LOG_NUMBER) return;

		// 如果正在跨天过程中，则先将日志缓存起来 [7/29/2015 yangzhen]
		if((m_nLastBackUpTime != elapsedDay) && m_isUseBackUPBuff)
		{
			cacheBackupLog(eChannel, msg);
		}
		else
		{
			char szLogPathName[_MAX_PATH] = {0};
			GetChannelNameLocal(eChannel, szLogPathName);

			{
				//开始写日志
				lock_guard<FastLock> guard(m_channelLock[eChannel]);

				_NET_TRY
				{
					//打开日志文件
					FILE *f = NULL;
					f = fopen(szLogPathName, "a+");

					//打开有效,写日志
					if(f)
					{
						_NET_TRY
						{
							fwrite(msg, 1, strlen(msg), f);
						}
						_NET_CATCH
						{
						}

						fclose(f);
					}
				}
				_NET_CATCH
				{
				}
			}
		}

		if(TRUE == __print_log__ && IsPrintToConsole(eChannel)) 
			printf("%s", msg);
		__UNGUARD__
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::cache_log_notime(int32 eChannel, char *szMsg, int32 elapsedDay)
	{
		__GUARD__;
		if(__active_log__ != 1) return;
		if(eChannel < 0 || eChannel >= ALL_LOG_NUMBER) return;

		// 如果正在跨天过程中，则先将日志缓存起来 [7/29/2015 yangzhen]
		if((m_nLastBackUpTime != elapsedDay) && m_isUseBackUPBuff)
		{
			cacheBackupLog(eChannel, szMsg);
		}
		else
		{
			// 只有QUICK_LOG才会在内存中缓存日志 [yangzhen]
			if(NULL == m_pMemoryLog[eChannel] || eQUICK_LOG != m_LogType[eChannel])
			{
				return;
			}

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			int32	iLen = (int32) strlen(szMsg);
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			if(iLen <= 0) return;

			if(m_WritePos[eChannel] > (m_MemorySize - iLen - 4096))
			{
				_NET_TRY
				{
					char	szLogName[_MAX_PATH] = { 0 };
					GetChannelNameLocal(eChannel, szLogName);
					strcat( szLogName, ".discard" );

					FILE	*f = fopen(szLogName, "a+");
					if(f)
					{
						_NET_TRY
						{
							fwrite(szMsg, 1, strlen(szMsg), f);
						}

						_NET_CATCH
						{
						}

						fclose(f);
					}
				}

				_NET_CATCH
				{
				}

				return;
			}
			{
				_NET_TRY
				{
					{
						lock_guard<FastLock> guard(m_channelLock[eChannel]);
						memcpy(m_pMemoryLog[eChannel] + m_WritePos[eChannel], szMsg, iLen);
						m_WritePos[eChannel] += iLen;
					}
				
	//#ifdef __WINDOWS__
	#ifndef __RELEASE_SERVER__
					UpdateNoTime(eChannel);
	#endif	
				}

				_NET_CATCH
				{
				}
			}
		}

		if(m_isPrintToConsole[eChannel])
		{
			showmsg(szMsg);
		}
		
		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::showmsg(const char *cszMsg)
	{
		__GUARD__;
		if(TRUE == __print_log__) printf("%s", cszMsg);
		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::cacheBackupLog(int32 eChannel, char *szMsg)
	{
		__GUARD__
		if(eChannel < 0 || eChannel >= ALL_LOG_NUMBER) return;
		// 只有QUICK_LOG才会在内存中缓存日志 [yangzhen]
		if(NULL == m_pBackupCacheLog[eChannel])
		{
			return;
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		int32	iLen = (int32) strlen(szMsg);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(iLen <= 0) return;

		if(m_BackupLogWritePos[eChannel] > (m_MemorySize - iLen - 4096))
		{
			_NET_TRY
			{
				char	szLogName[_MAX_PATH] = { 0 };
				GetChannelNameLocal(eChannel, szLogName);
				strcat( szLogName, ".discard" );

				FILE	*f = fopen(szLogName, "a+");
				if(f)
				{
					_NET_TRY
					{
						fwrite(szMsg, 1, strlen(szMsg), f);
					}

					_NET_CATCH
					{
					}

					fclose(f);
				}
			}

			_NET_CATCH
			{
			}

			return;
		}
		{
			_NET_TRY
			{
				lock_guard<FastLock> guard(m_channelLock[eChannel]);
				memcpy(m_pBackupCacheLog[eChannel] + m_BackupLogWritePos[eChannel], szMsg, iLen);
				m_BackupLogWritePos[eChannel] += iLen;
			}

			_NET_CATCH
			{
			}
		}
		__UNGUARD__
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::flushCacheBackupLog(int32 eChannel)
	{
		__GUARD__
		if(eQUICK_LOG == m_LogType[eChannel])
		{
			{
				lock_guard<FastLock> guard(m_channelLock[eChannel]);

				// 需要确保前一帧缓存的数据都已经刷到文件中了 [7/29/2015 yangzhen]
				KCheck(m_WritePos[eChannel] == 0);

				memcpy(m_pMemoryLog[eChannel], m_pBackupCacheLog[eChannel], m_MemorySize);
				m_WritePos[eChannel] = m_BackupLogWritePos[eChannel];

				memset(m_pBackupCacheLog[eChannel], 0, m_MemorySize);
				m_BackupLogWritePos[eChannel] = 0;
			}

			UpdateNoTime(eChannel);
		}
		else
		{
			write_log_realtime(eChannel, m_pBackupCacheLog[eChannel], m_nLastBackUpTime);

			memset(m_pBackupCacheLog[eChannel], 0, m_MemorySize);
			m_BackupLogWritePos[eChannel] = 0;
		}

		__UNGUARD__
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	#if __MAPSERVER__
	void QLogger::GetChannelNameRemote(int32 channel, char *szName)
	{
		__GUARD__;
		if(channel < 0 || channel >= ALL_LOG_NUMBER || !szName) return;
		memset(szName, 0, _MAX_PATH);

		tsnprintf
			(
			szName,
			_MAX_PATH - 1,
			"./LogRemoteServer/%d/%s_%.4d-%.2d-%.2d.log",
			g_Config.m_WorldInfo.m_WorldID,
			m_szLogPath[channel],
			GET_TIME().year(),
			GET_TIME().month(),
			GET_TIME().day()
			);
	
		__UNGUARD__;
	}
	#endif
	void QLogger::GetChannelNameLocal(int32 channel, char *szName)
	{
		__GUARD__;
		if(channel < 0 || channel >= ALL_LOG_NUMBER || !szName) return;
		memset(szName, 0, _MAX_PATH);

		if (!m_isUseHoral[channel])
		{
			tsnprintf
				(
				szName,
				_MAX_PATH - 1,
				"./" LOG_DIR "/%s.log",
				m_szLogPath[channel]
				);
		}
		else
		{
			tsnprintf
				(
				szName,
				_MAX_PATH - 1,
				"./" LOG_DIR "/%s_%.4d-%.2d-%.2d_%.2d.log",
				m_szLogPath[channel],
				GET_TIME().year(),
				GET_TIME().month(),
				GET_TIME().day(),
				GET_TIME().hour()
				);
		}
		

		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::UpdateNoTime(int32 channel)
	{
		__GUARD__;

		// 只有QUICK_LOG才会在这里写文件，SLOW_LOG在调用的当时就已经写文件 [yangzhen]
		if(NULL == m_pMemoryLog[channel] || eQUICK_LOG != m_LogType[channel])
		{
			return;
		}

		/*~~~~~~~~~~~~~~~~~~~~~~*/
		char	szName[_MAX_PATH];
		/*~~~~~~~~~~~~~~~~~~~~~~*/

		GetChannelNameLocal(channel, szName);

		/*~~~~~~~~~~~~~~~~~~~~~~*/
		int32	nNeedSaveSize = 0;
		/*~~~~~~~~~~~~~~~~~~~~~~*/
		{
			lock_guard<FastLock> guard(m_channelLock[channel]);

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			char	*pTemp = m_pMemoryLog[channel];
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			// 这里把日志的指针进行了调换，必须清除之前保存日志的指针的数据，否则会出现日志写错的问题 [xingzhishan]
			memset(m_pSaveLog, 0, sizeof(char) * m_MemorySize);
			m_pMemoryLog[channel] = m_pSaveLog;
			m_pSaveLog = pTemp;
			nNeedSaveSize = m_WritePos[channel];
			m_WritePos[channel] = 0;
		}

		_NET_TRY
		{
			if(nNeedSaveSize == 0) return;

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			FILE	*f = fopen(szName, "a+");
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			if(f)
			{
				_NET_TRY
				{
					fwrite(m_pSaveLog, 1, nNeedSaveSize, f);
				}

				_NET_CATCH
				{
				}

				fclose(f);
			}
		}

		_NET_CATCH
		{
		}

		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/

	void QLogger::MoveLog(char* szOldName, int32 nCurYear, int32 nCurMonth, int32 nCurDay, eLOG_BACKUP_TYPE eType)
	{
		uint32 uSize = strlen(szOldName);
		int32 nYear, nMonth, nDay;

		if (!GetFileData(szOldName, nYear, nMonth, nDay))
			return;

		if(eLOG_BACKUP_SERVER_START == eType)
		{
			if (nYear == nCurYear && nMonth == nCurMonth && nDay == nCurDay)
				return;
		}
		// 创建备份的文件夹
		CreateBackUpFolder(nYear, nMonth, nDay);

		// 获取新路径文件名
		char szFileName[MAX_FILE_PATH] = {0};
		GetNewPathFileName(szOldName, szFileName, nYear, nMonth, nDay);
		rename(szOldName, szFileName);
	}

	void QLogger::MoveLog(int32 eChannel, int32 nCurYear, int32 nCurMonth, int32 nCurDay, eLOG_BACKUP_TYPE eType)
	{
		lock_guard<FastLock> guard(m_channelLock[eChannel]);

		int32 nYear, nMonth, nDay;
		char szOldName[_MAX_PATH] = {0};
		GetChannelNameLocal(eChannel, szOldName);
		uint32 uSize = strlen(szOldName);

		if (!GetFileData(szOldName, nYear, nMonth, nDay))
			return;

		if(eLOG_BACKUP_SERVER_START == eType)
		{
			if (nYear == nCurYear && nMonth == nCurMonth && nDay == nCurDay)
				return;
		}

		// 创建备份的文件夹
		CreateBackUpFolder(nYear, nMonth, nDay);

		// 获取新路径文件名
		char szFileName[MAX_FILE_PATH] = {0};
		GetNewPathFileName(szOldName, szFileName, nYear, nMonth, nDay);
		rename(szOldName, szFileName);
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::BackUpLog(eLOG_BACKUP_TYPE eType)
	{
		int32 nHour = GET_TIME().hour();
		int32 nMin = GET_TIME().minute();

		// 跨天备份日志，因为跨天在外层检测到了，所以这里就不需要做检测了
		if (eType == eLOG_BACKUP_BY_TIME)
		{
			
		}
		else if (eType == eLOG_BACKUP_SERVER_START)
		{
			m_nLastBackUpTime = GET_TIME().GetDaysNumEplised();
		}

		int32 nCurYear = GET_TIME().year();
		int32 nCurMonth = GET_TIME().month();
		int32 nCurDay = GET_TIME().day();

		// 备份所有定义了枚举的日志
		for (int32 i = 0; i < ALL_LOG_NUMBER; i++)
		{
			MoveLog(i, nCurYear, nCurMonth, nCurDay, eType);
		}

		// 未加锁移
		{
			MoveLog(("./" LOG_DIR "/kcheck_login.log"), nCurYear, nCurMonth, nCurDay, eType);
			MoveLog(("./" LOG_DIR "/kcheck_map.log"), nCurYear, nCurMonth, nCurDay, eType);
			MoveLog(("./" LOG_DIR "/database.log"), nCurYear, nCurMonth, nCurDay, eType);
		}
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	BOOL QLogger::GetFileData(char* szFileName, int32 &nYear, int32 &nMonth, int32 &nDay)
	{
		nYear = 0;
		nMonth = 0;
		nDay = 0;
#if defined(__WINDOWS__)
		HANDLE hFile;
		hFile = CreateFile(szFileName, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			DWORD dw = GetLastError();
			CloseHandle(hFile);
			return FALSE;
		}
		
		FILETIME ftCreate, ftAccess, ftWrite;
		SYSTEMTIME stUTC, stLocal;
		if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		{
			CloseHandle(hFile);
			return FALSE;
		}

		FileTimeToSystemTime(&ftWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

		nYear = stLocal.wYear;
		nMonth = stLocal.wMonth;
		nDay = stLocal.wDay;

		CloseHandle(hFile);

#elif defined(__LINUX__)
		struct stat buf;
		tm* tim;
		if (lstat(szFileName, &buf) != 0)
		{
			return FALSE;
		}
		tim = localtime(&buf.st_mtime);
		nYear = tim->tm_year + 1900;
		nMonth = tim->tm_mon + 1;
		nDay = tim->tm_mday;
#endif
		return TRUE;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/

	void QLogger::CreateBackUpFolder(int32 nYear, int32 nMonth, int32 nDay)
	{
		//创建备份日志的文件夹
		if (access("./logBackUp", 0) == -1)
		{
			createDir("./logBackUp");
		}
		char	szFolderName[_MAX_PATH];
		char	szDBName[_MAX_PATH];
		char	szAuditName[_MAX_PATH];
		memset(szFolderName, 0, _MAX_PATH);
		memset(szDBName, 0, _MAX_PATH);
		memset(szAuditName, 0, _MAX_PATH);

		tsnprintf
			(
			szFolderName,
			_MAX_PATH,
			"%s/%s_%.4d-%.2d-%.2d",
			"./logBackUp",
			"log",
			nYear,
			nMonth,
			nDay
			);
		if (access(szFolderName, 0) == -1)
		{
			createDir(szFolderName);
		}

		tsnprintf
			(
			szDBName,
			_MAX_PATH,
			"%s/db",
			szFolderName
			);
		if (access(szDBName, 0) == -1)
		{
			createDir(szDBName);
		}

		tsnprintf
			(
			szAuditName,
			_MAX_PATH,
			"%s/audit",
			szFolderName
			);
		if (access(szAuditName, 0) == -1)
		{
			createDir(szAuditName);
		}

	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::GetNewPathFileName(char* szOldName, char *szName, int32 &nYear, int32 &nMonth, int32 &nDay)
	{
		__GUARD__;
		uint32 uSize = strlen(szOldName);
		if (uSize > MAX_FILE_PATH)
		{
			uSize = MAX_FILE_PATH;
		}
		char szTmp[MAX_FILE_PATH];
		memset(szTmp, 0, MAX_FILE_PATH);
		int32 nIndex = 0;
		char* pTmp = szOldName;
		int32 num = 0;
		for (int32 i = 0; i < uSize; i++)
		{
			if (*pTmp == '/')
			{
				nIndex++;

				if (nIndex == 2)
				{
					// 路径取到第二个'/' [xingzhishan]
					memset(szTmp, 0, MAX_FILE_PATH);
					memcpy(szTmp, ++pTmp, uSize - i - 5);
					break;
				}

			}
			pTmp++;
			
		}

		tsnprintf
			(
			szName,
			_MAX_PATH - 1,
			"%s/%s_%04d-%02d-%02d/%s_%04d%02d%02d.log",
			"./logBackUp",
			"log",
			nYear,
			nMonth,
			nDay,
			szTmp,
			nYear,
			nMonth,
			nDay
			);

		return;
		__UNGUARD__;
	}

	void	QLogger::UploadingLog(int32 eChannel, const char * buff )
	{
		__GUARD__
#if __MAPSERVER__
		if( g_Config.m_LogServerInfo.m_bEnable != 1 ) return;
		if (!isUploadingLog(eChannel)) return;
		char FileName[_MAX_PATH] = {0};
		GetChannelNameRemote(eChannel, FileName);

		XRLRemoteLog *pMsg = CREATE_MESSAGE_OBJ(XRLRemoteLog, MESSAGE_XRL_LOG);
		KCheck(pMsg);
		pMsg->SetServerID(g_Config.m_WorldInfo.m_WorldID);
		pMsg->SetFileName(FileName);
		pMsg->SetLogInfo(buff);
		g_pServerConnectorManager_RemoteLog->SendMessage(pMsg, -1);
#endif
		__UNGUARD__ 
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void QLogger::UpdateAll()
	{
		__GUARD__;
		for(int32 _Idx = 0; _Idx < ALL_LOG_NUMBER; _Idx++) 
		{
			UpdateNoTime(_Idx);
		}

		// 因为把备份(m_isUseBackUPBuff)关闭了，所以不要执行备份 [11/3/2016 scw]
#if 0
		int32 elapsedDay = GET_TIME().GetDaysNumEplised();
		if(elapsedDay != m_nLastBackUpTime)
		{
			// 跨天后开始备份昨日日志 [7/29/2015 yangzhen]
			BackUpLog(eLOG_BACKUP_BY_TIME);

			// 将跨天缓存的日志，写入新日志文件 [7/29/2015 yangzhen]
			for(int32 _Idx = 0; _Idx < ALL_LOG_NUMBER; _Idx++) 
			{
				flushCacheBackupLog(_Idx);
			}

			m_nLastBackUpTime = elapsedDay;
		}
#endif

		__UNGUARD__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	BOOL __get_logger_guard_state(void)
	{
		return __logger_guard__.mStatus;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void __open_print_log(BOOL bIsOpen)
	{
		__print_log__ = bIsOpen;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	BOOL __is_open_print_log(void)
	{
		return __print_log__;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	void __open_allinone(BOOL bIsOpen)
	{
		__allinone_log__ = bIsOpen;
	}

	/*
	=======================================================================================================================
	=======================================================================================================================
	*/
	BOOL __is_open_allinone(void)
	{
		return __allinone_log__;
	}




}	/* namespace QLogSystem end . */