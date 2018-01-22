#include "KwNetPrivate.h"
#include "KwNetCompressMinilzo.h"

Compress_Minilzo::Compress_Minilzo()
{
	m_uWrkmemSize = 0;
	m_uUnCmDataSize = 0;
	m_uCmDataSize = 0;
	for( int32 i=0; i<MAX_THREAD_SIZE; ++i )
	{
		m_wrkmem[i].pMem = NULL;
		m_wrkmem[i].threadID = 0;
	}

	Init();
}

Compress_Minilzo::~Compress_Minilzo()
{
	Close();
}

BOOL Compress_Minilzo::Init()
{
	if( lzo_init() != LZO_E_OK )
		return FALSE;
	return TRUE;
}

void Compress_Minilzo::Close()
{
	for( int32 i=0; i<MAX_THREAD_SIZE; ++i )
	{
		if( m_wrkmem[i].pMem )
			delete[] m_wrkmem[i].pMem;
		m_wrkmem[i].pMem = NULL;
		m_wrkmem[i].threadID = 0;
	}
	m_uWrkmemSize = 0;
	m_uUnCmDataSize = 0;
	m_uCmDataSize = 0;
}

void* Compress_Minilzo::AllocWrkMem(TID threadID)
{
	minilock_guard<MiniLock> autolock(m_Lock);

	for( int32 i=0; i<MAX_THREAD_SIZE; ++i )
	{
		if( m_wrkmem[i].threadID == threadID )
			return m_wrkmem[i].pMem;
	}

	if( m_uWrkmemSize >= MAX_THREAD_SIZE )
		return FALSE;

	lzo_align_t* pWrkMem = new lzo_align_t[WRKMEM_SIZE];
	m_wrkmem[m_uWrkmemSize].pMem = pWrkMem;
	m_wrkmem[m_uWrkmemSize].threadID = threadID;
	m_uWrkmemSize++;
	return pWrkMem;
}

BOOL Compress_Minilzo::Compress( const uchar* pIn, lzo_uint uInsize, uchar* pOut, lzo_uint& uOutSize, void* wrkmem )
{
	//int32 ret = LZO_SPEED_COMPRESS( pIn, uInsize, pOut, &uOutSize, wrkmem );
	int32 ret = LZO_STANDARD_COMPRESS( pIn, uInsize, pOut, &uOutSize, wrkmem );
	if( ret != LZO_E_OK || uOutSize+2 >= uInsize )
	{
		return FALSE;
	}
	return TRUE;
}

int32 Compress_Minilzo::DeCompress( const uchar* pIn, lzo_uint uInsize, uchar* pOut, lzo_uint& uOutSize )
{
	int32 ret = LZO_DECOMPRESS(pIn,uInsize,pOut, &uOutSize,NULL);
	return ret;
}

void Compress_Minilzo::AddUnCmDataSize(uint64 uSize)
{
	minilock_guard<MiniLock> autolock(m_Lock);
	m_uUnCmDataSize += uSize;
}

void Compress_Minilzo::AddCmDataSize(uint64 uSize)
{
	minilock_guard<MiniLock> autolock(m_Lock);
	m_uCmDataSize += uSize;
}

uint64 Compress_Minilzo::GetUnCmDataSize()
{
	minilock_guard<MiniLock> autolock(m_Lock);
	return m_uUnCmDataSize;
}

uint64 Compress_Minilzo::GetCmDataSize()
{
	minilock_guard<MiniLock> autolock(m_Lock);
	return m_uCmDataSize;
}

BOOL Compress_Minilzo::IsEnableLog()
{
	return m_bEnableLog;
}

void Compress_Minilzo::EnableLog(BOOL bEnable)
{
	m_bEnableLog = bEnable;
}



//========================================================================================================

Compress_Assistant::Compress_Assistant()
{
	m_wrkmem = NULL;
	m_bEnable = FALSE;
	m_bEnableLog = FALSE;
	m_uCmFrame = 0;
	m_uCmSuccFrame = 0;
}

Compress_Assistant::~Compress_Assistant()
{}

void Compress_Assistant::SetWrkMem(void* pMem)
{
	m_wrkmem = pMem;
}

void* Compress_Assistant::GetWrkMem()
{
	return m_wrkmem;
}

BOOL Compress_Assistant::IsEanble()
{
	return m_bEnable;
}

void Compress_Assistant::Enable(BOOL bEnable,TID threadID)
{
	if( bEnable == TRUE )
	{
		m_wrkmem = Compress_Minilzo::GetInstance().AllocWrkMem(threadID);
		if( m_wrkmem == NULL )
		{
			m_bEnable = FALSE;
			return;
		}
	}

	m_bEnable = bEnable;
}

BOOL Compress_Assistant::IsEnableLog()
{
	return m_bEnableLog;
}

void Compress_Assistant::EnableLog(BOOL bEnable)
{
	m_bEnableLog = bEnable;
}

void Compress_Assistant::IncCmFrame()
{
	m_uCmFrame ++;
}

uint32 Compress_Assistant::GetCmFrame()
{
	return m_uCmFrame;
}

void Compress_Assistant::IncCmSuccFrame()
{
	m_uCmSuccFrame ++;
}

uint32 Compress_Assistant::GetCmSuccFrame()
{
	return m_uCmSuccFrame;
}
