#include "KwNetPrivate.h"
#include "KwNetLocoStreamSystem.h"
#include "KwNetMessage.h"

//----------------------------------------------------------------------------------------------
BOOL LocoRecieveStream::ReciveMessage( IMessage* pMsg ) 
{
	GUARD

	BOOL ret = SkipByte( MSG_LOCO_HEADER_SIZE );

	if( !ret )
		return FALSE ;

	return pMsg->Recieve( *this ) ;

	UNGUARD

	return FALSE ;
}

BOOL LocoSendStream::SendMessage( const IMessage* pMsg, bool bEncrypt/*=false*/ )
{
	GUARD

	int32 msgID = pMsg->GetMsgID();
	uint32 MsgSize = pMsg->GetMsgSize( ) ;
	uint32 MsgIndex = pMsg->GetComID( ) ;
	uint32 w;

	int32		separator = MSG_LOCO_SEPERATOR;						//分隔符	固定值5439
	int32		protocol = 1;										//接口命令标识	一般传1
	int32		action = 1;											//接口标识	一般传1
	int32		status = 0;											//状态	一般为0
	int32		locoPacketSize = MsgSize + 5*sizeof(int32);			//乐动包大小 乐动消息包大小与imessage的大小不同 需要减去40个字节才是imessage的大小

	w = Send_i((char*)&separator, sizeof(int32));
	w = Send_i((char*)&locoPacketSize, sizeof(int32));
	w = Send_i((char*)&protocol, sizeof(int32));
	w = Send_i((char*)&action, sizeof(int32));
	w = Send_i((char*)&MsgIndex, sizeof(uint32));
	w = Send_i((char*)&status, sizeof(int32));
	w = Send_i((char*)&msgID, sizeof(int32));

	if( pMsg->Send( *this ) )
		return TRUE;
	else
		return FALSE;

	UNGUARD
	return FALSE ;
}

void write_dump_buff(char* data, uint32 len)
{
	char dumpChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char* tempBuf = new char[len * 2 + 1];
	memset(tempBuf, 0, len * 2 + 1);
	for(int i = 0; i < len; ++i)
	{
		tempBuf[i * 2] = dumpChars[(unsigned char)data[i] / 16];
		tempBuf[i * 2 + 1] = dumpChars[(unsigned char)data[i] % 16];
	}
	tempBuf[len * 2] = 0;
	WriteNetLog("Loco Send: %s", tempBuf);
	delete tempBuf;
}

//----------------------------------------------------------------------------------------------
BOOL LocoUnifiedAndroidRecieveStream::ReciveMessage( IMessage* pMsg ) 
{
	GUARD

	BOOL ret = SkipByte( MSG_LOCO_UNIFIEDANDROID_HEADER_SIZE );
	if( !ret )
		return FALSE ;

	return pMsg->Recieve( *this ) ;

	UNGUARD

	return FALSE ;
}

BOOL LocoUnifiedAndroidSendStream::SendMessage( const IMessage* pMsg, bool bEncrypt/*=false*/ )
{
	GUARD

	uint16 msgID = (uint16)pMsg->GetMsgID();
	uint16 MsgSize = (uint16)pMsg->GetMsgSize( ) ;

	uint32 w;
	w = Send_i((char*)&msgID, sizeof(msgID));
	w = Send_i((char*)&MsgSize, sizeof(MsgSize));

	if( pMsg->Send( *this ) )
		return TRUE;
	else
		return FALSE;

	UNGUARD
	return FALSE ;
}

//----------------------------------------------------------------------------------------------
BOOL LocoGMRecieveStream::ReciveMessage( IMessage* pMsg ) 
{
	GUARD

		BOOL ret = SkipByte( MSG_LOCO_GM_HEADER_SIZE );
	if( !ret )
		return FALSE ;

	return pMsg->Recieve( *this ) ;

	UNGUARD

		return FALSE ;
}

BOOL LocoGMSendStream::SendMessage( const IMessage* pMsg, bool bEncrypt/*=false*/ )
{
	GUARD

	uint32 msgID = RevertWordEndian(GetLastMsgID());
	uint16 MsgSize = RevertWordEndian((uint16)pMsg->GetMsgSize( )) ;

	uint32 w;
	w = Send_i((char*)&msgID, sizeof(msgID));
	w = Send_i((char*)&MsgSize, sizeof(MsgSize));

	if( pMsg->Send( *this ) )
		return TRUE;
	else
		return FALSE;

	UNGUARD
		return FALSE ;
}