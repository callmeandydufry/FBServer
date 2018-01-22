#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"
#include "BaseTool.h"
#include<sstream>

//-----------------------------------------------------------------------
const String StringUtil::BLANK;
//-----------------------------------------------------------------------
const String& StringUtil::trim(String& str, bool left, bool right)
{
	static const String delims = " \t\r";
	if (right)
		str.erase(str.find_last_not_of(delims) + 1); // trim right
	if (left)
		str.erase(0, str.find_first_not_of(delims)); // trim left

	return str;
}

//-----------------------------------------------------------------------
std::vector<String> StringUtil::split(const String& str, const String& delims, unsigned int maxSplits)
{
	std::vector<String> ret;
	// Pre-allocate some space for performance
	ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

	unsigned int numSplits = 0;

	// Use STL methods 
	size_t start, pos;
	start = 0;
	do
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the string
			ret.push_back(str.substr(start));
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back(str.substr(start, pos - start));
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != String::npos);



	return ret;
}

//-----------------------------------------------------------------------
void StringUtil::toLowerCase(String& str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		tolower);
}

//-----------------------------------------------------------------------
void StringUtil::toUpperCase(String& str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		toupper);
}
//-----------------------------------------------------------------------
bool StringUtil::startsWith(const String& str, const String& pattern, bool lowerCase)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	String startOfThis = str.substr(0, patternLen);
	if (lowerCase)
		StringUtil::toLowerCase(startOfThis);

	return (startOfThis == pattern);
}
//-----------------------------------------------------------------------
bool StringUtil::endsWith(const String& str, const String& pattern, bool lowerCase)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	String endOfThis = str.substr(thisLen - patternLen, patternLen);
	if (lowerCase)
		StringUtil::toLowerCase(endOfThis);

	return (endOfThis == pattern);
}
//-----------------------------------------------------------------------
String StringUtil::standardisePath(const String& init)
{
	String path = init;

	std::replace(path.begin(), path.end(), '\\', '/');
	if (path[path.length() - 1] != '/')
		path += '/';

	return path;
}
//-----------------------------------------------------------------------
void StringUtil::splitFilename(const String& qualifiedName,
	String& outBasename, String& outPath)
{
	String path = qualifiedName;
	// Replace \ with / first
	std::replace(path.begin(), path.end(), '\\', '/');
	// split based on final /
	size_t i = path.find_last_of('/');

	if (i == String::npos)
	{
		outPath.clear();
		outBasename = qualifiedName;
	}
	else
	{
		outBasename = path.substr(i + 1, path.size() - i - 1);
		outPath = path.substr(0, i + 1);
	}
}
//-----------------------------------------------------------------------
void StringUtil::splitBaseFilename(const String& fullName,
	String& outBasename, String& outExtention)
{
	size_t i = fullName.find_last_of(".");
	if (i == String::npos)
	{
		outExtention.clear();
		outBasename = fullName;
	}
	else
	{
		outExtention = fullName.substr(i + 1);
		outBasename = fullName.substr(0, i);
	}
}
// ----------------------------------------------------------------------------------------------------------------------------------------------
void StringUtil::splitFullFilename(const String& qualifiedName,
	String& outBasename, String& outExtention, String& outPath)
{
	String fullName;
	splitFilename(qualifiedName, fullName, outPath);
	splitBaseFilename(fullName, outBasename, outExtention);
}
//-----------------------------------------------------------------------
bool StringUtil::match(const String& str, const String& pattern, bool caseSensitive)
{
	String tmpStr = str;
	String tmpPattern = pattern;
	if (!caseSensitive)
	{
		StringUtil::toLowerCase(tmpStr);
		StringUtil::toLowerCase(tmpPattern);
	}

	String::const_iterator strIt = tmpStr.begin();
	String::const_iterator patIt = tmpPattern.begin();
	String::const_iterator lastWildCardIt = tmpPattern.end();
	while (strIt != tmpStr.end() && patIt != tmpPattern.end())
	{
		if (*patIt == '*')
		{
			lastWildCardIt = patIt;
			// Skip over looking for next character
			++patIt;
			if (patIt == tmpPattern.end())
			{
				// Skip right to the end since * matches the entire rest of the string
				strIt = tmpStr.end();
			}
			else
			{
				// scan until we find next pattern character
				while (strIt != tmpStr.end() && *strIt != *patIt)
					++strIt;
			}
		}
		else
		{
			if (*patIt != *strIt)
			{
				if (lastWildCardIt != tmpPattern.end())
				{
					// The last wildcard can match this incorrect sequence
					// rewind pattern to wildcard and keep searching
					patIt = lastWildCardIt;
					lastWildCardIt = tmpPattern.end();
				}
				else
				{
					// no wildwards left
					return false;
				}
			}
			else
			{
				++patIt;
				++strIt;
			}
		}

	}
	// If we reached the end of both the pattern and the string, we succeeded
	if (patIt == tmpPattern.end() && strIt == tmpStr.end())
	{
		return true;
	}
	else
	{
		return false;
	}

}

