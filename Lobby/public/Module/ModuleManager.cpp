#include "stdafx.h"
#include "ModuleManager.h"
#include "ModuleConnectorManager.h"
#include "ModulePacketMessage.h"


ModuleManager* g_pModuleManager = NULL;

ModuleManager::ModuleManager()
{

}

ModuleManager::~ModuleManager()
{
}

BOOL ModuleManager::init(const char* procedureName)
{
	__GUARD__


		__UNGUARD__
		return TRUE;
}

void ModuleManager::clear()
{

}

BOOL ModuleManager::registerModule(int32 moduleName, const char* procedureName, BOOL force)
{
	return FALSE;
}

BOOL ModuleManager::registerLocalModule(int32 moduleName, ModuleBase* modulePtr, BOOL force)
{
	return FALSE;
}

BOOL ModuleManager::unregisterLocalModule(int32 moduleName, BOOL force)
{
	return FALSE;
}

BOOL ModuleManager::registerProcedure(const char* procedureName, const char* ip, uint16 port, BOOL force)
{
	return FALSE;
}

void ModuleManager::startWork()
{

}

void ModuleManager::stopWork()
{

}

void ModuleManager::pauseWork()
{

}

void ModuleManager::resumeWork()
{

}

BOOL ModuleManager::waitThreadExit()
{
	return TRUE;
}

void ModuleManager::sendPacket(int32 moduleName, DataPacket* packet)
{

}

void ModuleManager::receivePacket(int32 moduleName, DataPacket* packet)
{

}