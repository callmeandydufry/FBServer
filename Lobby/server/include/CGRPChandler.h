#ifndef __CG_RPC_HANDLER_H__
#define __CG_RPC_HANDLER_H__

#include "ServerRPCMethodInterface.h"
#include "ClientSession.h"

class CGRPCHandler : public ServerRPCMethodHandler
{
public:
	CGRPCHandler(ClientSession* dataReceiver)
		: mClientSession(dataReceiver),
		ServerRPCMethodHandler(dataReceiver)
	{}

	virtual void sendChatMsg2OtherPlayer(const char* chatContent, SNID_t u64ReceiverID);
	virtual void requestPlayerData(SNID_t snid);
	//-------------------------------------------------------------------------------------------------------------
	// 邮件相关 [1/31/2018 Chief]
	//-------------------------------------------------------------------------------------------------------------
	// 请求当前是否有新的邮件 [1/31/2018 Chief]
	virtual void requestNewMailList(SNID_t snid);
	// 根据类型请求邮件list [1/31/2018 Chief]
	virtual void requestTypeMailList(SNID_t snid, int32 mailType);
	// 获得邮件详情,标记当前邮件已读 [1/31/2018 Chief]
	virtual void requestMailDetial(SNID_t snid, int32 mailID);
	// 获取邮件附件 [1/31/2018 Chief]
	virtual void requestGainMail(SNID_t snid, int32 mailID);
	// 删除单个邮件 [1/31/2018 Chief]
	virtual void requestDelMailByID(SNID_t snid, int32 mailID);
	// 批量删除某一类型的邮件 [1/31/2018 Chief]
	virtual void requestDelMailByType(SNID_t snid, int32 mailType);

private:
	ClientSession*			mClientSession;
};

#endif