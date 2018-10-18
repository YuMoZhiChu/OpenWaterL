#include "../engineL/stdafx.h"

#include "Console.h"

// 创建控制台
CConsole::CConsole(const char* title)
{
	try
	{
		// 注销OpenGL创建的控制台
		::FreeConsole();
		::AllocConsole();

		m_ConsoleScreen = ::CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
		m_Window = 0;
		if (title) ::SetConsoleTitleA(title);
	}
	catch (int err)
	{
		err = 0;
		MessageBoxA(0, "Console Build Fail", "Error", 0);
	}
}

void CConsole::Show(bool is_show)
{
	if (is_show) {
		::ShowWindow(m_Window, SW_SHOW);
	}
	else {
		::ShowWindow(m_Window, SW_HIDE);
	}
}

CConsole& CConsole::Instance()
{
	static CConsole _instance("OpenWaterL Console");
	return _instance;
}
