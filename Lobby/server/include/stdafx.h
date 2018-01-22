
#ifndef _STD_AFX_PRE_H_
#define _STD_AFX_PRE_H_

#pragma warning (disable:4311)
#pragma warning (disable:4312)
#pragma warning (disable:4996)

#ifdef __WINDOWS__
#define WIN32_LEAN_AND_MEAN
#include <tchar.h>
#endif
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "stdarg.h"

#include "Version.h"
#include "TypeDefine.h"
#include "KwNetBasics.h"

#include "Setting.h"
#include "FileConfigConst.h"
#include "BaseTool.h"

#include "KwNetMessage.h"
#include "MessageDefine.h"
#include "MessageFactory.h"

#include "Util.h"

#include "Define.h"
#include "Enum.h"
#include "DefineResult.h"

#include "KwNetStreamSystem.h"

#include "ConnectorPool.h"
#include "MessageFactoryManager.h"

#endif
