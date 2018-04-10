#include "stdafx.h"
#include "GameMailStruct.h"


void BaseMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBBaseMailState", mDBState, (int32)sizeof(mDBState));
}

void AttachmentMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBAttachmentMailState", mDBState, (int32)sizeof(mDBState));
}
void ContentMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBContentMailState", mDBState, (int32)sizeof(mDBState));
}
void SystemMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("DBSystemMailState", mDBState, (int32)sizeof(mDBState));
}

void BatchBaseMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("BatchNum", mBatchNum, (int32)sizeof(mBatchNum));
	serializer->serialize("BatchBaseMailData", mBatchBaseMailData, (int32)sizeof(mBatchBaseMailData));
}

void BatchAttachmentMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("BatchNum", mBatchNum, (int32)sizeof(mBatchNum));
	serializer->serialize("BatchAttachmentMailData", mBatchAttachmentMailData, (int32)sizeof(mBatchAttachmentMailData));
}

void BatchContentMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("BatchNum", mBatchNum, (int32)sizeof(mBatchNum));
	serializer->serialize("BatchContentMailData", mBatchContentMailData, (int32)sizeof(mBatchContentMailData));
}

void BatchSystemMailArchive::serializeJson(JsonSerializer* serializer)
{
	serializer->serialize("BatchNum", mBatchNum, (int32)sizeof(mBatchNum));
	serializer->serialize("BatchSystemMailData", mBatchSystemMailData, (int32)sizeof(mBatchSystemMailData));
}