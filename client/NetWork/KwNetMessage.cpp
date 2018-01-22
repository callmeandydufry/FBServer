
#include "KwNetPrivate.h"
#include "KwNetMessage.h"

uint32	GetMsgIndex(uint32 a)
{
	return	((a)&0x000000ff);//((a)>>24);
}
void	SetMsgIndex(uint32& a,uchar index)
{
	((a)=((a)&0xffffff00)+(index));//((a)=(((a)&0xffffff)+((index)<<24)));
}
uint32	GetMsgLen(uint32 a)
{
	return	(a)>>8;//((a)&0xffffff);
}
void	SetMsgLen(uint32& a,uint32 Len)
{
	((a)=(((a)&0x000000ff)+((Len)<<8)));//((a)=((a)&0xff000000)+(Len));
}




IMessage::IMessage( )
{
GUARD



UNGUARD
}

IMessage::~IMessage( )
{
GUARD



UNGUARD
}

