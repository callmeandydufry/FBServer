exports.MSG_HEADER_SIZE = 6;
exports.PLAT_DEFAULT = 1;


function GetServerName(ServerID)
{

}


function GetSystemTypeByServerID(ServerID)
{
	if (ServerID >= 17000 && ServerID <= 17099) 
	{
		return 0;
	}
	if (ServerID >= 17100 && ServerID <= 17199) 
	{
		return 0;
	}
	if (ServerID >= 17200 && ServerID <= 17299) 
	{
		return 1;
	}

	if (ServerID >= 18000 && ServerID <= 18099) 
	{
		return 0;
	}
	if (ServerID >= 18100 && ServerID <= 18199) 
	{
		return 0;
	}
	if (ServerID >= 18200 && ServerID <= 18299) 
	{
		return 1;
	}

	return -1;
	
}

exports.GetServerName = GetServerName;
exports.GetSystemTypeByServerID = GetSystemTypeByServerID;