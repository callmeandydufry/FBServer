/*$T Share/TAB/TAB.cpp GC 1.140 10/10/07 10:06:57 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include <string>
#include <map>
#include <assert.h>
#include "TAB.h"
#include <exception>

namespace	TAB
{
	const char* TABFile::FIELD_TYPE_TEXTS[] =
	{
		"NUM",
		"FLOAT",
		"TEXT",
	};
    
#ifdef KW_OS_64
    union FIELD32
    {
        float	fValue;
        int iValue;
        
        FIELD32()
        {
        }
        FIELD32(int32 value)
        {
            iValue = value;
        }
        FIELD32 (float value)
        {
            fValue = value;
        }
    };
#endif
    
    /*
     =======================================================================================================================
 =======================================================================================================================
 */
BOOL TABFile::Open_From_Memory_Impl_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    register const char	*pMem = pMemory;
    FILE_HEAD		theHead;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    
    memcpy(&theHead, pMem, sizeof(FILE_HEAD));
    if(theHead.m_Identify != 0XDDBBCC00)
    {
        return FALSE;
    }
    
    
    BOOL bFailed = FALSE;
    
    if
        (
         sizeof(FILE_HEAD) +
         sizeof(uint32) *
         theHead.m_nFieldsNum +
         sizeof(FIELD) *
         theHead.m_nRecordsNum *
         theHead.m_nFieldsNum +
         theHead.m_i_StringBlockSize > (uint32) (pDeadEnd - pMemory)
         )
    {
        bFailed = TRUE;
    }
    
#ifdef KW_OS_64
    BOOL bLoad32Tab = FALSE;
    if(bFailed)
    {
        if
            (
             sizeof(FILE_HEAD) +
             sizeof(uint32) *
             theHead.m_nFieldsNum +
             sizeof(FIELD32) *
             theHead.m_nRecordsNum *
             theHead.m_nFieldsNum +
             theHead.m_i_StringBlockSize > (uint32) (pDeadEnd - pMemory)
             )
        {
            return FALSE;
        }
        bLoad32Tab = TRUE;
        bFailed = FALSE;
    }
#endif
    
    if(bFailed)
    {
        return FALSE;
    }
    
    pMem += sizeof(FILE_HEAD);
    
    m_nRecordsNum = theHead.m_nRecordsNum;
    m_nFieldsNum = theHead.m_nFieldsNum;
    m_i_StringBufSize = theHead.m_i_StringBlockSize;
    
    m_pStringBuf = new char[theHead.m_i_StringBlockSize];
    if(!m_pStringBuf) return FALSE;
    
    /*~~~~~~~~~~~~~~~~~~~~~~~*/
    vector<uint32>	vFieldType;
    /*~~~~~~~~~~~~~~~~~~~~~~~*/
    
    vFieldType.resize(theHead.m_nFieldsNum);
    memcpy(&(vFieldType[0]), pMem, sizeof(uint32) * theHead.m_nFieldsNum);
    pMem += sizeof(uint32) * theHead.m_nFieldsNum;
    
    m_theType.resize(theHead.m_nFieldsNum);
    for(int32 _Idx = 0; _Idx < (int32) theHead.m_nFieldsNum; _Idx++)
    {
        switch(vFieldType[_Idx])
        {
            case EFT_NUM:
            case EFT_FLOAT:
            case EFT_TEXT:
                m_theType[_Idx] = (FIELD_TYPE) vFieldType[_Idx];
                break;
                
            default:
                KSafeDelete_ARRAY(m_pStringBuf);
                return FALSE;
        }
    }
    
    m_vDataBuf.resize(theHead.m_nRecordsNum * theHead.m_nFieldsNum);
    
