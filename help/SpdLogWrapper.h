#pragma once

#include <spdlog.h>
#include "EngineConfig.h"

void SpdLogInit();
void SpdLogRelease();
std::shared_ptr<spdlog::logger> __GetEngineFileSpdLogs();
std::shared_ptr<spdlog::logger> __GetStdoutSpdLog();

template <typename ... Args>
void StdoutSpdLog(const char* fmt, const Args&... args)
{
#ifdef _CONSOLE_
	__GetStdoutSpdLog()->info(fmt, args...);
#endif
}

template <typename ... Args>
void FileSpdLog(const char* fmt, const Args&... args)
{
#ifdef _CONSOLE_
	__GetEngineFileSpdLogs()->info(fmt, args...);
#endif
}

void SpdLogFileFlush();

const std::string& GetSpdLogFilePath();
