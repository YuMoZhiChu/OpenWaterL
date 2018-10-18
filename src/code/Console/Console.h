#pragma once

#include "../engineL/CoreInclude.h"

#include <windows.h>

class CConsole
{
	HANDLE m_ConsoleScreen;
	HWND m_Window;

public:
	CConsole(const char* title = 0);

	void Show(bool is_show);

	static CConsole& Instance(void);
};

inline CConsole& CConsoleInstance()
{
	return CConsole::Instance();
}
