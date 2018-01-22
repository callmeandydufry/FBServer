/*$T MapServer/Server/Base/StringHashMap.h GC 1.140 10/10/07 10:07:21 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __STRTABLE_H__
#define __STRTABLE_H__

#include "TypeDefine.h"

template<typename TVALUE>
class	TStringHashMap
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	struct STableItemStr
	{
		char	*m_pString;
		TVALUE	m_Value;
		uint32	m_Status;

		STableItemStr()
		{
			m_pString = NULL;
			memset(&m_Value, 0, sizeof(m_Value));
			m_Status = 0;
		}
		~STableItemStr()
		{
			KSafeDelete_ARRAY(m_pString);
			memset(&m_Value, 0, sizeof(m_Value));
			m_Status = 0;
		}

		void ClearValue()
		{
			memset(&m_Value, 0, sizeof(m_Value));
		}
	};

	TStringHashMap()
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	m_pTable = NULL;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		m_Count = 0;
		m_StringSize = 0;

		__UNGUARD__
	}

	~TStringHashMap()
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	KSafeDelete_ARRAY(m_pTable);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		m_Count = 0;
		m_StringSize = 0;

		__UNGUARD__
	}

	void InitTable(uint32 uMaxItem, uint32 uSize)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__	m_Count = uMaxItem;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		m_StringSize = uSize;
		KSafeDelete_ARRAY(m_pTable);
		m_pTable = new STableItemStr[m_Count];
		KCheck(m_pTable);

		for (uint32 _Idx = 0; _Idx < m_Count; _Idx++)
		{
			m_pTable[_Idx].m_pString = new char[m_StringSize];
			memset(m_pTable[_Idx].m_pString, 0, sizeof(char) * m_StringSize);

			m_pTable[_Idx].ClearValue();
			m_pTable[_Idx].m_Status = STRTS_EMPTY;
		}

		__UNGUARD__
	}

	BOOL Add(const char *szString, const TVALUE& val)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__ uint32	id = KLCRC(szString);
		uint32			c = m_Count ? (id / 4) % m_Count : m_Count;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		for (uint32 _Idx = 0; _Idx < m_Count; _Idx++)
		{
			if (m_pTable[c].m_Status == STRTS_SET)
			{
				c++;
				if (c >= m_Count) c = 0;

				continue;
			}

			if (m_pTable[c].m_pString == NULL)
			{
				m_pTable[c].m_pString = new char[m_StringSize];
			}

			memset(m_pTable[c].m_pString, 0, sizeof(char) * m_StringSize);
			strncpy(m_pTable[c].m_pString, szString, m_StringSize - 1);

			m_pTable[c].m_Value = val;
			m_pTable[c].m_Status = STRTS_SET;

			return TRUE;
		}

		return FALSE;

		__UNGUARD__ return FALSE;
	}

	const TVALUE& Get(const char *szString)
	{
		static TVALUE nullVal = (TVALUE)0;
		__GUARD__ if (szString == NULL || szString[0] == 0 || 0 == m_Count)
		{
			return nullVal;
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		uint32	id = KLCRC(szString);
		uint32	c = (id / 4) % m_Count;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		for (uint32 _Idx = 0; _Idx < m_Count; _Idx++)
		{
			if (m_pTable[c].m_Status == STRTS_EMPTY)
			{
				return nullVal;
			}
			else if (m_pTable[c].m_Status == STRTS_USE)
			{
				c++;
				if (c >= m_Count) c = 0;
				continue;
			}

			if (strcmp(m_pTable[c].m_pString, szString) == 0)
			{
				return m_pTable[c].m_Value;
			}

			c++;
			if (c >= m_Count) c = 0;
		}

		__UNGUARD__ return nullVal;
	}
	
	BOOL Get(const char *szString, TVALUE& val)
	{
		__GUARD__ if (szString == NULL || szString[0] == 0 || 0 == m_Count)
		{
			return FALSE;
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		uint32	id = KLCRC(szString);
		uint32	c = (id / 4) % m_Count;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		for (uint32 _Idx = 0; _Idx < m_Count; _Idx++)
		{
			if (m_pTable[c].m_Status == STRTS_EMPTY)
			{
				return FALSE;
			}
			else if (m_pTable[c].m_Status == STRTS_USE)
			{
				c++;
				if (c >= m_Count) c = 0;
				continue;
			}

			if (strcmp(m_pTable[c].m_pString, szString) == 0)
			{
				val = m_pTable[c].m_Value;
				return TRUE;
			}

			c++;
			if (c >= m_Count) c = 0;
		}

		__UNGUARD__ return FALSE;
	}

	void Remove(const char *szString)
	{
		if (0 == m_Count) return;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__GUARD__ uint32	id = KLCRC(szString);
		uint32			c = (id / 4) % m_Count;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		for (uint32 _Idx = 0; _Idx < m_Count; _Idx++)
		{
			if (m_pTable[c].m_Status == STRTS_EMPTY)
			{
				return;
			}
			else if (m_pTable[c].m_Status == STRTS_USE)
			{
				c++;
				if (c >= m_Count) c = 0;
				continue;
			}

			if (strcmp(m_pTable[c].m_pString, szString) == 0)
			{
				memset(m_pTable[c].m_pString, 0, sizeof(char) * m_StringSize);
				m_pTable[c].ClearValue();
				m_pTable[c].m_Status = STRTS_USE;

				return;
			}

			c++;
			if (c >= m_Count) c = 0;
		}

		__UNGUARD__
	}

	void Clear()
	{
		__GUARD__ if (m_pTable == NULL) return;

		for (uint32 _Idx = 0; _Idx < m_Count; _Idx++)
		{
			memset(m_pTable[_Idx].m_pString, 0, sizeof(char) * m_StringSize);
			m_pTable[_Idx].ClearValue();
			m_pTable[_Idx].m_Status = STRTS_EMPTY;
		}

		__UNGUARD__
	}

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	enum { STRTS_EMPTY = 0, STRTS_SET = 1, STRTS_USE = 2, };

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
	STableItemStr	*m_pTable;
	uint32		m_Count;
	uint32		m_StringSize;
};
#endif
