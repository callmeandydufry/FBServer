#include "GameNetStream.h"

#ifdef __LINUX__
#include <sys/utsname.h>
#include <sys/time.h>
#endif


#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif


#ifndef MAX
#define MAX(a,b)    (((a)>(b)) ? (a) : (b))
#endif

uint32 GetMsgIndex(uint32 a)
{
	return	((a) & 0x000000ff);//((a)>>24);
}
void SetMsgIndex(uint32& a, uchar index)
{
	((a) = ((a) & 0xffffff00) + (index));
}
uint32 GetMsgLen(uint32 a)
{
	return	(a) >> 8;
}
void SetMsgLen(uint32& a, uint32 Len)
{
	((a) = (((a) & 0x000000ff) + ((Len) << 8)));
}

#define NET_ABS(x) (x)>0?(x):(-x)
void* NetEncrptor::Decode(void * dst, const void * src, uint32 count)
{
	void * ret = dst;
	while (count--)
	{
		*(code_char *)dst = *(code_char *)src;
		if (enable)
		{
			code_char   low = 0;
			code_char   high = 0;
			low = (*(code_char *)dst) & 0x0F;
			high = (*(code_char *)dst) & 0xF0;

			high = high ^ (pdata[low] & 0xF0);
			low = (((low ^ 0x0F) & 0x0F) + pdata[0] & 0x0F) & 0x0F;
			*(code_char *)dst = high + low;
		}
		else
			*(code_char *)dst = (*(code_char *)dst) ^ 0xFF;
		dst = (code_char *)dst + 1;
		src = (code_char *)src + 1;
	}
	return(ret);
}


void* NetEncrptor::Encode(void * dst, const void * src, uint32 count)
{
	void * ret = dst;
	while (count--) {
		*(code_char *)dst = *(code_char *)src;
		if (enable)
		{
			code_char   low = 0;
			code_char   high = 0;
			low = (*(code_char *)dst) & 0x0F;
			high = (*(code_char *)dst) & 0xF0;
			low = ((low - pdata[0] & 0x0F) & 0x0F) ^ 0x0F;
			high = high ^ (pdata[low] & 0xF0);
			*(code_char *)dst = high + low;
		}
		else
			*(code_char *)dst = (*(code_char *)dst) ^ 0xFF;
		dst = (code_char *)dst + 1;
		src = (code_char *)src + 1;
	}
	return(ret);
}

void  NetEncrptor::SetPdata(const code_char* p, uint32 uMagicNum)
{
	char buf[ED_DATA_LENGTH] = { 0 };
	memcpy(buf, p, ED_DATA_LENGTH);
	for (int32 i = 0; i < ED_DATA_LENGTH; i++)
	{
		buf[i] += (code_char)uMagicNum;
	}
	memcpy(pdata, buf, ED_DATA_LENGTH);
}

code_char* NetEncrptor::GetPdata()
{
	return pdata;
}

NetEncrptor::NetEncrptor()
{
	memset(pdata, 0, sizeof(pdata));
	enable = 0;
};

NetEncrptor::~NetEncrptor()
{

}

void *  read_memcpy(void * dst,
	const void * src,
	uint32 count
)
{
	void * ret = dst;
	while (count--) {
		*(code_char *)dst = *(code_char *)src;
		*(code_char *)dst = (*(code_char *)dst) ^ 0xFF;
		dst = (code_char *)dst + 1;
		src = (code_char *)src + 1;
	}

	return(ret);
}

NetRecieveStream::NetRecieveStream(uint32 BufferLen, uint32 MaxBufferLen)
{
	m_pSocket = NULL;
	m_BufferLen = BufferLen;
	m_MaxBufferLen = MaxBufferLen;

	m_Head = 0;
	m_Tail = 0;

	m_Buffer = new uint8[m_BufferLen];
	memset(m_Buffer, 0, m_BufferLen);
	m_Setting = 0;
	m_pEncrptor = new NetEncrptor();

	m_uMsgHeadMagicNum = 0;

	m_FormatMode = 0;
}

