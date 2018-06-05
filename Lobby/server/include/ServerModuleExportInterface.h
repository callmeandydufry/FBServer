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
#include "MailDefine.h"
#include "GameMailStruct.h"

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
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcFetchRegisterPlayerInSnidRange, BatchRegisterPlayerArchive&, int32, serverID, SNID_t, begin, int32, limit) \
/*ÓÊ¼þ*/\
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcRequestAllMailsNum, tagMailsNum&, int32, nGroup) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcFetchBaseMailData, BatchBaseMailArchive&, int32, nStartNum, int32, nSelectNum, int32, nGroup) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcFetchAttachmentMailData, BatchAttachmentMailArchive&, int32, nStartNum, int32, nSelectNum, int32, nGroup) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcFetchContentMailData, BatchContentMailArchive&, int32, nStartNum, int32, nSelectNum, int32, nGroup) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcFetchSystemMailData, BatchSystemMailArchive&, int32, nStartNum, int32, nSelectNum, int32, nGroup) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcOperatorBaseMailData, BOOL, BaseMailArchive&, stArchive, int32, nGroup, int32, nOper) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcOperatorAttachmentMailData, void, AttachmentMailArchive&, stArchive, int32, nGroup, int32, nOper) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcOperatorContentMailData, void, ContentMailArchive&, stArchive, int32, nGroup, int32, nOper) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcOperatorSystemMailData, void, SystemMailArchive&,  stArchive, int32, nGroup, int32, nOper) \
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcFetchGlobalVal, int32, int32,  nGroup, int32, eType) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcSaveGlobalVal, void, int32,  nGroup, int32, eType, int32, nVal) \

CREATE_CLASS_INTERFACE(IDBProxyManager, EXPORT_DECLARATION_IDBProxyManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IRegisterPlayerManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcRequestPlayerOnline, BOOL, const char*, account, int64, sessionID) \
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcUpdatePlayerOnlineState, BOOL, SNID_t, snid, BOOL, isOnline) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcRequestUnreadMailNums, int32, SNID_t, PlayerID) \

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
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcRequestReadMail, void, int32, nMailID) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcMgrSynMailList2Player, void, tagMailConfirm&, stListInfo) \
DECLARE_EXPORT_METHOD_PARAM0(CLS, OPERATION, rpcRequestPlayerMailList, void) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcRequestDelMail, void, int32, nMailID) \

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
/*mail*/ \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcMgrSynMailList2Session, void, tagSendMailListInfo&, stSendList) \
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcMgrSynMailDetail2Session, void, tagSendMailDetail&, stSendMainDetail, tagSendMailAttachMent&, stAttach) \

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
#define EXPORT_DECLARATION_IMailManager(CLS, OPERATION)	\
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcSendAttachmentMail, BOOL, DBBaseMailState&, stBaseMail,tagSendMailAttachMent&, stAttachMent) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcSendContentMail, BOOL, DBBaseMailState&, stBaseMail, FixedString<DB_TITLE_LEN>&, title, FixedString<DB_CONTENT_LEN>&, content) \
DECLARE_EXPORT_METHOD_PARAM4(CLS, OPERATION, rpcSendSystemMail, BOOL, DBBaseMailState&, stBaseMail, int32, nServerID, int32, nMaxLv, int32, nMinLv) \
DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, rpcSynMailList, void, SNID_t, snid) \
DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, rpcRequestAllPlayerMailsData, void, SNID_t, snid, tagMailConfirm&, stMaillistInfo, uint64, nSessionID) \
DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, rpcRequestMailDetail, void, int32, nMailID, uint64, nSessionID) \

CREATE_CLASS_INTERFACE(IMailManager, EXPORT_DECLARATION_IMailManager)

//----------------------------------------------------------------------------
#define EXPORT_DECLARATION_IRoomMgr(CLS, OPERATION)	\

CREATE_CLASS_INTERFACE(IRoomMgr, EXPORT_DECLARATION_IRoomMgr)

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
	IMailManager::registerFunctions();\
	

#endif
