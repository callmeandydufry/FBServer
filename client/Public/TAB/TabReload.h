/*$T Share/TAB/TabReload.h GC  */


/*$6
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef __TAB_RELOAD_H__
#define __TAB_RELOAD_H__

#include "StringHashMap.h"
#include "TypeDefine.h"

#ifdef __LINUX__
#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <setjmp.h>
#include "Atomic.h"
#endif

#define TAB_COMMAND_LENGTH			(128)
#define TAB_COMMAND_COUNT			(256)

// 当重读完成后内存拷贝的实现
#define DEFINE_TAB_RELOAD_TEMP(TabClass) extern TabClass* g_pBack##TabClass; extern uint32 g_uBack##TabClass##Count; \
	extern void Init##TabClass##ReloadTable(void*&, uint32&); extern void Load##TabClass##Table(); extern void Copy##TabClass##FromBack();

//当重读完成后内存拷贝的实现--用于固定结构的读表结构
#define DEFINE_TAB_RELOAD_TEMP_MISC(TabClass) extern TabClass g_Back##TabClass;\
	extern void Init##TabClass##ReloadTable(TabClass&);extern void Load##TabClass##Table(); extern void Copy##TabClass##FromBack();

//必要的基础定义和声明
#define BEGIN_DEFINE_TAB_RELOAD_VAR(TabClass, TabClassMaxCount)	TabClass *g_pBack##TabClass = NULL; \
	uint32 g_uBack##TabClass##Count = 0; uint32 u##TabClass##Count = TabClassMaxCount;

#define DEFINE_TAB_RELOAD_COMMAND(Key, TabClass)	\
	RegisterTabHandler(Key, &TabReloadDefine::Load##TabClass##Table, &TabReloadDefine::Copy##TabClass##FromBack);

// 为了跟Begin凑对
#define END_DEFINE_TAB_RELOAD_VAR(TabClass)	

#define BEGIN_DEFINE_TAB_RELOAD_MISC_VAR(TabClass)	TabClass g_pBack##TabClass; 
#define END_DEFINE_TAB_RELOAD_MISC_VAR(TabClass)	// 为了跟Begin凑对

/*------------------------------------------------------以上是相关宏定义---------------------------------------------------*/

class SplitStringLevelOne;
class SplitStringLevelTwo;
class TabReloadHandle;

typedef	void (*TabReloadCallBackFun) ();
typedef	void (*TabMemcpyCallBackFun) ();

// 主线程加载表的状态
enum TabReloadState
{
	eNORMAL_STATE = 0,
	eLOAD_TAB_STATE = 1,				// 主线程正在做表的加载
	eLOAD_TAB_FINISH = 2,

	eLOAD_TAB_STATE_NUM,
};

// 子线程发送Reload的命令状态
enum TabReloadCommandState
{
	eCOMMAND_FREE = 0,					// 空闲命令状态
	eCOMMAND_READY_SEND = 1,			// 子线程准备发送命令状态
	eCOMMAND_SEND_FINISH = 2,			// 子线程命令发送完成状态

	eCOMMAND_STATE_NUM,				// 指令状态数
};

struct TabReloadHandleStruct
{
	char m_szCommand[256];
	TabReloadCallBackFun m_ReloadCallBackFun;
	TabMemcpyCallBackFun m_MemcpyCallBackFun;

	TabReloadHandleStruct()
	{
		m_ReloadCallBackFun = NULL;
		m_MemcpyCallBackFun = NULL;
		memset(m_szCommand, 0, sizeof(m_szCommand));
	}
};

class TabReloadHandle
{
public:
	TabReloadHandle();
	virtual ~TabReloadHandle();

public:
	virtual BOOL	Init();

	BOOL	SendReloadTabCommand(const char* cTabCommandName);

	//		主线程执行的逻辑
	void	MainThreadProcessTabReloadCommand(const uint32 uCurTime);

	//		子线程执行的逻辑
	BOOL	LogicThreadProcessTabReloadCommand(int32 nContainerPos);


