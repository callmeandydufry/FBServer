
#ifndef __ModuleComponent_H__
#define __ModuleComponent_H__

#include "TypeDefine.h"
#include "ModulePacket.h"
#include "IntegerHashMap.h"


class ModuleBase;
class PacketReturnCallable;
class PacketProcessFunction;

class ModuleComponent
{
public:
	ModuleComponent();
	virtual ~ModuleComponent();

	virtual int64 getComponentId() = 0;

	virtual void setModule(ModuleBase* module) { mModule = module; }
	virtual ModuleBase* getModule() const { return mModule; }

	virtual void registerModule(ModuleBase* module);
	virtual void unregisterModule();

	virtual BOOL executePacket(DataPacket* packet);
	virtual BOOL executePacketFunction(DataPacket* packet, PacketProcessFunction* processFuncInfo);
	virtual BOOL executeReturnPacket(DataPacket* packet);

	virtual void addExternalReturnCallback(PacketReturnCallable* callback);
	virtual void removeAllReturnCallback();

	virtual void tickComponent(uint32 uTick);

	const DataPacket* getCurrentExecutingPacket() const { return mCurrentExecutingPacket; }

protected:
	ModuleBase* mModule;
	std::vector<PacketReturnCallable*> mOperateCallbacks;
	uint32 mLastCheckOvertime;
	DataPacket* mCurrentExecutingPacket;
};

#endif
