#ifndef __EXCEPTIONHANDLER_H__
#define __EXCEPTIONHANDLER_H__

#include "TypeDefine.h"

// 异常处理管理器 [yangzhen]
class ExceptionHandler
{
public:
	ExceptionHandler();

#ifdef __LINUX__
	static void	Handler(int32);
	static void	INTHooker(int32);
	static void	TERMHooker(int32);
	static void	ABORTHooker(int32);
	static void	ILLHooker(int32);
	static void	FPEHooker(int32);
	static void	SEGHooker(int32);
	static void	XFSZHooker(int32);
	static void	BUSHooker(int32);
#endif
};
#endif