NetRecieveStream::~NetRecieveStream()
{
	if (m_Buffer)
	{
		delete[] m_Buffer;
	}
	if (m_pEncrptor)
	{
		delete m_pEncrptor;
	}
}

void NetRecieveStream::SetMagicNum(uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum)
{
	m_uMsgHeadMagicNum = uMsgHeadMagicNum;
	m_MessageEncryptor.SetMagicNum(uMsgMagicNum);
}

void NetRecieveStream::DeEncryptMsgHead(MSG_ID* MsgID)
{
	(*MsgID) = (*MsgID) ^ m_uMsgHeadMagicNum;
	if (((*MsgID) & 0x8000) != 0)
		(*MsgID) &= 0x7FFF;
}

void NetRecieveStream::DeEncryptMsgHead(uint32* MsgUint)
{
	(*MsgUint) = (*MsgUint) ^ m_uMsgHeadMagicNum;
}

void NetRecieveStream::DeEncryptMessage(MSG_ID MsgID, uint32 uSize)
{
	m_MessageEncryptor.Decode(MsgID, (uchar*)m_Buffer, (m_Head + 6) % m_BufferLen, (m_Head + uSize + 6) % m_BufferLen, m_BufferLen);
}

uint32 NetRecieveStream::Length() const
{
	if (m_Head < m_Tail)
		return m_Tail - m_Head;

	else if (m_Head > m_Tail)
		return m_BufferLen - m_Head + m_Tail;

	return 0;
}

uint32 NetRecieveStream::Reci(char* buf, uint32 len)
{
	if (m_FormatMode == 0)
	{
		return Reci_i(buf, len);
	}
	else
	{
		//first recv len
		uint16 uLen = 0;

		if (Reci_i((char*)&uLen, sizeof(uLen)) > 0)
		{
			//get the min len
			uint32 uMinLen = len > uLen ? uLen : len;

			uMinLen = Reci_i(buf, uMinLen);
			if (uMinLen > 0)
			{
				//set left buf to 0
				uint32 uLeft = len - uMinLen;
				if (uLeft > 0)
				{
					memset(buf + uMinLen, 0, uLeft);
				}

				return len;
			}

			return 0;
		}

		return 0;
	}
}

uint32 NetRecieveStream::Reci_i(char* buf, uint32 len)
{
	if (len == 0)
		return 0;

	if (len > Length())
		return 0;

	if (m_Head < m_Tail)
	{
		if (m_Setting == 0)
		{
			m_pEncrptor->Decode(buf, &m_Buffer[m_Head], len);
		}
		else
		{
			memcpy(buf, &m_Buffer[m_Head], len);
		}
	}
	else
	{
		uint32 rightLen = m_BufferLen - m_Head;
		if (len <= rightLen)
		{
			if (m_Setting == 0)
			{
				m_pEncrptor->Decode(buf, &m_Buffer[m_Head], len);
			}
			else
			{
				memcpy(buf, &m_Buffer[m_Head], len);
			}
		}
		else
		{
			if (m_Setting == 0)
			{
				m_pEncrptor->Decode(buf, &m_Buffer[m_Head], rightLen);
				m_pEncrptor->Decode(&buf[rightLen], m_Buffer, len - rightLen);
			}
			else
			{
				memcpy(buf, &m_Buffer[m_Head], rightLen);
				memcpy(&buf[rightLen], m_Buffer, len - rightLen);
			}

		}
	}

	m_Head = (m_Head + len) % m_BufferLen;

	return len;
}


bool NetRecieveStream::ReciveMessage(IGameMessage* pMsg)
{
	bool ret;

	ret = SkipByte(MSG_HEADER_SIZE);
	if (!ret)
		return false;

	return pMsg->recieve(*this);
}


