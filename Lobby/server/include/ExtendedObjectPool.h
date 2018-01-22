#ifndef __EXTENDED_OBJECT_POOL_H__
#define __EXTENDED_OBJECT_POOL_H__

#include <queue>
#include <vector>
#include <stdexcept>
#include <memory>
#include "Define.h"

using std::queue;
using std::vector;

template <typename T>
class ExtendedObjectPool
{
public:
	
	ExtendedObjectPool() : mChunkSize(1000){};
	~ExtendedObjectPool() 
	{
		// free each of the allocation chunks
		for_each(mAllObjects.begin(), mAllObjects.end(), arrayDeleteObject);
	}

	T* acquireObject()
	{
		if (mFreeList.empty())
		{
			allocateChunk();
		}
		T* obj = mFreeList.front();
		mFreeList.pop();
		return (obj);
	}

	void releaseObject(T& obj)
	{
		mFreeList.push(&obj);
	}

	void setDefaultChunkSize(size_t nSize)
	{
		if (nSize > 0)
		{
			mChunkSize = nSize;
		}
	}

	void allocateChunk()
	{
		T* newObjects = new T[mChunkSize];
		mAllObjects.push_back(newObjects);
		for (int i = 0; i < mChunkSize; i++)
		{
			mFreeList.push(&newObjects[i]);
		}
	}
private:
	static void arrayDeleteObject(T* obj)
	{
		delete[] obj;
	}
protected:
	queue<T*> mFreeList;
	vector<T*> mAllObjects;

	int mChunkSize;
private:
	ExtendedObjectPool(const ExtendedObjectPool<T>& src);
	ExtendedObjectPool<T>& operator=(const ExtendedObjectPool<T>& rhs);
};

#endif // __EXTENDED_OBJECT_POOL_H__

// http://blog.csdn.net/chaoyuan899/article/details/9018197