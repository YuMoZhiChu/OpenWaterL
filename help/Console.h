#pragma once
#include "EngineConfig.h"
#include "CoreInclude.h"

#if PLATFORM_WINDOWS

#ifdef _CONSOLE_

#include <windows.h>

class CConsole
{
	HANDLE m_Thread;
	HANDLE m_ConsoleScreen;
	HANDLE m_ConsoleInput;
	HANDLE m_ConsoleOutput;
	HANDLE m_ConsoleError;
	HANDLE m_EndEvent;
	HANDLE m_InvisibleEvent;
	HWND m_Window;
	HANDLE m_InputAvailable;
	HANDLE m_InputHandled;

	// 只是一个记录颜色的变量，目的就是为了减少对windows接口的调用
	BYTE m_TextAttibute;

	// 一个结构体, 包含了一些计数器，句柄，和指针(其实还是计数器)
	CRITICAL_SECTION m_hCS;
	char m_CommandLine[256];
	bool m_bInput;
	
	// 这里的 WINAPI 就是 _stdcal 固定参数 从右到左入栈
	static DWORD WINAPI Process(void *arg);

// 上面都是默认 private
public:
	// 4 + 2 + 1 = 7
	enum TextAttr { Default = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_GREEN};
public:
	CConsole(const char* title = 0);
	~CConsole();

	DWORD WaitInput(DWORD timeout);

	void InvisibleNextLine(const char *p = 0);
	int GetLine(char *buffer);
	HWND GetHandle();
	void Topmost();
	void Show(bool is_show);
	void ConsoleCommand();

	void SetTextAttribute(unsigned short m) const
	{
		// 调用 WIN API 的接口
		// SetConsoleTextAttribute函数是靠一个字节的低四来控制前景色，高四位来控制背景色。
		SetConsoleTextAttribute(m_ConsoleOutput, m);
	}

	static CConsole& Instance(void);
private:
	FILE* m_FO;
	FILE* m_FI;
	FILE* m_FE;
};

#endif

#ifdef _CONSOLE_
inline CConsole& CConsoleInstance()
{
	return CConsole::Instance();
}

#endif // _CONSOLE_


#endif

