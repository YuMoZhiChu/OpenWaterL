#pragma once
#include <atomic>
#include "Platform.h"

// spdlog switch
#if PLATFORM_WINDOWS
#define _SPDLOG_
#endif

// console switch
#if PLATFORM_WINDOWS
#define _CONSOLE_
#endif
