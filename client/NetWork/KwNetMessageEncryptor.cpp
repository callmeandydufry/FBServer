#include "KwNetPrivate.h"
#include "KwNetMessageEncryptor.h"
#include "KwNetStreamPointer.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */
ShuffleDataArray::ShuffleData ShuffleDataArray::m_Array[MAX_SHUFFLE_DATA_ARRAY_SIZE] = 
{
        {16,13,5,22,21,4,8,17,9,6,10,11,12,23,18,14,19,20,15,3,7,2,0,1,},
        {12,1,2,9,16,18,7,22,8,21,17,0,4,13,15,6,10,11,23,3,5,14,20,19,},
        {5,2,8,0,6,21,13,22,16,17,19,23,12,14,11,18,7,1,9,4,10,15,3,20,},
        {11,16,5,7,4,18,22,3,12,23,2,13,1,21,14,15,17,0,6,9,10,8,19,20,},
        {13,9,5,8,10,7,4,2,16,11,18,17,3,1,0,23,22,19,21,6,12,20,14,15,},
        {12,19,8,15,16,5,3,21,17,14,13,4,20,1,22,10,18,11,2,23,6,0,7,9,},
        {6,20,22,2,0,1,21,16,4,18,15,17,8,11,10,7,3,13,5,23,12,19,9,14,},
        {18,11,22,0,15,6,17,9,12,20,21,19,10,14,1,16,2,23,7,5,4,8,3,13,},
        {19,9,23,10,17,15,12,20,18,11,4,13,8,3,1,6,7,2,14,0,22,16,5,21,},
        {1,0,12,22,15,17,7,18,13,6,23,21,3,2,10,20,4,5,9,11,19,16,14,8,},
        {3,1,17,4,14,18,19,13,7,10,15,8,22,9,11,23,2,21,0,5,6,20,12,16,},
        {12,16,8,7,5,18,10,14,20,3,9,23,1,15,0,22,6,19,4,21,2,13,11,17,},
        {8,17,19,7,22,12,6,13,14,0,15,3,4,20,16,23,21,18,11,1,9,10,2,5,},
        {16,10,2,4,6,14,20,8,11,7,12,23,22,21,1,13,17,0,19,5,9,18,3,15,},
        {23,6,3,19,1,12,14,10,4,15,8,2,22,9,21,17,16,18,11,13,20,7,0,5,},
        {15,6,2,4,16,10,3,13,14,22,1,18,21,20,7,19,12,17,8,9,0,11,23,5,},
        {3,2,9,21,0,6,22,20,16,14,12,7,23,18,1,5,19,4,11,8,10,15,13,17,},
        {1,18,0,14,22,23,8,4,10,17,13,7,12,11,15,6,16,20,5,9,21,2,19,3,},
        {1,0,14,4,13,8,15,16,5,11,2,17,23,7,18,12,3,21,10,9,22,6,20,19,},
        {7,8,2,3,14,16,9,4,0,17,23,6,13,15,18,5,11,19,1,21,20,10,12,22,},
        {8,22,15,14,7,9,0,23,3,4,16,19,11,18,20,2,1,21,13,6,5,17,12,10,},
        {20,16,18,23,2,0,19,3,12,15,22,17,4,21,11,10,8,6,5,13,1,7,14,9,},
        {22,0,13,11,15,6,5,16,10,21,17,3,12,8,18,7,20,19,2,23,14,9,4,1,},
        {16,10,5,14,1,11,9,8,17,0,20,3,2,22,4,15,23,18,21,13,19,7,6,12,},
        {11,8,23,17,7,2,18,6,21,15,20,14,16,10,22,13,19,12,5,1,9,0,4,3,},
        {0,8,3,14,17,15,10,16,21,5,4,23,7,20,6,12,2,19,9,18,11,22,13,1,},
        {0,17,8,18,7,22,2,12,21,16,6,9,11,23,20,4,13,10,5,14,1,3,15,19,},
        {14,1,9,5,3,15,8,19,16,2,21,6,7,18,11,20,12,0,4,17,10,22,23,13,},
        {6,11,18,2,7,15,0,17,10,23,16,3,9,13,8,5,21,22,12,4,19,1,14,20,},
        {22,7,2,11,9,21,5,13,1,6,17,14,23,15,4,20,10,12,3,8,18,0,16,19,},
        {4,6,14,11,15,5,0,22,16,8,13,19,17,23,20,7,12,18,2,3,10,1,9,21,},
        {9,7,8,13,18,10,2,0,14,1,11,12,17,23,15,3,20,16,5,21,19,22,6,4,},
};

