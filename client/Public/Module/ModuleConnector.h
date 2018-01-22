#ifndef __ModuleConnector_H__
#define __ModuleConnector_H__

#include "ServerConnector.h"

class ModuleConnector : public Connector
{
public:
	ModuleConnector();
	virtual ~ModuleConnector();

	virtual void InitSocket()
	{
		Connector::InitSocket();
		SetEncryptMode(ENCPYPT_TYPE_PLATFORMSERVER);
	}

	virtual BOOL Handle_Command();

	virtual BOOL	IsGameConnector()				{ return FALSE; }
	virtual BOOL	IsServerConnector()				{ return TRUE; }
};

#endif