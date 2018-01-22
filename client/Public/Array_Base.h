/*$T Common/Array_Base.h GC 1.140 10/10/07 10:06:26 */
#pragma once

/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef Array_Base_h__
#define Array_Base_h__

#include "TypeDefine.h"
#include "ModuleDataStream.h"
#include "TemplateUtils.h"

#include <malloc.h>

// 静态预分配内存的数组 [12/7/2017 yz]
template<class T, int ARRAY_SIZE>
class TStaticArray
{
public:
	TStaticArray()
		: m_iCount(0)
	{
	}

	template<int32 OTHER_ARRAY_SIZE>
	TStaticArray(const TStaticArray<T, OTHER_ARRAY_SIZE>& otherArray)
		: m_iCount(0)
	{
		Copy(otherArray);
	}

	~TStaticArray()
	{
		RemoveAll();
	}

	int32 GetSize() const
	{
		return m_iCount;
	}

	int32 GetMaxSize() const
	{
		return ARRAY_SIZE;
	}

	void RemoveAll()
	{
		Remove(0, m_iCount);
	}

	void Remove(int32 iIndex, int32 iCount = 1)
	{
		if (iIndex < m_iCount)
		{
			if (iCount > m_iCount - iIndex)
			{
				iCount = m_iCount - iIndex;
			}

			if (TTypeInfo<T>::NeedsDestructor)
			{
				for (int i = iIndex; i<iIndex + iCount; i++)
					(&m_ArrayData[i])->~T();
			}

			if (iCount < m_iCount - iIndex)
			{
				memcpy(&m_ArrayData[iIndex], &m_ArrayData[iIndex + iCount], (m_iCount - iIndex - iCount) * sizeof(T));
			}
			m_iCount -= iCount;
		}
	}

	void RemoveItem(const T &value)
	{
		int32 iIndex = FindIndex(value);
		if (iIndex >= 0)
		{
			Remove(iIndex);
		}
	}

	void RemoveSwap(int32 iIndex)
	{
		if (iIndex < m_iCount)
		{
			if (TTypeInfo<T>::NeedsDestructor)
			{
				(&m_ArrayData[iIndex])->~T();
			}

			if (iIndex != m_iCount - 1)
			{
				memcpy(&m_ArrayData[iIndex], &m_ArrayData[m_iCount - 1], sizeof(T));
			}
			m_iCount--;
		}
	}

	T* Add(int32 iCount = 1)
	{
		if (m_iCount + iCount <= ARRAY_SIZE)
		{
			if (TTypeInfo<T>::NeedsDestructor)
			{
				for (int32 Index = 0; Index < iCount; ++Index)
				{
					::new(&m_ArrayData[m_iCount + Index]) T();
				}
			}
			else
			{
				memset(&m_ArrayData[m_iCount], 0, sizeof(T) * iCount);
			}
			T* ret = &m_ArrayData[m_iCount];
			m_iCount += iCount;

			return ret;
		}
		return NULL;
	}

	BOOL AddItem(const T &value)
	{
		if (m_iCount < ARRAY_SIZE)
		{
			::new(&m_ArrayData[m_iCount]) T(value);
			m_iCount++;
			return TRUE;
		}
		return FALSE;
	}

	BOOL AddUnique(const T &value)
	{
		for (int Index = 0; Index < m_iCount; Index++)
		{
			if (m_ArrayData[Index] == value)
				return FALSE;
		}

		return Add(value);
	}

	int32 FindIndex(const T &value)
	{
		for (int Index = 0; Index < m_iCount; Index++)
		{
			if (m_ArrayData[Index] == value)
				return Index;
		}
		return -1;
	}