#ifdef KW_OS_64
    if(bLoad32Tab)
    {
        if(m_vDataBuf.size() > 0)
        {
            FIELD32* pField32Mem = (FIELD32*)pMem;
            int totalRecord = theHead.m_nRecordsNum * theHead.m_nFieldsNum;
            for(int r = 0; r < theHead.m_nRecordsNum; ++r)
            {
                for(int f = 0; f < theHead.m_nFieldsNum; ++f)
                {
                    if(vFieldType[f] == EFT_FLOAT)
                    {
                        m_vDataBuf[r * m_nFieldsNum + f].fValue = pField32Mem[r * m_nFieldsNum + f].fValue;
                    }
                    else
                    {
                        m_vDataBuf[r * m_nFieldsNum + f].iValue = pField32Mem[r * m_nFieldsNum + f].iValue;
                    }
                }
                
            }
            
            pMem += sizeof(FIELD32) * theHead.m_nRecordsNum * theHead.m_nFieldsNum;
        }
    }
    else
#endif
    {
        if(m_vDataBuf.size() > 0)
        {
            memcpy(&(m_vDataBuf[0]), pMem, sizeof(FIELD) * theHead.m_nRecordsNum * theHead.m_nFieldsNum);
            pMem += sizeof(FIELD) * theHead.m_nRecordsNum * theHead.m_nFieldsNum;
        }
    }
    
    memcpy(m_pStringBuf, pMem, m_i_StringBufSize);
    m_pStringBuf[m_i_StringBufSize - 1] = 0;
    
    for(int32 j = 0; j < (int32) theHead.m_nFieldsNum; j++)
    {
        if(vFieldType[j] != EFT_TEXT) continue;
        
        /*~~~~~~~~~~~~~~~*/
        string	str;
        int32	nFieldsNum;
        /*~~~~~~~~~~~~~~~*/
        
        for(int32 _Idx = 0; _Idx < (int32) theHead.m_nRecordsNum; _Idx++)
        {
            nFieldsNum = GetFieldsNum();
            
            m_vDataBuf[_Idx * nFieldsNum + j].pString += reinterpret_cast<uint64> (m_pStringBuf);
        }
    }
    
    CreateIndex(0, szFileName);
    
    return TRUE;
}
    
    /*
     =======================================================================================================================
     =======================================================================================================================
 */
