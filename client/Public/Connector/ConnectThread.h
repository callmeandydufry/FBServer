/*********************************************************************************************
filename: 	ConnectThread.h
purpose:	���߳��Ǹ��������˽�������
history:
			1.added by Michael Lin 2011-10-27
**********************************************************************************************/

#ifndef __CONNECTTHREAD_H__
#define __CONNECTTHREAD_H__

#include "TypeDefine.h"
#include "BaseTool.h"
#include "ConnectorManager.h"

class ConnectThread :public KThread
{

public:
	ConnectThread(ConnectorManager* pConnectorManager);
	~ConnectThread();

	//�߳����к���
	virtual void	run();


	virtual void stop()
	{
		m_Active = FALSE;
	};


	BOOL IsActive()
	{
		return m_Active;
	};

private:
	BOOL			  m_Active;
	ConnectorManager* m_pConnectorManager;   //���ӹ�����
};
#endif //__CONNECTTHREAD_H__