bool NetRecieveStream::Peek(char* buf, uint32 len)
{
	if (len == 0)
		return false;

	if (len > Length())
		return false;

	if (m_Head < m_Tail)
	{
		if (m_Setting == 0)
		{
			m_pEncrptor->Decode(buf, &m_Buffer[m_Head], len);
		}
		else
		{
			memcpy(buf, &m_Buffer[m_Head], len);
		}

	}
	else
	{
		uint32 rightLen = m_BufferLen - m_Head;
		if (len <= rightLen)
		{
			if (m_Setting == 0)
			{
				m_pEncrptor->Decode(&buf[0], &m_Buffer[m_Head], len);
			}
			else
			{
				memcpy(&buf[0], &m_Buffer[m_Head], len);
			}
		}
		else
		{
			if (m_Setting == 0)
			{
				m_pEncrptor->Decode(&buf[0], &m_Buffer[m_Head], rightLen);
				m_pEncrptor->Decode(&buf[rightLen], &m_Buffer[0], len - rightLen);
			}
			else
			{
				memcpy(&buf[0], &m_Buffer[m_Head], rightLen);
				memcpy(&buf[rightLen], &m_Buffer[0], len - rightLen);
			}
		}
	}

	return true;
}

bool NetRecieveStream::SkipByte(uint32 len)
{
	if (len == 0) return false;

	if (len > Length())	return false;
	m_Head = (m_Head + len) % m_BufferLen;
	return true;
}

void NetRecieveStream::Initsize()
{
	m_Head = 0;
	m_Tail = 0;
	if (m_Buffer)
	{
		delete[] m_Buffer;
		m_Buffer = NULL;
	}
	m_Buffer = new uint8[INITRECSIZE];
	m_BufferLen = INITRECSIZE;
	memset(m_Buffer, 0, m_BufferLen);
}

int32 NetRecieveStream::ReadFull()
{
	if (NULL == m_pSocket)
		return 0;

	int32 nFilled = 0;
	int32 nRec = 0;
	int32 nFree = 0;
	if (m_Head <= m_Tail)
	{
		if (m_Head == 0)
		{
			nRec = 0;
			nFree = m_BufferLen - m_Tail - 1;
			if (nFree != 0)
			{
				if (!m_pSocket->Recv(&m_Buffer[m_Tail], nFree, nRec))
				{
					return NET_SOCKET_ERROR;
				}
				m_Tail += nRec;
				nFilled += nRec;
			}

			if (nRec == nFree)
			{
				uint32 available;
				if (m_pSocket->HasPendingData(available))
				{
					if ((m_BufferLen + available + 1) > m_MaxBufferLen)
					{
						Initsize();
						return NET_SOCKET_ERROR - 1;
					}
					if (!Resize(available + 1))
						return 0;

					if (!m_pSocket->Recv(&m_Buffer[m_Tail], available, nRec))
					{
						return NET_SOCKET_ERROR - 2;
					}

					m_Tail += nRec;
					nFilled += nRec;
				}
			}
		}
		else
		{
			nFree = m_BufferLen - m_Tail;

			if (!m_pSocket->Recv(&m_Buffer[m_Tail], nFree, nRec))
			{
				return NET_SOCKET_ERROR - 3;
			}

			m_Tail = (m_Tail + nRec) % m_BufferLen;
			nFilled += nRec;

			if (nRec == nFree)
			{
				nRec = 0;
				nFree = m_Head - 1;
				if (nFree != 0)
				{
					if (!m_pSocket->Recv(&m_Buffer[0], nFree, nRec))
					{
						return NET_SOCKET_ERROR - 4;
					}

					m_Tail += nRec;
					nFilled += nRec;
				}

				if (nRec == nFree)
				{
					uint32 available;
					if (m_pSocket->HasPendingData(available))
					{
						if ((m_BufferLen + available + 1) > m_MaxBufferLen)
						{
							Initsize();
							return NET_SOCKET_ERROR - 5;
						}
						if (!Resize(available + 1))
							return 0;

						if (!m_pSocket->Recv(&m_Buffer[m_Tail], available, nRec))
						{
							return NET_SOCKET_ERROR - 6;
						}

						m_Tail += nRec;
						nFilled += nRec;
					}
				}
			}
		}

	}
	else
	{
		nRec = 0;
		nFree = m_Head - m_Tail - 1;
		if (nFree != 0)
		{
			if (!m_pSocket->Recv(&m_Buffer[m_Tail], nFree, nRec))
			{
				return NET_SOCKET_ERROR - 7;
			}

			m_Tail += nRec;
			nFilled += nRec;
		}
		if (nRec == nFree)
		{
			uint32 available;
			if (m_pSocket->HasPendingData(available))
			{
				if ((m_BufferLen + available + 1) > m_MaxBufferLen)
				{
					Initsize();
					return NET_SOCKET_ERROR - 8;
				}
				if (!Resize(available + 1))
					return 0;

				if (!m_pSocket->Recv(&m_Buffer[m_Tail], available, nRec))
				{
					return NET_SOCKET_ERROR - 9;
				}

				m_Tail += nRec;
				nFilled += nRec;
			}
		}
	}


	return nFilled;
}

