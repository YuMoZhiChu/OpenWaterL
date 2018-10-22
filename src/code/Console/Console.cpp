#include "../engineL/stdafx.h"
#include "../Core/Print.h"

#include "Console.h"

// 控制台线程
DWORD WINAPI CConsole::Process(void *arg)
{
	static char buffer[256];
	memset(buffer, 0, 256);
	CConsole *console = static_cast<CConsole*> (arg);
	DWORD cRead;
	for (;;)
	{
		cRead = 0;
		if (ReadFile(console->m_ConsoleInput, buffer, 255, &cRead, NULL) == 0)
			break;
		if (cRead != 0)
		{
			int i;
			for (i = cRead - 1; i >= 0; --i)
			{
				if (buffer[i] == '\n' || buffer[i] == '\r') --cRead;
				else break;
			}
			buffer[cRead] = 0;

			// 临界区域, 处理 input(read in buff) -> commandLine
			EnterCriticalSection(&(console->m_hCS));

			strncpy(console->m_CommandLine, buffer, 255);
			console->m_bInput = true;
			::ResetEvent(console->m_InputHandled);

			LeaveCriticalSection(&(console->m_hCS));

			// waiting for imputHandled aviable, 控制台线程可以等待
			while (::WaitForSingleObject(console->m_InputHandled, 0) != WAIT_OBJECT_0);
		}
	}
	return 0;
}

// 在主线程中被调用
void CConsole::ConsoleCommand()
{
	char buf[256];
	if (!GetLine(buf)) return;

	Print("test\n");

	/*std::string str_python(buf);
	Print(str_python.c_str());*/
	// TODO PyRun_SimpleString
}

int CConsole::GetLine(char *buffer)
{
	// waiting for m_bInput aviable, 主线程不能等待
	if (!m_bInput)
	{
		return 0;
	}

	// 临界区域, 处理 commandLine() -> clear
	EnterCriticalSection(&m_hCS);

	strncpy(buffer, m_CommandLine, 255);
	m_bInput = false;
	memset(m_CommandLine, 0, 256);
	::SetEvent(m_InputHandled);

	LeaveCriticalSection(&m_hCS);

	return 1;
}

// 创建控制台
CConsole::CConsole(const char* title)
{
	try
	{
		m_bInput = false;
		m_Window = 0;
		memset(m_CommandLine, 0, sizeof(m_CommandLine));

		InitializeCriticalSection(&m_hCS);

		// 注销OpenGL创建的控制台
		::FreeConsole();
		::AllocConsole();

		m_ConsoleScreen = ::CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
		m_ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
		m_InputHandled = CreateEvent(NULL, TRUE, FALSE, NULL);

		::SetConsoleMode(m_ConsoleInput, ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT
			| ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_LINE_INPUT);

		if (title) ::SetConsoleTitleA(title);

		// 开启控制台线程,交替运行,注意线程冲突
		m_Thread = CreateThread(NULL, 0, Process, this, 0, NULL);
	}
	catch (int err)
	{
		err = 0;
		MessageBoxA(0, "Console Build Fail", "Error", 0);
	}
	// 打开控制台,可写
	freopen("CONOUT$", "w", stdout);
	// 无视 ctrl
	SetConsoleCtrlHandler(NULL, TRUE);
}

CConsole::~CConsole()
{
	CloseHandle(m_Thread);
}

HWND CConsole::GetHandle()
{
	if (m_Window) return m_Window;
	static char buffer[256];
	GetConsoleTitleA(buffer, 256);
	m_Window = FindWindowA("ConsoleWindowClass", buffer);
	return m_Window;
}

void CConsole::Show(bool is_show)
{
	if (is_show) {
		::ShowWindow(GetHandle(), SW_SHOW);
	}
	else {
		::ShowWindow(GetHandle(), SW_HIDE);
	}
}

CConsole& CConsole::Instance()
{
	static CConsole _instance("OpenWaterL Console");
	return _instance;
}
