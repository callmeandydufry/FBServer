
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

	// �Ƿ����ģ���������¼���������� [12/16/2017 yz]
	virtual BOOL isAllWordDone() { return FALSE; }

protected:
	virtual void processPackets();

	int32 mModuleName;

	// �����淢�͵�ģ���е���Ϣ��������Ϣ���� [11/22/2016 yz]
	DataPacketQueue mRecvPacketQueue;

	// Ϊ�˼���Player��SNID��HashMap��keyʹ����64λ [11/22/2016 yz]
	typedef HASH_MAP<int64, ModuleComponent*> ComponentMap;
	ComponentMap mComponents;

	// ����һ��Ĭ�ϵĽӿڣ�����Ϣ��δָ��TargetObjID��ʱ��������ӿ���ִ�� [11/28/2016 yz]
	ModuleComponent* mDefaultComponent;
};

#endif
