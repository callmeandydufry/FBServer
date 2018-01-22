/*$T MapServer/Server/Base/IntegerHashMap.h GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __IDTABLE_H__
#define __IDTABLE_H__

#include "TypeDefine.h"

#ifdef __LINUX__
#include <ext/hash_map>
using __gnu_cxx::	hash_map;
#else
#include <hash_map>
using namespace		std;
#endif
struct STableItem
{
	uint32	m_ID;
	void	*m_pPtr;
	uint32	m_Status;
};

#ifndef __WINDOWS__
namespace __gnu_cxx
{
	template<>
	struct hash<long long int>
	{
		size_t operator()(long long int x) const
		{
			return x;
		}
	};

	template<>
	struct hash<unsigned long long int>
	{
		size_t operator()(unsigned long long int x) const
		{
			return x;
		}
	};
}

#endif

#ifdef __WINDOWS__
#define HASH_MAP stdext::hash_map
#else
#define HASH_MAP __gnu_cxx::hash_map
#endif

template<typename T>
class TIntegerHashMap
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	typedef HASH_MAP<uint64, T>	IDTableHashMap;

	typedef typename IDTableHashMap::iterator IDTableIterator;
	TIntegerHashMap();
	~TIntegerHashMap();

	void InitTable(uint32 MaxItem);

	BOOL Add(uint64 id, T pPtr);

	T Get(uint64 id);

	BOOL Remove(uint64 id);

	void Clear();

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */

	uint32 GetCount()
	{
		return(uint32) m_IDTableHashMap.size();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	uint32 GetMaxCount()
	{
		return m_MaxCount;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	IDTableIterator Begin()
	{
		m_CurrentIterator = m_IDTableHashMap.begin();
		return m_CurrentIterator;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	IDTableIterator End()
	{
		return m_IDTableHashMap.end();
	}

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
	IDTableHashMap m_IDTableHashMap;
	IDTableIterator m_CurrentIterator;
	uint32 m_MaxCount;
};

template<typename T>
TIntegerHashMap<T>::TIntegerHashMap()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	m_MaxCount = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}

template<typename T>
TIntegerHashMap<T>::~TIntegerHashMap()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	m_MaxCount = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}

template<typename T>
void TIntegerHashMap<T>::InitTable(uint32 MaxItem)
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	Clear();
	/*~~~~~~~~~~~~~~~~~~~~*/

	m_MaxCount = MaxItem;
}

template<typename T>
BOOL TIntegerHashMap<T>::Add(uint64 id, T pPtr)
{
	if (m_IDTableHashMap.size() >= m_MaxCount) return FALSE;

	m_IDTableHashMap.insert(IDTableHashMap::value_type(id, pPtr));

	return TRUE;
}

template<typename T>
T TIntegerHashMap<T>::Get(uint64 id)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	IDTableHashMap::iterator	it = m_IDTableHashMap.find(id);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (it != m_IDTableHashMap.end()) return it->second;

	return NULL;
}

template<typename T>
BOOL TIntegerHashMap<T>::Remove(uint64 id)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	IDTableHashMap::iterator	it = m_IDTableHashMap.find(id);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (it != m_IDTableHashMap.end())
	{
		m_IDTableHashMap.erase(it);
		return TRUE;
	}

	return FALSE;
}

template<typename T>
void TIntegerHashMap<T>::Clear()
{
	m_IDTableHashMap.clear();
}

#endif
