/*$T Share/TAB/TAB.h GC 1.140 10/10/07 10:06:57 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef _TAB_H_
#define _TAB_H_

#include <vector>
#ifdef __WINDOWS__
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include "TypeDefine.h"

namespace	TAB
{

#ifdef KW_OS_64
	typedef int64	TAB_FIELD_INT;
	typedef double	TAB_FIELD_FLOAT;
#else
	typedef int32	TAB_FIELD_INT;
	typedef float	TAB_FIELD_FLOAT;
#endif

class	TABFile
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	enum FIELD_TYPE { EFT_NUM = 0, EFT_FLOAT = 1, EFT_TEXT = 2, };

	struct FILE_HEAD
	{
		uint32	m_Identify;
		int32	m_nFieldsNum;
		int32	m_nRecordsNum;
		int32	m_i_StringBlockSize;
	};

	union FIELD
	{
		TAB_FIELD_FLOAT	fValue;
		TAB_FIELD_INT iValue;
		const char	*pString;

		FIELD()
		{
		}
		FIELD(int32 value)
		{
			iValue = value;
		}
		FIELD (float value)
		{
			fValue = value;
		}
		FIELD (const char *value)
		{
			pString = value;
		}
	};

	typedef vector<FIELD_TYPE>			FILEDS_TYPE;
	typedef vector<FIELD>				DATA_BUF;
	typedef vector<string>				FILEDS_NAME;

	static const char* FIELD_TYPE_TEXTS[];
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:
#ifdef __SGI_STL_PORT
	typedef std::hash_map<int32, FIELD *>		FIELD_HASHMAP;
#else
#ifdef __WINDOWS__
	typedef stdext::hash_map<int32, FIELD *>	FIELD_HASHMAP;
#else
	typedef __gnu_cxx::hash_map<int32, FIELD *>	FIELD_HASHMAP;
#endif
#endif
	uint32 m_ID;

	FILEDS_TYPE m_theType;

	FILEDS_NAME	m_FieldNames; 

	int32 m_nRecordsNum;

	int32 m_nFieldsNum;

	DATA_BUF m_vDataBuf;

	char *m_pStringBuf;

	int32 m_i_StringBufSize;

	FIELD_HASHMAP m_hashIndex;

	int32 m_nIndexColum;

	char m_szFileName[MAX_FILE_PATH];

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	static int32		sString2Vector
				(
					const char		*strStrINTgSource,
					vector<std::string>	&vRet,
					const char		*szKey,
					BOOL bOneOfKey,
					BOOL bIgnoreEmpty
				);

	static const char	*sGetLineFromMemory
				(
					char	*pStringBuf,
					int32 nBufSize,
					const char *pMemory,
					const char *pDeadEnd
				);

	template<FIELD_TYPE T>
	static bool _FieldEqu(const FIELD & a, const FIELD & b);

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	// 查找索引等于nValue的值， 没有找到返回0
	virtual const FIELD	*SearchIndexEQU(int32 nValue) const;
	// 查找值 nRecordLine 行，ColumNum 列
	virtual const FIELD	*SearchField(int32 nRecordLine, int32 nColumNum) const;
	// 查找nColumnNum列第一个等于value的值， 没有找到返回0
	virtual const FIELD	*SearchFirstColumnEqu(int32 nColumnNum, const FIELD &value) const;

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:
	BOOL	Open_From_Memory_Impl_Text(const char *pMemory, const char *pDeadEnd, const char *szFileName = 0);

	BOOL	Open_From_Memory_Impl_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName = 0);
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:

	// 没有用到
	uint32 GetID(void) const
	{
		return m_ID;
	}

	// 有效行数
	int32 GetRowsNum(void) const
	{
		return m_nRecordsNum;
	}

	// 列
	int32 GetFieldsNum(void) const
	{
		return m_nFieldsNum;
	}

	// 以某一列创建hash索引，方便查找。默认是把第一列创建为索引
	void	CreateIndex(int32 nColum = 0, const char *szFileName = 0);

	// 取得字段类型
	FIELD_TYPE GetFieldType(int field) const { return m_theType[field]; }

	const string& GetFieldName(int field) const { return m_FieldNames[field]; }

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	// 从文件中打开
	BOOL	OpenFile_From_TEXT(const char *szFileName);
	// 从内存中打开
	BOOL	OpenFile_From_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName = 0);

	// 写入表格到二进制内存 [8/12/2015 yangzhen]
	BOOL	Write_Binary_Tab_Memory(char* pMemory, int32 nMemSize);
	// 写入表格到二进制文件格式 [8/12/2015 yangzhen]
	BOOL	Write_Binary_Tab_File(const char *szFileName);
	// 获取二进制格式的tab文件的内存大小 [8/12/2015 yangzhen]
	int32	GetBinaryTabSize();

	// 因为该table类 客户端和编辑器要统一使用一个 而且编辑器中需要重复 加载表问题 所以添加以下函数
	void	Clear();
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	TABFile(uint32 id = 0);
	virtual ~TABFile();
};
}
#endif