int32 NetRecieveStream::ReadFullFromMemory(const char* pBuf, uint32 uSize)
{
	uint32 uFree = 0;
	uint32 uFilled = 0;

	if (m_Head <= m_Tail)
	{
		if (m_Head == 0)
		{
			uFree = m_BufferLen - m_Tail - 1;
			uint32 uCpSize = uFree > uSize ? uSize : uFree;
			if (m_Setting == 0)
			{
				m_pEncrptor->Encode(&m_Buffer[m_Tail], pBuf, uCpSize);
			}
			else
			{
				memcpy(&m_Buffer[m_Tail], pBuf, uCpSize);
			}
			uFilled += uCpSize;
			m_Tail += uCpSize;
		}
		else
		{
			uFree = m_BufferLen - m_Tail;
			uint32 uCpSize1 = uFree > uSize ? uSize : uFree;
			if (m_Setting == 0)
			{
				m_pEncrptor->Encode(&m_Buffer[m_Tail], pBuf, uCpSize1);
			}
			else
			{
				memcpy(&m_Buffer[m_Tail], pBuf, uCpSize1);
			}
			uFilled += uCpSize1;
			m_Tail = (m_Tail + uCpSize1) % m_BufferLen;
			pBuf += uCpSize1;

			uFree = m_Head - 1;
			uint32 uCpSize2 = uFree > (uSize - uCpSize1) ? (uSize - uCpSize1) : uFree;
			if (m_Setting == 0)
			{
				m_pEncrptor->Encode(&m_Buffer[m_Tail], pBuf, uCpSize2);
			}
			else
			{
				memcpy(&m_Buffer[m_Tail], pBuf, uCpSize2);
			}
			uFilled += uCpSize2;
			m_Tail += uCpSize2;
		}
	}
	else
	{
		uFree = m_Head - m_Tail - 1;
		uint32 uCpSize = uFree > uSize ? uSize : uFree;
		if (m_Setting == 0)
		{
			m_pEncrptor->Encode(&m_Buffer[m_Tail], pBuf, uCpSize);
		}
		else
		{
			memcpy(&m_Buffer[m_Tail], pBuf, uCpSize);
		}
		uFilled += uCpSize;
		m_Tail += uCpSize;
	}

	return uFilled;
}

bool NetRecieveStream::Resize(int32 size)
{
	size = MAX(size, (int)(m_BufferLen >> 1));
	uint32 newBufferLen = m_BufferLen + size;
	uint32 len = Length();

	if (size < 0)
	{
		if (newBufferLen < 0 || newBufferLen < len)
			return false;
	}

	uint8 * newBuffer = new uint8[newBufferLen];
	if (m_Head < m_Tail)
	{
		memcpy(newBuffer, &m_Buffer[m_Head], m_Tail - m_Head);
	}
	else if (m_Head > m_Tail)
	{
		memcpy(newBuffer, &m_Buffer[m_Head], m_BufferLen - m_Head);
		memcpy(&newBuffer[m_BufferLen - m_Head], m_Buffer, m_Tail);
	}

	delete[] m_Buffer;

	m_Buffer = newBuffer;
	m_BufferLen = newBufferLen;
	m_Head = 0;
	m_Tail = len;

	return true;
}

