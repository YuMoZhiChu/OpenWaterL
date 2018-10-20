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

	// ֻ��һ����¼��ɫ�ı�����Ŀ�ľ���Ϊ�˼��ٶ�windows�ӿڵĵ���
	BYTE m_TextAttibute;

	// һ���ṹ��, ������һЩ���������������ָ��(��ʵ���Ǽ�����)
	CRITICAL_SECTION m_hCS;
	char m_CommandLine[256];
	bool m_bInput;
	
	// ����� WINAPI ���� _stdcal �̶����� ���ҵ�����ջ
	static DWORD WINAPI Process(void *arg);

// ���涼��Ĭ�� private
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
		// ���� WIN API �Ľӿ�
		// SetConsoleTextAttribute�����ǿ�һ���ֽڵĵ���������ǰ��ɫ������λ�����Ʊ���ɫ��
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

