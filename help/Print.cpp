#include "Printf.h"

#include <memory>
#include <stdarg.h>
#include <stdio.h>

#include "EngineConfig.h"
#ifdef _SPDLOG_
#include "SpdLogWrapper.h"
#endif

#if PLATFORM_WINDOWS
#define MAX_PRINT_LENGTH 4096
#else 
#define MAX_PRINT_LENGTH 2048
#endif

void Print(const char* format, ...)
{
	thread_local std::unique_ptr<char[]> buf(new char[MAX_PRINT_LENGTH]);

	// what you printf --> s 这里就直接填充成了字符串了
	// http://www.cplusplus.com/reference/cstdio/vsnprintf/?kw=vsnprintf
	char* s = buf.get();
	va_list args;
	va_start(args, format);
	vsnprintf(s, MAX_PRINT_LENGTH, format, args);
	va_end(args);

#ifdef _SPDLOG_
#ifdef _CONSOLE_
	// log to file
	StdoutSpdLog("{}", s);
#endif
	FileSpdLog("{}", s);
#endif // _SPDLOG_

}