	template<int32 OTHER_ARRAY_SIZE>
	void Copy(const TStaticArray<T, OTHER_ARRAY_SIZE>& Source)
	{
		if (this != &Source)
		{
			if (Source.m_iCount > 0)
			{
				RemoveAll();

				if (TTypeInfo<T>::NeedsConstructor)
				{
					for (int Index = 0; Index < Source.m_iCount && Index < ARRAY_SIZE; Index++)
					{
						::new(m_ArrayData + m_iCount) T(Source[Index]);
						//::new(m_ArrayData + m_iCount) (RegisterPlayer*)(Source[Index]);
						m_iCount++;
					}
				}
				else
				{
					int32 iCopyNum = std::min(Source.m_iCount, ARRAY_SIZE);
					memcpy(m_ArrayData, &Source[0], sizeof(T) * iCopyNum);
					m_iCount = iCopyNum;
				}
			}
			else
			{
				RemoveAll();
			}
		}
	}

	const T& Get(int32 iIndex) const
	{
		return m_ArrayData[iIndex];
	}

	T& Get(int32 iIndex)
	{
		return m_ArrayData[iIndex];
	}

	T& operator[](int32 iIndex)
	{
		return m_ArrayData[iIndex];
	}

	const T& operator[](int32 iIndex) const
	{
		return m_ArrayData[iIndex];
	}

	template<int32 OTHER_ARRAY_SIZE>
	TStaticArray<T, ARRAY_SIZE>& operator=(const TStaticArray<T, OTHER_ARRAY_SIZE>& otherArr)
	{
		Copy(otherArr);
		return *this;
	}

	BOOL serialize(DataStream &oStream) const
	{
		oStream.pushValue(m_iCount);
		for (int i = 0; i < m_iCount; ++i)
		{
			oStream.pushValue(m_ArrayData[i]);
		}
		return TRUE;
	}

	BOOL unserialize(DataStream &iStream)
	{
		RemoveAll();

		iStream.getValue(m_iCount);
		m_iCount = std::min(m_iCount, ARRAY_SIZE);
		for (int i = 0; i < m_iCount; ++i)
		{
			iStream.getValue(m_ArrayData[i]);
		}
		return TRUE;
	}

private:
	int32 m_iCount;
	T m_ArrayData[ARRAY_SIZE];
};

// 动态可扩展容量数组 [12/7/2017 yz]
template<class T>
class TDynaArray
{
public:
	TDynaArray()
		: m_ArrayData(NULL)
		, m_iCount(0)
		, m_iReservedCount(0)
	{

	}

	TDynaArray(const TDynaArray<T>& otherArray)
		: m_ArrayData(NULL)
		, m_iCount(0)
		, m_iReservedCount(0)
	{
		Copy(otherArray);
	}

	~TDynaArray()
	{
		RemoveAll();
	}

	int32 GetSize() const
	{
		return m_iCount;
	}

	int32 GetMaxSize() const
	{
		return m_iReservedCount;
	}

	void Reserve(int iCount)
	{
		if (iCount > m_iReservedCount)
		{
			m_iReservedCount = CalculateSlack(iCount);
			Realloc(m_iReservedCount);
		}
	}

	void RemoveAll()
	{
		Remove(0, m_iCount);

		if (m_ArrayData)
		{
			::free(m_ArrayData);
			m_ArrayData = NULL;
		}
	}

	void Remove(int32 iIndex, int32 iCount = 1)
	{
		if (m_ArrayData && iIndex < m_iCount)
		{
			if (iCount > m_iCount - iIndex)
			{
				iCount = m_iCount - iIndex;
			}

			if (TTypeInfo<T>::NeedsDestructor)
			{
				for (int i = iIndex; i < iIndex + iCount; i++)
					(&m_ArrayData[i])->~T();
			}

			if (iCount < m_iCount - iIndex)
			{
				memcpy(&m_ArrayData[iIndex], &m_ArrayData[iIndex + iCount], (m_iCount - iIndex - iCount) * sizeof(T));
			}
			m_iCount -= iCount;
		}
	}

	void RemoveSwap(int32 iIndex)
	{
		if (m_ArrayData && iIndex < m_iCount)
		{
			if (TTypeInfo<T>::NeedsDestructor)
			{
				(&m_ArrayData[iIndex])->~T();
			}

			if (iIndex != m_iCount - 1)
			{
				memcpy(&m_ArrayData[iIndex], &m_ArrayData[m_iCount - 1], sizeof(T));
			}
			m_iCount--;
		}
	}

