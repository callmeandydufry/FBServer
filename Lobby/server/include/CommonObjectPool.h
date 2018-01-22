#ifndef __COMMON_OBJECT_POOL_H__
#define __COMMON_OBJECT_POOL_H__

template<typename T>
class CommonObjectPool
{
public:
	CommonObjectPool()
	{
		mMaxCount = 0;
		mUsedCount = 0;
		mObjectPool = NULL;
	}

	~CommonObjectPool()
	{
		// 确保所有对象已经被归还到池子里 [12/15/2017 yz]
		KCheck(mUsedCount == 0);

		if (mObjectPool)
		{
			for (int32 i = 0; i < mMaxCount; ++i)
			{
				delete mObjectPool[i];
			}
			delete[] mObjectPool;
			mObjectPool = NULL;
		}
	}

	void initPool(int32 maxCount)
	{
		if (NULL == mObjectPool && maxCount > 0)
		{
			mUsedCount = 0;
			mMaxCount = maxCount;
			mObjectPool = new T*[maxCount];
			for (int32 i = 0; i < maxCount; ++i)
			{
				mObjectPool[i] = new T();
			}
		}
	}

	T* allocObject()
	{
		if (mUsedCount < mMaxCount)
		{
			T* newObj = mObjectPool[mUsedCount];
			mObjectPool[mUsedCount] = NULL;
			mUsedCount++;
			return newObj;
		}
		return NULL;
	}

	void freeObject(T* obj)
	{
		if (mUsedCount > 0)
		{
			mObjectPool[mUsedCount - 1] = obj;
			mUsedCount--;
		}
		else
		{
			delete obj;
		}
	}

	int32 getLeftCount()
	{
		return mMaxCount - mUsedCount;
	}

protected:
	int32 mMaxCount;
	int32 mUsedCount;
	T** mObjectPool;
};

#endif