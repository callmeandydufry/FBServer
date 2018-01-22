
#ifndef __ModuleSupprot_H__
#define __ModuleSupprot_H__

#include "TypeDefine.h"
#include "ModuleDefine.h"
#include "ModulePacket.h"
#include "IntegerHashMap.h"
#include "ModuleComponent.h"

class ModuleBase
{
public:
	ModuleBase();
	~ModuleBase();

	virtual BOOL initModule(int32 moduleName, ModuleComponent* defaultComponent);
	virtual void uinitModule();

	virtual void tickModule(uint32 uTick);

	virtual void recvPacket(DataPacket* packet);

	virtual void registerModuleComponent(ModuleComponent* component);
	virtual void unregisterModuleComponent(ModuleComponent* component);
	virtual ModuleComponent* getComponent(int64 componentId);

	int32 getModuleName() const { return mModuleName; }

	// 是否这个模块内所有事件都处理完成 [12/16/2017 yz]
	virtual BOOL isAllWordDone() { return FALSE; }

protected:
	virtual void processPackets();

	int32 mModuleName;

	// 从外面发送到模块中的消息，接收消息队列 [11/22/2016 yz]
	DataPacketQueue mRecvPacketQueue;

	// 为了兼容Player的SNID，HashMap的key使用了64位 [11/22/2016 yz]
	typedef HASH_MAP<int64, ModuleComponent*> ComponentMap;
	ComponentMap mComponents;

	// 定义一个默认的接口，当消息中未指定TargetObjID的时候在这个接口上执行 [11/28/2016 yz]
	ModuleComponent* mDefaultComponent;
};

#endif
