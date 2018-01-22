/** \file RingBuffer.h
 ** \desc 环形Buffer
 **	\date  2012-07-17
 **	\author Michael
**/

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include "stdafx.h"

//支持多生产者消费者的环形buffer
template <typename T,size_t nSize = 1024>
class RingBuffer
{
public:
	RingBuffer():m_nReadPos(0),m_nWritePos(0)
	{
		memset(m_Buffer,0,sizeof(T) * nSize);
	}

	~RingBuffer(){}

public:
	//向缓冲队列尾写入数据
	BOOL Push_Back(const T& data)
	{
		lock_guard<FastLock> autolock(m_WriteLock);

		if (IsFull())
		{
			return FALSE;
		}

		m_Buffer[m_nWritePos] = data;

		if (m_nWritePos+1 >= nSize)
		{
			m_nWritePos = 0;
		}
		else
		{
			++m_nWritePos;
		}

		return TRUE;
	}

	//向缓冲队列头写入数据
	BOOL Push_Front(const T& data)
	{
		lock_guard<FastLock> autolock_w(m_WriteLock);		
	
		if (IsFull())
		{
			return FALSE;
		}

		lock_guard<FastLock> autolock_r(m_ReadLock);

		if (m_nReadPos <= 0)
		{
			m_nReadPos = nSize-1;			
		}
		else
		{
			--m_nReadPos;
		}

		m_Buffer[m_nReadPos] = data;

		return TRUE;
	}

	//从缓冲队列头读取数据
	BOOL Pop(T& data)
	{		
		lock_guard<FastLock> autolock(m_ReadLock);

		if (IsEmpty())
		{
			return FALSE;
		}
		
		data = m_Buffer[m_nReadPos];
		
		if (m_nReadPos+1 >=  nSize)
		{
			m_nReadPos = 0;			
		}
		else
		{
			++m_nReadPos;
		}
		return TRUE;
	}

	//缓冲区是否为空
	BOOL IsEmpty()
	{
		if (m_nReadPos == m_nWritePos)
		{
			return TRUE;
		}
		return FALSE;
	}

	//缓冲区是否满
	BOOL IsFull()
	{
		if ((m_nWritePos+1) % nSize == m_nReadPos)
		{
			return TRUE;
		}
		
		return FALSE;
	}

private:
	size_t     m_nReadPos;         //可读数据位置
	size_t     m_nWritePos;        //可写数据位置
	T	       m_Buffer[nSize];    //缓冲区
	FastLock   m_ReadLock;         //读数据时锁
	FastLock   m_WriteLock;        //写数据时锁
};

#endif
