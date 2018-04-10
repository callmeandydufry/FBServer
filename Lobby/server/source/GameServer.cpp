/*********************************************************************************************
created:	16/11/25
filename: 	
file base:
file ext:	cpp
author: yangzhen
purpose:	战场服务器启动管理器
**********************************************************************************************/

#include "stdafx.h"
#include "ExceptionHandler.h"
#include "GameServerMainThread.h"
#include "ServerModuleManager.h"
#include "ServerModuleThreadContainer.h"
#include "GameServer.h"
#include "Version.h"
#include "PrivateTest.h"

int32	g_Command_CommandDeny = FALSE;
int32	g_Command_Enable = FALSE;

int32	g_UnSavedDataCount = 0;
GameServer g_GameServer;

int32 main(int32 argc, char *argv[])
{
// 	PrivateTt::ThreadTest testThread;
// 	testThread.CreateMyThread();
// 	WaitForMultipleObjects(10, testThread.mThreadHand, TRUE, INFINITE);

	PrivateTt::testVirtualDestructor tVD;
	tVD.test();


	//#if defined(__WINDOWS__)
#ifdef __WINDOWS__
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif
#if defined __LINUX__
#endif

	// 注册异常处理函数 [yangzhen]
	ExceptionHandler _ExceptionHandle;
	const char* szProcName = NULL;

	__GUARD__;

	if (argc > 1)
	{
		for (int _Idx = 1; _Idx < argc; _Idx++)
		{
			if (strcmp(argv[_Idx], "-ignoreassert") == 0)
			{
				IgnoreKCheck();
			}
			else if (strcmp(argv[_Idx], "-retryassert") == 0)
			{
				RetryKCheck();
			}
			else if (strcmp(argv[_Idx], "-ignoremessagebox") == 0)
			{
				IgnoreMessage();
			}
			else if (strcmp(argv[_Idx], "-commanddeny") == 0)
			{
				g_Command_CommandDeny = TRUE;
			}
			else if (strcmp(argv[_Idx], "-commandenable") == 0)
			{
				g_Command_Enable = TRUE;
			}
			else if (strcmp(argv[_Idx], "-onelog") == 0)
			{
				QLOGGER_ALLINONE_OPEN();
			}
			else if (tstricmp(argv[_Idx], "-noprintlog") == 0)
			{
				QLOGGER_PRINTLOG_CLOSE();
			}
			else if (strcmp(argv[_Idx], "-procedure") == 0)
			{
				szProcName = argv[_Idx + 1];
				_Idx++;
			}
			else if (strcmp(argv[_Idx], "-localserver") == 0)
			{
				gIsOnlyLocalServer = TRUE;
			}
			else if (strcmp(argv[_Idx], "-singlethread") == 0)
			{
				gIsEnableModuleMultiThread = FALSE;
			}
		}
	}

	BOOL ret;
	
	INIT_TIMESYSTEM();
	ret = QLOGGER_INITIALIZE();
	KCheck(ret);

	GET_TIME().Update();

	// 检查日志备份 [wangxiaoyong]
	QLOGGER_SYSTEM_MOVELOG(eLOG_BACKUP_SERVER_START);

	QLogSystem::QUICK_LOG
		(
		SLOW_LOG_SERVER,
		"GameServer Starting... (%.10d)(%d)",
		GET_TIME().Time2DWORD(),
		GET_TIME().OrigineTime()
		);

	srand(GET_TIME().GetCTime());

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "\n Current VERSION %x\n\n", GET_CURRENT_VERSION());
	
	ret = g_GameServer.InitServer(szProcName);
	KCheck(ret);

	ret = g_GameServer.Loop();
	KCheck(ret);

	ret = g_GameServer.ExitServer();
	KCheck(ret);

	return 0;

	__UNGUARD__;
	return 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
GameServer::GameServer()
{
	__GUARD__
#if defined(__WINDOWS__)
	uint16 wVersionRequested;

	/*~~~~~~~~~~~~*/
	WSADATA wsaData;
	int32	err;
	/*~~~~~~~~~~~~*/

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
#endif

	__UNGUARD__
}