ShuffleDataArray::ShuffleData* ShuffleDataArray::GetShuffleData(uint32 uIndex)
{
	if( uIndex >= MAX_SHUFFLE_DATA_ARRAY_SIZE )
		return NULL;
	return &m_Array[uIndex];
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
EncryptFunction::func_ptr EncryptFunction::m_pDoFuncPtr[MAX_ENCRYPT_FUNC_SIZE] = 
{
	DoFunc0,
	DoFunc1,
	DoFunc2,
	DoFunc3,
	DoFunc4,
	DoFunc5,
	DoFunc6,
	DoFunc7,
	DoFunc8,
	DoFunc9,
	DoFunc10,
	DoFunc11,
};

EncryptFunction::func_ptr EncryptFunction::m_pDeFuncPtr[MAX_ENCRYPT_FUNC_SIZE] = 
{
	DeFunc0,
	DeFunc1,
	DeFunc2,
	DeFunc3,
	DeFunc4,
	DeFunc5,
	DeFunc6,
	DeFunc7,
	DeFunc8,
	DeFunc9,
	DeFunc10,
	DeFunc11,
};


EncryptFunction::func_ptr EncryptFunction::GetDoFuncPtr(uint32 uIndex)
{
	if( uIndex >= MAX_ENCRYPT_FUNC_SIZE )
		return NULL;
	return m_pDoFuncPtr[uIndex];
}

EncryptFunction::func_ptr EncryptFunction::GetDeFuncPtr(uint32 uIndex)
{
	if( uIndex >= MAX_ENCRYPT_FUNC_SIZE )
		return NULL;
	return m_pDeFuncPtr[uIndex];
}

void EncryptFunction::DoFunc0(uchar* p,uint32 uMagic)		{ *p += 50; }
void EncryptFunction::DoFunc1(uchar* p,uint32 uMagic)		{ *p += 94; }
void EncryptFunction::DoFunc2(uchar* p,uint32 uMagic)		{ *p += 163; }
void EncryptFunction::DoFunc3(uchar* p,uint32 uMagic)		{ *p += 214; }
void EncryptFunction::DoFunc4(uchar* p,uint32 uMagic)		{ *p = ((*p)>>2) | ((*p)<<6); }
void EncryptFunction::DoFunc5(uchar* p,uint32 uMagic)		{ *p = ((*p)>>3) | ((*p)<<5); }
void EncryptFunction::DoFunc6(uchar* p,uint32 uMagic)		{ *p = ((*p)>>5) | ((*p)<<3); }
void EncryptFunction::DoFunc7(uchar* p,uint32 uMagic)		{ *p = ((*p)>>7) | ((*p)<<1); }
void EncryptFunction::DoFunc8(uchar* p,uint32 uMagic)		{ *p = (*p) ^ uMagic; }
void EncryptFunction::DoFunc9(uchar* p,uint32 uMagic)		{ *p = (*p) ^ (uMagic >> 8); }
void EncryptFunction::DoFunc10(uchar* p,uint32 uMagic)		{ *p = (*p) ^ (uMagic >> 16); }
void EncryptFunction::DoFunc11(uchar* p,uint32 uMagic)		{ *p = (*p) ^ (uMagic >> 24); }

void EncryptFunction::DeFunc0(uchar* p,uint32 uMagic)		{ *p -= 50; }
void EncryptFunction::DeFunc1(uchar* p,uint32 uMagic)		{ *p -= 94; }
void EncryptFunction::DeFunc2(uchar* p,uint32 uMagic)		{ *p -= 163; }
void EncryptFunction::DeFunc3(uchar* p,uint32 uMagic)		{ *p -= 214; }
void EncryptFunction::DeFunc4(uchar* p,uint32 uMagic)		{ *p = ((*p)>>6) | ((*p)<<2); }
void EncryptFunction::DeFunc5(uchar* p,uint32 uMagic)		{ *p = ((*p)>>5) | ((*p)<<3); }
void EncryptFunction::DeFunc6(uchar* p,uint32 uMagic)		{ *p = ((*p)>>3) | ((*p)<<5); }
void EncryptFunction::DeFunc7(uchar* p,uint32 uMagic)		{ *p = ((*p)>>1) | ((*p)<<7); }
void EncryptFunction::DeFunc8(uchar* p,uint32 uMagic)		{ *p = (*p) ^ uMagic; }
void EncryptFunction::DeFunc9(uchar* p,uint32 uMagic)		{ *p = (*p) ^ (uMagic >> 8); }
void EncryptFunction::DeFunc10(uchar* p,uint32 uMagic)		{ *p = (*p) ^ (uMagic >> 16); }
void EncryptFunction::DeFunc11(uchar* p,uint32 uMagic)		{ *p = (*p) ^ (uMagic >> 24); }



/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL MessageEncryptor::Shuffle( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize )
{
	return _Shuffle( MsgID, pBuf, uHead, uTail, uBufMaxSize, eMode_Shuffle );
}

BOOL MessageEncryptor::DeShuffle( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize )
{
	return _Shuffle( MsgID, pBuf, uHead, uTail, uBufMaxSize, eMode_DeShuffle );
}

BOOL MessageEncryptor::_Shuffle( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize, eSHUFFLE_MODE eShuffleMode )
{
	uint32 uIndex = ((uint32)MsgID+m_uMagicNum) % MAX_SHUFFLE_DATA_ARRAY_SIZE;
	ShuffleDataArray::ShuffleData *pShuffle = ShuffleDataArray::GetShuffleData(uIndex);
	if( pShuffle == NULL )
		return FALSE;

	StreamPointer Pt(pBuf,uHead,uTail,uBufMaxSize);

	uint32 uCount = 0;
	while( TRUE )
	{
		uint32 uSize = Pt.GetSize();
		if( uSize > MAX_SHUFFLE_DATA_SIZE )
			uSize = MAX_SHUFFLE_DATA_SIZE;
		uchar tmpBuf[MAX_SHUFFLE_DATA_SIZE] = {0};
		Pt.CopyData( tmpBuf, uSize );

		uint32 uPtIndex = 0;
		for( uint32 i=0; i<MAX_SHUFFLE_DATA_SIZE; i++ )
		{
			if( pShuffle->m_Data[i] >= uSize )
				continue;

			if( eShuffleMode == eMode_Shuffle )
			{
				*Pt.GetPointer(uPtIndex) = tmpBuf[pShuffle->m_Data[i]];
			}
			else if( eShuffleMode == eMode_DeShuffle )
			{
				*Pt.GetPointer(pShuffle->m_Data[i]) = tmpBuf[uPtIndex];
			}

			uPtIndex++;
		}

		Pt += uSize;
		if( Pt.GetSize() == 0 )
			break;

		if( ++uCount >= SHUFFLE_OPERATE_COUNT_LIMIT )
			break;
	}

	return TRUE;
}



BOOL MessageEncryptor::Encrypt( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize )
{
	return _Encrypt( MsgID, pBuf, uHead, uTail, uBufMaxSize, eMode_Encrypt );
}

BOOL MessageEncryptor::DeEncrypt( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize )
{
	return _Encrypt( MsgID, pBuf, uHead, uTail, uBufMaxSize, eMode_DeEncrypt );
}

BOOL MessageEncryptor::_Encrypt( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize, eENCRYPT_MODE eEncryptMode )
{
	uint32 uIndex = (m_uMagicNum-(uint32)MsgID) % MAX_ENCRYPT_FUNC_SIZE;
	EncryptFunction::func_ptr pFunc = NULL;
	if( eEncryptMode == eMode_Encrypt )
		pFunc = EncryptFunction::GetDoFuncPtr( uIndex );
	else if( eEncryptMode == eMode_DeEncrypt )
		pFunc = EncryptFunction::GetDeFuncPtr( uIndex );

	if( pFunc == NULL )
		return FALSE;

	StreamPointer Pt(pBuf,uHead,uTail,uBufMaxSize);

	uint32 uSize = Pt.GetSize();
	if( uSize > ENCRYPT_BYTE_LIMIT )
		uSize = ENCRYPT_BYTE_LIMIT;

	for( uint32 i=0; i<uSize; ++i )
	{
		pFunc( Pt.GetPointer(), m_uMagicNum );
		++Pt;
	}

	return TRUE;
}


BOOL MessageEncryptor::Encode( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize )
{
	if( FALSE == Encrypt( MsgID, pBuf, uHead, uTail, uBufMaxSize ) )
		return FALSE;

	if( FALSE == Shuffle( MsgID, pBuf, uHead, uTail, uBufMaxSize ) )
		return FALSE;

	return TRUE;
}

BOOL MessageEncryptor::Decode( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize )
{
	if( FALSE == DeShuffle( MsgID, pBuf, uHead, uTail, uBufMaxSize ) )
		return FALSE;

	if( FALSE == DeEncrypt( MsgID, pBuf, uHead, uTail, uBufMaxSize ) )
		return FALSE;

	return TRUE;	
}

