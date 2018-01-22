/*$T Share/Util.cpp GC 1.140 10/10/07 10:06:30 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "TypeDefine.h"
#include "Util.h"
#include "BaseTool.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

#ifdef __LINUX__
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef __WINDOWS__
#include "windows.h"
#include <io.h>
#include <direct.h>
#endif

#ifdef __LINUX__
	// linux下需要包含此头文件为了getPid函数 [JiangWei]
	#include <unistd.h>
    #include <iconv.h>
	#if ANDROID
		//#include <sys/atomics.h>
	#elif IOS
		#include <execinfo.h>
	#else
		// 非Android编译环境下包含这些头文件 [JiangWei]
		#include <execinfo.h>
		#include "Atomic.h"
	#endif
#include <signal.h>
#include <exception>
#include <setjmp.h>
#include <Atomic.h>
#endif

ThreadIndent	g_ThreadIndent;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
ThreadIndent::ThreadIndent()
{
	for(int32 _Idx = 0; _Idx < THREAD_CAPS; _Idx++)
	{
		m_ThreadID[_Idx] = 0;
		m_ThreadIndent[_Idx] = 0;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
ThreadIndent::~ThreadIndent()
{
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void ThreadIndent::AddThread(TID tid)
{
	{
		/*~~~~~~~~~~~~~~~~~~*/
		static FastLock tlock;
		/*~~~~~~~~~~~~~~~~~~*/

		lock_guard<FastLock> autolock(tlock);

		for(int32 _Idx = 0; _Idx < THREAD_CAPS; _Idx++)
		{
			if(m_ThreadID[_Idx] == tid)
			{
				break;
			}

			if(m_ThreadID[_Idx] == 0)
			{
				m_ThreadID[_Idx] = tid;
				break;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void ThreadIndent::SetThreadIndent(TID tid, int32 nValue)
{
	for(int32 _Idx = 0; _Idx < THREAD_CAPS; _Idx++)
	{
		if(m_ThreadID[_Idx] == 0)
		{
			break;
		}

		if(m_ThreadID[_Idx] == tid)
		{
			m_ThreadIndent[_Idx] = nValue;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 ThreadIndent::GetThreadIndent(TID tid)
{
	for(int32 _Idx = 0; _Idx < THREAD_CAPS; _Idx++)
	{
		if(m_ThreadID[_Idx] == 0)
		{
			return 0;
		}

		if(m_ThreadID[_Idx] == tid)
		{
			return m_ThreadIndent[_Idx];
		}
	}

	return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void shm_lock(atword *flag, char type)
{
	/*~~~~~~~~~~~~~~*/
	int32	count = 0;
	/*~~~~~~~~~~~~~~*/

#ifdef __LINUX__
	int32 count2 = 0;
	while(!atomic_inc_and_test(reinterpret_cast < atomic_t * > (flag)))
	{
		count++;
		atomic_dec(reinterpret_cast < atomic_t * > (flag));
		SafeSleep(0);
		if(count > 1000)
		{
			//这句话先注掉，adnroid平台下会开__LINUX__宏
			//QLogSystem::SLOW_LOG("./Log/smError.log", "sm_lock error flag=%d,count=%d", (*flag), count2);
			count = 0;
			count2++;
		}
	}

#else
	while(InterlockedCompareExchange (const_cast<LPLONG>(flag), 0, SM_FREE) != SM_FREE)
	{
		count++;
		SafeSleep(0);
		if(count > 10)
		{
			printf("lock fails.....\r\n");
			count = 0;
		}
	}
#endif
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void shm_unlock(atword *flag, char type)
{
	/*~~~~~~~~~~~~~~*/
	int32	count = 0;
	/*~~~~~~~~~~~~~~*/

#ifdef __LINUX__
	if((uint64) reinterpret_cast < int32 * > (flag) != SM_FREE) atomic_dec(reinterpret_cast < atomic_t * > (flag));
#else
	while(InterlockedCompareExchange (const_cast<LPLONG>(flag), SM_FREE, 0) != 0)
	{
		printf("unlock fails.....\r\n");
		SafeSleep(0);
		count++;
		if(count > 100)
		{
			InterlockedExchange (const_cast<LPLONG>(flag), SM_FREE);
			throw;
		}
	}
#endif
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void shm_tryunlock(atword *flag, char type)
{
	enum { MAX_TRY_TIMES=100,};

	int32	count = 0;
	int32	count2 = 0;

#ifndef _70_CLIENT
#ifdef __LINUX__
	while(!atomic_inc_and_test(reinterpret_cast < atomic_t * > (flag)))
	{
		count++;
		atomic_dec(reinterpret_cast < atomic_t * > (flag));
		SafeSleep(0);
		if(count > 1000)
		{
			QLogSystem::QUICK_LOG("./Log/smError.log", "shm_tryunlock : sm_lock error flag=%d,count=%d", (*flag), count2 );
			count = 0;
			count2++;
			if( count2 >= MAX_TRY_TIMES )
				break;
		}
	}

	if( count2 >= MAX_TRY_TIMES )
	{
		QLogSystem::QUICK_LOG("./Log/smError.log", "shm_tryunlock : force unlock!");
		shm_unlock(flag,type);
	}
	else
		shm_unlock(flag,type);

#else

	while(InterlockedCompareExchange (const_cast<LPLONG>(flag), 0, SM_FREE) != SM_FREE)
	{
		count++;
		SafeSleep(0);
		if(count > 1000)
		{
			QLogSystem::QUICK_LOG(SLOW_LOG_SMERROR, "shm_tryunlock : sm_lock error flag=%d,count=%d", (*flag), count2 );
			count = 0;
			count2++;
			if( count2 >= MAX_TRY_TIMES )
				break;
		}
	}

	if( count2 >= MAX_TRY_TIMES )
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_SMERROR, "shm_tryunlock : force unlock!");
		shm_unlock(flag,type);
	}
	else
		shm_unlock(flag,type);

#endif	

#endif //_70_CLIENT
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SafeSleep(uint32 millionseconds)
{
	if(millionseconds > (uint32) 30000) KCheck(FALSE);
#if defined(__WINDOWS__)
	Sleep(millionseconds);

	/*~~~~~~~~~~~~~~~*/
#elif defined(__LINUX__)
	struct timeval	to;
	/*~~~~~~~~~~~~~~~*/

	to.tv_sec = 0;
	to.tv_usec = millionseconds * 1000;
	select(0, NULL, NULL, NULL, &to);
#endif
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
TID KGetCurrentTID()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	TID	tRet = GetOrigineThreadID();
	int32	nValue = g_ThreadIndent.GetThreadIndent(tRet);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	tRet += nValue;

	return tRet;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
TID GetOrigineThreadID()
{
#if defined(__WINDOWS__)
	return GetCurrentThreadId() * 1000;
#elif defined(__LINUX__)
	return pthread_self();
#endif
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 MyGetCurrentPID()
{
	/*~~~~~~~~*/
	uint32	pid;
	/*~~~~~~~~*/

#if defined(__WINDOWS__)
	pid = GetCurrentProcessId();
#elif defined(__LINUX__)
	pid = getpid();
#endif
	return pid;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
char *KLSocketError()
{
#ifndef _ESIZE
#define _ESIZE	256
#endif

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	extern char	ErrorMessage[_ESIZE];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return ErrorMessage;
}

uint32	__crc32[256] =
{
	0x00000000,
	0x77073096,
	0xEE0E612C,
	0x990951BA,
	0x076DC419,
	0x706AF48F,
	0xE963A535,
	0x9E6495A3,
	0x0EDB8832,
	0x79DCB8A4,
	0xE0D5E91E,
	0x97D2D988,
	0x09B64C2B,
	0x7EB17CBD,
	0xE7B82D07,
	0x90BF1D91,
	0x1DB71064,
	0x6AB020F2,
	0xF3B97148,
	0x84BE41DE,
	0x1ADAD47D,
	0x6DDDE4EB,
	0xF4D4B551,
	0x83D385C7,
	0x136C9856,
	0x646BA8C0,
	0xFD62F97A,
	0x8A65C9EC,
	0x14015C4F,
	0x63066CD9,
	0xFA0F3D63,
	0x8D080DF5,
	0x3B6E20C8,
	0x4C69105E,
	0xD56041E4,
	0xA2677172,
	0x3C03E4D1,
	0x4B04D447,
	0xD20D85FD,
	0xA50AB56B,
	0x35B5A8FA,
	0x42B2986C,
	0xDBBBC9D6,
	0xACBCF940,
	0x32D86CE3,
	0x45DF5C75,
	0xDCD60DCF,
	0xABD13D59,
	0x26D930AC,
	0x51DE003A,
	0xC8D75180,
	0xBFD06116,
	0x21B4F4B5,
	0x56B3C423,
	0xCFBA9599,
	0xB8BDA50F,
	0x2802B89E,
	0x5F058808,
	0xC60CD9B2,
	0xB10BE924,
	0x2F6F7C87,
	0x58684C11,
	0xC1611DAB,
	0xB6662D3D,
	0x76DC4190,
	0x01DB7106,
	0x98D220BC,
	0xEFD5102A,
	0x71B18589,
	0x06B6B51F,
	0x9FBFE4A5,
	0xE8B8D433,
	0x7807C9A2,
	0x0F00F934,
	0x9609A88E,
	0xE10E9818,
	0x7F6A0DBB,
	0x086D3D2D,
	0x91646C97,
	0xE6635C01,
	0x6B6B51F4,
	0x1C6C6162,
	0x856530D8,
	0xF262004E,
	0x6C0695ED,
	0x1B01A57B,
	0x8208F4C1,
	0xF50FC457,
	0x65B0D9C6,
	0x12B7E950,
	0x8BBEB8EA,
	0xFCB9887C,
	0x62DD1DDF,
	0x15DA2D49,
	0x8CD37CF3,
	0xFBD44C65,
	0x4DB26158,
	0x3AB551CE,
	0xA3BC0074,
	0xD4BB30E2,
	0x4ADFA541,
	0x3DD895D7,
	0xA4D1C46D,
	0xD3D6F4FB,
	0x4369E96A,
	0x346ED9FC,
	0xAD678846,
	0xDA60B8D0,
	0x44042D73,
	0x33031DE5,
	0xAA0A4C5F,
	0xDD0D7CC9,
	0x5005713C,
	0x270241AA,
	0xBE0B1010,
	0xC90C2086,
	0x5768B525,
	0x206F85B3,
	0xB966D409,
	0xCE61E49F,
	0x5EDEF90E,
	0x29D9C998,
	0xB0D09822,
	0xC7D7A8B4,
	0x59B33D17,
	0x2EB40D81,
	0xB7BD5C3B,
	0xC0BA6CAD,
	0xEDB88320,
	0x9ABFB3B6,
	0x03B6E20C,
	0x74B1D29A,
	0xEAD54739,
	0x9DD277AF,
	0x04DB2615,
	0x73DC1683,
	0xE3630B12,
	0x94643B84,
	0x0D6D6A3E,
	0x7A6A5AA8,
	0xE40ECF0B,
	0x9309FF9D,
	0x0A00AE27,
	0x7D079EB1,
	0xF00F9344,
	0x8708A3D2,
	0x1E01F268,
	0x6906C2FE,
	0xF762575D,
	0x806567CB,
	0x196C3671,
	0x6E6B06E7,
	0xFED41B76,
	0x89D32BE0,
	0x10DA7A5A,
	0x67DD4ACC,
	0xF9B9DF6F,
	0x8EBEEFF9,
	0x17B7BE43,
	0x60B08ED5,
	0xD6D6A3E8,
	0xA1D1937E,
	0x38D8C2C4,
	0x4FDFF252,
	0xD1BB67F1,
	0xA6BC5767,
	0x3FB506DD,
	0x48B2364B,
	0xD80D2BDA,
	0xAF0A1B4C,
	0x36034AF6,
	0x41047A60,
	0xDF60EFC3,
	0xA867DF55,
	0x316E8EEF,
	0x4669BE79,
	0xCB61B38C,
	0xBC66831A,
	0x256FD2A0,
	0x5268E236,
	0xCC0C7795,
	0xBB0B4703,
	0x220216B9,
	0x5505262F,
	0xC5BA3BBE,
	0xB2BD0B28,
	0x2BB45A92,
	0x5CB36A04,
	0xC2D7FFA7,
	0xB5D0CF31,
	0x2CD99E8B,
	0x5BDEAE1D,
	0x9B64C2B0,
	0xEC63F226,
	0x756AA39C,
	0x026D930A,
	0x9C0906A9,
	0xEB0E363F,
	0x72076785,
	0x05005713,
	0x95BF4A82,
	0xE2B87A14,
	0x7BB12BAE,
	0x0CB61B38,
	0x92D28E9B,
	0xE5D5BE0D,
	0x7CDCEFB7,
	0x0BDBDF21,
	0x86D3D2D4,
	0xF1D4E242,
	0x68DDB3F8,
	0x1FDA836E,
	0x81BE16CD,
	0xF6B9265B,
	0x6FB077E1,
	0x18B74777,
	0x88085AE6,
	0xFF0F6A70,
	0x66063BCA,
	0x11010B5C,
	0x8F659EFF,
	0xF862AE69,
	0x616BFFD3,
	0x166CCF45,
	0xA00AE278,
	0xD70DD2EE,
	0x4E048354,
	0x3903B3C2,
	0xA7672661,
	0xD06016F7,
	0x4969474D,
	0x3E6E77DB,
	0xAED16A4A,
	0xD9D65ADC,
	0x40DF0B66,
	0x37D83BF0,
	0xA9BCAE53,
	0xDEBB9EC5,
	0x47B2CF7F,
	0x30B5FFE9,
	0xBDBDF21C,
	0xCABAC28A,
	0x53B39330,
	0x24B4A3A6,
	0xBAD03605,
	0xCDD70693,
	0x54DE5729,
	0x23D967BF,
	0xB3667A2E,
	0xC4614AB8,
	0x5D681B02,
	0x2A6F2B94,
	0xB40BBE37,
	0xC30C8EA1,
	0x5A05DF1B,
	0x2D02EF8D,
};

/*
 =======================================================================================================================
 =======================================================================================================================
 */

uint32 KLCRC(const char *szString)
{
	if(szString == NULL || szString[0] == 0) return 0;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	dwCrc32 = 0xFFFFFFFF;
	int32	nSize = (int32) strlen(szString);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(int32 _Idx = 0; _Idx < nSize; _Idx++)
	{
		dwCrc32 = dwCrc32 * 33 + (uchar) szString[_Idx];
	}

	return dwCrc32;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
char Data2Ascii(char in)
{
	__GUARD__ switch(in)
	{
	case 0:		return '0';
	case 1:		return '1';
	case 2:		return '2';
	case 3:		return '3';
	case 4:		return '4';
	case 5:		return '5';
	case 6:		return '6';
	case 7:		return '7';
	case 8:		return '8';
	case 9:		return '9';
	case 10:	return 'A';
	case 11:	return 'B';
	case 12:	return 'C';
	case 13:	return 'D';
	case 14:	return 'E';
	case 15:	return 'F';
	default:	KCheck(FALSE); return '?';
	}

	__UNGUARD__ return '?';
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
char Ascii2Data(char in)
{
	__GUARD__ switch(in)
	{
	case '0':	return 0;
	case '1':	return 1;
	case '2':	return 2;
	case '3':	return 3;
	case '4':	return 4;
	case '5':	return 5;
	case '6':	return 6;
	case '7':	return 7;
	case '8':	return 8;
	case '9':	return 9;
	case 'A':	return 10;
	case 'B':	return 11;
	case 'C':	return 12;
	case 'D':	return 13;
	case 'E':	return 14;
	case 'F':	return 15;
	default:	KCheck(FALSE); return '?';
	}

	__UNGUARD__ return '?';
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CharToString(const char *pIn, uint32 InLength, char *pOut)
{
	__GUARD__ if(InLength == 0)
	{
		return FALSE;
	}

	/*~~~~~~~~~~~~~*/
	uint32	iOut = 0;
	/*~~~~~~~~~~~~~*/

	for(uint32 _Idx = 0; _Idx < InLength; _Idx++)
	{
		if(pIn[_Idx] == 0)
		{
			pOut[iOut] = -125;
		}
		else
		{
			pOut[iOut] = pIn[_Idx];
		}

		iOut++;
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL StringToChar(const char *pIn, uint32 InLength, char *pOut, uint32 OutLimit, uint32 &OutLength)
{
	__GUARD__ if(InLength == 0)
	{
		return FALSE;
	}

	/*~~~~~~~~~~~~~*/
	uint32	iOut = 0;
	uint32	_Idx;
	/*~~~~~~~~~~~~~*/

	for(_Idx = 0; _Idx < InLength - 1; ++_Idx)
	{
		if(pIn[_Idx] == '\0')
		{
			break;
		}

		if(pIn[_Idx] == -125)
		{
			pOut[iOut] = 0;
		}
		else
		{
			pOut[iOut] = pIn[_Idx];
		}

		iOut++;
		if(iOut >= OutLimit) break;
	}

	OutLength = iOut;
	return TRUE;
	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL Binary2Str(const char *pIn, uint32 InLength, char *pOut, uint32 OutLimit)
{
	__GUARD__ if(InLength == 0)
	{
		return FALSE;
	}

	KCheck(InLength * 2 < OutLimit);

	/*~~~~~~~~~~~~~*/
	uint32	iOut = 0;
	/*~~~~~~~~~~~~~*/

	for(uint32 _Idx = 0; _Idx < InLength; _Idx++)
	{
		pOut[iOut] = Data2Ascii(((uchar) pIn[_Idx] & 0xF0) >> 4);
		iOut++;
		pOut[iOut] = Data2Ascii(pIn[_Idx] & 0x0F);
		iOut++;
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL Str2Binary(const char *pIn, uint32 InLength, char *pOut, uint32 OutLimit, uint32 &OutLength)
{
	__GUARD__ if(InLength == 0)
	{
		return FALSE;
	}

	/*~~~~~~~~~~~~~*/
	uint32	iOut = 0;
	uint32	_Idx;
	/*~~~~~~~~~~~~~*/

	for(_Idx = 0; _Idx < InLength - 1;)
	{
		if(pIn[_Idx] == '\0' || pIn[_Idx + 1] == '\0')
		{
			break;
		}

		pOut[iOut] = (Ascii2Data(pIn[_Idx]) << 4) + Ascii2Data(pIn[_Idx + 1]);
		iOut++;
		_Idx += 2;
		if(iOut >= OutLimit) break;
	}

	OutLength = iOut;
	return TRUE;
	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CheckStr(const char *pIn, uint32 InLength)
{
	if(InLength == 0)
	{
		return FALSE;
	}

	for(uint32 _Idx = 0; _Idx < InLength; _Idx++)
	{
		switch(pIn[_Idx])
		{
		case '\0':
			{
				return TRUE;
				break;
			}

		case '\'':
		case '\"':
		case ')':
		case '(':
		case '=':
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void ODBCEscapeString(const char *in, uint32 len, char *out)
{
	const unsigned char byANSIBegin = 0X20;
	const unsigned char byANSIEnd = 0X80;
	const unsigned char by1GBKBegin = 0X81;
	const unsigned char by1GBKEnd = 0XFE;
	const unsigned char by2GBKBegin1 = 0X40;
	const unsigned char by2GBKEnd1 = 0X7E;
	const unsigned char by2GBKBegin2 = 0X80;
	const unsigned char by2GBKEnd2 = 0XFE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	unsigned char	*from = (unsigned char *) in;
	unsigned char	*to =  (unsigned char *) out;
	uint32	tolen = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(len < 2) return;

	len--;
	while(*from != '\0' && tolen < len)
	{
		// 先判断接下来的两个字符是否为一个GBK汉字，如果是汉字就不需要做转义 [9/30/2015 yangzhen]
		if(*from >= by1GBKBegin && *from <= by1GBKEnd && (tolen + 1 < len))
		{
			unsigned char nextChar = *(from + 1);
			if(nextChar >= by2GBKBegin1 && nextChar <= by2GBKEnd1 || nextChar >= by2GBKBegin2 && nextChar <= by2GBKEnd2)
			{
				// 确定是一个汉字，直接将汉字拷贝到结果中 [9/30/2015 yangzhen]
				*to++ = *from++;
				*to++ = *from++;
				tolen += 2;
				continue;
			}
		}

		if(*from == '\\' || *from == '\'')
		{
			if(tolen >= (len - 2)) break;
			*to++ = '\\';
			tolen++;
		}

		*to++ = *from++;
		tolen++;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
char MyToLower(char c)
{
	return(c < 'A' || c > 'Z') ? (c) : (c + 'a' - 'A');
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void StringLower(char *pStr)
{
	KCheck(pStr);
	for(int32 _Idx = 0; _Idx < strlen(pStr); _Idx++)
	{
		pStr[_Idx] = MyToLower(pStr[_Idx]);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 GetValueByBit(uint32 nDataValue, int32 nOffset, int32 nSize)
{
	KCheck(nOffset >= 0);
	KCheck(nSize > 0);
	KCheck((nOffset + nSize) <= 32);

	if(nOffset >= 0) nDataValue = nDataValue << (32 - nSize - nOffset);
	if(nSize != 32) nDataValue = nDataValue >> (32 - nSize);

	return nDataValue;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 SetValueByBit(uint32 nDataValue, int32 nOffset, int32 nSize, int32 nValue)
{
	KCheck(nOffset >= 0);
	KCheck(nSize > 0);
	KCheck((nOffset + nSize) <= 32);

	for(int32 _Idx = nOffset; _Idx < nSize + nOffset; _Idx++)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		int32	nBit = nValue & (1 << (_Idx - nOffset));
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(nBit)
		{
			nDataValue |= (1 << _Idx);
		}
		else
		{
			nDataValue &= (~(1 << _Idx));
		}
	}

	return nDataValue;
}

// #if defined(_70_CLIENT) && defined(_70_LOGIN) && defined(_70_WORLD)
// #define DUMP_LOG	"./Log/logincenter_dump.log"
// #endif
#if defined(_70_CLIENT) && defined(_70_SERVER) && defined(_70_WORLD)
#define DUMP_LOG	"./Log/mapserver_dump.log"
#endif
// #if defined(_70_SERVER) && defined(_70_WORLD) && defined(_70_LOGIN)
// #define DUMP_LOG	"./Log/glserver_dump.log"
// #endif
#if defined(__SHARE_MEM__)
#define DUMP_LOG	"./Log/shm_dump.log"
#endif
#if defined(_70_BILLING) && defined(_70_LOGIN) && (!defined(_70_WORLD))
#define DUMP_LOG	"./Log/billing_dump.log"
#endif
#if defined(_LOG_CENTER_DB) && defined(_70_DBCENTER) && defined(_70_LOGIN) && defined(_70_SHAREMEM)
#define DUMP_LOG	"./Log/dbcenter_dump.log"
#endif

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void StackDump(const char *type)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#if defined(__LINUX__) && !ANDROID && !IOS
	// Android和IOS下没有调用栈追踪函数 [JiangWei]
	void	*DumpArray[100];
	int32	Size = backtrace(DumpArray, 100);
	char	**symbols = backtrace_symbols(DumpArray, Size);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(symbols)
	{
		if(Size > 95) Size = 95;
		if(Size > 0)
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			FILE	*f = fopen(DUMP_LOG, "a");
			char	threadinfo[256] = { 0 };
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			sprintf( threadinfo, 
					"Version=%X threadid = %lu cause dump, (T=%d-%d-%d_%d:%d:%d) \r\n", 
					GET_CURRENT_VERSION(), 
					KGetCurrentTID(),
					GET_TIME().year(),
					GET_TIME().month(),
					GET_TIME().day(),
					GET_TIME().hour(),
					GET_TIME().minute(),
					GET_TIME().second() );
			fwrite(threadinfo, 1, strlen(threadinfo), f);
			fwrite(type, 1, strlen(type), f);
			for(int32 _Idx = 0; _Idx < Size; _Idx++)
			{
				printf("%s\r\n", symbols[_Idx]);
				fwrite(symbols[_Idx], 1, strlen(symbols[_Idx]), f);
				fwrite("\r\n", 1, 2, f);
			}

			fwrite("\r\n", 1, 2, f);

			fclose(f);
		}

		free(symbols);
	}
	else
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		FILE	*f = fopen(DUMP_LOG, "a");
		char	buffer[256] = { 0 };
		char	threadinfo[256] = { 0 };
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		sprintf( threadinfo, 
				"Version=%X threadid = %lu cause dump, (T=%d-%d-%d_%d:%d:%d) \r\n", 
				GET_CURRENT_VERSION(), 
				KGetCurrentTID(),
				GET_TIME().year(),
				GET_TIME().month(),
				GET_TIME().day(),
				GET_TIME().hour(),
				GET_TIME().minute(),
				GET_TIME().second() );
		fwrite(threadinfo, 1, strlen(threadinfo), f);
		fwrite(type, 1, strlen(type), f);
		fclose(f);
	}

#endif
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void tsnprintf_s(char *buffer, size_t count, const char *format, ...)
{
	/*~~~~~~~~~~~*/
	va_list argptr;
	/*~~~~~~~~~~~*/

	va_start(argptr, format);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32	nchars = tvsnprintf((char *) buffer, count, format, argptr);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	va_end(argptr);

	if(nchars == count)
	{
		buffer[count - 1] = '\0';
	}

	if(nchars == -1)
	{
		KCheck(FALSE);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL ReachTime(uint32 desttime, uint32 now)
{
	if(desttime > now)
	{
		return((uint32) (desttime - now)) > ((uint32) 0x7FFFFFFF);
	}
	else
	{
		return((uint32) (now - desttime)) < ((uint32) 0x7FFFFFFF);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
inline void CalcCrc32(const uchar byte, uint32 &dwCrc32)
{
	dwCrc32 = ((dwCrc32) >> 8) ^ __crc32[(byte) ^ ((dwCrc32) & 0x000000FF)];
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 CRC(const char *szBuff, int32 nLen)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	dwCrc32 = 0xFFFFFFFF;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(int32 _Idx = 0; _Idx < nLen; _Idx++)
	{
		CalcCrc32(szBuff[_Idx], dwCrc32);
	}

	return dwCrc32;
}

uint32 CheckSum(const char *szBuff, int32 nLen)
{
	uint32	dwCrc32 = 0xFFFFFFFF;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(int32 _Idx = nLen - 1; _Idx >= 0 ; --_Idx)
	{
		CalcCrc32(szBuff[_Idx], dwCrc32);
	}

	return dwCrc32;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint64 StrToUINT64(const char *szBuff)
{
	/*~~~~~~~~~~~~*/
	uint64	ret = 0;
	/*~~~~~~~~~~~~*/

#if defined(__WINDOWS__)
	sscanf(szBuff, "%I64u", &ret);
#elif defined(__LINUX__)
	sscanf(szBuff, "%llu", &ret);
#endif
	return ret;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void PrintU64(uint64 value)
{
#if defined(__WINDOWS__)
	printf("%I64u\n", value);
#elif defined(__LINUX__)
	printf("%llu", value);
#endif
}

uint32			GetHighSection(uint64 uValue)
{
	return (uint32)((uValue >> 32) & 0xFFFFFFFF);
}

uint32			GetLowSection(uint64 uValue)
{
	return (uint32)(uValue & 0xFFFFFFFF);
}

uint64			ToUint64(uint32 uHigh, uint32 uLow)
{
	uint64 u64 = uHigh;
	u64 = ((u64 << 32) | uLow);
	return u64;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL GetStrIP(IP_t IP, char *szIP)
{
	/*~~~~~~~~~*/
	in_addr addr;
	/*~~~~~~~~~*/

#ifdef __WINDOWS__
	addr.S_un.S_addr = IP;
#else
	addr.s_addr = IP;
#endif

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	*szRet = inet_ntoa(addr);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(NULL == szRet) return FALSE;
	tsnprintf_s(szIP, IP_SIZE, szRet);
	return TRUE;
}


uint32 KLRandom::		m_uKey = 0;
char KLRandom::		m_pKey[MAX_KEY_SIZE];
FastLock KLRandom::	m_Lock;

RandGenerator				g_RandGenerator((unsigned) time(NULL));

/*
=======================================================================================================================
=======================================================================================================================
*/

KLRandom::KLRandom()
{
}

/*
=======================================================================================================================
=======================================================================================================================
*/
KLRandom::~KLRandom()
{
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void KLRandom::SetRand(uint32 uKey)
{
	lock_guard<FastLock> autolock(m_Lock);

	m_uKey = uKey % MAX_KEY_SIZE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 KLRandom::Rand()
{
	lock_guard<FastLock> autolock(m_Lock);

	if(m_uKey == 0)
	{
		srand((unsigned) time(NULL));
		for(int32 _Idx = 0; _Idx < MAX_KEY_SIZE; _Idx++)
		{
			m_pKey[_Idx] = rand() % 255;
		}
	}

	m_uKey++;
	if(m_uKey >= MAX_KEY_SIZE - sizeof(uint32)) m_uKey = 1;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	nRet = *((uint32 *) (m_pKey + m_uKey));
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return nRet;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 RandGenerator::GetRand(int32 nStart, int32 nEnd)
{
	KCheck((nEnd - nStart + 1) > 0);
	return(g_RandGenerator.RandUInt()) % (nEnd - nStart + 1) + (nStart);
}

/*
=======================================================================================================================
=======================================================================================================================
*/
SeedRand::SeedRand()
{
	holdrand = 1;
	seed = 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
SeedRand::~SeedRand()
{
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void SeedRand::SetSeed(int32 s)
{
	holdrand = seed = s;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32 SeedRand::Rand()
{
	return(((holdrand = holdrand * 214013 + 2531011) >> 16) & 0x7fff);
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32 SeedRand::GetSeed()
{
	return seed;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32 SeedRand::GetRandByKey(int32 s, int32 key)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32	rand = key * 214013 + s;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return rand;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32 SeedRand::GetKeyByRand(int32 s, int32 rand)
{
	/*~~~~~~~~~~~~~~~~~~~*/
	int32	key = rand - s;
	/*~~~~~~~~~~~~~~~~~~~*/

	key = key / 214013;

	return key;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL CheckAlphaNumFilter(const char* pStr)
{
	KCheck(pStr);
	int len = (int)strlen(pStr);
	for(int i = 0; i < len; ++i)
	{
		char c = pStr[i];
		if ('a' <= c && c <= 'z')
		{
			continue;
		}
		if ('A' <= c && c <= 'Z')
		{
			continue;
		}
		if ('0' <= c && c <= '9')
		{
			continue;
		}
		if ('-' == c)
		{
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CheckIsANumString(const char* pStr)
{
	if (NULL == pStr)
	{
		return FALSE;
	}
	int len = (int)strlen(pStr);
	for(int i = 0; i < len; ++i)
	{
		char c = pStr[i];
		if (!('0' <= c && c <= '9'))
		{
			return FALSE;
		}
	}
	return TRUE;
}

const char BASE64_CODE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  

//编码，参数：要编码的字符串指针，解码后存放的位置（编码字串长度的4/3），要编码的字符串长度 ->返回结果长度  
int Base64Encode(char *lpBuffer, const char *lpString, int sLen)   
{ 
	int vLen = 0;
	while(sLen > 0)      //处理整个字符串  
	{   
		*lpBuffer++ = BASE64_CODE[(lpString[0] >> 2 ) & 0x3F];  //右移两位，与00111111是防止溢出，自加  
		if(sLen > 2) //够3个字符  
		{   
			*lpBuffer++ = BASE64_CODE[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];  
			*lpBuffer++ = BASE64_CODE[((lpString[1] & 0xF) << 2) | (lpString[2] >> 6)];  
			*lpBuffer++ = BASE64_CODE[lpString[2] & 0x3F];  
		}
		else  
		{   
			switch(sLen)    //追加“=”  
			{  
			case 1:  
				*lpBuffer ++ = BASE64_CODE[(lpString[0] & 3) << 4 ];  
				*lpBuffer ++ = '=';  
				*lpBuffer ++ = '=';  
				break;  
			case 2:  
				*lpBuffer ++ = BASE64_CODE[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];  
				*lpBuffer ++ = BASE64_CODE[((lpString[1] & 0x0F) << 2) | (lpString[2] >> 6)];  
				*lpBuffer ++ = '=';  
				break;  
			}  
		} 
		lpString += 3;  
		sLen -= 3;  
		vLen +=4;  
	}  
	*lpBuffer = 0;  
	return vLen;
} 

//子函数 - 取密文的索引  
inline char Base64GetCharIndex(char c) //内联函数可以省去函数调用过程，提速  
{   
	if((c >= 'A') && (c <= 'Z'))  
	{   
		return c - 'A';  
	}
	else if((c >= 'a') && (c <= 'z'))  
	{   
		return c - 'a' + 26;  
	}
	else if((c >= '0') && (c <= '9'))  
	{   
		return c - '0' + 52;  
	}
	else if(c == '+')  
	{   
		return 62;  
	}
	else if(c == '/')  
	{   
		return 63;  
	}
	else if(c == '=')  
	{   
		return 0;  
	}  
	return 0;  
}  

//解码，参数：结果，密文，密文长度  
int Base64Decode(char *lpBuffer, const char *lpSrc, int sLen)   //解码函数  
{   
	static char lpCode[4];  
	int vLen = 0;  
	if(sLen % 4)        //Base64编码长度必定是4的倍数，包括'='  
	{   
		lpBuffer[0] = '\0';  
		return -1;  
	}  
	while(sLen > 2)      //不足三个字符，忽略  
	{   
		lpCode[0] = Base64GetCharIndex(lpSrc[0]);  
		lpCode[1] = Base64GetCharIndex(lpSrc[1]);  
		lpCode[2] = Base64GetCharIndex(lpSrc[2]);  
		lpCode[3] = Base64GetCharIndex(lpSrc[3]);  

		*lpBuffer++ = (lpCode[0] << 2) | (lpCode[1] >> 4);  
		*lpBuffer++ = (lpCode[1] << 4) | (lpCode[2] >> 2);  
		*lpBuffer++ = (lpCode[2] << 6) | (lpCode[3]);  

		lpSrc += 4;  
		sLen -= 4;  
		vLen += 3;  
	}
	return vLen;  
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
unsigned char UrlCharToHex(unsigned char x)   
{   
	return  x > 9 ? x + 55 : x + 48;   
}  

unsigned char UrlCharFromHex(unsigned char x)   
{   
	unsigned char y;  
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;  
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;  
	else if (x >= '0' && x <= '9') y = x - '0';  
	else y = 0;	// 不应出现的情况 [4/27/2015 yangzhen]
	return y;  
}  

int32 UrlEncode(const char* szSrc, uint32 uSrcLen, char* szDest, uint32 uDestLen)
{
	if(szSrc == NULL || szDest == NULL)
		return -1;

	memset(szDest, 0, uDestLen);
	uint32 uOffset = 0;
	for (uint32 i = 0; i < uSrcLen; i++)
	{
		if ((isalnum((unsigned char)szSrc[i]) ||   
			(szSrc[i] == '-') ||  
			(szSrc[i] == '_') ||   
			(szSrc[i] == '.') ||   
			(szSrc[i] == '~')) && uOffset < uDestLen)
		{
			szDest[uOffset++] = szSrc[i];
		}
		else if (szSrc[i] == ' ' && uOffset < uDestLen)
		{
			szDest[uOffset++] = '+';
		}
		else if(uOffset + 2 < uDestLen)
		{
			szDest[uOffset++] = '%';
			szDest[uOffset++] = UrlCharToHex((unsigned char)szSrc[i] >> 4);
			szDest[uOffset++] = UrlCharToHex((unsigned char)szSrc[i] % 16);
		}
		else
		{
			return -1;
		}

		if(uOffset >= uDestLen)
		{
			return -1;
		}
	}  
	return uOffset;
}

int32 UrlDecode(const char* szSrc, uint32 uSrcLen, char* szDest, uint32 uDestLen)
{
	if(szDest == NULL || szSrc == NULL)
		return -1;

	uint32 uOffset = 0;
	uint32 uDestSize = 0;
	memset(szDest, 0, uDestLen);
	for (uDestSize = 0; uDestSize < uDestLen; uDestSize++)  
	{
		if (szSrc[uOffset] == '+') 
		{
			szDest[uDestSize] += ' ';
			uOffset++;
		}
		else if (szSrc[uOffset] == '%')
		{
			if(uOffset + 2 < uSrcLen)
			{
				uOffset++;
				unsigned char high = UrlCharFromHex((unsigned char)szSrc[uOffset++]);  
				unsigned char low = UrlCharFromHex((unsigned char)szSrc[uOffset++]);
				szDest[uDestSize] += high*16 + low;
			}
			else
			{
				break;
			}
		}
		else
		{
			szDest[uDestSize] += szSrc[uOffset];
			uOffset++;
		}

		if(uOffset >= uSrcLen)
		{
			break;
		}
	}
	szDest[uDestLen - 1] = 0;
	return uDestSize;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
//  (_start, _end] 左开右闭
bool IntIsBetweenlR(int x, int _start, int _end) { return ((x > _start) && (x <= _end)); }

//  (_start, _end) 左开右开
bool IntIsBetweenlr(int x, int _start, int _end) { return ((x > _start) && (x < _end)); }

//  [_start, _end) 左闭右开
bool IntIsBetweenLr(int x, int _start, int _end) { return ((x >= _start) && (x < _end)); }

//  [_start, _end] 左闭右闭
bool IntIsBetweenLR(int x, int _start, int _end) { return ((x >= _start) && (x <= _end)); }
/*
=======================================================================================================================
=======================================================================================================================
*/

#if defined(_70_WORLD)

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL MyDecoder::DecodeLogin(const char *szVerifyCode, char *szAccount)
{
	if(NULL == szVerifyCode)
	{
		return FALSE;
	}

	if(strlen(szAccount) > MAX_ACCOUNT) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	tmpAccount[MAX_ACCOUNT + 1] = { 0 };
	char	szCode[MAX_CL_VERIFY_CODE + 1] = { 0 };
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	memcpy(szCode, szVerifyCode, MAX_CL_VERIFY_CODE);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	offset = szCode[MAX_CL_VERIFY_CODE - 1];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(int32 verifyIndex = 0; verifyIndex < MAX_ACCOUNT; verifyIndex++)
	{
		szCode[verifyIndex] += szCode[verifyIndex + MAX_ACCOUNT];
	}

	if(offset >= 0 && offset < MAX_ACCOUNT)
	{
		memcpy(tmpAccount, &szCode[offset], MAX_ACCOUNT - offset);

		if(strlen(tmpAccount) < strlen(szAccount))
		{
			memcpy(tmpAccount + strlen(tmpAccount), szCode, strlen(szAccount) - strlen(tmpAccount));
		}

		tmpAccount[strlen(szAccount)] = 0;
	}

	if(strcmp(szAccount, tmpAccount) == 0)
	{
		return TRUE;
	}

	return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL MyDecoder::DecodeCharMove(const char *szVerifyCode, const GLPos *paTarPos, uint32 offset)
{
	__GUARD__ if(NULL == paTarPos || NULL == szVerifyCode)
	{
		return FALSE;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	checkSum = 0;
	char	szTmp[MAX_KHAN_CHAR_PATH_NODE_NUMBER + 1] = { 0 };
	int32	nPosX = Float2Int(paTarPos[0].m_fX);
	int32	nPosZ = Float2Int(paTarPos[0].m_fZ);
	int32	nPos = nPosX + nPosZ;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	tsnprintf(szTmp, MAX_KHAN_CHAR_PATH_NODE_NUMBER, "%d", nPos);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32	nStrLen = (int32) strlen(szTmp);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(nStrLen >= MAX_KHAN_CHAR_PATH_NODE_NUMBER)
	{
		return FALSE;
	}

	for(int32 len = 0; len < nStrLen; ++len) checkSum = checkSum ^ szTmp[len];

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	n3 = offset % 10;
	uint32	n2 = ((offset % 100) - n3) / 10;
	uint32	n1 = offset / 100;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(n3 >= MAX_CHAR_MOVE_VERIFY_CODE || n2 >= MAX_CHAR_MOVE_VERIFY_CODE || n1 >= MAX_CHAR_MOVE_VERIFY_CODE)
	{
		return TRUE;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	csh = (char) ((szVerifyCode[n3] & 0x0F) + ((szVerifyCode[n2] - n1) << 4));
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(checkSum != csh || 0 < szVerifyCode[n1] - n1)
	{
		return FALSE;
	}

	return TRUE;
	__UNGUARD__ return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL MyDecoder::DecodeLockTarget(const char *szVerifyCode, ut_id_obj nTargetID, uint32 offset)
{
	if(NULL == szVerifyCode)
	{
		return FALSE;
	}

	if(szVerifyCode[1] == TRUE)
	{
		return FALSE;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	n3 = offset % 10;
	uint32	n2 = ((offset % 100) - n3) / 10;
	uint32	n1 = offset / 100;
	char	szTarget[MAX_LOCK_TARGET_VERIFY_CODE + 1] = { 0 };
	char	len = szVerifyCode[MAX_LOCK_TARGET_VERIFY_CODE - 1];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(len < 0 || len >= MAX_LOCK_TARGET_VERIFY_CODE)
	{
		return FALSE;
	}

	memcpy(szTarget, szVerifyCode + 2, len);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	ut_id_obj target = (ut_id_obj) atoi(szTarget);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	target = ~target;
	target -= (n2 * 100 + n1 * 10 + n3);
	target = target ^ offset;

	if(nTargetID != target)
	{
		return FALSE;
	}

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL MyDecoder::DecodeHeartBeat(const char *szVerifyCode, uint32 randNum, uint32 offset)
{
	if(NULL == szVerifyCode)
	{
		return FALSE;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	n3 = offset % 10;
	uint32	n2 = ((offset % 100) - n3) / 10;
	uint32	n1 = offset / 100;
	char	szTarget[MAX_LOCK_HEARTBEAT_VERIFY_CODE] = { 0 };
	char	len = szVerifyCode[MAX_LOCK_HEARTBEAT_VERIFY_CODE - 1];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(len < 0 || len >= MAX_LOCK_HEARTBEAT_VERIFY_CODE)
	{
		return FALSE;
	}

	memcpy(szTarget, szVerifyCode + 4, len);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	ut_id_obj targetId = (ut_id_obj) atoi(szTarget);
	int32	tmp = (n3 & n2) | n1;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	targetId = targetId ^ tmp;

	if(randNum != targetId)
	{
		return FALSE;
	}

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL MyDecoder::DecodeMD5PW(const char *szVerifyCode, char *szSource, int32 iSize, uint32 key)
{
	if(NULL == szVerifyCode || NULL == szSource)
	{
		return FALSE;
	}

	for(int32 _Idx = 0; _Idx < iSize; ++_Idx) szSource[_Idx] = szVerifyCode[_Idx] ^ key;

	return TRUE;
}
#endif

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 MathSinCos::	mTrigTableSize;
float MathSinCos::	m_Trig_Table_Factor;
float *MathSinCos:: mSinTable = 0;

MathSinCos		g_MathSinCos;

MathSinCos::MathSinCos(uint32 trigTableSize)
{
	mTrigTableSize = trigTableSize;
	m_Trig_Table_Factor = mTrigTableSize / (2 * __PI);

	mSinTable = new float[mTrigTableSize];

	buildTrigTables();
}

MathSinCos::~MathSinCos()
{
	delete[] mSinTable;
}

void MathSinCos::buildTrigTables(void)
{
	float	angle;

	for(int32 _Idx = 0; _Idx < mTrigTableSize; ++_Idx)
	{
		angle = 2 * __PI * _Idx / mTrigTableSize;
		mSinTable[_Idx] = sin(angle);
	}
}

float MathSinCos::SinTable(float fValue)
{
	int32	idx;
	if(fValue >= 0)
	{
		idx = int32(fValue * m_Trig_Table_Factor) % mTrigTableSize;
	}
	else
	{
		idx = mTrigTableSize - (int32(-fValue * m_Trig_Table_Factor) % mTrigTableSize) - 1;
	}

	return mSinTable[idx];
}

/////////////////////////////////////////////////////////////////////////////////////////
// Constants for MD5Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

// F, G, H and I are basic MD5 functions.
inline MD5Encoder::uint4 MD5Encoder::F(uint4 x, uint4 y, uint4 z) {
	return x&y | ~x&z;
}

inline MD5Encoder::uint4 MD5Encoder::G(uint4 x, uint4 y, uint4 z) {
	return x&z | y&~z;
}

inline MD5Encoder::uint4 MD5Encoder::H(uint4 x, uint4 y, uint4 z) {
	return x^y^z;
}

inline MD5Encoder::uint4 MD5Encoder::I(uint4 x, uint4 y, uint4 z) {
	return y ^ (x | ~z);
}

// rotate_left rotates x left n bits.
inline MD5Encoder::uint4 MD5Encoder::rotate_left(uint4 x, int n) {
	return (x << n) | (x >> (32-n));
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void MD5Encoder::FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a+ F(b,c,d) + x + ac, s) + b;
}

inline void MD5Encoder::GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + G(b,c,d) + x + ac, s) + b;
}

inline void MD5Encoder::HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + H(b,c,d) + x + ac, s) + b;
}

inline void MD5Encoder::II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + I(b,c,d) + x + ac, s) + b;
}

//////////////////////////////////////////////

// default ctor, just initailize
MD5Encoder::MD5Encoder()
{
	init();
}

// nifty shortcut ctor, compute MD5 for string and finalize it right away
MD5Encoder::MD5Encoder(const std::string &text)
{
	init();
	update(text.c_str(), text.length());
	finalize();
}

MD5Encoder::MD5Encoder(const char* text, int len)
{
	init();
	update(text, len);
	finalize();
}

void MD5Encoder::init()
{
	finalized=false;

	count[0] = 0;
	count[1] = 0;

	// load magic initialization constants.
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
}

// decodes input (unsigned char) into output (uint4). Assumes len is a multiple of 4.
void MD5Encoder::decode(uint4 output[], const uint1 input[], size_type len)
{
	for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((uint4)input[j]) | (((uint4)input[j+1]) << 8) |
		(((uint4)input[j+2]) << 16) | (((uint4)input[j+3]) << 24);
}

// encodes input (uint4) into output (unsigned char). Assumes len is
// a multiple of 4.
void MD5Encoder::encode(uint1 output[], const uint4 input[], size_type len)
{
	for (size_type i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = input[i] & 0xff;
		output[j+1] = (input[i] >> 8) & 0xff;
		output[j+2] = (input[i] >> 16) & 0xff;
		output[j+3] = (input[i] >> 24) & 0xff;
	}
}

// apply MD5 algo on a block
void MD5Encoder::transform(const uint1 block[blocksize])
{
	uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	decode (x, block, blocksize);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	// Zeroize sensitive information.
	memset(x, 0, sizeof(x));
}

// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block
void MD5Encoder::update(const unsigned char input[], size_type length)
{
	// compute number of bytes mod 64
	size_type index = count[0] / 8 % blocksize;

	// Update number of bits
	if ((count[0] += (length << 3)) < (length << 3))
		count[1]++;
	count[1] += (length >> 29);

	// number of bytes we need to fill in buffer
	size_type firstpart = 64 - index;

	size_type i;

	// transform as many times as possible.
	if (length >= firstpart)
	{
		// fill buffer first, transform
		memcpy(&buffer[index], input, firstpart);
		transform(buffer);

		// transform chunks of blocksize (64 bytes)
		for (i = firstpart; i + blocksize <= length; i += blocksize)
			transform(&input[i]);

		index = 0;
	}
	else
		i = 0;

	// buffer remaining input
	memcpy(&buffer[index], &input[i], length-i);
}

// for convenience provide a verson with signed char
void MD5Encoder::update(const char input[], size_type length)
{
	update((const unsigned char*)input, length);
}

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
MD5Encoder& MD5Encoder::finalize()
{
	static unsigned char padding[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	if (!finalized) {
		// Save number of bits
		unsigned char bits[8];
		encode(bits, count, 8);

		// pad out to 56 mod 64.
		size_type index = count[0] / 8 % 64;
		size_type padLen = (index < 56) ? (56 - index) : (120 - index);
		update(padding, padLen);

		// Append length (before padding)
		update(bits, 8);

		// Store state in digest
		encode(digest, state, 16);

		// Zeroize sensitive information.
		memset(buffer, 0, sizeof buffer);
		memset(count, 0, sizeof count);

		finalized=true;
	}

	return *this;
}

// return hex representation of digest as string
std::string MD5Encoder::hexdigest() const
{
	if (!finalized)
		return "";

	char buf[33];
	for (int i=0; i<16; i++)
		sprintf(buf+i*2, "%02x", digest[i]);
	buf[32]=0;

	return std::string(buf);
}

void MD5Encoder::hexdigest(char* output, int outlen) const
{
	if (!finalized)
	{
		memset(output, 0, outlen);
	}

	for (int i=0; i<16 && i*2 < outlen; i++)
		sprintf(output+i*2, "%02x", digest[i]);
	output[32]=0;
}
//////////////////////////////////////////////////////////////////////////////////////////

char* strlwr(char *str, int32 nLen)
{
	int32 nStrLen = strlen(str);
	nStrLen = (nStrLen <= nLen) ? nStrLen : nLen;

	for (int32 i = 0; i < nStrLen; i++)
	{
		*(str + i)= tolower(*(str + i));
	}
	return str;
}

char* strupr(char *str, int32 nLen)
{
	int32 nStrLen = strlen(str);
	nStrLen = (nStrLen <= nLen) ? nStrLen : nLen;

	for (int32 i = 0; i < nStrLen; i++)
	{
		*(str + i) = toupper(*(str + i));
	}
	return str;
}


char* CheckLawLessChar(char *pInStr, int32 nLen)
{
#define LAWLESS_LEN (5)
	static char szLawLess[LAWLESS_LEN] = {'\n','\\','\t','\r','|'};
	char *pStr = pInStr;
	for (int32 _Idx = 0 ; *pStr != '\0' && _Idx < nLen; pStr++, _Idx++)
	{
		for(int32 i = 0; i < LAWLESS_LEN; i++)
		{
			if (*pStr == szLawLess[i])
			{
				*pStr = ' ';
				break;
			}
		}
	}
	return	pInStr;
}

BOOL UTF8ToGBK(const char *inbuf, size_t inlen,char *outbuf, size_t outlen)
{
	#if defined(__WINDOWS__)
		return WindowsUTF8ToGBK(inbuf, inlen, outbuf, outlen);
	#else
		return CodeConvert("UTF-8", "GBK", inbuf, inlen, outbuf, outlen);
	#endif
}

BOOL GBKToUTF8(const char *inbuf, size_t inlen,char *outbuf, size_t outlen)
{
	#if defined(__WINDOWS__)
		return WindowsGBKToUTF8(inbuf, inlen, outbuf, outlen);
	#else
		return CodeConvert("GBK", "UTF-8", inbuf, inlen, outbuf, outlen);
	#endif
}

BOOL Big5ToGBK(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	#if defined(__WINDOWS__)
		return WindowsBig5ToGBK(inbuf, inlen, outbuf, outlen);
	#else
		return CodeConvert("BIG5", "GBK", inbuf, inlen, outbuf, outlen);
	#endif
}

BOOL GBKToBig5(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	#if defined(__WINDOWS__)
		return WindowsGBKToBig5(inbuf, inlen, outbuf, outlen);
	#else
		return CodeConvert("GBK", "BIG5", inbuf, inlen, outbuf, outlen);
	#endif
}

BOOL Big5ToUTF8(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
#if defined(__WINDOWS__)
	return WindowsBig5ToUTF8(inbuf, inlen, outbuf, outlen);
#else
	return CodeConvert("Big5", "UTF8", inbuf, inlen, outbuf, outlen);
#endif
}

BOOL UTF8ToBig5(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
#if defined(__WINDOWS__)
	return WindowsUTF8ToBig5(inbuf, inlen, outbuf, outlen);
#else
	return CodeConvert("UTF8", "Big5", inbuf, inlen, outbuf, outlen);
#endif
}

#if defined(__WINDOWS__)
//GBK编码转换到UTF8编码
BOOL WindowsGBKToUTF8(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)  
{ 
	if (inbuf == NULL || outbuf == NULL)
	{
		return FALSE;
	}

	// gbk to unicode [7/17/2015 yangzhen]
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, inbuf, -1, NULL, 0);  
	wchar_t* strUnicode = new wchar_t[unicodeLen + 1];  
	memset(strUnicode, 0, sizeof(wchar_t) * (unicodeLen + 1));
	MultiByteToWideChar(CP_ACP, 0, inbuf, -1, strUnicode, unicodeLen);  

	// unicode to utf8 [7/17/2015 yangzhen]
	int32 utf8Len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);  
	if(utf8Len >= outlen)
	{
		delete[] strUnicode;  
		strUnicode = NULL;  
		return FALSE;
	}
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, outbuf, utf8Len, NULL, NULL);
	delete[] strUnicode;  
	strUnicode = NULL;  

	return TRUE;
}  

BOOL WindowsUTF8ToGBK(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)  
{
	if (inbuf == NULL || outbuf == NULL)
	{
		return FALSE;
	}

	// utf8 to unicode [7/17/2015 yangzhen]
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, NULL, 0);  
	wchar_t* strUnicode = new wchar_t[unicodeLen + 1];  
	memset(strUnicode, 0, sizeof(wchar_t) * (unicodeLen + 1));  
	MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, strUnicode, unicodeLen);  

	// unicode to gbk [7/17/2015 yangzhen]
	int32 gbkLen = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);  
	if(gbkLen >= outlen)
	{
		delete[] strUnicode;  
		strUnicode = NULL;  
		return FALSE;
	}
	WideCharToMultiByte(CP_ACP,0, strUnicode, -1, outbuf, gbkLen, NULL, NULL); 
	delete[] strUnicode;  
	strUnicode = NULL;  
	
	return TRUE;
}

BOOL WindowsBig5ToGBK(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	if (inbuf == NULL || outbuf == NULL)
	{
		return FALSE;
	}

	// big5 to unicode [7/17/2015 yangzhen]
	int unicodeLen = MultiByteToWideChar(950, 0, inbuf, -1, NULL, 0);  
	wchar_t* strUnicode = new wchar_t[unicodeLen + 1];  
	memset(strUnicode, 0, sizeof(wchar_t) * (unicodeLen + 1));  
	MultiByteToWideChar(950, 0, inbuf, -1, strUnicode, unicodeLen);

	// unicode to gbk [7/17/2015 yangzhen]
	int32 gbkLen = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);  
	if(gbkLen >= outlen)
	{
		delete[] strUnicode;  
		strUnicode = NULL;  
		return FALSE;
	}
	WideCharToMultiByte(CP_ACP,0, strUnicode, -1, outbuf, gbkLen, "?", NULL); 
	delete[] strUnicode;  
	strUnicode = NULL;

	return TRUE;
}

BOOL WindowsGBKToBig5(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	if (inbuf == NULL || outbuf == NULL)
	{
		return FALSE;
	}

	// gbk to unicode [7/17/2015 yangzhen]
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, inbuf, -1, NULL, 0);  
	wchar_t* strUnicode = new wchar_t[unicodeLen + 1];  
	memset(strUnicode, 0, sizeof(wchar_t) * (unicodeLen + 1));
	MultiByteToWideChar(CP_ACP, 0, inbuf, -1, strUnicode, unicodeLen);

	// unicode to big5 [7/17/2015 yangzhen]
	int32 gbkLen = WideCharToMultiByte(950, 0, strUnicode, -1, NULL, 0, NULL, NULL); 
	if(gbkLen >= outlen)
	{
		delete[] strUnicode;
		strUnicode = NULL;
		return FALSE;
	}
	WideCharToMultiByte(950,0, strUnicode, -1, outbuf, gbkLen, "?", NULL); 
	delete[] strUnicode;  
	strUnicode = NULL;  

	return TRUE;
}

extern BOOL WindowsBig5ToUTF8(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	if (inbuf == NULL || outbuf == NULL)
	{
		return FALSE;
	}

	// big5 to unicode [7/17/2015 yangzhen]
	int unicodeLen = MultiByteToWideChar(950, 0, inbuf, -1, NULL, 0);  
	wchar_t* strUnicode = new wchar_t[unicodeLen + 1];  
	memset(strUnicode, 0, sizeof(wchar_t) * (unicodeLen + 1));  
	MultiByteToWideChar(950, 0, inbuf, -1, strUnicode, unicodeLen);

	// unicode to gbk [7/17/2015 yangzhen]
	int32 utfLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);  
	if(utfLen >= outlen)
	{
		delete[] strUnicode;  
		strUnicode = NULL;  
		return FALSE;
	}
	WideCharToMultiByte(CP_UTF8,0, strUnicode, -1, outbuf, utfLen, NULL, NULL); 
	delete[] strUnicode;  
	strUnicode = NULL;

	return TRUE;
}

extern BOOL WindowsUTF8ToBig5(const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	if (inbuf == NULL || outbuf == NULL)
	{
		return FALSE;
	}

	// big5 to unicode [7/17/2015 yangzhen]
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, NULL, 0);  
	wchar_t* strUnicode = new wchar_t[unicodeLen + 1];  
	memset(strUnicode, 0, sizeof(wchar_t) * (unicodeLen + 1));  
	MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, strUnicode, unicodeLen);

	// unicode to gbk [7/17/2015 yangzhen]
	int32 bigLen = WideCharToMultiByte(950, 0, strUnicode, -1, NULL, 0, NULL, NULL);  
	if(bigLen >= outlen)
	{
		delete[] strUnicode;  
		strUnicode = NULL;  
		return FALSE;
	}
	WideCharToMultiByte(950, 0, strUnicode, -1, outbuf, bigLen, NULL, NULL); 
	delete[] strUnicode;  
	strUnicode = NULL;

	return TRUE;
}

#else
BOOL CodeConvert(const char *from_charset, const char *to_charset,const char *inbuf, size_t inlen,char *outbuf, size_t outlen)
{  
	iconv_t cd;
    char* tmpInBuf = const_cast<char*>(inbuf);
	char **pin = &tmpInBuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return FALSE;
	memset(outbuf,0,outlen);
	if (iconv(cd, pin, &inlen,pout, &outlen)==-1)
		return FALSE;
	iconv_close(cd);
	outbuf[outlen - 1] = 0;

	return TRUE;
}
#endif

#ifdef __LINUX__
void createDir( const char* dir )
{
	mkdir( dir, S_IRWXU | S_IRWXG | S_IRWXO );
}
#endif

#ifdef __WINDOWS__
void createDir( const char* dir )
{
	mkdir( dir );
}
#endif

uint64 GetUniqueID(uint16 nServerID, uint16 nType, uint32 nID)
{

	SUniqueID UniqueID;
	UniqueID.mSUniqueIDInfo.mServerID = nServerID;
	UniqueID.mSUniqueIDInfo.mTypeID = nType;
	UniqueID.mSUniqueIDInfo.mID = nID;
	return UniqueID.mSUniqueID;
}

SUniqueID GetUniqueIDStruct(uint64 uUniqueID)
{
	SUniqueID UniqueID;
	UniqueID.mSUniqueID = uUniqueID;
	return UniqueID;
}
