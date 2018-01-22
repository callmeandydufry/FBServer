#ifndef __STREAM_POINTER_H__
#define __STREAM_POINTER_H__

#include "KwNetBasics.h"

class StreamPointer
{
public:
	StreamPointer( uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize );
	~StreamPointer();

	uchar*	GetPointer();
	uchar*	GetPointer(uint32 offset);
	void	CopyData(uchar* pDst,uint32 uSize);
	uint32	GetSize();

	StreamPointer& operator += (uint32 u);
	StreamPointer& operator ++ ();

private:
	uchar*	m_pPointer;
	uint32	m_uHead;
	uint32	m_uTail;
	uint32	m_uBufMaxSize;
};


#endif
