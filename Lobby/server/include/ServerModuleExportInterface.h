#ifndef __SERVER_MODULE_EXPORT_INTERFACE_H__
#define __SERVER_MODULE_EXPORT_INTERFACE_H__


#include "TypeDefine.h"
#include "ModuleExportInterface.h"
#include "CommonModuleStruct.h"
#include "ModuleInlineMacros.h"
#include "PlayerArchiveStruct.h"
#include "RegisterPlayerStruct.h"
#include "ServerModuleUtils.h"
#include "SnidAllocArchiveData.h"
#include "ChatDefine.h"

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IServerModuleRouter(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcSyncLocal2RemoteModuleRegList, ModuleRegList, ModuleRegList&, regList) \

CREATE_CLASS_INTERFACE(IServerModuleRouter, EXPORT_DECLARATION_IServerModuleRouter)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IIndexAllocManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM0(CLS, OPERATION, rpcAllocPlayerSnid, uint64) \
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcRequestID, uint64, int32, eType, BOOL, bStart) \

CREATE_CLASS_INTERFACE(IIndexAllocManager, EXPORT_DECLARATION_IIndexAllocManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_ISnidAllocManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcAllocPlayerUniqueSnid,uint64, const char*, szAccount, int64, sessionID) \
DECLARE_EXPORT_METHOD_PARAM0(CLS, OPERATION, rpcGetValidPlayerSnid, uint64) \

CREATE_CLASS_INTERFACE(ISnidAllocManager, EXPORT_DECLARATION_ISnidAllocManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IDBProxyManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcFetchOnlinePlayerArchive, PlayerArchive&, SNID_t, snid) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcCreateNewPlayerArchive, void, RegisterPlayerArchive&, archive) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcSavePlayerArchive, BOOL, PlayerArchive&, archive) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcFetchPlayerNumInSnidRange, int32, int32, serverID, SNID_t, begin, SNID_t, end) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcFetchRegisterPlayerInSnidRange, BatchRegPlayerArchive&, int32, serverID, SNID_t, begin, int32, limit) \

CREATE_CLASS_INTERFACE(IDBProxyManager, EXPORT_DECLARATION_IDBProxyManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IRegisterPlayerManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcRequestPlayerOnline, BOOL, const char*, account, int64, sessionID) \
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcUpdatePlayerOnlineState, BOOL, SNID_t, snid, BOOL, isOnline) \

CREATE_CLASS_INTERFACE(IRegisterPlayerManager, EXPORT_DECLARATION_IRegisterPlayerManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IOnlinePlayerManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcReqeustOnline, BOOL, const char*, account, SNID_t, snid, int64, sessionID) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcRequestOffline, BOOL, SNID_t, snid) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcRequestPlayerExtData, void, SNID_t, snid) \

CREATE_CLASS_INTERFACE(IOnlinePlayerManager, EXPORT_DECLARATION_IOnlinePlayerManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IClientGateManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcCreateRobotNewSession, uint64, const char*, account, const char*, token)\
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcS2CSynWorldChatData, uint64,const char*, szMsg, uint32, chatID, SNID_t, u64Sender)\

CREATE_CLASS_INTERFACE(IClientGateManager, EXPORT_DECLARATION_IClientGateManager)
//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IOnlinePlayer(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcRequestPlayerDetialData, void, uint64, nSessionID) \
DECLARE_EXPORT_METHOD_PARAM0(CLS, OPERATION, rpcSessionCheckPlayerIsOnline, BOOL) \

CREATE_CLASS_INTERFACE(IOnlinePlayer, EXPORT_DECLARATION_IOnlinePlayer)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IClientSession(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcNotifyPlatAuthResult, void, BOOL, success) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcNotifyPlayerOnline, void, SNID_t, snid) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcNotifyPlayerOffline, void, SNID_t, snid) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcS2CSynPlayerExtData, void, DBPlayerState&, stState) \
DECLARE_EXPORT_METHOD_PARAM4(CLS, OPERATION, rpcS2CSynPlayerChatData, void, const char*, szMsg, int32, nChatID, int32, nChatChanel,SNID_t, snid) \
DECLARE_EXPORT_METHOD_PARAM0(CLS, OPERATION, rpcRobotRequestLogon, void) \
DECLARE_EXPORT_METHOD_PARAM0(CLS, OPERATION, rpcPlayerCheckSessionIsOnline, BOOL) \


CREATE_CLASS_INTERFACE(IClientSession, EXPORT_DECLARATION_IClientSession)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IChatManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcHandlePlayerChatMsg, BOOL, tagChatMsgDealUtil&, tagMsgUtil) \

CREATE_CLASS_INTERFACE(IChatManager, EXPORT_DECLARATION_IChatManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IRobotManager(CLS, OPERATION)	\

CREATE_CLASS_INTERFACE(IRobotManager, EXPORT_DECLARATION_IRobotManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IRobotData(CLS, OPERATION)	\

CREATE_CLASS_INTERFACE(IRobotData, EXPORT_DECLARATION_IRobotData)

//----------------------------------------------------------------------------

//
#define REGISTER_SERVER_MODULE_EXPORT_METHODS() \
	IServerModuleRouter::registerFunctions(); \
	IIndexAllocManager::registerFunctions(); \
	IDBProxyManager::registerFunctions();	\
	ISnidAllocManager::registerFunctions(); \
	IRegisterPlayerManager::registerFunctions(); \
	IOnlinePlayerManager::registerFunctions();	\
	IClientGateManager::registerFunctions();	\
	IOnlinePlayer::registerFunctions();	\
	IClientSession::registerFunctions(); \
	IChatManager::registerFunctions();\
	

#endif
