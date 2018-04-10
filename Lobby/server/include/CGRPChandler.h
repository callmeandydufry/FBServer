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
	// �ʼ���� [1/31/2018 Chief]
	//-------------------------------------------------------------------------------------------------------------
	// ����ǰ�Ƿ����µ��ʼ� [1/31/2018 Chief]
	virtual void requestNewMailList(SNID_t snid);
	// �������������ʼ�list [1/31/2018 Chief]
	virtual void requestTypeMailList(SNID_t snid, int32 mailType);
	// ����ʼ�����,��ǵ�ǰ�ʼ��Ѷ� [1/31/2018 Chief]
	virtual void requestMailDetial(SNID_t snid, int32 mailID);
	// ��ȡ�ʼ����� [1/31/2018 Chief]
	virtual void requestGainMail(SNID_t snid, int32 mailID);
	// ɾ�������ʼ� [1/31/2018 Chief]
	virtual void requestDelMailByID(SNID_t snid, int32 mailID);
	// ����ɾ��ĳһ���͵��ʼ� [1/31/2018 Chief]
	virtual void requestDelMailByType(SNID_t snid, int32 mailType);

private:
	ClientSession*			mClientSession;
};

#endif