#ifndef __PLAYER_ARCHIVE_STRUCT_H__
#define __PLAYER_ARCHIVE_STRUCT_H__

#include "PropertyPlayerState.h"

//----------------------------------------------------------------------------------------------
// 玩家全部属性，包括基本属性、背包、装备等 [12/15/2017 yz]
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
	// 玩家基本状态 [12/15/2017 yz]
	DBPlayerState mDBState;
};

#endif