#define STARTING_BUFFER_SIZE		128
String StringUtil::printf(const char* format, ...)
{
#if _MSC_VER
	va_list argList;
	va_start(argList, format);
	int length = _vscprintf(format, argList);
	char* buffer = new char[length + 1];
	//_vsnprintf(buffer,length,format,argList);
	int hr = _vsnprintf_s(buffer, length + 1, _TRUNCATE, format, argList);
	//int hr = sprintf_s(buffer,length,format);
	va_end(argList);

	String str(buffer);
	delete [] buffer;

	return str;
#else
	int		BufferSize = STARTING_BUFFER_SIZE;
	char	StartingBuffer[STARTING_BUFFER_SIZE];
	char*	Buffer = StartingBuffer;
	int		Result = -1;

	// 先尝试print到128字节的字符串中 [JiangWei]
	GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, format, format, Result);

	// 如果失败，则不停的尝试Realloc内存，直到能放下结果未知 [JiangWei]
	if (Result == -1)
	{
		Buffer = NULL;
		while (Result == -1)
		{
			BufferSize *= 2;
			if (Buffer)
			{
				Buffer = (char*)::realloc(Buffer, BufferSize * sizeof(char));
			}
			else
			{
				Buffer = (char*)::malloc(BufferSize * sizeof(char));
			}
			
			GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, format, format, Result);
		};
	}

	Buffer[Result] = 0;
	String ResultString(Buffer);
	if (BufferSize != STARTING_BUFFER_SIZE)
	{
		::free(Buffer);
	}

	return ResultString;
#endif
}

#ifdef _MSC_VER

// 得到变长参数列表 [JiangWei]
int kwGetVarArgs(char* inDest, ulong inDestSize, int inCount, const char*& inFmt, va_list inArgPtr)
{
#if ENABLE_SECURE_CRT
	int Result = _vsntprintf_s(inDest, inDestSize, inCount/*_TRUNCATE*/, inFmt, inArgPtr);
#else
	int Result = _vsntprintf(inDest, inCount, inFmt, inArgPtr);
#endif
	va_end(inArgPtr);
	return Result;
}

#elif __LINUX__

int kwGetVarArgs(char* inDest, ulong inDestSize, int inCount, const char*& inFmt, va_list inArgPtr)
{
#if UNICODE
	int Result = vswprintf_Replacement(Dest, Count, Fmt, ArgPtr);
#else
	int Result = vsnprintf(inDest, inCount, inFmt, inArgPtr);
#endif
	va_end(inArgPtr);
	return Result;
	}

#endif

int StringUtil::sprintf(char* inDest, const char* inFmt, ...)
{
	int	result = -1;
	va_list ap;
	va_start(ap, inFmt);
	result = kwGetVarArgs(inDest, MAX_SPRINTF, MAX_SPRINTF - 1, inFmt, ap);

	return result;
}

int StringUtil::snprintf(char* inDest, uint32 inSize, const char* inFmt, ...)
{
	int	result = -1;
	va_list ap;
	va_start(ap, inFmt);
	result = kwGetVarArgs(inDest, inSize, inSize - 1, inFmt, ap);

	return result;
}

float StringUtil::parseFloat(const String& val)
{
#ifdef ANDROID
	float ret = 0;
	int n = sscanf(val.c_str(), "%f", &ret);

	if (n == 0) {
		// Nothing read, so try integer format
		int ret2 = 0;
		n = sscanf(val.c_str(), "%d", &ret2);
		if (n == 1)
			ret = (float)ret2;
	}
#else
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	float ret = 0;
	str >> ret;
#endif
	return ret;
}

double StringUtil::parseDouble(const String& val)
{
#ifdef ANDROID
	double ret = 0;
	int n = sscanf(val.c_str(), "%lf", &ret);

	if (n == 0) {
		// Nothing read, so try integer format
		int64 ret2 = 0;
		n = sscanf(val.c_str(), "%lld", &ret2);
		if (n == 1)
			ret = (double)ret2;
	}
#else
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	double ret = 0;
	str >> ret;
#endif
	return ret;
}

int StringUtil::parseInt(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	int ret = 0;
	str >> ret;

	return ret;
}

unsigned int StringUtil::parseUnsignedInt(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	unsigned int ret = 0;
	str >> ret;

	return ret;
}

long StringUtil::parseLong(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	long ret = 0;
	str >> ret;

	return ret;
}

int64 StringUtil::parseInt64(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	int64 ret = 0;
	str >> ret;

	return ret;
}

uint64 StringUtil::parseUnsignedInt64(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	uint64 ret = 0;
	str >> ret;

	return ret;
}

bool StringUtil::parseBool(const String& val)
{
	return (StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
		|| StringUtil::startsWith(val, "1"));
}

uint32 gCRCTable[256];
void kwInitStrCrc()
{
#define CRC32_POLY 0x04c11db7

	for (uint32 iCRC = 0; iCRC < 256; iCRC++)
	{
		for (uint32 c = iCRC << 24, j = 8; j != 0; j--)
		{
			gCRCTable[iCRC] = c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
		}
	}
}

uint32 kwStrCrc(const String& Data)
{
	//int length = appStrlen( Data );
	size_t length = Data.length();
	uint32 CRC = 0xFFFFFFFF;
	for (size_t i = 0; i < length; i++)
	{
		char C = Data[i];
		int   CL = (C & 255);
		CRC = (CRC << 8) ^ gCRCTable[(CRC >> 24) ^ CL];;
#if UNICODE
		int   CH = (C >> 8) & 255;
		CRC = (CRC << 8) ^ gCRCTable[(CRC >> 24) ^ CH];;
#endif
	}
	return ~CRC;
}

char* kwItoa(int inNum, char* inOutStr, int radix)
{
	StringUtil::sprintf(inOutStr, "%d", inNum);

	return inOutStr;
}

//-----------------------------------------------------------------------