/*
=======================================================================================================================
=======================================================================================================================
*/
GameServer::~GameServer()
{
	__GUARD__
#if defined(__WINDOWS__)
		WSACleanup();
#endif
	__UNGUARD__
}

/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL GameServer::InitServer(const char* szProcName)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ BOOL	ret = FALSE;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "InitServer PROCNAME=%s FD_SETSIZE=%d...", szProcName, FD_SETSIZE);

	ret = g_Config.InitLog(SLOW_LOG_SERVER);
	KCheck(ret);
	ret = g_Config.Init();
	KCheck(ret);

	ret = LaunchModules();
	KCheck(ret);

	ret = BuildModules(szProcName);
	KCheck(ret);

	__UNGUARD__ return TRUE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL GameServer::Loop()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ BOOL	ret = FALSE;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "\r\nLoop...");

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	FILE	*fPid = fopen("gameserver.pid", "w+");
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (fPid)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		uint32	pid = MyGetCurrentPID();
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		fprintf(fPid, "%u", (uint32)pid);
		fclose(fPid);
	}

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "g_pGuardThread->Loop( )...");
	g_pGameServerMainThread->Loop();

	__UNGUARD__ return TRUE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL GameServer::ExitServer()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "\r\nExitServer...");
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	// 通知各个模块进行退出处理 [11/25/2016 yz]

	if (g_pModuleThreadContainer)
	{
		delete g_pModuleThreadContainer;
		g_pModuleThreadContainer = NULL;
	}
	
	if (g_pModuleManager)
	{
		delete g_pModuleManager;
		g_pModuleManager = NULL;
	}

	if (g_pGameServerMainThread)
	{
		delete g_pGameServerMainThread;
		g_pGameServerMainThread = NULL;
	}

	// 服务器关闭，写日志
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "\nQUIT: ExitServer...OK");

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL GameServer::LaunchModules()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ BOOL	ret = FALSE;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "\r\nNewStaticServer( )...OK");

	//守护线程(假的)
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[New] Begin new g_pGuardThread ... ");
	g_pGameServerMainThread = new GameServerMainThread;
	KCheck(g_pGameServerMainThread);
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[New] new g_pGuardThread OK ! ");

	// 模块管理器 [11/25/2016 yz]
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[New] Begin new g_pModuleManager ... ");
	g_pModuleManager = new ServerModuleManager();
	KCheck(g_pModuleManager);
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[New] new g_pModuleManager OK ! ");

	// 模块执行线程管理器 [11/25/2016 yz]
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[New] Begin new g_pModuleThreadContainer ... ");
	g_pModuleThreadContainer = new ServerModuleThreadContainer();
	KCheck(g_pModuleThreadContainer);
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[New] new g_pModuleThreadContainer OK ! ");
	
	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void GameServer::Stop()
{
	__GUARD__

	if (g_pGameServerMainThread)
	{
		g_pGameServerMainThread->Stop();
	}

	QLOGGER_UPDATE();

	__UNGUARD__
}

//对各种管理器的初始化 具体的管理器请参考上文 
BOOL GameServer::BuildModules(const char* szProcName)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ BOOL	ret = FALSE;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32 abc = 0;

	//std::cin >> abc;

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "\r\nInitStaticServer( )...OK");

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] Begin INIT_MESSAGE_FACTORY() ... ");
	ret = INIT_MESSAGE_FACTORY();
	KCheck(ret);
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] INIT_MESSAGE_FACTORY() OK ! ");

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] Begin g_pGameServerMainThread->Init() ... ");
	ret = g_pGameServerMainThread->Init();
	KCheck(ret);
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] g_pGameServerMainThread->Init() OK ! ");

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] Begin g_pModuleThreadContainer->Init() ... ");
	ret = g_pModuleThreadContainer->Init();
	KCheck(ret);
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] g_pModuleThreadContainer->Init() OK ! ");

	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] Begin g_pModuleManager->Init() ... ");
	ret = g_pModuleManager->init(szProcName);
	KCheck(ret);
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Init] g_pModuleManager->Init() OK ! ");

	return TRUE;

	__UNGUARD__ return FALSE;
}
