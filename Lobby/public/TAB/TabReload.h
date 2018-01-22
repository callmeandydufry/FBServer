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

// ���ض���ɺ��ڴ濽����ʵ��
#define DEFINE_TAB_RELOAD_TEMP(TabClass) extern TabClass* g_pBack##TabClass; extern uint32 g_uBack##TabClass##Count; \
	extern void Init##TabClass##ReloadTable(void*&, uint32&); extern void Load##TabClass##Table(); extern void Copy##TabClass##FromBack();

//���ض���ɺ��ڴ濽����ʵ��--���ڹ̶��ṹ�Ķ���ṹ
#define DEFINE_TAB_RELOAD_TEMP_MISC(TabClass) extern TabClass g_Back##TabClass;\
	extern void Init##TabClass##ReloadTable(TabClass&);extern void Load##TabClass##Table(); extern void Copy##TabClass##FromBack();

//��Ҫ�Ļ������������
#define BEGIN_DEFINE_TAB_RELOAD_VAR(TabClass, TabClassMaxCount)	TabClass *g_pBack##TabClass = NULL; \
	uint32 g_uBack##TabClass##Count = 0; uint32 u##TabClass##Count = TabClassMaxCount;

#define DEFINE_TAB_RELOAD_COMMAND(Key, TabClass)	\
	RegisterTabHandler(Key, &TabReloadDefine::Load##TabClass##Table, &TabReloadDefine::Copy##TabClass##FromBack);

// Ϊ�˸�Begin�ն�
#define END_DEFINE_TAB_RELOAD_VAR(TabClass)	

#define BEGIN_DEFINE_TAB_RELOAD_MISC_VAR(TabClass)	TabClass g_pBack##TabClass; 
#define END_DEFINE_TAB_RELOAD_MISC_VAR(TabClass)	// Ϊ�˸�Begin�ն�

/*------------------------------------------------------��������غ궨��---------------------------------------------------*/

class SplitStringLevelOne;
class SplitStringLevelTwo;
class TabReloadHandle;

typedef	void (*TabReloadCallBackFun) ();
typedef	void (*TabMemcpyCallBackFun) ();

// ���̼߳��ر��״̬
enum TabReloadState
{
	eNORMAL_STATE = 0,
	eLOAD_TAB_STATE = 1,				// ���߳���������ļ���
	eLOAD_TAB_FINISH = 2,

	eLOAD_TAB_STATE_NUM,
};

// ���̷߳���Reload������״̬
enum TabReloadCommandState
{
	eCOMMAND_FREE = 0,					// ��������״̬
	eCOMMAND_READY_SEND = 1,			// ���߳�׼����������״̬
	eCOMMAND_SEND_FINISH = 2,			// ���߳���������״̬

	eCOMMAND_STATE_NUM,				// ָ��״̬��
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

	//		���߳�ִ�е��߼�
	void	MainThreadProcessTabReloadCommand(const uint32 uCurTime);

	//		���߳�ִ�е��߼�
	BOOL	LogicThreadProcessTabReloadCommand(int32 nContainerPos);


	/************************************************************************
	�ı䵱ǰReload����״̬
	nExpState		Ԥ��ֵ
	nDesState		����ֵ
	���سɹ�����ʧ��
	************************************************************************/
	BOOL	ChangeReloadCommandState(int32 nExpState, int32 nDesState);

	/************************************************************************
	���̵߳���Ӧ�¼�
	************************************************************************/
	void	ThreadResponse(int32 nContainerPos);

	/************************************************************************
	�ж����߳��Ƿ��ڴ���ָ��,��Ϊ���̴߳������,��Ҫ����Щ���,��������ʱ,
	���̻߳������Լ���ʹ��reload����,�����߳���ȫ������ɺ�,��󽫴˱�־�ó�
	���
	************************************************************************/
	BOOL	IsProcessCommand()
	{
		return m_bIsProcessCommand;
	}

	//	ע���߳�,Ϊͳ���̵߳Ļ�Ӧʹ��
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

	//		ע�ᴦ��Handle
	virtual	void	RegisterAllHandler();

	/************************************************************************
	ע��ص�����
	@Tab			Key��ֵ
	@reloadCallBack reload��ʵ��
	@memcpyCallBack �ڴ渴�Ƶ�ʵ��
	************************************************************************/
	void	RegisterTabHandler(char* Tab, TabReloadCallBackFun reloadCallBack, TabMemcpyCallBackFun memcpyCallBack);

	//		��ִ����Ϻ�,��������״̬
	void	ResetAllState();

	void	ResetAllThreadSignal();

	// �ж��Ƿ�����Logic�̶߳��Ѿ����˻ظ��ź�
	BOOL					IsAllThreadReponse();

	void	ExecReloadCommand(const char* TabCommand);
	void	ExecMemcpyCommand(const char* TabCommand);

	//	�ж��Ƿ�ע���˴�����
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

	// ���̱߳���ص�״̬
	TabReloadState			m_eReloadSate;					
	
	// ReloadTab�����״̬
	atword					m_aTabReloadCommandState;
	
	// ��ʶ�������Ƿ��ڴ���ָ����
	BOOL					m_bIsProcessCommand;

	/*	
	���̵߳ļ�����,�����߳��յ�Ҫreloadtab��ָ���,�������̷߳���ȷ���ź�,�˱�����������ۼ�,
	���ñ�����m_uThreadCount����Ⱥ�,�����ִ��reload��Ϊ
	*/
	BOOL*					m_pThreadSignal;

	//	�������߳���Logic�̵߳Ķ�ʱ��
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