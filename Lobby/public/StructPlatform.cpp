#include "stdafx.h"
#include "StructPlatform.h"
#include "Util.h"
//#include "hmac_sha1.h"

// 
// void PlatAuthReqData::SetMemberID(const char* key)
// {
// 	m_MemberIDSize = 0;
// 	memset(m_MemberID, 0, sizeof(m_MemberID));
// 
// 	if (key)
// 	{
// 		m_MemberIDSize = min((int)strlen(key), MAX_ACCOUNT);
// 		memcpy(m_MemberID, key, m_MemberIDSize);
// 	}
// }
// 
// void PlatAuthReqData::SetToken(const char* key)
// {
// 	m_TokenSize = 0;
// 	memset(m_Token, 0, sizeof(m_Token));
// 
// 	if (key)
// 	{
// 		m_TokenSize = min((int)strlen(key), MAX_TOKEN_LEN);
// 		memcpy(m_Token, key, m_TokenSize);
// 	}
// }
// 
// void PlatAuthReqData::SetMacAddress(const char* key)
// {
// 	m_MacAddressSize = 0;
// 	memset(m_MacAddress, 0, sizeof(m_MacAddress));
// 
// 	if (key)
// 	{
// 		m_MacAddressSize = min((int)strlen(key), MAX_MAC_ADDRESS);
// 		memcpy(m_MacAddress, key, m_MacAddressSize);
// 		CheckLawLessChar(m_MacAddress, MAX_MAC_ADDRESS);
// 	}
// }
// 
// void PlatAuthReqData::SetIp(const char* key)
// {
// 	m_IpSize = 0;
// 	memset(m_Ip, 0, sizeof(m_Ip));
// 
// 	if (key)
// 	{
// 		m_IpSize = min((int)strlen(key), IP_SIZE);
// 		memcpy(m_Ip, key, m_IpSize);
// 		CheckLawLessChar(m_Ip, m_IpSize);
// 	}
// }
// 
// void PlatAuthReqData::SetDeviceType(const char* key)
// {
// 	m_DeviceTypeSize = 0;
// 	memset(m_DeviceType, 0, sizeof(m_DeviceType));
// 
// 	if (key)
// 	{
// 		m_DeviceTypeSize = min((int)strlen(key), MAX_UNIQUECODE_LEN);
// 		memcpy(m_DeviceType, key, m_DeviceTypeSize);
// 		CheckLawLessChar(m_DeviceType, MAX_UNIQUECODE_LEN);
// 	}
// }
// 
// void PlatAuthReqData::SetDeviceIDFA(const char* key)
// {
// 	m_nDeviceIDFALen = 0;
// 	memset(m_szDeviceIDFA, 0, sizeof(m_szDeviceIDFA));
// 
// 	if (key)
// 	{
// 		m_nDeviceIDFALen = min((int)strlen(key), MAX_DEVICE_IDFA_LENGTH);
// 		memcpy(m_szDeviceIDFA, key, m_nDeviceIDFALen);
// 		CheckLawLessChar(m_szDeviceIDFA, MAX_DEVICE_IDFA_LENGTH);
// 	}
// }
// 
// void PlatAuthReqData::SetDeviceIDFV(const char* key)
// {
// 	m_nDeviceIDFVLen = 0;
// 	memset(m_szDeviceIDFV, 0, sizeof(m_szDeviceIDFV));
// 
// 	if (key)
// 	{
// 		m_nDeviceIDFVLen = min((int)strlen(key), MAX_DEVICE_IDFV_LENGTH);
// 		memcpy(m_szDeviceIDFV, key, m_nDeviceIDFVLen);
// 		CheckLawLessChar(m_szDeviceIDFV, MAX_DEVICE_IDFV_LENGTH);
// 	}
// }
// 
// void PlatAuthReqData::SetDeviceID(const char* key)
// {
// 	m_DeviceIDSize = 0;
// 	memset(m_DeviceID, 0, sizeof(m_DeviceID));
// 
// 	if (key)
// 	{
// 		m_DeviceIDSize = min((int)strlen(key), MAX_DEVICE_IDFA_LENGTH);
// 		memcpy(m_DeviceID, key, m_DeviceIDSize);
// 		CheckLawLessChar(m_DeviceID, MAX_DEVICE_IDFA_LENGTH);
// 	}
// }
// 
// BOOL PlatAuthReqData::Send(SendStream &oStream) const
// {
// 	oStream.Send((char*)&m_MemberIDSize, sizeof(m_MemberIDSize));
// 	if (m_MemberIDSize > 0)
// 	{
// 		oStream.Send((char*)m_MemberID, m_MemberIDSize);
// 	}
// 	oStream.Send((char*)&m_PlatAccountIDSize, sizeof(m_PlatAccountIDSize));
// 	if (m_PlatAccountIDSize > 0)
// 	{
// 		oStream.Send((char*)m_PlatAccountID, m_PlatAccountIDSize);
// 	}
// 	oStream.Send((char*)&m_TokenSize, sizeof(m_TokenSize));
// 	if (m_TokenSize > 0)
// 	{
// 		oStream.Send((char*)m_Token, m_TokenSize);
// 	}
// 	oStream.Send((char*)&m_MacAddressSize, sizeof(m_MacAddressSize));
// 	if (m_MacAddressSize > 0)
// 	{
// 		oStream.Send((char*)m_MacAddress, m_MacAddressSize);
// 	}
// 	oStream.Send((char*)&m_IpSize, sizeof(m_IpSize));
// 	if (m_IpSize > 0)
// 	{
// 		oStream.Send((char*)m_Ip, m_IpSize);
// 	}
// 	oStream.Send((char*)&m_DeviceTypeSize, sizeof(m_DeviceTypeSize));
// 	if (m_DeviceTypeSize > 0)
// 	{
// 		oStream.Send((char*)m_DeviceType, m_DeviceTypeSize);
// 	}
// 	oStream.Send((char*)&m_nDeviceIDFALen, sizeof(m_nDeviceIDFALen));
// 	if (m_nDeviceIDFALen > 0)
// 	{
// 		oStream.Send((char*)m_szDeviceIDFA, m_nDeviceIDFALen);
// 	}
// 	oStream.Send((char*)&m_nDeviceIDFVLen, sizeof(m_nDeviceIDFVLen));
// 	if (m_nDeviceIDFVLen > 0)
// 	{
// 		oStream.Send((char*)m_szDeviceIDFV, m_nDeviceIDFVLen);
// 	}
// 	oStream.Send((char*)&m_DeviceIDSize, sizeof(m_DeviceIDSize));
// 	if (m_DeviceIDSize > 0)
// 	{
// 		oStream.Send((char*)m_DeviceID, m_DeviceIDSize);
// 	}
// 	oStream.Send((char*)&m_Version, sizeof(m_Version));
// 	oStream.Send((char*)&m_DeviceOSType, sizeof(m_DeviceOSType));
// 	oStream.Send((char*)&m_UserType, sizeof(m_UserType));
// 	oStream.Send((char*)&m_AppId, sizeof(m_AppId));
// 	oStream.Send((char*)&m_ChannelId, sizeof(m_ChannelId));
// 	oStream.Send((char*)&m_ChildChannel, sizeof(m_ChildChannel));
// 	oStream.Send((char*)&m_GameId, sizeof(m_GameId));
// 	oStream.Send((char*)&m_GameServerId, sizeof(m_GameServerId));
// 	oStream.Send((char*)&m_ClientId, sizeof(m_ClientId));
// 	oStream.Send((char*)&m_UserId, sizeof(m_UserId));
// 
// 	return TRUE;
// }
// 
// BOOL PlatAuthReqData::Reci(RecieveStream &iStream) const
// {
// 	iStream.Reci((char*)&m_MemberIDSize, sizeof(m_MemberIDSize));
// 	if (m_MemberIDSize > 0)
// 	{
// 		iStream.Reci((char*)m_MemberID, m_MemberIDSize);
// 	}
// 	iStream.Reci((char*)&m_PlatAccountIDSize, sizeof(m_PlatAccountIDSize));
// 	if (m_PlatAccountIDSize > 0)
// 	{
// 		iStream.Reci((char*)m_PlatAccountID, m_PlatAccountIDSize);
// 	}
// 	iStream.Reci((char*)&m_TokenSize, sizeof(m_TokenSize));
// 	if (m_TokenSize > 0)
// 	{
// 		iStream.Reci((char*)m_Token, m_TokenSize);
// 	}
// 	iStream.Reci((char*)&m_MacAddressSize, sizeof(m_MacAddressSize));
// 	if (m_MacAddressSize > 0)
// 	{
// 		iStream.Reci((char*)m_MacAddress, m_MacAddressSize);
// 	}
// 	iStream.Reci((char*)&m_IpSize, sizeof(m_IpSize));
// 	if (m_IpSize > 0)
// 	{
// 		iStream.Reci((char*)m_Ip, m_IpSize);
// 	}
// 	iStream.Reci((char*)&m_DeviceTypeSize, sizeof(m_DeviceTypeSize));
// 	if (m_DeviceTypeSize > 0)
// 	{
// 		iStream.Reci((char*)m_DeviceType, m_DeviceTypeSize);
// 	}
// 	iStream.Reci((char*)&m_nDeviceIDFALen, sizeof(m_nDeviceIDFALen));
// 	if (m_nDeviceIDFALen > 0)
// 	{
// 		iStream.Reci((char*)m_szDeviceIDFA, m_nDeviceIDFALen);
// 	}
// 	iStream.Reci((char*)&m_nDeviceIDFVLen, sizeof(m_nDeviceIDFVLen));
// 	if (m_nDeviceIDFVLen > 0)
// 	{
// 		iStream.Reci((char*)m_szDeviceIDFV, m_nDeviceIDFVLen);
// 	}
// 	iStream.Reci((char*)&m_DeviceIDSize, sizeof(m_DeviceIDSize));
// 	if (m_DeviceIDSize > 0)
// 	{
// 		iStream.Reci((char*)m_DeviceID, m_DeviceIDSize);
// 	}
// 	iStream.Reci((char*)&m_Version, sizeof(m_Version));
// 	iStream.Reci((char*)&m_DeviceOSType, sizeof(m_DeviceOSType));
// 	iStream.Reci((char*)&m_UserType, sizeof(m_UserType));
// 	iStream.Reci((char*)&m_AppId, sizeof(m_AppId));
// 	iStream.Reci((char*)&m_ChannelId, sizeof(m_ChannelId));
// 	iStream.Reci((char*)&m_ChildChannel, sizeof(m_ChildChannel));
// 	iStream.Reci((char*)&m_GameId, sizeof(m_GameId));
// 	iStream.Reci((char*)&m_GameServerId, sizeof(m_GameServerId));
// 	iStream.Reci((char*)&m_ClientId, sizeof(m_ClientId));
// 	iStream.Reci((char*)&m_UserId, sizeof(m_UserId));
// 	
// 	return TRUE;
// }
// 
// uint32 PlatAuthReqData::GetSize() const
// {
// 	return sizeof(m_MemberIDSize) + m_MemberIDSize +
// 		sizeof(m_PlatAccountIDSize) + m_PlatAccountIDSize +
// 		sizeof(m_TokenSize) + m_TokenSize +
// 		sizeof(m_MacAddressSize) + m_MacAddressSize +
// 		sizeof(m_IpSize) + m_IpSize +
// 		sizeof(m_DeviceTypeSize) + m_DeviceTypeSize +
// 		sizeof(m_nDeviceIDFALen) + m_nDeviceIDFALen +
// 		sizeof(m_nDeviceIDFVLen) + m_nDeviceIDFVLen +
// 		sizeof(m_DeviceIDSize) + m_DeviceIDSize +
// 		sizeof(m_Version) + sizeof(m_DeviceOSType) + sizeof(m_UserType) + 
// 		sizeof(m_AppId) + sizeof(m_ChannelId) + sizeof(m_ChildChannel) +
// 		sizeof(m_GameId) + sizeof(m_GameServerId) + sizeof(m_ClientId)+
// 		sizeof(m_UserId);
// }
// 
// BOOL PlatAuthReq::Send(SendStream &oStream) const
// {
// 	oStream.Send((char*)&m_plat, sizeof(m_plat));
// 	oStream.Send((char*)&m_nPlayerID, sizeof(m_nPlayerID));
// 	m_authReqData.Send(oStream);
// 
// 	return TRUE;
// }
// 
// BOOL PlatAuthReq::Recieve(RecieveStream &iStream) const
// {
// 	iStream.Reci((char*)&m_plat, sizeof(m_plat));
// 	iStream.Reci((char*)&m_nPlayerID, sizeof(m_nPlayerID));
// 	m_authReqData.Reci(iStream);
// 
// 	return TRUE;
// }
// 
// uint32	PlatAuthReq::GetSize() const
// {
// 	uint32 size = sizeof(m_plat)+
// 		sizeof(m_nPlayerID);
// 	size += m_authReqData.GetSize();
// 
// 	return size;
// }
// 
// //---------------------------------------------------------------------------------------------
// BOOL PlatAuthRet::Send(SendStream &oStream) const
// {
// 	oStream.Send((char*)&m_plat, sizeof(m_plat));
// 	oStream.Send((char*)&m_nPlayerID, sizeof(m_nPlayerID));
// 	oStream.Send((char*)&m_Result, sizeof(m_Result));
// 	oStream.Send((char*)&m_MemberIDSize, sizeof(m_MemberIDSize));
// 	oStream.Send((char*)m_MemberID, m_MemberIDSize);
// 
// 	return TRUE;
// }
// 
// BOOL PlatAuthRet::Recieve(RecieveStream &iStream)
// {
// 	iStream.Reci((char*)&m_plat, sizeof(m_plat));
// 	iStream.Reci((char*)&m_nPlayerID, sizeof(m_nPlayerID));
// 	iStream.Reci((char*)&m_Result, sizeof(m_Result));
// 	iStream.Reci((char*)&m_MemberIDSize, sizeof(m_MemberIDSize));
// 	iStream.Reci((char*)m_MemberID, m_MemberIDSize);
// 
// 	return TRUE;
// }
// 
// uint32 PlatAuthRet::GetSize() const
// {
// 	uint32 size = 0;
// 	size += sizeof(m_plat);
// 	size += sizeof(m_nPlayerID);
// 	size += sizeof(m_Result);
// 	size += sizeof(m_MemberIDSize);
// 	size += m_MemberIDSize;
// 
// 	return size;
// }
// 
// 
// //-----------------------------------------------------------------------------------------------------------------
// void PlatOrderRetData::Clear()
// {
// 	init_netmessage_string(ServerID);
// 	init_netmessage_string(UID);
// 	m_ProductIndex = INVALID_ID;
// 	m_ProductPrice = 0;
// 	m_ChargeDiamond = 0;
// 	m_DonateDiamond = 0;
// 	init_netmessage_string(BillNo);
// }
// 
// BOOL PlatOrderRetData::Recieve(RecieveStream &iStream)
// {
// 	RECV_NET_MESSAGE_STRING_VAR(ServerID);
// 	RECV_NET_MESSAGE_STRING_VAR(UID);
// 	RECV_NET_MESSAGE_ATOM_VAR(ProductIndex);
// 	RECV_NET_MESSAGE_ATOM_VAR(ChargeDiamond);
// 	RECV_NET_MESSAGE_ATOM_VAR(DonateDiamond);
// 	RECV_NET_MESSAGE_ATOM_VAR(ProductPrice);
// 	RECV_NET_MESSAGE_STRING_VAR(BillNo);
// 	return TRUE;
// }
// 
// //-----------------------------------------------------------------------------------------------------------------
// BOOL PlatOrderRet::Recieve(RecieveStream &iStream)
// {
// 	iStream.Reci((char*)&m_plat, sizeof(m_plat));
// 	iStream.Reci((char*)&m_nPlayerSNID, sizeof(m_nPlayerSNID));
// 	iStream.Reci((char*)&m_Result, sizeof(m_Result));
// 	m_OrderRetData.Recieve(iStream);
// 
// 	return TRUE;
// }