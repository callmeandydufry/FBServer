
#ifndef __ARCHIVEHOLDER_H__
#define __ARCHIVEHOLDER_H__

////////////////////////////////////////////////////////////////////////////////
// API Module
////////////////////////////////////////////////////////////////////////////////
#include "TypeDefine.h"

#ifdef __WINDOWS__
#define INVALID_KHAN_SM_HANDLE	((void *) 0)
#elif __LINUX__
#define INVALID_KHAN_SM_HANDLE	- 1
#include "DBOperMgr.h"
#endif
class	ArchiveNode
{
	/*
	-----------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------
	*/
public:
	static MKHandle CreateMemKeeper(MK_VALUE key, uint32 Size);
	static MKHandle OpenMenKeeper(MK_VALUE key, uint32 Size);
	static char	*ViewMemKeeper(MKHandle handle);
	static void	UnViewMemKeeper(char *MemoryPtr);
	static void	TerminateMemKeeper(MKHandle handle);
};

////////////////////////////////////////////////////////////////////////////////
// Unit Module
////////////////////////////////////////////////////////////////////////////////

#define SHMEM_LOG_PATH	"./Log/DBShare"

class	ArchiveNodeWrapper
{
	/*
	-----------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------
	*/
public:

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	ArchiveNodeWrapper()
	{
		m_pDataPtr = 0;
		m_hold = 0;
		m_Size = 0;
		m_pHeader = 0;
		m_LogIndex = -1;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	~ArchiveNodeWrapper()
	{
	};

	/*
	===============================================================================================================
	===============================================================================================================
	*/

	BOOL InitLog(int32 LogIndex)
	{

		__GUARD__	
		m_LogIndex = LogIndex;
		return TRUE;
		__UNGUARD__ return FALSE;
	}

	BOOL	Create(MK_VALUE key, uint32 Size);
	void	Destory();
	BOOL	ViewMap(MK_VALUE, uint32 Size);

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	char *GetDataPtr()
	{
		return m_pDataPtr;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	char *GetTypePtr(uint32 tSize, uint32 tIndex)
	{
		KCheck(tSize > 0);
		KCheck(tSize * tIndex < m_Size);
		if(tSize <= 0 || tIndex >= m_Size) return NULL;
		return m_pDataPtr + tSize * tIndex;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	uint32 GetSize()
	{
		return m_Size;
	}

	uint32	GetUpdateTime();
	void	SetUpdateTime(uint32 ver);

	/*
	-----------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------
	*/

private:
	uint32		m_Size;
	char		*m_pHeader;
	char		*m_pDataPtr;	
	MKHandle	m_hold;
	int32		m_LogIndex;
};

////////////////////////////////////////////////////////////////////////////////
// Manager Module
////////////////////////////////////////////////////////////////////////////////
#define MAX_MANAGER_MK 5000
#define MAX_SM_OBJ_NUM	20

enum MKPOOL_TYPE { SMPT_SHAREMEM, SMPT_SERVER, SMPT_WORLD };

template<typename T>
class ArchiveNodePool
{
	/*
	-----------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------
	*/
public:

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	ArchiveNodePool()
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		m_pInstancePtr = new ArchiveNodeWrapper();
		KCheck(m_pInstancePtr);
		m_hObj = NULL;
		m_nMaxSize = -1;
		m_nPosition = -1;
		__UNGUARD__
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	~ArchiveNodePool()
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	KCheck(m_pInstancePtr);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		m_pInstancePtr->Destory();
		KSafeDelete(m_pInstancePtr);
		KSafeDelete_ARRAY(m_hObj);

		__UNGUARD__
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	BOOL Init(uint32 nMaxCount, MK_VALUE key, MKPOOL_TYPE SMPT, int32 LogIndex, BOOL& bCreateShareMem)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		KCheck(m_pInstancePtr);

		if(!m_pInstancePtr) return FALSE;

		m_pInstancePtr->InitLog(LogIndex);

		/*~~~~~~~~*/
		BOOL	ret;
		/*~~~~~~~~*/

		bCreateShareMem = FALSE;
		ret = m_pInstancePtr->ViewMap(key, sizeof(T) * nMaxCount + sizeof(SMHead));


		if(SMPT == SMPT_SERVER)
		{
			if(!ret)
			{
				ret = m_pInstancePtr->Create(key, sizeof(T) * nMaxCount + sizeof(SMHead));
				bCreateShareMem = TRUE;
			}
		}
		else
		{
			if(!ret)
			{
				return FALSE;
			}
		}

		if(!ret)
		{
			QLogSystem::QUICK_LOG(SLOW_LOG_MAPSERVER_SHAREMEM, "Setup MK block fail!");
			KCheck(ret);
			return ret;
		}

		SMHead* pSMHead = new SMHead;
		KCheck(pSMHead);

		m_nMaxSize = nMaxCount;
		m_nPosition = 0;
		m_hObj = new T *[m_nMaxSize];

		/*~~~~~~*/
		int32	_Idx;
		/*~~~~~~*/

		for(_Idx = 0; _Idx < m_nMaxSize; _Idx++)
		{
			m_hObj[_Idx] = reinterpret_cast < T * > (m_pInstancePtr->GetTypePtr(sizeof(T), _Idx));
			if(m_hObj[_Idx] == NULL)
			{
				KCheck(m_hObj[_Idx] != NULL);
				return FALSE;
			}
			if (bCreateShareMem)
			{
				m_hObj[_Idx]->Clear();
			}
		}

		m_key = key;

		return TRUE;
		__UNGUARD__ return FALSE;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	BOOL Finalize()
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	KCheck(m_pInstancePtr);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		m_pInstancePtr->Destory();
		return TRUE;

		__UNGUARD__ return FALSE;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	T *NewObj(void)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	KCheck(m_nPosition < m_nMaxSize);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(m_nPosition >= m_nMaxSize)
		{
			return NULL;
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		T	*pObj = m_hObj[m_nPosition];
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		pObj->SetPoolID((uint32) m_nPosition);
		m_nPosition++;
		return pObj;
		__UNGUARD__ return NULL;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void DeleteObj(T *pObj)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	KCheck(pObj != NULL);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(pObj == NULL)
		{
			return;
		}

		KCheck(m_nPosition > 0);
		if(m_nPosition <= 0)
		{
			return;
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		uint32 uDelIndex = pObj->GetPoolID();
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		KCheck(uDelIndex < (uint32) m_nPosition);
		if(uDelIndex >= (uint32) m_nPosition)
		{
			return;
		}

		m_nPosition--;

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		T	*pDelObj = m_hObj[uDelIndex];
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		m_hObj[uDelIndex] = m_hObj[m_nPosition];
		m_hObj[m_nPosition] = pDelObj;

		m_hObj[uDelIndex]->SetPoolID(uDelIndex);
		m_hObj[m_nPosition]->SetPoolID(INVALID_KHAN_ID);
		__UNGUARD__
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	T *GetPoolObj(int32 iIndex)
	{
		KCheck(iIndex < m_nMaxSize);
		return m_hObj[iIndex];
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	int32 GetPoolMaxSize()
	{
		return m_nMaxSize;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	int32 GetPoolSize()
	{
		return m_nPosition;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	MK_VALUE GetKey()
	{
		return m_key;
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	BOOL DumpToFile(char *FilePath)
	{
		if(!m_pInstancePtr)
		{
			KCheck(m_pInstancePtr);
			return FALSE;
		}

		return TRUE;

		/*
		* return m_pInstancePtr->DumpToFile(FilePath);
		*/
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	BOOL MergeFromFile(char *FilePath)
	{
		if(!m_pInstancePtr)
		{
			KCheck(m_pInstancePtr);
			return FALSE;
		}

		return TRUE;

		/*
		* return m_pInstancePtr->MergeFromFile(FilePath);
		*/
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	uint32 GetHeadVer()
	{
		KCheck(m_pInstancePtr);
		return m_pInstancePtr->GetUpdateTime();
	}

	/*
	===============================================================================================================
	===============================================================================================================
	*/
	void SetHeadVer(uint32 ver)
	{
		KCheck(m_pInstancePtr);
		return m_pInstancePtr->SetUpdateTime(ver);
	}

	/*
	-----------------------------------------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------------------------------------
	*/
	BOOL Mem2File(const char* szDumpFile)
	{
		__GUARD__
		FILE *fp = NULL;
		fp = fopen(szDumpFile,"wb");
		KCheckEx(fp, "Fopen Fail");
		fwrite(*m_hObj, sizeof(T) * m_nMaxSize, 1, fp);
		fclose(fp);
		fp = NULL;
		return TRUE;
			__UNGUARD__ return FALSE;
	}

	BOOL File2Mem(const char* szDumpFile)
	{
		__GUARD__

			FILE *fp = NULL;
		fp = fopen(szDumpFile,"rb");
		KCheckEx(fp, "Fopen Fail");
			fread(*m_hObj, sizeof(T) * m_nMaxSize, 1, fp);
		fclose(fp);
		fp = NULL;

		__UNGUARD__ return FALSE;
	}

	BOOL Recover()
	{
		__GUARD__
			m_nPosition = 0;
		for(int32 iIndex = 0; iIndex < m_nMaxSize; iIndex++)
		{			
			T* pObj = m_hObj[iIndex];			
			KCheck(pObj);
			if (pObj->IsValid())
			{
				DBOper* pDBCore = g_pDBOperMgr->GetDBOper();
				KCheckEx(pDBCore,"Recover pDBCore == NULL !!!!!");

				BOOL bRet = pDBCore->Save(*m_hObj[iIndex]);
				//如果存档失败,终止程序，保证共享内存数据不丢 以防止丢档
				KCheck(bRet);
			}

			m_hObj[iIndex]->Clear();
		}
		return TRUE;

		__UNGUARD__	return FALSE;
	}
private:
	T	**m_hObj;
	int32 m_nMaxSize;
	int32 m_nPosition;
	ArchiveNodeWrapper *m_pInstancePtr;
	MK_VALUE m_key;
};


#endif
