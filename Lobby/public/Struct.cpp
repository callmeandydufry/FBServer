/*$T Share/Struct.cpp GC 1.140 10/10/07 10:06:55 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "TypeDefine.h"
#include "Struct.h"
#include "KwNetStreamSystem.h"
//#include "ModulePacket.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SQLString::Parse(const char *pTemplate, ...)
{
	/*~~~~~~~~~~~*/
	va_list argptr;
	/*~~~~~~~~~~~*/

	va_start(argptr, pTemplate);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32	nchars = tvsnprintf((char *) m_SqlStr, MAX_SQL_LENGTH, pTemplate, argptr);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	va_end(argptr);

	if(nchars == -1 || nchars > MAX_SQL_LENGTH)
	{
		KCheck(FALSE);
		return;
	}
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void SQLString::SetSql(const char* sqlString)
{
	strncpy((char*)m_SqlStr, sqlString, MAX_SQL_LENGTH);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void RSA_VLOLG_MESSAGE::Reci(RecieveStream &iStream)
{
	iStream.Reci((char *) &m_Negative, sizeof(m_Negative));
	iStream.Reci((char *) &m_Share, sizeof(m_Share));
	iStream.Reci((char *) &m_Num, sizeof(m_Num));
	iStream.Reci((char *) (m_Array), sizeof(uint32)*RSA_MESSAGE_ARRAY_NUM);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void RSA_VLOLG_MESSAGE::Send(SendStream &oStream) const
{
	oStream.Send((char *) &m_Negative, sizeof(m_Negative));
	oStream.Send((char *) &m_Share, sizeof(m_Share));
	oStream.Send((char *) &m_Num, sizeof(m_Num));
	oStream.Send((char *) m_Array, sizeof(uint32)*RSA_MESSAGE_ARRAY_NUM);

}

void RSA_VLOLG_MESSAGE::SetArray(uint32 index,uint32 val)
{
	__GUARD__
	KCheck(index>=0 && index<RSA_MESSAGE_ARRAY_NUM);
	m_Array[index] = val;
	__UNGUARD__

}

uint32 RSA_VLOLG_MESSAGE::GetArray(uint32 index)
{
	__GUARD__

	KCheck(index>=0 && index<RSA_MESSAGE_ARRAY_NUM);
	return m_Array[index];
	__UNGUARD__

	return 0;
}
