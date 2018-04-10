#ifndef __REGISTER_PLAYER_H__
#define __REGISTER_PLAYER_H__

#include "TypeDefine.h"
#include "RegisterPlayerStruct.h"
#include "GameMail.h"
#include "MailDefine.h"
#include <queue>

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
		if (!mUnReadMailIDQueue.empty())
		{
			for (int32 i = 0; i < mUnReadMailIDQueue.size(); ++i)
				mUnReadMailIDQueue.pop();
		}
	}

	void clear()
	{
		mPlayerArchive.clear();
		mIsOnline = FALSE;
		if (!mUnReadMailIDQueue.empty())
		{
			for (int32 i = 0; i < mUnReadMailIDQueue.size(); ++i)
				mUnReadMailIDQueue.pop();
		}
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

	int32 getUnReadMailNums()
	{
		return mUnReadMailIDQueue.size();
	}

protected:
	RegisterPlayerArchive	mPlayerArchive;

	BOOL					mIsOnline;

	// 玩家离线时接受到的邮件 [1/29/2018 Chief]
	std::queue<int32>		mUnReadMailIDQueue;
};

#endif