#ifndef __PLAYER_ARCHIVE_STRUCT_H__
#define __PLAYER_ARCHIVE_STRUCT_H__

#include "PropertyPlayerState.h"

//----------------------------------------------------------------------------------------------
// ���ȫ�����ԣ������������ԡ�������װ���� [12/15/2017 yz]
class PlayerArchive
{
public:
	PlayerArchive();
	~PlayerArchive(){}

	void clear();

	BOOL serialize(DataStream &oStream) const;
	BOOL serialize(DataStream &oStream, BOOL forceAll);

	BOOL unserialize(DataStream &iStream);

	void serializeJson(JsonSerializer* serializer);

public:
	BOOL mIsIgnorePropDirtyFlag;
	// ��һ���״̬ [12/15/2017 yz]
	DBPlayerState mDBState;
};

#endif