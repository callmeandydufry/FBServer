/*********************************************************************************************
created:	16/11/25
filename: 	MapServer.h
file base:
file ext:	cpp
author: yangzhen
purpose:	
**********************************************************************************************/

#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "TypeDefine.h"

class GameServer
{
public:
	GameServer();
	~GameServer();

	BOOL	InitServer(const char* szProcName);

	BOOL	Loop();

	BOOL	ExitServer();

	BOOL	LaunchModules();

	BOOL	BuildModules(const char* szProcName);

	void	Stop();

public:

};

extern GameServer	g_GameServer;

#endif
