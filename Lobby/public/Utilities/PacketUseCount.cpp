/*$T Common/Utilities/MessageUseCount.cpp GC 1.140 10/10/07 10:06:57 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "PacketUseCount.h"

MessageUseCount *MessageUseCount::	m_pInstance = NULL;
const int32				MESSAGE_COUNT_MARK = 100000;

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void MessageUseCount::Reset()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	memset(m_aMessageInfo, 0, sizeof(m_aMessageInfo));
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_iMapID = INVALID_KHAN_ID;
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
MessageUseCount::MessageUseCount()
{
	Reset();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
MessageUseCount::~MessageUseCount()
{
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageUseCount::SetActive(ut_id_map iSceneID)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ lock_guard<FastLock>	autolock(m_Lock);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Reset();
	m_iMapID = iSceneID;
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageUseCount::SaveInfoToLogFile(int32 iType)
{
	__GUARD__ if(INVALID_KHAN_ID == m_iMapID)
	{
		return;
	}

	lock_guard<FastLock> autolock(m_Lock);

	Sort(iType);
	for(int32 _Idx = 0; _Idx < MESSAGE_MAX; ++_Idx)
	{
		if(0 >= m_aMessageInfoSort[_Idx].iMessageSendCount)
		{
			continue;
		}

		QLogSystem::QUICK_LOG
			(
				QUICK_LOG_DEBUG,
				"PacketUseCount packetID=%d, packetCount=%d, size=%d, sizeCount=%d",
				m_aMessageInfoSort[_Idx].iMessageID + MESSAGE_COUNT_MARK,
				m_aMessageInfoSort[_Idx].iMessageSendCount + MESSAGE_COUNT_MARK,
				m_aMessageInfoSort[_Idx].iMessageSizeCount /
					m_aMessageInfoSort[_Idx].iMessageSendCount +
					MESSAGE_COUNT_MARK,
				m_aMessageInfoSort[_Idx].iMessageSizeCount
			);
	}

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageUseCount::CacheInfomation(int32 iSize, int32 iMessageID, ut_id_map iSceneID)
{
	__GUARD__ if(INVALID_KHAN_ID == m_iMapID)
	{
		return;
	}

	if(iSceneID != m_iMapID)
	{
		return;
	}

	if(MESSAGE_NONE >= iMessageID || MESSAGE_MAX <= iMessageID)
	{
		return;
	}

	m_aMessageInfo[iMessageID].iMessageSendCount++;
	m_aMessageInfo[iMessageID].iMessageSizeCount += iSize;
	m_aMessageInfo[iMessageID].iMessageID = iMessageID;
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
MessageUseCount *MessageUseCount::GetInstance()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	KCheck(NULL != m_pInstance);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return m_pInstance;
	__UNGUARD__ return NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageUseCount::CreateInstance()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	KCheck(NULL == m_pInstance);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_pInstance = new MessageUseCount;
	KCheck(NULL != m_pInstance);
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageUseCount::DeleteInstance()
{
	__GUARD__ if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void MessageUseCount::Sort(int32 iType)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	memcpy(m_aMessageInfoSort, m_aMessageInfo, sizeof(m_aMessageInfoSort));
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(PUC_SORT_SEND_COUNT == iType)
	{
		qsort
		(
			m_aMessageInfoSort,
			MESSAGE_MAX,
			sizeof(MessageInfomation),
			(int32(*) (const void *, const void *)) CompareBySendCount
		);
		return;
	}
	else if(PUC_SORT_SIZE_COUNT == iType)
	{
		qsort
		(
			m_aMessageInfoSort,
			MESSAGE_MAX,
			sizeof(MessageInfomation),
			(int32(*) (const void *, const void *)) CompareBySizeCount
		);
	}

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 MessageUseCount::CompareBySendCount(const void *pArg1, const void *pArg2)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ int32		Serial1, Serial2;
	MessageInfomation	*pCount1 = (MessageInfomation *) pArg1;
	MessageInfomation	*pCount2 = (MessageInfomation *) pArg2;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Serial1 = pCount1->iMessageSendCount;
	Serial2 = pCount2->iMessageSendCount;

	if(Serial1 > Serial2)
	{
		return -1;
	}
	else if(Serial1 < Serial2)
	{
		return 1;
	}
	else
	{
		return 1;
	}

	__UNGUARD__ return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 MessageUseCount::CompareBySizeCount(const void *pArg1, const void *pArg2)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ int32		Serial1, Serial2;
	MessageInfomation	*pCount1 = (MessageInfomation *) pArg1;
	MessageInfomation	*pCount2 = (MessageInfomation *) pArg2;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Serial1 = pCount1->iMessageSizeCount;
	Serial2 = pCount2->iMessageSizeCount;

	if(Serial1 > Serial2)
	{
		return -1;
	}
	else if(Serial1 < Serial2)
	{
		return 1;
	}
	else
	{
		return 1;
	}

	__UNGUARD__ return 0;
}
