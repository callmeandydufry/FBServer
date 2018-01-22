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