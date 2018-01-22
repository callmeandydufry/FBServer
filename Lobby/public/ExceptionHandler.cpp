#include "stdafx.h"
#include "ExceptionHandler.h"

#ifdef __WIN32__
#include "winbase.h"
#include "psapi.h"

#pragma pack(push,8)
#include <DbgHelp.h>
#pragma pack(pop)

#pragma comment(lib,"DbgHelp.lib")

/** 调用栈追踪相关
*	
*/
typedef BOOL  (WINAPI *TFEnumProcesses)( DWORD * lpidProcess, DWORD cb, DWORD * cbNeeded);
typedef BOOL  (WINAPI *TFEnumProcessModules)(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded);
typedef DWORD (WINAPI *TFGetModuleBaseName)(HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize);
typedef DWORD (WINAPI *TFGetModuleFileNameEx)(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);
typedef BOOL  (WINAPI *TFGetModuleInformation)(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);

static TFEnumProcesses			enumProcessesFunc;
static TFEnumProcessModules		enumProcessModules;
static TFGetModuleBaseName		getModuleBaseName;
static TFGetModuleFileNameEx	getModuleFileNameEx;
static TFGetModuleInformation	getModuleInformation;

/////////////////////////////////////////////////////////////////////////////
// 初始化符号引擎，加载各个模块（当前Exe以及引用的Dll）的PDB文件，创建符号表
/////////////////////////////////////////////////////////////////////////////
void LoadProcessModules()
{
	INT			ErrorCode = 0;	
	HANDLE		ProcessHandle = GetCurrentProcess(); 
	const INT	MAX_MOD_HANDLES = 1024;
	HMODULE		ModuleHandleArray[MAX_MOD_HANDLES];
	HMODULE*	ModuleHandlePointer = ModuleHandleArray;
	DWORD		BytesRequired;
	MODULEINFO	ModuleInfo;

	// 先枚举出来所有的模块[第一个执行文件，后面的都是相关动态库]
	BOOL bEnumProcessModulesSucceeded = enumProcessModules( ProcessHandle, ModuleHandleArray, sizeof(ModuleHandleArray), &BytesRequired );
	if( !bEnumProcessModulesSucceeded )
	{
		ErrorCode = GetLastError();
		return;
	}

	// 如果1024不够，那么就动态分配一个数组 [JiangWei]
	BOOL bNeedToFreeModuleHandlePointer = FALSE;
	if( BytesRequired > sizeof( ModuleHandleArray ) )
	{
		bNeedToFreeModuleHandlePointer = TRUE;
		ModuleHandlePointer = (HMODULE*)::malloc(BytesRequired);
		enumProcessModules( ProcessHandle, ModuleHandlePointer, sizeof(ModuleHandleArray), &BytesRequired );
	}

	INT	ModuleCount = BytesRequired / sizeof( HMODULE );
	for( INT ModuleIndex=0; ModuleIndex<ModuleCount; ModuleIndex++ )
	{
		char ModuleName[1024];
		char ImageName[1024];
		getModuleInformation( ProcessHandle, ModuleHandleArray[ModuleIndex], &ModuleInfo,sizeof( ModuleInfo ) );
		getModuleFileNameEx( ProcessHandle, ModuleHandleArray[ModuleIndex], ImageName, 1024 );
		getModuleBaseName( ProcessHandle, ModuleHandleArray[ModuleIndex], ModuleName, 1024 );

		// 加载模块字符表 [JiangWei]
		if( !SymLoadModule64( ProcessHandle, ModuleHandleArray[ModuleIndex], ImageName, ModuleName, (DWORD64) ModuleInfo.lpBaseOfDll, (DWORD) ModuleInfo.SizeOfImage ) )
		{
			ErrorCode = GetLastError();
		}
	} 

	if( bNeedToFreeModuleHandlePointer )
	{
		::free( ModuleHandlePointer );
	}
}

