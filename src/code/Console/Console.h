#pragma once

#include "../Core/CoreInclude.h"

// 可控的控制台
class CConsole
{
	HANDLE m_ConsoleScreen;
	HANDLE m_ConsoleInput;
	HWND m_Window;

	HANDLE m_Thread;
	HANDLE m_InputHandled;

	CRITICAL_SECTION m_hCS;
	char m_CommandLine[256];
	bool m_bInput;

	static DWORD WINAPI Process(void *arg);

public:
	CConsole(const char* title = 0);
	~CConsole();

	HWND GetHandle();
	void Show(bool is_show);

	int GetLine(char *buffer);
	void ConsoleCommand();

	static CConsole& Instance(void);
};

// 单例模式
inline CConsole& CConsoleInstance()
{
	return CConsole::Instance();
}
