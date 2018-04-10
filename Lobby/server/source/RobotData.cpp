#include "stdafx.h"
#include "RobotData.h"

int64 RobotData::getComponentId()
{
	return m64PlayerID;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void RobotData::tickComponent(uint32 uTick)
{	

	switch (mStatus)
	{
	case ERobotStatus_INVALID:
		break;
	case ERobotStatus_Inited:
		break;
	case ERobotStatus_StableOnline:
		break;
	case ERobotStatus_UnStableOnline:
		// todo 每5秒请求离线
		break;
	case ERobotStatus_StableOffline:
		break;
	case ERobotStatus_UnStableOffline:
		// todo 每5秒请求重新连接，并上线，基于clientconnector并走完整登录流程的情况
		break;
	case ERobotStatus_Talk2Others:
		break;
	case ERobotStatus_TalkInTheWorld:
		break;
	default:
		break;
	}
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void RobotData::Clear()
{
	m64PlayerID = INVALID_SNID;
	m64Session = INVALID_ID;
	mAccountName.clear();
	mToken.clear();
}

void RobotData::setAccountName(FixedString<MAX_ACCOUNT>& strName)
{
	mAccountName = strName;
}

FixedString<MAX_ACCOUNT>& RobotData::getAccountName()
{
	return mAccountName;
}

void RobotData::setToken(FixedString<MAX_ACCOUNT>& strToken)
{
	mToken = strToken;
}

FixedString<MAX_ACCOUNT>& RobotData::getToken()
{
	return mToken;
}

void RobotData::setPlayerID(SNID_t snid)
{
	m64PlayerID = snid;
}

SNID_t RobotData::getPlayerID()
{
	return m64PlayerID;
}

void RobotData::setSession(uint64 nSessionID)
{
	mStatus = ERobotStatus_Inited;
	m64Session = nSessionID;
}

uint64 RobotData::getSession()
{
	return m64Session;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------