#include "Print.h"

#include <memory>
#include <stdarg.h>
#include <stdio.h>

#define MAX_PRINT_LENGTH 4096

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

	printf(s);

	// to do
}