void NetRecieveStream::Clear()
{
	m_Head = 0;
	m_Tail = 0;
}

void NetRecieveStream::SetEncryptMode(uint32 mode)
{
	m_Setting = mode;
}

uint32 NetRecieveStream::GetEncryptMode()
{
	return m_Setting;
}

void NetRecieveStream::EnableEncrpt(bool bEnable)
{
	m_pEncrptor->enable = bEnable ? 1 : 0;
}

bool NetRecieveStream::IsEnableEncrpt()
{
	return m_pEncrptor->enable > 0;
}

void NetRecieveStream::Encode(void * dst, const void * src, uint32 count)
{
	m_pEncrptor->Encode(dst, src, count);
}

void NetRecieveStream::Decode(void * dst, const void * src, uint32 count)
{
	m_pEncrptor->Decode(dst, src, count);
}

void NetRecieveStream::SetFormatMode(uint32 mode)
{
	m_FormatMode = mode;
}


void *  write_memcpy(void * dst,
	const void * src,
	uint32 count
)
{
	void * ret = dst;
	while (count--) {
		*(char *)dst = *(char *)src;
		*(char *)dst = (*(char *)dst) ^ 0xFF;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}
	return(ret);
}

NetSendStream::NetSendStream(uint32 BufferLen, uint32 MaxBufferLen)
{
	m_pSocket = NULL;
	m_BufferLen = BufferLen;
	m_nInitBufferLen = BufferLen;
	m_MaxBufferLen = MaxBufferLen;
	m_Head = 0;
	m_Tail = 0;
	m_uSendTail = 0;

	m_Buffer = new uint8[m_BufferLen];
	memset(m_Buffer, 0, m_BufferLen);
	m_Setting = 0;
	m_bStatic = false;
	m_bServerData = true;
	m_bFull = false;
	m_QosBytes = 1024 * 3;
	m_nowTime = 0;
	m_LastSendTime = 0;
	m_pEncrptor = new NetEncrptor();

	m_uMsgHeadMagicNum = 0;

	m_FormatMode = 0;
}

NetSendStream::~NetSendStream()
{
	if (m_pEncrptor)
	{
		delete m_pEncrptor;
		m_pEncrptor = NULL;
	}
	if (m_Buffer)
	{
		delete m_Buffer;
		m_Buffer = NULL;
	}
}

void NetSendStream::SetMagicNum(uint32 uMsgHeadMagicNum, uint32 uMsgMagicNum)
{
	m_uMsgHeadMagicNum = uMsgHeadMagicNum;
	m_MessageEncryptor.SetMagicNum(uMsgMagicNum);
}


void NetSendStream::UpdateTime(uint32 nowtime)
{
	m_nowTime = nowtime;
}

void NetSendStream::ResetQos()
{
	m_QosBytes = 1024 * 3;
	m_LastSendTime = m_nowTime;
}

void NetSendStream::CalQos(uint32 nFlushbytes)
{
	m_QosBytes -= nFlushbytes;

	if (m_QosBytes < 0)
	{
		m_QosBytes = 0;
	}
}
uint32 NetSendStream::Length() const
{
	if (m_Head < m_Tail)
		return m_Tail - m_Head;

	else if (m_Head > m_Tail)
		return m_BufferLen - m_Head + m_Tail;

	return 0;
}

uint32 NetSendStream::Send(const char* buf, uint32 len)
{
	if (m_FormatMode == 0)
	{
		return Send_i(buf, len);
	}
	else
	{
		//first send len
		uint16 uLen = (uint16)len;

		if (Send_i((const char*)&uLen, sizeof(uLen)) > 0)
		{
			m_uSendCount += sizeof(uLen);
			return Send_i(buf, len);
		}

		return 0;
	}
}

