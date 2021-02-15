#include "../engineL/stdafx.h"
#include "../Core/Print.h"

#include "Console.h"
#include "StringHasher.h"

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
DemoTitle CConsole::ConsoleCommand()
{
	char buf[256];
	if (!GetLine(buf)) return DemoTitle::Demo0_Zero;

	// 交互系统，绘制不同的 demo
	switch (_openwaterl_hash_compile_time(buf))
	{
	case "demo1"_openwaterl_hash_compile_time:
		// 第1个demo, 显示绿色背景
		Print("-->demo1 show : pure green show\n");
		return DemoTitle::Demo1_Pure_Green;
	case "demo2"_openwaterl_hash_compile_time:
		// 第2个demo, 显示红色背景
		Print("-->demo2 show : pure red show\n");
		return DemoTitle::Demo2_Pure_Red;
	case "demo3"_openwaterl_hash_compile_time:
		// 第3个demo, 显示 一个三角形
		Print("-->demo3 show : A Triangle\n");
		return DemoTitle::Demo3_Triangle;
	case "demo4"_openwaterl_hash_compile_time:
		// 第4个demo, 显示 2个三角形, 使用了 IBO
		Print("-->demo4 show : Two Triangle with IBO\n");
		return DemoTitle::Demo4_Two_Triangle_IBO;
	case "demo5"_openwaterl_hash_compile_time:
		// 第5个demo, 显示 1个变色的三角形, 使用了 shader_s
		Print("-->demo5 show : One Changing Triangle\n");
		return DemoTitle::Demo5_Changing_Triangle;
	case "demo6"_openwaterl_hash_compile_time:
		// 第6个demo, 显示 2张纹理，并做混合
		Print("-->demo6 show : Two Merge Texture\n");
		return DemoTitle::Demo6_Texture;
	case "demo7"_openwaterl_hash_compile_time:
		// 第7个demo, 引入 glm 库，做简单的矩阵变换
		Print("-->demo7 show : Simple Transform Show\n");
		return DemoTitle::Demo7_Transform;
	case "demo8"_openwaterl_hash_compile_time:
		// 第8个demo, 引入 MVP 矩阵，开始用透视投影
		Print("-->demo8 show : Simple MVP Show\n");
		return DemoTitle::Demo8_Coordinate_Systems;
	case "demo9"_openwaterl_hash_compile_time:
		// 第9个demo, 简单的冯氏光照
		Print("-->demo9 show : Simple Phong Show\n");
		return DemoTitle::Demo9_Simple_Phong;
	case "demo10"_openwaterl_hash_compile_time:
		// 第10个demo, 简单的材质
		Print("-->demo10 show : Simple Material Show\n");
		return DemoTitle::Demo10_Material;
	default:
		Print("Input code to show demo\n");
		Print("demo1 : pure green show\n");
		Print("demo2 : pure red show\n");
		Print("demo3 : A Triangle\n");
		Print("demo4 : Two Triangle with IBO\n");
		Print("demo5 : One Changing Triangle\n");
		Print("demo6 : Two Merge Texture\n");
		Print("demo7 : Simple Transform Show\n");
		Print("demo8 : Simple MVP Show\n");
		Print("demo9 : Simple Phong Show\n");
		Print("demo10 : Simple Material Show\n");
		return DemoTitle::Demo0_Zero;
	}
	return DemoTitle::Demo0_Zero;

	// Print("test\n");
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