	/************************************************************************
	改变当前Reload命令状态
	nExpState		预期值
	nDesState		最终值
	返回成功或者失败
	************************************************************************/
	BOOL	ChangeReloadCommandState(int32 nExpState, int32 nDesState);

	/************************************************************************
	子线程的响应事件
	************************************************************************/
	void	ThreadResponse(int32 nContainerPos);

	/************************************************************************
	判定主线程是否还在处理指令,因为主线程处理完后,还要清理些标记,在清理标记时,
	子线程还不可以继续使用reload功能,当主线程完全处理完成后,最后将此标志置成
	完成
	************************************************************************/
	BOOL	IsProcessCommand()
	{
		return m_bIsProcessCommand;
	}

	//	注册活动线程,为统计线程的回应使用
	void	RegistActiveThread()
	{
		m_uThreadCount ++;
	}

protected:
	void	ClearCommand();

	BOOL	IsHasCommand()
	{
		return !(eCOMMAND_FREE == GetReloadCommandState());
	}

	//		注册处理Handle
	virtual	void	RegisterAllHandler();

	/************************************************************************
	注册回调函数
	@Tab			Key键值
	@reloadCallBack reload的实现
	@memcpyCallBack 内存复制的实现
	************************************************************************/
	void	RegisterTabHandler(char* Tab, TabReloadCallBackFun reloadCallBack, TabMemcpyCallBackFun memcpyCallBack);

	//		当执行完毕后,重置所有状态
	void	ResetAllState();

	void	ResetAllThreadSignal();

	// 判定是否所有Logic线程都已经给了回复信号
	BOOL					IsAllThreadReponse();

	void	ExecReloadCommand(const char* TabCommand);
	void	ExecMemcpyCommand(const char* TabCommand);

	//	判断是否注册了此命令
	BOOL	HasThisCommand(const char* TabCommand);

	int32	GetReloadCommandState()
	{
		int32 nState = -1;
#ifdef __LINUX__
		nState = atomic_read(&m_aTabReloadCommandState);
#else
		nState = (int32)m_aTabReloadCommandState;
#endif 
		return nState;
	}

protected:
	uint32					m_uThreadCount;
	
	TabReloadHandleStruct	m_CommandHandle[TAB_COMMAND_COUNT];
	uint32					m_uCommandCount;

	char					m_CurrentCommnd[TAB_COMMAND_LENGTH];

	// 主线程表加载的状态
	TabReloadState			m_eReloadSate;					
	
	// ReloadTab命令的状态
	atword					m_aTabReloadCommandState;
	
	// 标识主进程是否还在处理指令中
	BOOL					m_bIsProcessCommand;

	/*	
	子线程的计数器,当子线程收到要reloadtab的指令后,会向主线程发送确认信号,此变量便会依次累加,
	当该变量与m_uThreadCount数相等后,便可以执行reload行为
	*/
	BOOL*					m_pThreadSignal;

	//	设置主线程与Logic线程的定时器
	CTinyTimer				m_MainThreadProcessTimer;
	CTinyTimer				m_LogicThreadProcessTimer;
};
// 
// namespace TabReloadDefine
// {
// 	void	ReadTiemrDoingData
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		int32			*pVal,
// 		int32			MaxCnt
// 		);
// 	void	ReadTiemrDoingData_MapList
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		ut_id_map			*pVal,
// 		int32			MaxCnt
// 		);
// 	void	ReadTiemrDoingReserveEX
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		int32			*pVal,
// 		int32			MaxCnt
// 		);
// 	void	ReadTiemrDoingMapInfo
// 		(
// 		SplitStringLevelOne	*pSplitL1,
// 		SplitStringLevelTwo	*pSplitL2,
// 		const char		*pSplitString,
// 		int32			*pVal1,
// 		int32			*pVal2,
// 		int32			MaxCnt
// 		);
// 
// 	void	ReadSplitStringToTable
// 		(
// 		SplitStringLevelOne *pSplitL1,
// 		const char *pSplitString,
// 		int32 *pArry
// 		);
// 
// }

#endif