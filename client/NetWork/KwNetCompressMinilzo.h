#ifndef KwNetCompressMinilzo_h__
#define KwNetCompressMinilzo_h__

#include "KwNetBasics.h"
#include "lzo/lzoconf.h"
#include "lzo/lzopro/lzo1x.h"
#include "lzo/lzopro/lzo1y.h"
#include "lzo/lzo1f.h"
#include "KwNetMiniLock.h"

#define MAX_THREAD_SIZE		128

#define LZO_STANDARD_MEM		LZOPRO_LZO1X_1_14_MEM_COMPRESS
#define LZO_SPEED_MEM			LZOPRO_LZO1X_1_08_MEM_COMPRESS
#define LZO_SIZE_MEM			0

#define LZO_WORK_MEM_SIZE		(max<int>(max<int>(LZO_STANDARD_MEM, LZO_SPEED_MEM), LZO_SIZE_MEM))

// 压缩函数必须匹配(比如lzo1x)
#define LZO_STANDARD_COMPRESS	lzopro_lzo1x_1_14_compress
#define LZO_SPEED_COMPRESS		lzopro_lzo1x_1_08_compress
#define LZO_SIZE_COMPRESS(in, in_len, out, out_len, wrkmem) lzopro_lzo1x_99_compress(in, in_len, out, out_len, &GLZOCallbacks, 10);

#define LZO_DECOMPRESS			lzopro_lzo1x_decompress

#define WRKMEM_SIZE			LZO_WORK_MEM_SIZE

class Compress_Minilzo
{
	struct WRKMEM
	{
		void*	pMem;
		TID		threadID;
	};
public:
	Compress_Minilzo();
	~Compress_Minilzo();

public:
	static Compress_Minilzo& GetInstance()
	{
		static Compress_Minilzo instance;
		return instance;
	}

	BOOL	Init();
	void	Close();
	void*	AllocWrkMem(TID threadID);
	BOOL	Compress( const uchar* pIn, lzo_uint uInsize, uchar* pOut, lzo_uint& uOutSize, void* wrkmem );
	int32	DeCompress( const uchar* pIn, lzo_uint uInsize, uchar* pOut, lzo_uint& uOutSize );

	void	AddUnCmDataSize(uint64 uSize);
	void	AddCmDataSize(uint64 uSize);
	uint64	GetUnCmDataSize();
	uint64	GetCmDataSize();

	BOOL	IsEnableLog();
	void	EnableLog(BOOL bEnable);

private:
	BOOL	m_bEnableLog;

	WRKMEM	m_wrkmem[MAX_THREAD_SIZE];
	uint32	m_uWrkmemSize;
	
	uint64	m_uUnCmDataSize;
	uint64	m_uCmDataSize;
	
	MiniLock m_Lock;

};


class Compress_Assistant
{
public:
	Compress_Assistant();
	~Compress_Assistant();

	void	SetWrkMem(void* pMem);
	void*	GetWrkMem();
	BOOL	IsEanble();
	void	Enable(BOOL bEnable,TID threadID=0);
	BOOL	IsEnableLog();
	void	EnableLog(BOOL bEnable);

	void	IncCmFrame();
	uint32	GetCmFrame();
	void	IncCmSuccFrame();
	uint32	GetCmSuccFrame();

private:
	void*	m_wrkmem;
	BOOL	m_bEnable;
	BOOL	m_bEnableLog;
	uint32	m_uCmFrame;
	uint32	m_uCmSuccFrame;
};

#endif // KwNetCompressMinilzo_h__
