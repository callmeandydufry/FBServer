#include "stdafx.h"
#include "RegisterPlayerStruct.h"

//----------------------------------------------------------------------------------------------
void RegisterPlayerArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBPlayerState", mDBState, (int32)sizeof(mDBState));
}

//----------------------------------------------------------------------------------------------
void BatchRegisterPlayerArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("BatchNum", mBatchNum, (int32)sizeof(mBatchNum));
	serializer->serialize("BatchPlayerData", mBatchRegisterPlayerData, (int32)sizeof(mBatchRegisterPlayerData));
}