uint32 NetSendStream::Send_i(const char* buf, uint32 len)
{
	uint32 nFree = ((m_Head <= m_Tail) ? (m_BufferLen - m_Tail + m_Head - 1) : (m_Head - m_Tail - 1));

	if (len >= nFree)
	{
		if (!Resize(len - nFree + 1))
			return 0;
	}

	if (m_Head <= m_Tail)
	{
		if (m_Head == 0)
		{
			nFree = m_BufferLen - m_Tail - 1;
			if (m_Setting == 0)
			{
				m_pEncrptor->Encode(&m_Buffer[m_Tail], buf, len);
			}
			else
			{

				memcpy(&m_Buffer[m_Tail], buf, len);
			}
		}
		else
		{
			nFree = m_BufferLen - m_Tail;
			if (len <= nFree)
			{
				if (m_Setting == 0)
				{
					m_pEncrptor->Encode(&m_Buffer[m_Tail], buf, len);
				}
				else
				{
					memcpy(&m_Buffer[m_Tail], buf, len);
				}
			}
			else
			{
				if (m_Setting == 0)
				{
					m_pEncrptor->Encode(&m_Buffer[m_Tail], buf, nFree);
					m_pEncrptor->Encode(m_Buffer, &buf[nFree], len - nFree);
				}
				else
				{
					memcpy(&m_Buffer[m_Tail], buf, nFree);
					memcpy(m_Buffer, &buf[nFree], len - nFree);
				}
			}
		}
	}
	else
	{
		if (m_Setting == 0)
		{
			m_pEncrptor->Encode(&m_Buffer[m_Tail], buf, len);
		}
		else
		{
			memcpy(&m_Buffer[m_Tail], buf, len);
		}
	}

	m_Tail = (m_Tail + len) % m_BufferLen;

	return len;
}

bool NetSendStream::SendMessage(const IGameMessage* pMsg, bool bEncrypt/*=false*/)
{
	MSG_ID msgID = pMsg->getMsgID();
	if (bEncrypt)
	{
		msgID = msgID ^ m_uMsgHeadMagicNum;
		if ((msgID & 0x8000) != 0)
			msgID &= 0x7FFF;
	}
	uint32 w = Send_i((char*)&msgID, sizeof(MSG_ID));
	uint32 temppacketUINT;
	uint32 tempMsgSize = pMsg->getMsgSize();
	uint32 tempMsgIndex = pMsg->getMsgIndex();
	if (m_Setting == 0)
	{
		SET_MSG_INDEX(temppacketUINT, tempMsgIndex);
		SET_MSG_LEN(temppacketUINT, tempMsgSize);
	}
	else
	{
		BILL_SET_MSG_INDEX(temppacketUINT, tempMsgIndex);
		BILL_SET_MSG_LEN(temppacketUINT, tempMsgSize);
	}

	if (bEncrypt)
	{
		temppacketUINT = temppacketUINT ^ m_uMsgHeadMagicNum;
	}

	// for client format
	if (m_FormatMode > 0)
	{
		m_uSendCount = 0;
		// save offset
		m_iUintOffset = m_Tail;
	}

	w = Send_i((char*)&temppacketUINT, sizeof(uint32));

	if (!bEncrypt)
	{
		if (pMsg->send(*this))
		{
			if (m_FormatMode > 0)
			{
				//reset size
				uint32 uTemp = m_Tail;

				uint32 packetUINT;
				uint32 MsgSize = pMsg->getMsgSize() + m_uSendCount;
				uint32 MsgIndex = pMsg->getMsgIndex();
				if (m_Setting == 0)
				{
					SET_MSG_INDEX(packetUINT, MsgIndex);
					SET_MSG_LEN(packetUINT, MsgSize);
				}
				else
				{
					BILL_SET_MSG_INDEX(packetUINT, MsgIndex);
					BILL_SET_MSG_LEN(packetUINT, MsgSize);
				}

				if (bEncrypt)	packetUINT = packetUINT ^ m_uMsgHeadMagicNum;

				m_Tail = m_iUintOffset;
				w = Send_i((char*)&packetUINT, sizeof(uint32));
				m_Tail = uTemp;
			}
			return true;
		}
		else
			return false;
	}
	else
	{
		uint32 uTailBefore = m_Tail;
		if (pMsg->send(*this))
		{
			if (m_FormatMode > 0)
			{
				//reset size
				uint32 uTemp = m_Tail;

				uint32 packetUINT;
				uint32 MsgSize = pMsg->getMsgSize() + m_uSendCount;
				uint32 MsgIndex = pMsg->getMsgIndex();
				if (m_Setting == 0)
				{
					SET_MSG_INDEX(packetUINT, MsgIndex);
					SET_MSG_LEN(packetUINT, MsgSize);
				}
				else
				{
					BILL_SET_MSG_INDEX(packetUINT, MsgIndex);
					BILL_SET_MSG_LEN(packetUINT, MsgSize);
				}

				if (bEncrypt)	packetUINT = packetUINT ^ m_uMsgHeadMagicNum;

				m_Tail = m_iUintOffset;
				w = Send_i((char*)&packetUINT, sizeof(uint32));
				m_Tail = uTemp;
			}

			m_MessageEncryptor.Encode(pMsg->getMsgID() + pMsg->getMsgIndex(), (uchar*)m_Buffer, uTailBefore, m_Tail, m_BufferLen);
			return true;
		}
		else
			return false;
	}

	return false;
}

