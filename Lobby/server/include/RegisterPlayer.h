#ifndef __REGISTER_PLAYER_H__
#define __REGISTER_PLAYER_H__

#include "TypeDefine.h"
#include "RegisterPlayerStruct.h"

class RegisterPlayer
{
public:
	RegisterPlayer(){}
	~RegisterPlayer(){}

	DBRegisterPlayerState& getData()
	{
		return mPlayerArchive.mDBState;
	}

	void initDB(DBRegisterPlayerState& dbArchive)
	{
		mPlayerArchive.mDBState = dbArchive;
		mIsOnline = FALSE;
	}

	void clear()
	{
		mPlayerArchive.clear();
		mIsOnline = FALSE;
	}

	BOOL isOnline() const
	{
		return mIsOnline;
	}

	void setOnline(BOOL online)
	{
		mIsOnline = online;
	}

	SNID_t getSnid()
	{
		return mPlayerArchive.mDBState.getSnid();
	}

	const char* getAccount()
	{
		return mPlayerArchive.mDBState.getAccountName().c_str();
	}

protected:
	RegisterPlayerArchive mPlayerArchive;

	BOOL mIsOnline;
};

#endif