	T* Add(int32 iCount = 1)
	{
		Reserve(m_iCount + iCount);

		if (TTypeInfo<T>::NeedsDestructor)
		{
			for (int32 Index = 0; Index < iCount; ++Index)
			{
				::new(&m_ArrayData[m_iCount + Index]) T();
			}
		}
		else
		{
			memset(&m_ArrayData[m_iCount], 0, sizeof(T) * iCount);
		}
		T* ret = &m_ArrayData[m_iCount];
		m_iCount += iCount;

		return ret;
	}

	BOOL AddItem(const T &value)
	{
		Reserve(m_iCount + 1);

		::new(&m_ArrayData[m_iCount]) T(value);
		m_iCount++;
		return TRUE;
	}

	BOOL AddUnique(const T &value)
	{
		for (int Index = 0; Index < m_iCount; Index++)
		{
			if (m_ArrayData[Index] == value)
				return FALSE;
		}

		return Add(value);
	}

	int32 FindIndex(const T &value)
	{
		for (int Index = 0; Index < m_iCount; Index++)
		{
			if (m_ArrayData[Index] == value)
				return Index;
		}
		return -1;
	}

	void Copy(const TDynaArray<T>& Source)
	{
		if (this != &Source)
		{
			if (Source.m_iCount > 0)
			{
				RemoveAll();
				Reserve(Source.m_iCount);

				if (TTypeInfo<T>::NeedsConstructor)
				{
					for (int Index = 0; Index < Source.m_iCount; Index++)
					{
						::new(m_ArrayData + m_iCount) T(Source[Index]);
						m_iCount++;
					}
				}
				else
				{
					memcpy(m_ArrayData, &Source[0], sizeof(T) * Source.m_iCount);
					m_iCount = Source.m_iCount;
				}
			}
			else
			{
				RemoveAll();
			}
		}
	}

	const T& Get(int32 iIndex) const
	{
		return m_ArrayData[iIndex];
	}

	T& Get(int32 iIndex)
	{
		return m_ArrayData[iIndex];
	}

	T& operator[](int32 iIndex)
	{
		return m_ArrayData[iIndex];
	}

	const T& operator[](int32 iIndex) const
	{
		return m_ArrayData[iIndex];
	}

	TDynaArray<T>& operator=(const TDynaArray<T>& otherArr)
	{
		Copy(otherArr);
		return *this;
	}

	BOOL serialize(DataStream &oStream) const
	{
		oStream.pushValue(m_iCount);
		for (int i = 0; i < m_iCount; ++i)
		{
			oStream.pushValue(m_ArrayData[i]);
		}
		return TRUE;
	}

	BOOL unserialize(DataStream &iStream)
	{
		RemoveAll();

		iStream.getValue(m_iCount);
		Reserve(m_iCount);
		for (int i = 0; i < m_iCount; ++i)
		{
			iStream.getValue(m_ArrayData[i]);
		}
		return TRUE;
	}

protected:
	void Realloc(int32 NewSize)
	{
		if (NewSize > 0)
		{
			if (m_ArrayData)
			{
				m_ArrayData = (T*)::realloc(m_ArrayData, NewSize * sizeof(T));
			}
			else
			{
				m_ArrayData = (T*)::malloc(NewSize * sizeof(T));
			}
		}
		else if(m_ArrayData)
		{
			::free(m_ArrayData);
			m_ArrayData = NULL;
		}
	}

	// 计算每次扩容的时候应该是扩大多少合适 [12/7/2017 yz]
	int CalculateSlack(int32 iCount) const
	{
		return iCount + 3 * iCount / 8 + 16;
	}

private:
	T* m_ArrayData;
	int32 m_iCount;
	int32 m_iReservedCount;
};

#include "Array_Base.inl"
#endif