BOOL InitSymbolEngine(void)
{
	static BOOL bSymEngInitialized = FALSE;

	if( !bSymEngInitialized )
	{
		// 先加载process status application programming interface (PSAPI) 库 [JiangWei]
		void* DllHandle = (void*)LoadLibrary("PSAPI.dll");
		if( DllHandle == NULL )
		{
			return FALSE;
		}

		// 得到PSAPI中的导出函数 [JiangWei]
		enumProcessesFunc		= (TFEnumProcesses)			GetProcAddress( (HMODULE)DllHandle,TEXT("EnumProcesses"));
		enumProcessModules		= (TFEnumProcessModules)	GetProcAddress( (HMODULE)DllHandle,TEXT("EnumProcessModules"));
		getModuleFileNameEx		= (TFGetModuleFileNameEx)	GetProcAddress( (HMODULE)DllHandle,TEXT("GetModuleFileNameExA"));
		getModuleBaseName		= (TFGetModuleBaseName)		GetProcAddress( (HMODULE)DllHandle,TEXT("GetModuleBaseNameA"));
		getModuleInformation	= (TFGetModuleInformation)	GetProcAddress( (HMODULE)DllHandle,TEXT("GetModuleInformation"));

		if( !enumProcessesFunc || !enumProcessModules || !getModuleFileNameEx || !getModuleBaseName || !getModuleInformation )
		{
			return FALSE;
		}

		DWORD SymOpts = SymGetOptions();
		SymOpts |= SYMOPT_LOAD_LINES ;
		SymOpts |= SYMOPT_DEBUG;
		SymSetOptions ( SymOpts );
		SymInitialize ( GetCurrentProcess(), ".\\", TRUE );
		LoadProcessModules();

		bSymEngInitialized = TRUE;
	}
	return bSymEngInitialized;
}


