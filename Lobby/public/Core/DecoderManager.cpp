/*$T MapServer/Server/Base/DecoderManager.cpp GC 1.140 10/10/07 10:07:20 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "TypeDefine.h"
#include "DecoderManager.h"
#include "EncryptFunction.h"
#include "RsaDef.h"

#ifdef __LINUX__
#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <setjmp.h>
#include "Atomic.h"
#include "BaseTool.h"
#endif

/*
 =======================================================================================================================
 =======================================================================================================================
 */
DecoderManager::DecoderManager()
{
	for(int32 _Idx = 0; _Idx < MAX_ENCRYPT_ARITH_NUM; ++_Idx)
	{
		m_pDecoder[_Idx] = NULL;
		m_pEncrypt[_Idx] = NULL;
		m_pFun[_Idx] = NULL;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
DecoderManager::~DecoderManager()
{
	Clear();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DecoderManager::Clear()
{
	__GUARD__ for(int32 _Idx = 0; _Idx < MAX_ENCRYPT_ARITH_NUM; ++_Idx)
	{
		if(m_pDecoder[_Idx] != NULL)
		{
			delete m_pDecoder[_Idx];
			m_pDecoder[_Idx] = NULL;
		}

		if(m_pEncrypt[_Idx] != NULL)
		{
			delete m_pEncrypt[_Idx];
			m_pEncrypt[_Idx] = NULL;
		}

		m_pFun[_Idx] = NULL;
	}

	m_LogicTimer.Clear();

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DecoderManager::Init()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	m_BlockEncryptNum = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_KeyEncryptNum = 0;

	CreateRsaData();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	uTime = GET_TIME().TickCount();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_LogicTimer.BeginTimer(5 * 60 * 1000, uTime);
	m_RsaTimer.BeginTimer(60 * 60 * 1000,uTime);	

	m_uCurrAnswerId = 0;
	m_uRsaAritId = 0;

	for(int32 _Idx = 0; _Idx < MAX_ENCRYPT_ARITH_NUM; ++_Idx)
	{
		KCheck(NULL == m_pDecoder[_Idx]);
		KCheck(NULL == m_pEncrypt[_Idx]);
		KCheck(NULL == m_pFun[_Idx]);

		switch(_Idx)
		{
		case 0:
			{
				m_pDecoder[_Idx] = new RC2Decryption;
				m_pEncrypt[_Idx] = new RC2Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 1:
			{
				m_pDecoder[_Idx] = new RC5Decryption;
				m_pEncrypt[_Idx] = new RC5Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 2:
			{
				m_pDecoder[_Idx] = new DESDecryption;
				m_pEncrypt[_Idx] = new DESEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 3:
			{
				m_pDecoder[_Idx] = new GOSTDecryption;
				m_pEncrypt[_Idx] = new GOSTEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 4:
			{
				m_pDecoder[_Idx] = new IDEADecryption;
				m_pEncrypt[_Idx] = new IDEAEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 5:
			{
				m_pDecoder[_Idx] = new BlowfishDecryption;
				m_pEncrypt[_Idx] = new BlowfishEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 6:
			{
				m_pDecoder[_Idx] = new DES_EDE_Decryption;
				m_pEncrypt[_Idx] = new DES_EDE_Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 7:
			{
				m_pDecoder[_Idx] = new TripleDES_Decryption;
				m_pEncrypt[_Idx] = new TripleDES_Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 8:
			{
				m_pDecoder[_Idx] = new SAFER_K64_Decryption;
				m_pEncrypt[_Idx] = new SAFER_K64_Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 9:
			{
				m_pDecoder[_Idx] = new SAFER_K128_Decryption;
				m_pEncrypt[_Idx] = new SAFER_K128_Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 10:
			{
				m_pDecoder[_Idx] = new SAFER_SK128_Decryption;
				m_pEncrypt[_Idx] = new SAFER_SK128_Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 11:
			{
				m_pDecoder[_Idx] = new SAFER_SK64_Decryption;
				m_pEncrypt[_Idx] = new SAFER_SK64_Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 12:
			{
				m_pDecoder[_Idx] = new MARSDecryption;
				m_pEncrypt[_Idx] = new MARSEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 13:
			{
				m_pDecoder[_Idx] = new RC6Decryption;
				m_pEncrypt[_Idx] = new RC6Encryption;
				m_BlockEncryptNum++;
				break;
			}

		case 14:
			{
				m_pDecoder[_Idx] = new TEADecryption;
				m_pEncrypt[_Idx] = new TEAEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 15:
			{
				m_pDecoder[_Idx] = new DevalueDecryption;
				m_pEncrypt[_Idx] = new DevalueEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 16:
			{
				m_pDecoder[_Idx] = new IncreDecryption;
				m_pEncrypt[_Idx] = new IncreEncryption;
				m_BlockEncryptNum++;
				break;
			}

		case 17:
			{
				m_pDecoder[_Idx] = new BitOperDecryption1;
				m_pEncrypt[_Idx] = new BitOperEncryption1;
				m_BlockEncryptNum++;
				break;
			}

		case 18:
			{
				m_pDecoder[_Idx] = new BitOperDecryption2;
				m_pEncrypt[_Idx] = new BitOperEncryption2;
				m_BlockEncryptNum++;
				break;
			}

		default:
			break;
		}
	}

	m_pFun[0] = EncryptFunction1;
	m_pFun[1] = EncryptFunction2;
	m_pFun[2] = EncryptFunction3;
	m_pFun[3] = EncryptFunction4;
	m_pFun[4] = EncryptFunction5;
	m_pFun[5] = EncryptFunction6;
	m_pFun[6] = EncryptFunction7;
	m_pFun[7] = EncryptFunction8;
	m_pFun[8] = EncryptFunction9;
	m_pFun[9] = EncryptFunction10;

	m_pFun[10] = EncryptFunction11;
	m_pFun[11] = EncryptFunction12;
	m_pFun[12] = EncryptFunction13;
	m_pFun[13] = EncryptFunction14;
	m_pFun[14] = EncryptFunction15;
	m_pFun[15] = EncryptFunction16;
	m_pFun[16] = EncryptFunction17;
	m_pFun[17] = EncryptFunction18;
	m_pFun[18] = EncryptFunction19;
	m_pFun[19] = EncryptFunction20;

	m_pFun[20] = EncryptFunction21;
	m_pFun[21] = EncryptFunction22;
	m_pFun[22] = EncryptFunction23;
	m_pFun[23] = EncryptFunction24;
	m_pFun[24] = EncryptFunction25;
	m_pFun[25] = EncryptFunction26;
	m_pFun[26] = EncryptFunction27;
	m_pFun[27] = EncryptFunction28;
	m_pFun[28] = EncryptFunction29;
	m_pFun[29] = EncryptFunction30;

	m_KeyEncryptNum = 30;

	KCheck(m_BlockEncryptNum > 0);
	KCheck(m_KeyEncryptNum + m_BlockEncryptNum <= MAX_ENCRYPT_ARITH_NUM);

	return TRUE;
	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void DecoderManager::CreateRsaData()
{
	__GUARD__

		int tabId1 = 0;
	int tabId2 = 0;
	int tabId3 = 0;
	int tabId4 = 0;

	for ( int32 rsaIndex = 0; rsaIndex < MAX_RSA_ARITH_NUM; ++rsaIndex )
	{
		//读P
		m_RsaPrivatekey[rsaIndex].p.negative = rsa_private_p_negative_tab[rsaIndex];
		m_RsaPrivatekey[rsaIndex].p.setShare( rsa_private_p_share_tab[rsaIndex] ); 
		m_RsaPrivatekey[rsaIndex].p.setN( rsa_private_p_n_tab[rsaIndex] );
		m_RsaPrivatekey[rsaIndex].p.setZ( rsa_private_p_z_tab[rsaIndex] );

		for ( int _Idx=0;_Idx<m_RsaPrivatekey[rsaIndex].p.getZ();_Idx++ )
		{
			m_RsaPrivatekey[rsaIndex].p.setArr( _Idx,rsa_private_p_arr_tab[tabId1++] );
		}

		//读Q
		m_RsaPrivatekey[rsaIndex].q.negative = rsa_private_q_negative_tab[rsaIndex];
		m_RsaPrivatekey[rsaIndex].q.setShare( rsa_private_q_share_tab[rsaIndex] ); 
		m_RsaPrivatekey[rsaIndex].q.setN( rsa_private_q_n_tab[rsaIndex] );
		m_RsaPrivatekey[rsaIndex].q.setZ( rsa_private_q_z_tab[rsaIndex] );

		for ( int _Idx=0;_Idx<m_RsaPrivatekey[rsaIndex].q.getZ();_Idx++ )
		{
			m_RsaPrivatekey[rsaIndex].q.setArr( _Idx,rsa_private_q_arr_tab[tabId2++] );
		}

		//读M
		m_RsaPrivatekey[rsaIndex].m.negative = rsa_private_m_negative_tab[rsaIndex];
		m_RsaPrivatekey[rsaIndex].m.setShare( rsa_private_m_share_tab[rsaIndex] ); 
		m_RsaPrivatekey[rsaIndex].m.setN( rsa_private_m_n_tab[rsaIndex] );
		m_RsaPrivatekey[rsaIndex].m.setZ( rsa_private_m_z_tab[rsaIndex] );

		for ( int _Idx=0;_Idx<m_RsaPrivatekey[rsaIndex].m.getZ();_Idx++ )
		{
			m_RsaPrivatekey[rsaIndex].m.setArr( _Idx,rsa_private_m_arr_tab[tabId3++] );
		}

		//读E
		m_RsaPrivatekey[rsaIndex].e.negative = rsa_private_e_negative_tab[rsaIndex];
		m_RsaPrivatekey[rsaIndex].e.setShare( rsa_private_e_share_tab[rsaIndex] ); 
		m_RsaPrivatekey[rsaIndex].e.setN( rsa_private_e_n_tab[rsaIndex] );
		m_RsaPrivatekey[rsaIndex].e.setZ( rsa_private_e_z_tab[rsaIndex] );

		for ( int _Idx=0;_Idx<m_RsaPrivatekey[rsaIndex].e.getZ();_Idx++ )
		{
			m_RsaPrivatekey[rsaIndex].e.setArr( _Idx,rsa_private_e_arr_tab[tabId4++] );
		}

	}

	__UNGUARD__

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DecoderManager::HeartBeat(uint32 uTime)
{
	__GUARD__ if(m_LogicTimer.IsSetTimer())
	{
		if(m_LogicTimer.CountingTimer(uTime))
		{
			m_uCurrAnswerId = 0;
		}
	}

	if(m_RsaTimer.IsSetTimer())
	{
		if(m_RsaTimer.CountingTimer(uTime))
		{
			m_uRsaAritId++;
			if(m_uRsaAritId >= MAX_RSA_ARITH_NUM)
			{
				m_uRsaAritId = 0;
			}
		}
	}

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 DecoderManager::DoDecode(uchar arithId, uint32 key)
{
	arithId = arithId - m_BlockEncryptNum;

	if(arithId >= 0 && arithId < m_KeyEncryptNum)
	{
		if(m_pFun[arithId] != NULL)
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			uint32	userKey = (*m_pFun[arithId]) (key);
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			return userKey;
		}
	}

	return -1;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DecoderManager::DoDecode(uchar arithId, const char *szVerifyCode, uint32 key, int32 size)
{
	if(FALSE == g_Config.m_LoginInfo.m_EncryptToggle)
	{
		return;
	}

	if(NULL == szVerifyCode)
	{
		return;
	}

	/*~~~~~~~~~~~~~~~~~~~~*/
	int	fill = size % 8;
	/*~~~~~~~~~~~~~~~~~~~~*/

	if(fill != 0)
	{
		fill = 8 - fill;
	}

	size += fill;

	/*~~~~~~~~~~~~~~~~~~~~~~*/
	char	szKey[33] = { 0 };
	/*~~~~~~~~~~~~~~~~~~~~~~*/

	tsnprintf(szKey, 32, "%d", key);

	if(size > MAX_BLOCK_LEN)
	{
		return;
	}

	if(arithId >= 0 && arithId < m_BlockEncryptNum)
	{
		if(m_pDecoder[arithId] != NULL)
		{
			m_pDecoder[arithId]->SetUserKey((uchar *) szKey);
			m_pDecoder[arithId]->ProcessBlockBase((uchar *) szVerifyCode, size);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DecoderManager::RSADecode(uint32 arithId,char* szPassword,const int psSize,vlong& cliper,vlong& plantlong)
{
	if(arithId >= 0 && arithId < MAX_RSA_ARITH_NUM)
	{
		vlong output;
		char szDecodeBuf[512] = {0};
		m_RsaPrivatekey[arithId].RSA_Decode(cliper.tostring(szDecodeBuf,512),plantlong,output);
		strncpy(szDecodeBuf,output.tostring(szDecodeBuf,512),psSize*2);
		output.Binary2Str(szDecodeBuf,szPassword,RSA_ARITH_OFFSET,psSize*2);
	}
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void DecoderManager::DecodePassword(const int rsaIndex,char *szPassword,const int psSize,RSA_VLOLG_MESSAGE *pRsaInfo)
{
	__GUARD__

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	KCheck(szPassword);
	KCheck(pRsaInfo);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if ( FALSE == g_Config.m_LoginInfo.m_RsaToggle )
	{//不启用RSA算法时不做特殊处理
		return;
	}
	{
		if ( rsaIndex>=0 && rsaIndex<MAX_RSA_ARITH_NUM )
		{
			vlong cliper,plantlong;

			cliper.negative = pRsaInfo->GetNegative();
			cliper.setShare( pRsaInfo->GetShare() ); 
			cliper.setN( pRsaInfo->GetNum() );
			cliper.setZ( RSA_MESSAGE_ARRAY_NUM );

			for ( int _Idx=0;_Idx<cliper.getZ();_Idx++ )
			{
				cliper.setArr( _Idx,pRsaInfo->GetArray(_Idx) );
			}

			RSADecode(rsaIndex,szPassword,psSize,cliper,plantlong);

		}
	}

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DecoderManager::DoEncrypt(uchar arithId, const char *szVerifyCode, uint32 key, int32 size)
{
	if(NULL == szVerifyCode)
	{
		return;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~*/
	char	szKey[33] = { 0 };
	/*~~~~~~~~~~~~~~~~~~~~~~*/

	tsnprintf(szKey, 32, "%d", key);

	DoEncrypt(arithId, szVerifyCode, szKey, size);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DecoderManager::DoEncrypt(uchar arithId, const char *szVerifyCode, const char *szKey, int32 size)
{
	if(FALSE == g_Config.m_LoginInfo.m_EncryptToggle)
	{
		return;
	}

	if(NULL == szVerifyCode || NULL == szKey)
	{
		return;
	}

	if(size > MAX_BLOCK_LEN)
	{
		return;
	}

	if(arithId >= 0 && arithId < m_BlockEncryptNum)
	{
		if(m_pEncrypt[arithId] != NULL)
		{
			m_pEncrypt[arithId]->SetUserKey((uchar *) szKey);
			m_pEncrypt[arithId]->ProcessBlockBase((uchar *) szVerifyCode, size);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 DecoderManager::GenCrc(const char *szString)
{
	if(szString == NULL || szString[0] == 0) return 0;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	dwCrc32 = 0xFFFFFFFF;
	int	nSize = (int) strlen(szString);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(int32 _Idx = 0; _Idx < nSize; _Idx++)
	{
		CalcCrc32(szString[_Idx], dwCrc32);
	}

	return dwCrc32;
}
