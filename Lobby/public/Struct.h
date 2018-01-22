/*$T Share/Struct.h GC 1.140 10/10/07 11:01:40 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */




#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__
#pragma warning (disable:4996)

#include "TypeDefine.h"
#include "Define.h"
#include "KwNetMessage.h"
#include "KwNetStreamSystem.h"
#include "IntegerHashMap.h"
#include "SplitStringLevelOne.h"
#include "SplitStringLevelTwo.h"
#include "JsonSerialization.h"



#define MAX_SQL_LENGTH	409600
struct SQLString
{
	uchar	m_SqlStr[MAX_SQL_LENGTH];

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */

	void Clear()
	{
		memset(m_SqlStr, 0, MAX_SQL_LENGTH);
	}

	void Parse(const char *pTemplate, ...);

	void SetSql(const char* sqlString);
};

/************************************************************************/
/* 固定大小的Flag,为模块类,需要声明的时候指定,此类为Flag_T的增强版      */
/* NUM_FixedLen的值如果不是为2的次方值,则取最接近并且小于他的2的次方值  */
/************************************************************************/
template<uint32 NUM_FixedLen > class FixedFlag
{
public:

	///////////////////////////////////////////////////////////////////////////////////////////////
	//构造函数
	///////////////////////////////////////////////////////////////////////////////////////////////
	FixedFlag()																	
	{
		Clear();
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	//析构函数
	///////////////////////////////////////////////////////////////////////////////////////////////
	~FixedFlag()																
	{
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//清空数据
	///////////////////////////////////////////////////////////////////////////////////////////////
	void Clear( void )															
	{
		memset(m_uFlags, 0, sizeof(m_uFlags));
	}

public:

	///////////////////////////////////////////////////////////////////////////////////////////////
	//检查位是否已经设置
	///////////////////////////////////////////////////////////////////////////////////////////////
	BOOL IsSetBit(int32 bit) const												
	{
		uint32	_Idx = bit / 32;
		if(_Idx < (NUM_FixedLen + 31) / 32)
		{
			if(m_uFlags[_Idx] & (1 << (uint32) (bit - 32 * _Idx))) 
				return TRUE;
		}
		else
		{
			KCheckEx(FALSE, "数组越界！");
		}

		return FALSE;
	}
    
	///////////////////////////////////////////////////////////////////////////////////////////////
	//更新位状态
	///////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateBit(int32 bit, BOOL bUpDate)										
	{
		uint32	_Idx = bit / 32;
		if(_Idx < (NUM_FixedLen + 31) / 32)
		{
			if(bUpDate)		m_uFlags[_Idx] |= (1 << (uint32) (bit - 32 * _Idx));
			else			m_uFlags[_Idx] &= (~(1 << (uint32) (bit - 32 * _Idx)));
		}
		else
		{
			KCheckEx(FALSE, "数组越界！");
		}
	}

public:

	///////////////////////////////////////////////////////////////////////////////////////////////
	//取得大小
	///////////////////////////////////////////////////////////////////////////////////////////////
	uint32 GetMsgSize( void ) const												
	{
		return sizeof(m_uFlags);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//读包
	///////////////////////////////////////////////////////////////////////////////////////////////
	BOOL Read(RecieveStream &iStream)											
	{
		iStream.Reci((char *) (m_uFlags), sizeof(m_uFlags));
		return TRUE;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//解包
	///////////////////////////////////////////////////////////////////////////////////////////////
	BOOL Write(SendStream &oStream) const										
	{
		oStream.Send((char *) (m_uFlags), sizeof(m_uFlags));
		return TRUE;
	}

private:

	///////////////////////////////////////////////////////////////////////////////////////////////
	//内部数据
	///////////////////////////////////////////////////////////////////////////////////////////////
	uint32	m_uFlags[ (NUM_FixedLen + 31)/32 ];										
};


#define RSA_MESSAGE_ARRAY_NUM 34
struct RSA_VLOLG_MESSAGE 
{
	int		m_Negative;
	uint32	m_Share;
	uint32	m_Num;
	uint32	m_Array[RSA_MESSAGE_ARRAY_NUM];

	void Clear()
	{
		m_Negative = 0;
		m_Share = 0;
		m_Num = 0;
		memset( m_Array,0,sizeof(uint32) * RSA_MESSAGE_ARRAY_NUM );
	}

	void SetNegative(int val)
	{
		m_Negative = val;
	}

	int GetNegative()
	{
		return m_Negative;
	}

	void SetShare(uint32 val)
	{
		m_Share = val;
	}

	uint32 GetShare()
	{
		return m_Share;
	}

	void SetNum(uint32 val)
	{
		m_Num = val;
	}

	uint32 GetNum()
	{
		return m_Num;
	}

	void SetArray(uint32 index,uint32 val);
	uint32 GetArray(uint32 index);

	void Reci(RecieveStream &iStream);
	void Send(SendStream &oStream) const;

	uint32 GetSize() const
	{
		return sizeof(m_Negative) + sizeof(m_Share) + sizeof(m_Num) + sizeof(uint32) * RSA_MESSAGE_ARRAY_NUM;
	}

	enum
	{
		struct_size	= sizeof(int) + sizeof(uint32) + sizeof(uint32) + sizeof(uint32) * RSA_MESSAGE_ARRAY_NUM
	};

};

// 简单数组定义，用来装载进本数据类型 [yangzhen]
template<typename T, int32 MAX_COUNT>
struct TSimpleStaticArray
{
	TSimpleStaticArray()
		: m_pData(NULL)
		, m_nDataCount(0)
	{
	}

	TSimpleStaticArray(const TSimpleStaticArray& rhs)
		: m_pData(NULL)
	{
		*this = rhs;
	}

	~TSimpleStaticArray()
	{
		Clear();
	}

	void Clear()
	{
		if(m_pData)
		{
			delete[] m_pData;
			m_pData = NULL;
		}
		m_nDataCount = 0;
	}

	void Init(int32 nCount, const T& nDefault)
	{
		Clear();
		m_Default = nDefault;
		if(nCount < 1)
			return;
		else if(nCount > MAX_COUNT)
			nCount = MAX_COUNT;
		m_nDataCount = nCount;
		m_pData = new T[m_nDataCount];
		for(int32 i = 0; i < m_nDataCount; ++i)
		{
			m_pData[i] = nDefault;
		}
	}
	// 新增一个新的初始化方便读表 [ASSERT]
	void InitSplit(const char* str)
	{
		SplitStringLevelOne	pSplitL1;
		pSplitL1.Reset();
		pSplitL1.Init('|', NULL);
		pSplitL1.DoSplit(str);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		int32	iLineCount = pSplitL1.GetResultLineCount();
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		KCheck(MAX_COUNT >= iLineCount);

		Init(iLineCount, 0);
		for (int32 _Idx = 0; _Idx < iLineCount; ++_Idx)
		{
			m_pData[_Idx] = atoi(pSplitL1.GetResultLine(_Idx));
		}
	}

	TSimpleStaticArray& operator=(const TSimpleStaticArray& rhs)
	{
		m_Default = rhs.m_Default;
		if(m_nDataCount == rhs.m_nDataCount && m_nDataCount > 0)
		{
			memcpy(m_pData, rhs.m_pData, sizeof(T)*m_nDataCount);
		}
		else
		{
			Clear();
			if(rhs.m_nDataCount > 0)
			{
				Init(rhs.m_nDataCount, rhs.m_Default);
				memcpy(m_pData, rhs.m_pData, sizeof(T)*m_nDataCount);
			}
		}
		return *this;
	}

	T& operator[](int32 nIdx)
	{
		KCheck(nIdx >= 0);
		KCheck(nIdx < MAX_COUNT);
		if(nIdx >= m_nDataCount)
		{
			return m_Default;
		}
		return m_pData[nIdx];
	}

	const T& operator[](int32 nIdx) const
	{
		KCheck(nIdx >= 0);
		KCheck(nIdx < MAX_COUNT);
		if(nIdx >= m_nDataCount)
		{
			return m_Default;
		}
		return m_pData[nIdx];
	}

	// 扩展接口，保证在MAX_COUNT范围内的取值都有效，这个功能使其更接近静态预分配数组的行为 [yangzhen]
	T& GetItemExtend(int32 nIdx)
	{
		KCheck(nIdx >= 0);
		KCheck(nIdx < MAX_COUNT);
		if (0 == m_nDataCount)
		{
			return m_Default;
		}

		if(nIdx >= m_nDataCount)
		{
			KCheck(m_nDataCount > 0);
			return m_pData[m_nDataCount - 1];
		}
		return m_pData[nIdx];
	}

	// 扩展接口，保证在MAX_COUNT范围内的取值都有效，这个功能使其更接近静态预分配数组的行为 [yangzhen]
	const T& GetItemExtend(int32 nIdx) const
	{
		KCheck(nIdx >= 0);
		KCheck(nIdx < MAX_COUNT);

		if (0 == m_nDataCount)
		{
			return m_Default;
		}

		if(nIdx >= m_nDataCount)
		{
			KCheck(m_nDataCount > 0);
			return m_pData[m_nDataCount - 1];
		}
		return m_pData[nIdx];
	}

	int32 GetSize() const
	{
		return m_nDataCount;
	}

	int32 GetMaxSize() const
	{
		return MAX_COUNT;
	}

	const T& GetDefault() const
	{
		return m_Default;
	}

private:
	T m_Default;
	T* m_pData;
	int32 m_nDataCount;
};


#endif
