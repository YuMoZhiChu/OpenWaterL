#include "SpdLogWrapper.h"

#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>

#include <io.h>

std::shared_ptr<spdlog::logger> CreateStdoutLogger();
std::shared_ptr<spdlog::logger> CreateFileLogger(
	const std::string& log_name, const std::string& filename,
	size_t file_size, size_t max_file_num);
std::string GetCurrentTimeString();

namespace
{
	std::shared_ptr<spdlog::logger> g_StdoutLog;
	std::shared_ptr<spdlog::logger> g_EngineFileLog;
	std::string g_EngineFileName;
}

void SpdLogInit()
{
	g_StdoutLog = CreateStdoutLogger();

	if (-1 == _access("../EngineLog", 0))
	{
		::CreateDirectory(L"../EngineLog", NULL);
	}
	g_EngineFileName = "../EngineLog/scriptlog_" + GetCurrentTimeString() + ".txt";
	g_EngineFileLog = CreateFileLogger("EngineLogFile", g_EngineFileName, 4 * 1024 * 1024, 1);
}

void SpdLogRelease()
{
	g_EngineFileLog.reset();
	g_StdoutLog.reset();
	spdlog::drop_all();
}

std::shared_ptr<spdlog::logger> CreateStdoutLogger()
{
	auto logger = spdlog::get("console");
	if (!logger)
	{
		logger = spdlog::stdout_logger_st("console");
		logger->set_pattern("%v");
	}
	return logger;
}

std::shared_ptr<spdlog::logger> CreateFileLogger(
	const std::string& log_name, const std::string& filename,
	size_t file_size, size_t max_file_num)
{
	std::shared_ptr<spdlog::logger> logger = spdlog::get(log_name);
	if (logger)
	{
		return logger;
	}

	// 尝试5次防止 logger 创建失败
	int times = 5;
	std::string tempFilename(filename);
	while (times--)
	{
		try
		{
			// 16384 固定大小
			// discard_log_msg 不能写入就直接跑异常
			spdlog::set_async_mode(16384, spdlog::async_overflow_policy::discard_log_msg);
			logger = spdlog::rotating_logger_st(log_name, tempFilename, file_size, max_file_num);
			logger->set_pattern("[%Y-%m-%d %T:%e] %v");
			break;
		}
		catch (...)
		{
			// 往往异常的情况是已经存在了相同的文件
			tempFilename = filename + "_" + std::to_string(5 - times);
		}
	}

	if (!logger)
	{
		// 失败转成标注输出
		logger = spdlog::get("console");
		if (!logger)
		{
			logger = spdlog::stdout_logger_st("console");
		}
		logger->set_pattern("%v");
		logger->info("文件logger创建失败!转换为控制台logger");
	}

	return logger;
}

std::shared_ptr<spdlog::logger> __GetEngineFileSpdLogs()
{
	return g_EngineFileLog;
}

std::shared_ptr<spdlog::logger> __GetStdoutSpdLog()
{
	return g_StdoutLog;
}

std::string GetCurrentTimeString()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	struct tm timeInfo;
	localtime_s(&timeInfo, &in_time_t);

	std::stringstream ss;
	ss << std::put_time(&timeInfo, "%Y-%m-%d   %H-%M-%S");
	return ss.str();
}

void SpdLogFileFlush()
{
	g_EngineFileLog->flush();
}

const std::string& GetSpdLogFilePath()
{
	return g_EngineFileName;
}
