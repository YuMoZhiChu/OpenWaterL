#pragma once

#include "../engineL/CoreInclude.h"

#include <windows.h>

// 可控的控制台
class CConsole
{
	HANDLE m_ConsoleScreen;
	HWND m_Window;

public:
	CConsole(const char* title = 0);

	void Show(bool is_show);

	static CConsole& Instance(void);
};

// 单例模式
inline CConsole& CConsoleInstance()
{
	return CConsole::Instance();
}