void NetSendStream::Initsize()
{
	m_Head = 0;
	m_Tail = 0;
	if (m_bStatic)
	{
		m_bFull = false;
		return;
	}
	if (m_Buffer)
	{
		delete[] m_Buffer;
		m_Buffer = NULL;
	}
	m_Buffer = new uint8[INITWRITESIZE];
	m_BufferLen = INITWRITESIZE;
	memset(m_Buffer, 0, m_BufferLen);
}

uint32 NetSendStream::Flush()
{
	if (IsServerData())
	{
		return FlushServerStream();
	}
	else
	{
		return FlushClientStream();
	}
}

uint32 NetSendStream::FlushServerStream()
{
	if (NULL == m_pSocket)
		return 0;

	int32 nFlushed = 0;
	int32 nSent = 0;
	int32 nLeft;

	if (m_BufferLen > m_MaxBufferLen)
	{
		Initsize();
		return NET_SOCKET_ERROR - 1;
	}

	//try
	{
		if (m_Head < m_Tail)
		{
			nLeft = m_Tail - m_Head;
			while (nLeft > 0)
			{
				if (!m_pSocket->Send(&m_Buffer[m_Head], nLeft, nSent))
				{
					return NET_SOCKET_ERROR - 2;
				}

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		}
		else if (m_Head > m_Tail)
		{
			nLeft = m_BufferLen - m_Head;
			while (nLeft > 0)
			{
				if (!m_pSocket->Send(&m_Buffer[m_Head], nLeft, nSent))
				{
					return NET_SOCKET_ERROR - 3;
				}

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}

			m_Head = 0;
			nLeft = m_Tail;

			while (nLeft > 0)
			{
				if (!m_pSocket->Send(&m_Buffer[m_Head], nLeft, nSent))
				{
					return NET_SOCKET_ERROR - 4;
				}

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		}
	}
	//catch (...)
	//{
	//	if (nSent > 0)
	//	{
	//		m_Head += nSent;
	//	}
	//}
	m_Head = m_Tail = 0;
	return nFlushed;
}

uint32 NetSendStream::FlushClientStream()
{
	if (NULL == m_pSocket)
		return 0;

	int32 nFlushed = 0;
	int32 nSent = 0;
	int32 nLeft;

	if (m_BufferLen > m_MaxBufferLen || m_bFull)
	{
		Initsize();
		return NET_SOCKET_ERROR - 1;
	}

	//try
	{
	bool bTotalSend = true;
	if (m_Head < m_Tail)
	{
		nLeft = m_Tail - m_Head;
		if (nLeft > SOCKET_SERVER2CLINET_SIZE_PER_FRAME)
		{
			nLeft = SOCKET_SERVER2CLINET_SIZE_PER_FRAME;
			bTotalSend = false;
		}
		while (nLeft > 0)
		{
			if (!m_pSocket->Send(&m_Buffer[m_Head], nLeft, nSent))
			{
				return NET_SOCKET_ERROR - 2;
			}

			nFlushed += nSent;
			nLeft -= nSent;
			m_Head += nSent;
		}
		if (bTotalSend == false)
		{
			return nFlushed;
		}
	}
	else if (m_Head > m_Tail)
	{
		nLeft = m_BufferLen - m_Head;
		if (nLeft > SOCKET_SERVER2CLINET_SIZE_PER_FRAME)
		{
			nLeft = SOCKET_SERVER2CLINET_SIZE_PER_FRAME;
			bTotalSend = false;
		}
		while (nLeft > 0)
		{
			if (!m_pSocket->Send(&m_Buffer[m_Head], nLeft, nSent))
			{
				return NET_SOCKET_ERROR - 3;
			}

			nFlushed += nSent;
			nLeft -= nSent;
			m_Head += nSent;
		}
		if (bTotalSend == false)
		{
			return nFlushed;
		}
		m_Head = 0;
	}
	}
	//catch(...)
	//{
	//	if (nSent > 0)
	//	{
	//		m_Head += nSent;
	//	}
	//}

	if (m_Head == m_Tail)
		m_Head = m_Tail = 0;

	return nFlushed;
}

bool NetSendStream::Resize(int32 size)
{
	if (IsStaticStream())
	{
		m_bFull = true;
		return false;
	}

	int32 orgSize = size;

	size = MAX(size, (int)(m_BufferLen >> 1));
	uint32 newBufferLen = m_BufferLen + size;
	uint32 len = Length();

	if (size < 0)
	{
		if (newBufferLen < 0 || newBufferLen < len)
			return false;
	}

	uint8 * newBuffer = new uint8[newBufferLen];
	if (newBuffer == NULL)
		return false;

	if (m_Head < m_Tail)
	{
		memcpy(newBuffer, &m_Buffer[m_Head], m_Tail - m_Head);
	}
	else if (m_Head > m_Tail)
	{
		memcpy(newBuffer, &m_Buffer[m_Head], m_BufferLen - m_Head);
		memcpy(&newBuffer[m_BufferLen - m_Head], m_Buffer, m_Tail);
	}

	delete[] m_Buffer;

	m_Buffer = newBuffer;
	m_BufferLen = newBufferLen;
	m_Head = 0;
	m_Tail = len;

	return true;
}

void NetSendStream::Clear()
{
	m_Head = 0;
	m_Tail = 0;
	m_bFull = false;
	m_uSendTail = 0;

	// 如果之前分配的buffer过大，这里清理一下，防止connector持有的内存膨胀 [4/28/2017 yz]
	if (m_BufferLen > m_nInitBufferLen)
	{
		delete[] m_Buffer;
		m_Buffer = new uint8[m_nInitBufferLen];
		m_BufferLen = m_nInitBufferLen;
	}
}

void NetSendStream::SetEncryptMode(uint32 mode)
{
	m_Setting = mode;
}

bool NetSendStream::HasDataToSend()
{
	if (IsServerData())
	{
		return (Length() != 0);
	}
	else
	{
		if (Length() != 0)
			return true;
		return false;
	}
}

void NetSendStream::EnableEncrpt(bool bEnable)
{
	m_pEncrptor->enable = bEnable ? 1 : 0;
}

bool NetSendStream::IsEnableEncrpt()
{
	return m_pEncrptor->enable > 0;
}

void NetSendStream::SetFormatMode(uint32 mode)
{
	m_FormatMode = mode;
}