// 开始调用平台相关的StackWalk [JiangWei]
int StackWalking(uint64* inOutStack,int inMaxDepth,CONTEXT* inContext)
{
	STACKFRAME64		StackFrame64;
	HANDLE				ProcessHandle;
	HANDLE				ThreadHandle;
	ulong				LastError;
	BOOL				bStackWalkSucceeded	= TRUE;
	DWORD				CurrentDepth		= 0;

	__try
	{
		ProcessHandle	= GetCurrentProcess();
		ThreadHandle	= GetCurrentThread();
		memset( &StackFrame64, 0, sizeof(StackFrame64) );

		StackFrame64.AddrPC.Offset       = inContext->Eip;
		StackFrame64.AddrPC.Mode         = AddrModeFlat;
		StackFrame64.AddrStack.Offset    = inContext->Esp;
		StackFrame64.AddrStack.Mode      = AddrModeFlat;
		StackFrame64.AddrFrame.Offset    = inContext->Ebp;
		StackFrame64.AddrFrame.Mode      = AddrModeFlat;

		while( bStackWalkSucceeded && (CurrentDepth < (DWORD)inMaxDepth) )
		{
			bStackWalkSucceeded = StackWalk64(  IMAGE_FILE_MACHINE_I386, 
				ProcessHandle, 
				ThreadHandle, 
				&StackFrame64,
				inContext,
				NULL,
				SymFunctionTableAccess64,
				SymGetModuleBase64,
				NULL );

			inOutStack[CurrentDepth++] = StackFrame64.AddrPC.Offset;

			if( !bStackWalkSucceeded  )
			{
				// 失败了，获得错误代码 [JiangWei]
				LastError = GetLastError( );
				break;
			}

			if( StackFrame64.AddrFrame.Offset == 0 )
			{
				break;
			}
		}

	} 
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		// 如果这里发生了异常，就捕获后不要向上抛了，否则导致无限循环 [JiangWei]
		return EXCEPTION_EXECUTE_HANDLER;
	} 

	// 把剩下的数组值置空 [JiangWei]
	for ( ; CurrentDepth < (DWORD)inMaxDepth; CurrentDepth++ )
	{
		inOutStack[CurrentDepth] = NULL;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void CaptureCallStack(uint64* inOutStack,int inMaxDepth,CONTEXT* inContext)
{
	if (NULL == inOutStack || 0 == inMaxDepth)
	{
		return;
	}

	if (NULL != inContext)
	{
		StackWalking(inOutStack,inMaxDepth,inContext);
	}
	else
	{
		CONTEXT HelperContext;
		memset( &HelperContext, 0, sizeof(CONTEXT) );
		HelperContext.ContextFlags = CONTEXT_FULL;

		__asm
		{
			call FakeFunctionCall
FakeFunctionCall: 
			pop eax
				mov HelperContext.Eip, eax
				mov HelperContext.Ebp, ebp
		}

		StackWalking(inOutStack,inMaxDepth,&HelperContext);
	}
	return;
}

void GetDumpFilename(char* szDumpPath)
{
	SYSTEMTIME st;
	GetLocalTime( &st );
	sprintf_s(szDumpPath, MAX_FILE_PATH, "ServerDump_%i_%02i_%02i-%02i_%02i_%02i.dmp", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

void CreateMiniDump(LPEXCEPTION_POINTERS inExceptionInfo)
{
	char szDumpFilename[MAX_FILE_PATH];
	memset(szDumpFilename, 0, sizeof(szDumpFilename));
	// 获取dump文件保存文件名 [yangzhen]
	GetDumpFilename(szDumpFilename);
	HANDLE hDumpFileHandle	= CreateFile( szDumpFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (INVALID_HANDLE_VALUE != hDumpFileHandle)
	{
		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ClientPointers = TRUE;
		exceptionInfo.ExceptionPointers = inExceptionInfo;
		exceptionInfo.ThreadId = GetCurrentThreadId();

		// 产生minidump文件 [yangzhen]
		MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hDumpFileHandle,MiniDumpNormal,&exceptionInfo,NULL,NULL);

		CloseHandle(hDumpFileHandle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 转换函数调用地址为具体的字符串，比如：FunctionName:ABC() FileName:XXX.cpp Line:1234
// 如果没有找到相应的PDB，也就是没办法解析调试符号，那么直接输出函数调用地址
/////////////////////////////////////////////////////////////////////////////
void ConvertCallAddressToCallStackString(UINT64 inCallAddress,char* inOutStackStrings,int inStackStringTotalSize)
{
#define MAX_NAME_LENGTH 512
	char			SymbolBuffer[sizeof(IMAGEHLP_SYMBOL64) + MAX_NAME_LENGTH];
	PIMAGEHLP_SYMBOL64	Symbol;
	DWORD				SymbolDisplacement		= 0;
	DWORD64				SymbolDisplacement64	= 0;
	DWORD				LastError;

	HANDLE				ProcessHandle = GetCurrentProcess();

	InitSymbolEngine();

	Symbol					= (PIMAGEHLP_SYMBOL64) SymbolBuffer;
	Symbol->SizeOfStruct	= sizeof(SymbolBuffer);
	Symbol->MaxNameLength	= MAX_NAME_LENGTH;

	if( SymGetSymFromAddr64( ProcessHandle, inCallAddress, &SymbolDisplacement64, Symbol ) )
	{
		char FunctionName[MAX_FILE_PATH];

		// 只保留'a' - 'z' 'A'-'Z'的字符，其他字符都剃掉 [JiangWei]
		INT Offset = 0;
		while( Symbol->Name[Offset] < 32 || Symbol->Name[Offset] > 127 )
		{
			Offset++;
		}

		sprintf( FunctionName,  ("%s() "), (const char*)(Symbol->Name + Offset) );

		// 追加到目标字符串中 [JiangWei]
		_tcscat_s( inOutStackStrings, inStackStringTotalSize, FunctionName );
	}
	else
	{
		// 没找到字符表 [JiangWei]
		LastError = GetLastError( );
	}

	//if( inExtraFlags & EF_DISPLAY_FILENAME )
	// 显示文件名 [yangzhen]
	{
		IMAGEHLP_LINE64		ImageHelpLine;
		char			FileNameLine[MAX_FILE_PATH];

		// 接下来输出文件和行数信息 [JiangWei]
		ImageHelpLine.SizeOfStruct = sizeof( ImageHelpLine );
		if( SymGetLineFromAddr64( ProcessHandle, inCallAddress, &SymbolDisplacement, &ImageHelpLine) )
		{
			sprintf( FileNameLine, ("Address = 0x%-8x + %d bytes [File=%s line=%d] "), (DWORD) inCallAddress, SymbolDisplacement, (const char*)(ImageHelpLine.FileName), ImageHelpLine.LineNumber );
		}
		else    
		{
			// 如果没有找到相应的PDB，也就是没办法解析调试符号，那么直接输出函数调用地址
			sprintf( FileNameLine, "Address = 0x%-8x (filename not found) ", (DWORD) inCallAddress );
		}

		// 追加到目标字符串中 [JiangWei]
		_tcscat_s( inOutStackStrings, inStackStringTotalSize, FileNameLine );
	}

	//if( inExtraFlags & EF_DISPLAY_MODULE )
	// 显示所在模块 [yangzhen]
	{
		IMAGEHLP_MODULE64	ImageHelpModule;
		char			ModuleName[MAX_FILE_PATH];

		ImageHelpModule.SizeOfStruct = sizeof( ImageHelpModule );
		if( SymGetModuleInfo64( ProcessHandle, inCallAddress, &ImageHelpModule) )
		{
			sprintf( ModuleName, "[in %s]", (const char*)(ImageHelpModule.ImageName) );

			// 追加到目标字符串中 [JiangWei]
			_tcscat_s( inOutStackStrings, inStackStringTotalSize, ModuleName );
		}
		else
		{
			LastError = GetLastError( );
		}
	}
}

// 跟踪调用栈 [JiangWei]
void TracingCallStack(void* inContext,char* inOutStackStrings,int inStackStringTotalSize)
{
	// 先初始化捕获调用栈所需的模块 [JiangWei]
	if (!InitSymbolEngine())
	{
		return;
	}

#define MAX_DEPTH 100
	uint64 callStackAddress[MAX_DEPTH];
	memset(callStackAddress,0,sizeof(callStackAddress));

	CaptureCallStack(callStackAddress,MAX_DEPTH,(CONTEXT*)inContext);

	int currentDepth = 0;
	while(0 != callStackAddress[currentDepth])
	{
		// 把函数调用地址转换成可读的字符串 [JiangWei]
		ConvertCallAddressToCallStackString(callStackAddress[currentDepth],inOutStackStrings,inStackStringTotalSize);
		_tcscat_s( inOutStackStrings, inStackStringTotalSize, "\r\n" );
		currentDepth++;
	}
}

void CreateCrashCallStack(LPEXCEPTION_POINTERS inExceptionInfo)
{
	const ulong callStackStringSize = 65535;
	char* callStackString = (char*)::malloc( callStackStringSize );
	callStackString[0] = 0;

	TracingCallStack(inExceptionInfo->ContextRecord,callStackString,callStackStringSize);

	char szLogPath[MAX_FILE_PATH];
	SYSTEMTIME st;
	GetLocalTime( &st );
	sprintf_s(szLogPath, MAX_FILE_PATH, "CrashStack_%i_%02i_%02i-%02i_%02i_%02i.log", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	HANDLE crashFileHandle	= CreateFile( szLogPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (INVALID_HANDLE_VALUE != crashFileHandle)
	{
		DWORD numberBytesNeedToWrite = strlen(callStackString);
		DWORD numberOfBytesWritten = 0;

		WriteFile(crashFileHandle,callStackString,numberBytesNeedToWrite,&numberOfBytesWritten,NULL);

		CloseHandle(crashFileHandle);
	}

	// 服务器不能被MessageBox阻塞，暂时屏蔽 [yangzhen]
	//MessageBox(NULL, callStackString,TEXT("Message"),MB_OK | MB_SYSTEMMODAL);
	::free(callStackString);
}

//__callback
//	WINBASEAPI
//	LONG
//	WINAPI
//	UnhandledExceptionFilter(
//	__in struct _EXCEPTION_POINTERS *ExceptionInfo
//	);

LONG WINAPI CreateErrorReport(_EXCEPTION_POINTERS* inExceptionInfo)
{
	if(IsDebuggerPresent())
	{
		// 挂了调试器，将错误 [yangzhen]
		return EXCEPTION_CONTINUE_SEARCH;
	}
	// 先生成MiniDump文件 [JiangWei]
	CreateMiniDump(inExceptionInfo);

	// 再生成完整调用栈信息文件 [JiangWei]
	CreateCrashCallStack(inExceptionInfo);

	return EXCEPTION_EXECUTE_HANDLER;
}
#else
// 非Windows平台 [7/8/2014 yangzhen]
#include <signal.h>

#endif

//linux下才回使用 处理异常向量的回调函数
ExceptionHandler::ExceptionHandler()
{
#ifdef __WIN32__
	SetUnhandledExceptionFilter(CreateErrorReport);
#endif

#ifdef __LINUX__
	signal(SIGSEGV, SEGHooker);
	signal(SIGFPE, FPEHooker);
	signal(SIGILL, ILLHooker);
	signal(SIGINT, INTHooker);
	//	signal(SIGTERM, TERMHooker);
	//	signal(SIGABRT, ABORTHooker);
	signal(SIGXFSZ, XFSZHooker);
	signal(SIGBUS, BUSHooker);
#endif
}

#ifdef __LINUX__
void ExceptionHandler::Handler(int32)
{
	exit(0);
}

void ExceptionHandler::INTHooker(int32)
{
	StackDump("INT exception:\r\n");
	abort();
// 	g_MapServer.Stop();
// 	SafeSleep(20000);
// 	exit(0);
}

void ExceptionHandler::TERMHooker(int32)
{
	StackDump("TERM exception:\r\n");
	abort();
// 	g_MapServer.Stop();
// 	SafeSleep(20000);
// 	exit(0);
}

void ExceptionHandler::ABORTHooker(int32)
{
	StackDump("ABORT exception:\r\n");
	abort();
// 	g_MapServer.Stop();
// 	SafeSleep(20000);
// 	exit(0);
}

void ExceptionHandler::ILLHooker(int32)
{
	StackDump("ILL exception:\r\n");
	abort();
// 	g_MapServer.Stop();
// 	SafeSleep(20000);
// 	exit(0);
}

void ExceptionHandler::FPEHooker(int32)
{
	StackDump("FPE exception:\r\n");
	abort();
// 	g_MapServer.Stop();
// 	SafeSleep(20000);
// 	exit(0);
}

void ExceptionHandler::SEGHooker(int32)
{
	StackDump("SEG exception:\r\n");
	abort();
// 	g_MapServer.Stop();
// 	SafeSleep(20000);
// 	exit(0);
}

void ExceptionHandler::XFSZHooker(int32)
{
	StackDump("XFSZ exception:\r\n");
	abort();
// 	g_MapServer.Stop();
// 	SafeSleep(20000);
// 	exit(0);
}

void ExceptionHandler::BUSHooker(int32)
{
	StackDump("BUS exception:\r\n");
	abort();
	// 	g_MapServer.Stop();
	// 	SafeSleep(20000);
	// 	exit(0);
}
#endif
