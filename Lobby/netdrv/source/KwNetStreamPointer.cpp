#include "KwNetPrivate.h"
#include "KwNetStreamPointer.h"

StreamPointer::StreamPointer( uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize )
{
	m_pPointer = pBuf;
	m_uHead = uHead;
	m_uTail = uTail;
	m_uBufMaxSize = uBufMaxSize;
}

StreamPointer::~StreamPointer()
{}

uchar* StreamPointer::GetPointer()	
{ 
	return m_pPointer+m_uHead; 
}

uchar* StreamPointer::GetPointer(uint32 offset)
{
	uint32 uHead = m_uHead + offset;
	if( uHead >= m_uBufMaxSize )
	{
		uHead = uHead - m_uBufMaxSize;
	}
	return m_pPointer+uHead;
}

StreamPointer& StreamPointer::operator += (uint32 u)
{
	m_uHead += u;
	if( m_uHead >= m_uBufMaxSize )
	{
		m_uHead = m_uHead - m_uBufMaxSize;
	}
	return *this;
}

StreamPointer& StreamPointer::operator ++ ()
{
	m_uHead ++;
	if( m_uHead >= m_uBufMaxSize )
	{
		m_uHead = m_uHead - m_uBufMaxSize;
	}
	return *this;	
}

void StreamPointer::CopyData(uchar* pDst,uint32 uSize)
{
	uint32 uRealSize = GetSize();
	if( uSize > uRealSize )
		uSize = uRealSize;

	uint32 uLeftSize = 0;
	if( m_uTail >= m_uHead )
	{
		uLeftSize = m_uTail - m_uHead;
		if( uLeftSize > uSize )
			uLeftSize = uSize;
		memcpy( pDst, GetPointer(), uLeftSize );
	}
	else
	{
		uLeftSize = m_uBufMaxSize - m_uHead;
		if( uLeftSize >= uSize )
		{
			memcpy( pDst, GetPointer(), uSize );
		}
		else
		{
			memcpy( pDst, GetPointer(), uLeftSize );
			memcpy( pDst+uLeftSize, GetPointer(uLeftSize), uSize-uLeftSize );
		}
	}
}

uint32 StreamPointer::GetSize()
{
	uint32 uSize = 0;
	if( m_uTail >= m_uHead )
		uSize = m_uTail - m_uHead;
	else
		uSize = m_uBufMaxSize - m_uHead + m_uTail;
	return uSize;
}

