#ifndef __SHARE_STRUCT_DEFINE_H__
#define __SHARE_STRUCT_DEFINE_H__

#include "SharedTypes.h"
#include "PacketDataStream.h"
#include "FixedString.h"

NAMESPACE_LOBBY_BEGIN

#define STRUCT_SHARESTRUCT_MEMBER(TYPE, NAME, DEFAULTVAL)	\
	TYPE m##NAME;

#define STRUCT_SHARESTRUCT_RESETDEFAULT(TYPE, NAME, DEFAULTVAL)	\
	m##NAME = DEFAULTVAL;

#define STRUCT_SHARESTRUCT_SET_GET(TYPE, NAME, DEFAULTVAL)	\
	void set##NAME(const TYPE& val)	\
	{\
		m##NAME = val;\
	}\
	const TYPE& get##NAME() const\
	{\
		return m##NAME;\
	}\

#define STRUCT_SHARESTRUCT_SERIALIZE(TYPE, NAME, DEFAULTVAL)	\
	oStream.pushValue(m##NAME);

#define STRUCT_SHARESTRUCT_UNSERIALIZE(TYPE, NAME, DEFAULTVAL)	\
	iStream.getValue(m##NAME);


#define STRUCT_SHARESTRUCT_LIST(PROP_LIST)	\
	PROP_LIST(STRUCT_SHARESTRUCT_MEMBER)\
	public:\
	PROP_LIST(STRUCT_SHARESTRUCT_SET_GET)\
	BOOL serialize(DataStream &oStream) const\
	{\
		PROP_LIST(STRUCT_SHARESTRUCT_SERIALIZE)\
		return TRUE;\
	}\
	BOOL unserialize(DataStream &iStream)\
	{\
		PROP_LIST(STRUCT_SHARESTRUCT_UNSERIALIZE)\
		return TRUE;\
	}\
	void clear()\
	{\
		PROP_LIST(STRUCT_SHARESTRUCT_RESETDEFAULT)\
	}\

NAMESPACE_LOBBY_END


#endif // __SHARE_STRUCT_DEFINE_H__