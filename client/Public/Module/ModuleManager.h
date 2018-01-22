
#ifndef __ModuleManager_H__
#define __ModuleManager_H__

#include "TypeDefine.h"
#include "StringHashMap.h"
#include "BaseTool.h"
#include "ModuleSupport.h"
#include "ModuleConnectorManager.h"
#include "ModuleExportInterface.h"


class ModuleManager
{
public:
	ModuleManager();
	~ModuleManager();

	virtual BOOL init(const char* procedureName);
	virtual void clear();

	virtual BOOL registerModule(int32 moduleName, const char* procedureName, BOOL force = FALSE);
	virtual BOOL registerLocalModule(int32 moduleName, ModuleBase* modulePtr, BOOL force = FALSE);
	virtual BOOL unregisterLocalModule(int32 moduleName, BOOL force = FALSE);

	// 注册进程 [11/24/2016 yz]
	virtual BOOL registerProcedure(const char* procedureName, const char* ip, uint16 port, BOOL force = FALSE);

	virtual const FixedString<MAX_MODULE_NAME_LEN>& getLocalProcedureName() const
	{
		static FixedString<MAX_MODULE_NAME_LEN> EMPTY;
		return EMPTY;
	}

	virtual void startWork();
	virtual void stopWork();
	virtual void pauseWork();
	virtual void resumeWork();
	virtual BOOL waitThreadExit();

	template<typename EXPORT_INTERFACE>
	ExportMethodCallable<EXPORT_INTERFACE> getExportCallable(
		int32 targetModule, int64 objID,
		typename EXPORT_INTERFACE::CALLBACK_CLS* source = NULL,
		PacketSendProxy* sendProxy = NULL)
	{
		// 这里做一个编译期检查，接收回调的对象必须是ModuleComponent的派生类 [11/28/2016 yz]
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

	virtual uint32 getCurrentTickCount() { return 0; }

	//----------------------------------------------------------------------
	// 模块消息分发接口 [11/23/2016 yz]
	//----------------------------------------------------------------------
	virtual void sendPacket(int32 moduleName, DataPacket* packet);
	virtual void receivePacket(int32 moduleName, DataPacket* packet);
	//----------------------------------------------------------------------

protected:

};

extern ModuleManager* g_pModuleManager;

#endif
