/*$T MapServer/Server/Base/DecoderManager.h GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __DECODER_MANAGER_H__
#define __DECODER_MANAGER_H__

#include "TypeDefine.h"
#include "Define.h"
#include "Struct.h"
#include "KwNetStreamSystem.h"
#include "BlowFish.h"
#include "DES.h"
#include "Gost.h"
#include "Idea.h"
#include "rc2.h"
#include "rc5.h"
#include "safer.h"
#include "CRC.h"
#include "Mars.h"
#include "rc6.h"
#include "tea.h"
#include "Devalue.h"
#include "Increment.h"
#include "BitOper.h"
#include "rsa.h"

class	DecoderManager
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	DecoderManager();
	~	DecoderManager();

	BOOL	Init();
	void	CreateRsaData();
	void	Clear();

	BOOL	HeartBeat(uint32 uTime);

	void	DoDecode(uchar arithId, const char *szVerifyCode, uint32 key, int32 size = 8);

	void	DoEncrypt(uchar arithId, const char *szVerifyCode, uint32 key, int32 size = 8);
	void	DoEncrypt(uchar arithId, const char *szVerifyCode, const char *szKey, int32 size = 8);

	uint32	GenCrc(const char *szString);

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	inline void CalcCrc32(const uchar byte, uint32 &dwCrc32)
	{
		dwCrc32 = ((dwCrc32) >> 8) ^ crc_32_tab[(byte) ^ ((dwCrc32) & 0x000000FF)];
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	uchar GetBlockEncryptNum()
	{
		return m_BlockEncryptNum;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	uchar GetKeyEncryptNum()
	{
		return m_KeyEncryptNum;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	BOOL IsBlockEncrypt(uchar arithId)
	{
		return m_BlockEncryptNum > arithId;
	}

	uint32	DoDecode(uchar arithId, uint32 key);
	void	RSADecode(uint32 arithId,char* szPassword,const int psSize,vlong& cliper,vlong& plantlong);
	void	DecodePassword(const int rsaIndex,char *szPassword,const int psSize,RSA_VLOLG_MESSAGE *pRsaInfo);

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */

	uint32 GetCurrAnswerId()
	{
		return m_uCurrAnswerId;
	}

	uchar GetRsaAritId()
	{
		if ( FALSE == g_Config.m_LoginInfo.m_RsaToggle )
			return -1;

			return m_uRsaAritId;
	}

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
	uchar			m_BlockEncryptNum;
	uchar			m_KeyEncryptNum;

	BlockTransformation	*m_pDecoder[MAX_ENCRYPT_ARITH_NUM];
	BlockTransformation	*m_pEncrypt[MAX_ENCRYPT_ARITH_NUM];

	uint32 (*m_pFun[MAX_ENCRYPT_ARITH_NUM]) (uint32 nParam);

	private_key			m_RsaPrivatekey[MAX_RSA_ARITH_NUM];

	CTinyTimer	m_LogicTimer;
	uint32		m_uCurrAnswerId;	/* 当前题目ID */

	CTinyTimer	m_RsaTimer;			//RSA加密计时器，用于控制更新加密算法
	uchar		m_uRsaAritId;		//当前RSA算法ID

};
#endif
