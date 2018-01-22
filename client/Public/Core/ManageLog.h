/*$T MapServer/Server/Base/ManageLog.h GC 1.140 10/10/07 10:07:21 */

/*
 * created: 2008/09/26 created: 26:9:2008 14:35 filename:
 * MapServer\Server\Base\ManageLog.h file path: MapServer\Server\Base file base:
 * ManageLog file ext: h author: Richard purpose: 运营Log系统
 */
#include "TypeDefine.h"
#include "GameLogDefine.h"
#ifndef ManageLog_h__26_9_2008_19_24
#define ManageLog_h__26_9_2008_19_24

class	ManageLog
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	ManageLog();
	~	ManageLog();
	void	Init(void);
	void	Clear(void);
	BOOL	HeartBeat(ulong dwTime);
	void	ExecuteSaveLog();
	void	Reset();

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:
	ulong		m_dwTimer;
	CTinyTimer	m_MessageCatchLogTimer;
};
extern ManageLog	g_ManageLog;
#endif /* ManageLog_h__26_9_2008_19_24 */
