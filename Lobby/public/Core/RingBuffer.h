/** \file RingBuffer.h
 ** \desc ����Buffer
 **	\date  2012-07-17
 **	\author Michael
**/

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include "stdafx.h"

//֧�ֶ������������ߵĻ���buffer
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
	//�򻺳����βд������
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

	//�򻺳����ͷд������
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

	//�ӻ������ͷ��ȡ����
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

	//�������Ƿ�Ϊ��
	BOOL IsEmpty()
	{
		if (m_nReadPos == m_nWritePos)
		{
			return TRUE;
		}
		return FALSE;
	}

	//�������Ƿ���
	BOOL IsFull()
	{
		if ((m_nWritePos+1) % nSize == m_nReadPos)
		{
			return TRUE;
		}
		
		return FALSE;
	}

private:
	size_t     m_nReadPos;         //�ɶ�����λ��
	size_t     m_nWritePos;        //��д����λ��
	T	       m_Buffer[nSize];    //������
	FastLock   m_ReadLock;         //������ʱ��
	FastLock   m_WriteLock;        //д����ʱ��
};

#endif