BOOL TABFile::Open_From_Memory_Impl_Text(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char			szLine[1024 * 10] = { 0 };
	register const char	*pMem = pMemory;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	pMem = sGetLineFromMemory(szLine, 1024 * 10, pMem, pDeadEnd);
	if(!pMem) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	vector<std::string>	vRet;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	sString2Vector(szLine, vRet, "\t", TRUE, TRUE);
	if(vRet.empty()) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	FILEDS_TYPE	vFieldsType;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	vFieldsType.resize(vRet.size());

	for(int32 _Idx = 0; _Idx < (int32) vRet.size(); _Idx++)
	{
		if(vRet[_Idx] == "NUM")
			vFieldsType[_Idx] = EFT_NUM;
		else if(vRet[_Idx] == "FLOAT")
			vFieldsType[_Idx] = EFT_FLOAT;
		else if(vRet[_Idx] == "TEXT")
			vFieldsType[_Idx] = EFT_TEXT;
		else if(vRet[_Idx] == "CNC")
			vFieldsType[_Idx] = EFT_TEXT;
		else
		{
			return FALSE;
		}
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32					nRecordsNum = 0;
	int32					nFieldsNum = (int32) vFieldsType.size();
	vector<std::pair<std::string, int32> >	vStringBuf;
	std::map<std::string, int32>		mapStringBuf;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	pMem = sGetLineFromMemory(szLine, 1024 * 10, pMem, pDeadEnd);
	if(!pMem) return FALSE;

	m_FieldNames.clear();
	sString2Vector(szLine, m_FieldNames, "\t", true, true);

	/*~~~~~~~~~~~~~~~~~~~~~*/
	int32 nStringBufSize = 0;
	/*~~~~~~~~~~~~~~~~~~~~~*/

	do
	{
		pMem = sGetLineFromMemory(szLine, 1024 * 10, pMem, pDeadEnd);
		if(!pMem) break;;

		if(szLine[0] == '#') continue;

		sString2Vector(szLine, vRet, "\t", TRUE, FALSE);

		if(vRet.empty()) continue;
		if(vRet.size() != nFieldsNum)
		{
			if((int32) vRet.size() < nFieldsNum)
			{
				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
				int32 nSubNum = nFieldsNum - (int32) vRet.size();
				/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

				for(int32 _Idx = 0; _Idx < nSubNum; _Idx++)
				{
					vRet.push_back("");
				}
			}
		}

		if(vRet[0].empty()) continue;

		for(register int32 _Idx = 0; _Idx < nFieldsNum; _Idx++)
		{
			/*~~~~~~~~~~~*/
			FIELD newField;
			/*~~~~~~~~~~~*/

			switch(vFieldsType[_Idx])
			{
			case EFT_NUM:
				newField.iValue = atoi(vRet[_Idx].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case EFT_FLOAT:
				newField.fValue = (float) atof(vRet[_Idx].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case EFT_TEXT:
				if(vRet[_Idx].empty())
				{
					newField.iValue = 0;
				}
				else
				{
					/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
					const char				*p = vRet[_Idx].c_str();
					std::map<std::string, int32>::iterator	it = mapStringBuf.find(vRet[_Idx]);
					/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

					if(it == mapStringBuf.end())
					{
						vStringBuf.push_back(std::make_pair(vRet[_Idx], nStringBufSize));
						mapStringBuf.insert(std::make_pair(vRet[_Idx], (int32) vStringBuf.size() - 1));
						newField.iValue = nStringBufSize + 1;

						nStringBufSize += (int32) strlen(vRet[_Idx].c_str()) + 1;
					}
					else
					{
						newField.iValue = vStringBuf[it->second].second + 1;
					}
				}

				m_vDataBuf.push_back(newField);
				break;
			}
		}

		nRecordsNum++;
	} while(TRUE);

	m_nRecordsNum = nRecordsNum;
	m_nFieldsNum = nFieldsNum;
	m_i_StringBufSize = nStringBufSize + 1;

	m_pStringBuf = new char[m_i_StringBufSize];
	if(!m_pStringBuf) return FALSE;

	m_theType = vFieldsType;

	/*~~~~~~~~~~~~~~~~~*/
	uchar byBlank = '\0';
	/*~~~~~~~~~~~~~~~~~*/

	m_pStringBuf[0] = '\0';

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	register char	*p = m_pStringBuf + 1;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(int32 _Idx = 0; _Idx < (int32) vStringBuf.size(); _Idx++)
	{
		memcpy(p, vStringBuf[_Idx].first.c_str(), vStringBuf[_Idx].first.size());
		p += vStringBuf[_Idx].first.size();

		*(p++) = '\0';
	}

	for(register int32 _Idx = 0; _Idx < nFieldsNum; _Idx++)
	{
		if(vFieldsType[_Idx] != EFT_TEXT) continue;

		for(register int32 j = 0; j < nRecordsNum; j++)
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			FIELD	&theField = m_vDataBuf[j * nFieldsNum + _Idx];
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			theField.pString = m_pStringBuf + theField.iValue;
		}
	}

	CreateIndex(0, szFileName);

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
const char *TABFile::sGetLineFromMemory(char *pStringBuf, int32 nBufSize, const char *pMemory, const char *pDeadEnd)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	register const char	*pMem = pMemory;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(pMem >= pDeadEnd || *pMem == 0) return 0;

	while(pMem < pDeadEnd && pMem - pMemory + 1 < nBufSize && *pMem != 0 && *pMem != '\r' && *pMem != '\n')
		*(pStringBuf++) = *(pMem++);

	*pStringBuf = 0;

	while(pMem < pDeadEnd && *pMem != 0 && (*pMem == '\r' || *pMem == '\n')) pMem++;

	return pMem;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL TABFile::OpenFile_From_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{
	assert(pMemory && pDeadEnd);

	if(pDeadEnd - pMemory >= sizeof(FILE_HEAD) && *((uint32 *) pMemory) == 0XDDBBCC00)
	{
		return Open_From_Memory_Impl_Binary(pMemory, pDeadEnd, szFileName);
	}
	else
	{
		return Open_From_Memory_Impl_Text(pMemory, pDeadEnd, szFileName);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL TABFile::Write_Binary_Tab_Memory(char* pMemory, int32 nMemSize)
{
#ifdef __WINDOWS__
	// 计算保存整个表格的二进制文件需要多大的内存 [8/12/2015 yangzhen]
	if(nMemSize != GetBinaryTabSize())
		return FALSE;

	memset((void*)pMemory, 0, nMemSize);

	char* pBinPos = pMemory;

	FILE_HEAD* pFileHead = (FILE_HEAD*)pBinPos;
	pFileHead->m_Identify = 0XDDBBCC00;
	pFileHead->m_nFieldsNum = m_nFieldsNum;
	pFileHead->m_nRecordsNum = m_nRecordsNum;
	pFileHead->m_i_StringBlockSize = m_i_StringBufSize;
	pBinPos += sizeof(FILE_HEAD);

	// 写入每列的类型定义 [8/12/2015 yangzhen]
	for(int32 fieldIdx = 0; fieldIdx < m_nFieldsNum; ++fieldIdx)
	{
		*((uint32*)pBinPos) = (uint32)m_theType[fieldIdx];
		pBinPos += sizeof(uint32);
	}

	// 写入表格单元格的内存数据 [8/12/2015 yangzhen]
	for(int32 dataIndex = 0; dataIndex < m_vDataBuf.size(); dataIndex++)
	{
		FIELD& fieldData = *((FIELD*)pBinPos);
		pBinPos += sizeof(FIELD);

		fieldData = m_vDataBuf[dataIndex];
		FIELD_TYPE fieldType = m_theType[dataIndex % m_nFieldsNum];
		if(fieldType == EFT_TEXT)
		{
			fieldData.pString -= reinterpret_cast<uint64>(m_pStringBuf);
		}
	}

	// 写入字符串池 [8/12/2015 yangzhen]
	memcpy(pBinPos, m_pStringBuf, m_i_StringBufSize);

	return TRUE;
#else

	return FALSE;
#endif
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL TABFile::Write_Binary_Tab_File(const char *szFileName)
{
#ifdef __WINDOWS__
	FILE* fp = fopen(szFileName, "a");
	if(NULL == fp)
	{
		return FALSE;
	}

	// 计算保存整个表格的二进制文件需要多大的内存 [8/12/2015 yangzhen]
	int32 needMemSize =	sizeof(FILE_HEAD)
						+ sizeof(uint32) * m_nFieldsNum
						+ sizeof(FIELD) * m_nFieldsNum * m_nRecordsNum
						+ m_i_StringBufSize;
	// 分配一整块内存，用于保存二进制表格 [8/12/2015 yangzhen]
	char* pTabBinary = (char*)malloc(needMemSize);
	if(NULL == pTabBinary)
	{
		fclose(fp);
		return FALSE;
	}
	BOOL bRet = Write_Binary_Tab_Memory(pTabBinary, needMemSize);

	// 将内存写入文件 [8/12/2015 yangzhen]
	fwrite(pTabBinary, needMemSize, 1, fp);

	fclose(fp);
	free((void*)pTabBinary);

	return bRet;
#else

	return FALSE;
#endif
}

int32 TABFile::GetBinaryTabSize()
{
	// 计算保存整个表格的二进制文件需要多大的内存 [8/12/2015 yangzhen]
	int32 needMemSize =	sizeof(FILE_HEAD)
		+ sizeof(uint32) * m_nFieldsNum
		+ sizeof(FIELD) * m_nFieldsNum * m_nRecordsNum
		+ m_i_StringBufSize;
	return needMemSize;
}

const TABFile::FIELD * TABFile::SearchIndexEQU(int32 iIndex) const
{
	FIELD_HASHMAP::const_iterator	itFind = m_hashIndex.find(iIndex);
	if(itFind == m_hashIndex.end()) return NULL;

	return itFind->second;
}
const TABFile::FIELD * TABFile::SearchField(int32 nRecordLine, int32 nColumNum) const
{
	int32 nPosition = nRecordLine *
	GetFieldsNum()
	+ nColumNum;

	if(nPosition < 0 || nPosition >= (int32) m_vDataBuf.size())
	{
		char szTemp[260];
		tsnprintf(szTemp, MAX_FILE_PATH, "[TAB] Invalid Key! file=%s,line=%d,col=%d]", m_szFileName, nRecordLine, nColumNum);
#ifdef GAME_CLIENT
		//throw std::exception(szTemp);
#else
		KCheckEx(FALSE, szTemp);
#endif
		return NULL;
	}
	return &(m_vDataBuf[nPosition]);
}
const TABFile::FIELD * TABFile::SearchFirstColumnEqu(int32 nColumnNum, const FIELD & value) const
{
	if(nColumnNum < 0 || nColumnNum >= m_nFieldsNum) return 0;
	FIELD_TYPE type = m_theType[nColumnNum];

	for(register int32 _Idx = 0; _Idx < m_nRecordsNum; _Idx++)
	{
		const FIELD	&theField = m_vDataBuf[m_nFieldsNum * _Idx + nColumnNum];

		bool bRet;
		if(EFT_NUM == type)
			bRet = _FieldEqu<EFT_NUM> (theField, value);
		else if(EFT_FLOAT == type)
			bRet = _FieldEqu<EFT_FLOAT> (theField, value);
		else
			bRet = _FieldEqu<EFT_TEXT> (theField, value);

		if(bRet) return &(m_vDataBuf[m_nFieldsNum * _Idx]);
	}
	return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL TABFile::OpenFile_From_TEXT(const char *szFileName)
{
	__GUARD__

	assert(szFileName);

#ifdef __GLSERVER__
	QLogSystem::QUICK_LOG(SLOW_LOG_WORLD, "[TAB] Begin Load %s ... ", szFileName);
#elif defined __MAPSERVER__
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[TAB] Begin Load %s ... ", szFileName);
#endif

	strncpy( m_szFileName, szFileName, MAX_FILE_PATH - 1 );

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	FILE	*fp = fopen(szFileName, "rb");
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(NULL == fp)
	{
#ifdef __GLSERVER__
	QLogSystem::QUICK_LOG(SLOW_LOG_WORLD, "[Tab] Open File Failed! file=%s", szFileName);
#elif defined __MAPSERVER__
	QLogSystem::QUICK_LOG(SLOW_LOG_SERVER, "[Tab] Open File Failed! file=%s", szFileName);
#endif
	return FALSE;
	}

	fseek(fp, 0, SEEK_END);

	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32 nFileSize = ftell(fp);
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	fseek(fp, 0, SEEK_SET);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	*pMemory = new char[nFileSize + 1];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	fread(pMemory, 1, nFileSize, fp);
	fclose(fp);
	fp = 0;
	pMemory[nFileSize] = 0;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	BOOL bRet = OpenFile_From_Binary(pMemory, pMemory + nFileSize + 1, szFileName);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	KSafeDelete_ARRAY(pMemory);
	pMemory = 0;
	return bRet;

	__UNGUARD__
	return FALSE;
}

void TABFile::Clear()
{
	m_theType.clear();
	m_FieldNames.clear();
	m_nRecordsNum = 0;
	m_nFieldsNum = 0;
	m_vDataBuf.clear();

	if(m_pStringBuf) KSafeDelete_ARRAY(m_pStringBuf);
	m_pStringBuf = NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void TABFile::CreateIndex(int32 nColum, const char *szFileName)
{
	if(nColum < 0 || nColum >= m_nFieldsNum || m_nIndexColum == nColum) return;

	m_hashIndex.clear();

	for(int32 _Idx = 0; _Idx < (int32) m_nRecordsNum; _Idx++)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		FIELD			*p = &(m_vDataBuf[_Idx * m_nFieldsNum]);
		FIELD_HASHMAP::iterator itFind = m_hashIndex.find((int32)p->iValue);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(itFind != m_hashIndex.end())
		{
			/*~~~~~~~~~~~~~*/
			char szTemp[260];
			/*~~~~~~~~~~~~~*/

			tsnprintf
			(
				szTemp,
				260,
				"[%s]Multi index at line %d(SAME:value=%d)",
				szFileName,
				_Idx + 1,
				p->iValue
			);
#ifdef GAME_CLIENT
			//throw std::exception(szTemp);
			KCheckEx(FALSE, szTemp);
#else
			KCheckEx(FALSE, szTemp);
#endif
			return;
		}

		m_hashIndex.insert(std::make_pair((int32)p->iValue, p));
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
template<TABFile::FIELD_TYPE T>
bool TABFile::_FieldEqu(const FIELD &a, const FIELD &b)
{
	if(T == EFT_NUM)
	{
		return a.iValue == b.iValue;
	}
	else if(T == EFT_FLOAT)
	{
		return a.fValue == b.fValue;
	}
	else
	{
		try
		{
			return(strcmp(a.pString, b.pString) == 0);
		}
		catch(...)
		{
			return false;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
TABFile::TABFile(uint32 id)
{
	m_ID = id;
	m_pStringBuf = NULL;
	m_nIndexColum = -1;
	memset( m_szFileName, 0, MAX_FILE_PATH );

	m_theType.clear();
	m_FieldNames.clear();
	m_nRecordsNum = 0;
	m_nFieldsNum = 0;
	m_vDataBuf.clear();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 TABFile::sString2Vector
(
	const char		*strStrINTgSource,
	vector<std::string>	&vRet,
	const char		*szKey,
	BOOL			bOneOfKey,
	BOOL			bIgnoreEmpty
)
{
	vRet.clear();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	std::string	strSrc = strStrINTgSource;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(strSrc.empty())
	{
		return 0;
	}

	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}

	while(TRUE)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		std::string	strItem = strSrc.substr(nLeft, nRight - nLeft);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));

		if(bOneOfKey)
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			std::string	strTemp = strSrc.substr(nLeft);
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return(int32) vRet.size();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
TABFile::~TABFile()
{
	if(m_pStringBuf) KSafeDelete_ARRAY(m_pStringBuf);
	m_pStringBuf = NULL;

#ifdef __GLSERVER__
	//QLogSystem::SLOW_LOG(SLOW_LOG_WORLD, "[TAB] End   Load %s ! ", m_szFileName);
#elif defined __MAPSERVER__
	// 由于这里在析构的时候用到了锁的变量，这个变量在关服的时候提前释放了，会报错，所以去掉打日志的代码 [xingzhishan]
	//QLogSystem::SLOW_LOG(SLOW_LOG_SERVER, "[TAB] End   Load %s ! ", m_szFileName);
#endif
}



/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 Decrypt(const char *cSrc, char *cDest)
{
	if(0 == strcmp(cSrc, "")) return 1;

	/*~~~~~~~~~~*/
	int32	h, l;
	int32	m, n;
	int32	j = 0;
	/*~~~~~~~~~~*/

	for(size_t _Idx = 0; _Idx < strlen(cSrc); _Idx = _Idx + 2)
	{
		h = (cSrc[_Idx] - 'x');
		l = (cSrc[_Idx + 1] - 'z');

		m = (h << 4);
		n = (l & 0xf);

		cDest[j] = m + n;

		j++;
	}

	cDest[j] = '\0';

	return 0;
}

}
