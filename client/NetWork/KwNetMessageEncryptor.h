#ifndef __MESSAGE_ENCRYPTOR_H__
#define __MESSAGE_ENCRYPTOR_H__

#include "KwNetBasics.h"

#define MAX_SHUFFLE_DATA_SIZE				24
#define MAX_SHUFFLE_DATA_ARRAY_SIZE			32
#define MAX_ENCRYPT_FUNC_SIZE				12

#define SHUFFLE_OPERATE_COUNT_LIMIT			3
#define ENCRYPT_BYTE_LIMIT					64

/*
 =======================================================================================================================
 =======================================================================================================================
 */
class ShuffleDataArray
{
	friend class MessageEncryptor;
	struct ShuffleData 
	{
		uchar m_Data[MAX_SHUFFLE_DATA_SIZE];
	};
private:
	inline static ShuffleData* GetShuffleData(uint32 uIndex);

private:
	static ShuffleData m_Array[MAX_SHUFFLE_DATA_ARRAY_SIZE];
};

/*
 =======================================================================================================================
 =======================================================================================================================
 */
class EncryptFunction
{
	friend class MessageEncryptor;
	typedef void (*func_ptr) (uchar*,uint32);
private:
	static func_ptr GetDoFuncPtr(uint32 uIndex);
	static func_ptr GetDeFuncPtr(uint32 uIndex);

private:
	inline static void DoFunc0(uchar* p,uint32 uMagic);
	inline static void DoFunc1(uchar* p,uint32 uMagic);
	inline static void DoFunc2(uchar* p,uint32 uMagic);
	inline static void DoFunc3(uchar* p,uint32 uMagic);
	inline static void DoFunc4(uchar* p,uint32 uMagic);
	inline static void DoFunc5(uchar* p,uint32 uMagic);
	inline static void DoFunc6(uchar* p,uint32 uMagic);
	inline static void DoFunc7(uchar* p,uint32 uMagic);
	inline static void DoFunc8(uchar* p,uint32 uMagic);
	inline static void DoFunc9(uchar* p,uint32 uMagic);
	inline static void DoFunc10(uchar* p,uint32 uMagic);
	inline static void DoFunc11(uchar* p,uint32 uMagic);

	inline static void DeFunc0(uchar* p,uint32 uMagic);
	inline static void DeFunc1(uchar* p,uint32 uMagic);
	inline static void DeFunc2(uchar* p,uint32 uMagic);
	inline static void DeFunc3(uchar* p,uint32 uMagic);
	inline static void DeFunc4(uchar* p,uint32 uMagic);
	inline static void DeFunc5(uchar* p,uint32 uMagic);
	inline static void DeFunc6(uchar* p,uint32 uMagic);
	inline static void DeFunc7(uchar* p,uint32 uMagic);
	inline static void DeFunc8(uchar* p,uint32 uMagic);
	inline static void DeFunc9(uchar* p,uint32 uMagic);
	inline static void DeFunc10(uchar* p,uint32 uMagic);
	inline static void DeFunc11(uchar* p,uint32 uMagic);

private:
	static func_ptr m_pDoFuncPtr[MAX_ENCRYPT_FUNC_SIZE];
	static func_ptr m_pDeFuncPtr[MAX_ENCRYPT_FUNC_SIZE];
};

/*
 =======================================================================================================================
 =======================================================================================================================
 */
class MessageEncryptor
{
public:
	enum eSHUFFLE_MODE
	{
		eMode_Shuffle = 0,	
		eMode_DeShuffle,
	};

	enum eENCRYPT_MODE
	{
		eMode_Encrypt = 0,
		eMode_DeEncrypt,
	};

public:
	MessageEncryptor(){ m_uMagicNum=0; }
	~MessageEncryptor(){}

	BOOL		Encode( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize );
	BOOL		Decode( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize );

	void		SetMagicNum(uint32 u)	{ m_uMagicNum=u; }
	uint32		GetMagicNum()			{ return m_uMagicNum; }

private:
	inline BOOL	Shuffle( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize );
	inline BOOL	DeShuffle( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize );
	BOOL		_Shuffle( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxLen, eSHUFFLE_MODE eShuffleMode );

	inline BOOL	Encrypt( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize );
	inline BOOL	DeEncrypt( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize );
	BOOL		_Encrypt( MSG_ID MsgID, uchar* pBuf, uint32 uHead, uint32 uTail, uint32 uBufMaxSize, eENCRYPT_MODE eEncryptMode );

private:
	uint32	m_uMagicNum;
};

#endif
