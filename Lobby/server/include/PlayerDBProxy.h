#ifndef __PLAYER_DBPROXY_H__
#define __PLAYER_DBPROXY_H__

#include "TypeDefine.h"

class PlayerArchive;
class RegisterPlayerArchive;
class BatchRegPlayerArchive;
class DBProxyManager;

class PlayerDBProxy
{
public:
	PlayerDBProxy(DBProxyManager* mgr)
		: mDBManager(mgr)
	{}

	// ��������� [12/15/2017 yz]
	void doCreateNewPlayerArchive(RegisterPlayerArchive* archive);

	// �������ȫ�����ݵ�DB [12/15/2017 yz]
	BOOL doSavePlayerArchive(PlayerArchive* archive);

	// ͨ��SNID��ѯ������� [12/15/2017 yz]
	BOOL doFetchPlayerArchiveBySnid(SNID_t snid, PlayerArchive* archive);

	// ��ѯ����SNID�����ڵ�������� [1/2/2018 yz]
	BOOL doFetchPlayerNumInSnidRange(int32 serverID, SNID_t begin, SNID_t end, int32& playerNum);

	// ��ȡ���������ڶ����ҵ�ע��������� [1/2/2018 yz]
	BOOL doFetchRegisterPlayerInSnidRange(int32 serverID, SNID_t begin, int32 limit, BatchRegPlayerArchive* batchPlayerData);

protected:
	DBProxyManager*					mDBManager;

	DBModelPlayerState				mDBModelPlayerState;
	DBModelRegisterPlayerState		mDBModelRegisterPlayerState;
};

#endif