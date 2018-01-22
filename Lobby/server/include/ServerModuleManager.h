
#ifndef __ServerModuleManager_H__
#define __ServerModuleManager_H__

#include "TypeDefine.h"
#include "StringHashMap.h"
#include "BaseTool.h"
#include "ModuleSupport.h"
#include "ModuleConnectorManager.h"
#include "ModuleExportInterface.h"
#include "ModuleManager.h"
#include "CommonModuleStruct.h"
#include "ServerModuleUtils.h"
#include "ServerModuleBase.h"

class ModuleConnectorManager;
class ProcedureProxyModule;
class ServerModuleRouter;

extern BOOL gIsOnlyLocalServer;
extern BOOL gIsEnableModuleMultiThread;

// ģ�鹤����ÿ��ģ���ڷ���������ʱע���Լ��Ĺ����������� [12/7/2017 yz]
class ServerModuleFactory
{
public:
	virtual EServerModuleType getType() const = 0;
	virtual ServerModuleBase* createModule(EServerModuleType type, int32 group) = 0;
	virtual void destroyModule(ModuleBase* module) = 0;
};

class ServerModuleManager : public ModuleManager, public KThread
{
public:
	ServerModuleManager();
	~ServerModuleManager();

	virtual BOOL init(const char* procedureName);
	virtual void clear();

	// �߳�ִ�к��� [11/22/2016 yz]
	virtual void run();

	virtual BOOL registerModule(int32 moduleName, const char* procedureName, BOOL force = FALSE);

	// ע����� [11/24/2016 yz]
	virtual BOOL registerProcedure(const char* procedureName, const char* ip, uint16 port, BOOL force = FALSE);

	void getLocalModuleRegInfos(ModuleRegList& moduleRegList) const;
	void getAllModuleRegInfos(ModuleRegList& moduleRegList) const;

	// ��ͨ���������ô˺�����Э��������ͬ�����ص�ģ���б� [12/5/2017 yz]
	void syncRemoteModuleRegInfo();

	virtual const FixedString<MAX_MODULE_NAME_LEN>& getLocalProcedureName() const { return mMyProcedureName; }

	virtual void startWork();
	virtual void stopWork();
	virtual void pauseWork();
	virtual void resumeWork();
	virtual BOOL waitThreadExit();
	BOOL isWorking() const { return mIsWorking && !mIsPaused; }
	BOOL isAllModuleRegistered() const { return mIsAllModuleRegistered; }

	ModuleConnectorManager* getConnectorManager()
	{
		return mConnectorManager;
	}

	template<typename EXPORT_INTERFACE>
	ExportMethodCallable<EXPORT_INTERFACE> getExportCallable(
		int32 targetModule, int64 objID,
		typename EXPORT_INTERFACE::CALLBACK_CLS* source = NULL,
		PacketSendProxy* sendProxy = NULL)
	{
		// ������һ�������ڼ�飬���ջص��Ķ��������ModuleComponent�������� [11/28/2016 yz]
		static_cast<ModuleComponent*>(source);

		EXPORT_INTERFACE* exportInterface = new EXPORT_INTERFACE;
		static_cast<PacketCallable*>(exportInterface)->setTarget(targetModule, objID);
		if (NULL != source && source->getModule() != NULL)
		{
			static_cast<PacketCallable*>(exportInterface)->setSource(source->getModule()->getModuleName(), source->getComponentId());
			static_cast<PacketCallable*>(exportInterface)->setInvoker(source);
		}
		exportInterface->setSendPacketProxy(sendProxy);

		ExportMethodCallable<EXPORT_INTERFACE> callable(exportInterface);
		return callable;
	}

#define GetExportCallable(CLS) getExportCallable<CLS##ExportCallable>

	virtual uint32 getCurrentTickCount();

	//----------------------------------------------------------------------
	// ģ����Ϣ�ַ��ӿ� [11/23/2016 yz]
	//----------------------------------------------------------------------
	virtual void sendPacket(int32 moduleName, DataPacket* packet);
	virtual void receivePacket(int32 moduleName, DataPacket* packet);
	//----------------------------------------------------------------------
protected:

	//�˳�
	void quit();

	void checkExternalSocket();

	// ����Ҫ���͵��ⲿģ�����Ϣ��װ��socket���ݰ��������ͳ�ȥ [11/23/2016 yz]
	void translatePacket2Socket();


protected:
	ProcedureRegisterInfo mProcedureRegisterInfos[MAX_MODULE_PROCEDURE_COUNT];
	int32 mRegisterProcedureCount;
	TStringHashMap<ProcedureRegisterInfo*> mProcedureMap;


	ModuleRegisterInfo mModuleRegisterInfos[MAX_MODULE_COUNT];
	int32 mRegisterModuleCount;
	TIntegerHashMap<ModuleRegisterInfo*> mModuleMap;

	FastLock mModuleRegisterLock;

	// �����̵����ƣ�ʹ�ô˲���ȥ�����б��в鵽�Լ��ļ����˿� [11/24/2016 yz]
	FixedString<MAX_MODULE_NAME_LEN> mMyProcedureName;
	uint16 mMyListeningPort;

	BOOL mIsWorking;
	BOOL mIsPaused;
	BOOL mIsAllModuleRegistered;

	CTinyTimer mConnectCheckTimer;
	ModuleConnectorManager* mConnectorManager;

	CTinyTimer mCoordinatorSyncTimer;
	ServerModuleRouter* mServerRouter;
	// �������Ƿ����Coordinator [1/13/2018 yz]
	BOOL mIsSelfCoordinator;

	TIntegerHashMap<ServerModuleFactory*> mModuleFactories;
};


#endif
