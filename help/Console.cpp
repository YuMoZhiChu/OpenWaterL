#include "stdafx.h"
#include "Console.h"
#include <string>
#include <io.h>
#include <fcntl.h>

#include <Python.h>

#ifdef  _CONSOLE_

DWORD WINAPI CConsole::Process(void *arg)
{
	static char buffer[256];
	memset(buffer, 0, 256);
	CConsole *console = static_cast<CConsole*> (arg);
	bool echo = true;
	DWORD cRead;
	for (;;)
	{
		// WaitForSingleObject函数用来检测hHandle事件的信号状态
		// 在某一线程中调用该函数时，线程暂时挂起
		// 如果在挂起的dwMilliseconds毫秒内，线程所等待的对象变为有信号状态，则该函数立即返回；
		// 如果超时时间已经到达dwMilliseconds毫秒，但hHandle所指向的对象还没有变成有信号状态，函数照样返回。
		// 具体查看 https://msdn.microsoft.com/en-us/library/windows/desktop/ms687032(v=vs.85).aspx
		if (WaitForSingleObject(console->m_EndEvent, 0) == WAIT_OBJECT_0)
			break;
		if (WaitForSingleObject(console->m_InvisibleEvent, 0) == WAIT_OBJECT_0) echo = false;
		if (!echo)
		{
			// 函数名或变量名前面有::，但是没有类名，说明这个是全局变量或公共函数，并且不属于任何命名空间
			// 直接从全局找，可能会快一点
			::SetConsoleMode(console->m_ConsoleInput, ENABLE_PROCESSED_INPUT
				| ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
		}
		cRead = 0;
		// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365467(v=vs.85).aspx
		// 这是一个为同步和异步操作设计的函数（既可以同步也可以异步）
		// 参数意义: 设备句柄(在这里就是console的input), 缓冲区(这个缓冲区必须保证卡在这里进行等待时有效，并且没人能够使用)
		// 255 最大字节数， cRead [out 接受目的], 最后一个参数是 FILE_FLAG_OVERLAPPED 有关，这里不做考虑
		// 返回值: 非零 函数成功  函数失败或者异步完成 0 所以我们这里用的其实是它的同步功能，在等待输出的时候，我们这个线程就会卡在这里
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

			// https://msdn.microsoft.com/en-us/library/windows/desktop/ms682608(v=vs.85).aspx
			// 单个进程的线程可以使用临界区对象进行互斥同步。
			// 该进程负责分配临界区对象所使用的内存，它可以通过声明CRITICAL_SECTION类型的变量来实现。
			// 在使用临界区之前，进程的某个线程必须调用
			// InitializeCriticalSection或InitializeCriticalSectionAndSpinCount来初始化该对象。

			// 为了对共享资源进行互斥访问，每个线程在执行访问受保护资源的任何代码部分之前，
			// 都会调用EnterCriticalSection或TryEnterCriticalSection函数来请求关键部分的所有权。
			// 区别在于TryEnterCriticalSection立即返回，无论它是否获得关键部分的所有权，
			// 而EnterCriticalSection会阻塞，直到线程可以获得关键部分的所有权。
			// 当它完成执行受保护的代码时，线程使用LeaveCriticalSection函数放弃所有权，
			// 使另一个线程成为所有者并访问受保护的资源。关于等待线程将获得关键部分所有权的顺序不能保证。

			// 线程拥有关键部分的所有权后，可以对EnterCriticalSection或TryEnterCriticalSection进行额外调用，
			// 而不会阻止其执行。这可以防止线程在等待它已经拥有的关键部分时发生死锁。
			// 每次EnterCriticalSection和TryEnterCriticalSection成功时，线程都会进入临界区。
			// 线程每次进入关键部分时都必须调用LeaveCriticalSection一次。

			EnterCriticalSection(&(console->m_hCS));
			strncpy(console->m_CommandLine, buffer, 255);
			console->m_bInput = true;
			// https://baike.baidu.com/item/setevent/3560262
			// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686211(v=vs.85).aspx
			// SetEvent/ResetEvent分别将EVENT置为这两种状态分别是发信号与不发信号。
			//::SetEvent(console->m_InputAvailable);
			::ResetEvent(console->m_InputHandled);
			LeaveCriticalSection(&(console->m_hCS));

			while (::WaitForSingleObject(console->m_InputHandled, 0) != WAIT_OBJECT_0);
		}
	}
	return 0;
}

int CConsole::GetLine(char *buffer)
{
	if (!m_bInput)
	{
		//// 设置新的控制台字体颜色，这里确保的是用户的输出是绿色的，不过它这里的逻辑有问题
		BYTE new_attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		if (new_attr != m_TextAttibute)
		{
			m_TextAttibute = new_attr;
			SetTextAttribute(new_attr);
		}
		return 0;
	}

	EnterCriticalSection(&m_hCS);

	strncpy(buffer, m_CommandLine, 255);
	m_bInput = false;
	memset(m_CommandLine, 0, 256);
	//::ResetEvent(m_InputAvailable);
	::SetEvent(m_InputHandled);

	BYTE new_attr = FOREGROUND_GREEN;
	if (new_attr != m_TextAttibute)
	{
		m_TextAttibute = new_attr;
		SetTextAttribute(new_attr);
	}

	LeaveCriticalSection(&m_hCS);

	return 1;
}

CConsole::CConsole(const char* title)
{
	try
	{
		m_bInput = false;
		InitializeCriticalSection(&m_hCS);

		// 分离与调用进程相关联的控制台
		// 一个进程最多可以附加一个控制台。 如果调用进程尚未附加到控制台，则返回的错误代码为ERROR_INVALID_PARAMETER（87）。
		// 进程可以使用FreeConsole函数将其自身从其控制台分离。 
		// 如果其他进程共享控制台，则控制台不会被销毁，但是调用FreeConsole的进程不能引用它。 
		// 当连接到它的最后一个进程终止或调用FreeConsole时，控制台关闭。 
		// 在进程调用FreeConsole之后，它可以调用AllocConsole函数来创建一个新的控制台或AttachConsole来附加到另一个控制台。
		::FreeConsole();

		// 为调用进程分配一个新的控制台
		::AllocConsole();

		// 说真的，windows接口方面，百度真的很好用，中文，而且主要内容都有，细节去微软官网看
		// https://baike.baidu.com/item/CreateConsoleScreenBuffer
		m_ConsoleScreen = ::CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
		m_ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
		m_ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		m_ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
		// 这2步是将printf 输出到 console 的关键步骤
		m_FO = _fdopen(_open_osfhandle(INT_PTR(m_ConsoleOutput), _O_WTEXT), "w");
		*stdout = *m_FO;
		// https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/setvbuf
		setvbuf(stdout, NULL, _IONBF, 0);// 这是最方便的模式
		m_FI = _fdopen(_open_osfhandle(INT_PTR(m_ConsoleInput), _O_WTEXT), "r");
		*stdin = *m_FI;
		setvbuf(stdin, NULL, _IONBF, 0);
		m_FE = _fdopen(_open_osfhandle(INT_PTR(m_ConsoleError), _O_WTEXT), "w");
		*stderr = *m_FE;
		setvbuf(stderr, NULL, _IONBF, 0);

		// 设置标题
		if (title) ::SetConsoleTitleA(title);

		// ENABLE_ECHO_INPUT 支持 echo 的输入
		::SetConsoleMode(m_ConsoleInput, ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT
			| ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_LINE_INPUT);

		// https://baike.baidu.com/item/CreateEvent/917012
		// CreateEvent 接口，input相关的要调成手动调整事件状态
		m_EndEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_InvisibleEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		m_InputAvailable = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_InputHandled = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_TextAttibute = 0;

		m_Window = 0;
		memset(m_CommandLine, 0, sizeof(m_CommandLine));

		// https://baike.baidu.com/item/CreateThread
		// 在主线程上创建一个线程， 注意在析构函数上用closehandle 把函数关掉
		m_Thread = CreateThread(NULL, 0, Process, this, 0, NULL);
	}
	catch (int err)
	{
		err = 0;
		MessageBoxA(0, "Console Build Fail", "Error", 0);
	}
	// CONIN$ 和 CONOUT$ 是系统保留的2个路径名，他们用来做控制台的输入(键盘)缓冲区和活动的控制台屏幕输出缓冲区
	freopen("CONOUT$", "w", stdout);
	SetConsoleCtrlHandler(NULL, TRUE);// 直接把回调函数置为空，表示直接无视 ctrl
}

// 这个函数似乎没有用到
DWORD CConsole::WaitInput(DWORD timeout)
{
	// https://baike.baidu.com/item/ShowWindow
	// 该函数设置指定窗口的显示状态。 SW_SHOW：在窗口原来的位置以原来的尺寸激活和显示窗口。
	::ShowWindow(this->GetHandle(), SW_SHOW);
	printf("debug>");
	fflush(stdout);
	Topmost();
	return ::WaitForSingleObject(m_InputAvailable, timeout);
}

// 这个未用到
void CConsole::InvisibleNextLine(const char *p)
{
	if (p)
	{
		DWORD len = static_cast<DWORD>(strlen(p));
		SetConsoleTextAttribute(m_ConsoleOutput, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		SetEvent(m_InvisibleEvent);
		WriteFile(m_ConsoleOutput, p, len, &len, 0);
		SetConsoleTextAttribute(m_ConsoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	else {
		SetEvent(m_InvisibleEvent);
	}
}

HWND CConsole::GetHandle()
{
	if (m_Window) return m_Window;
	static char buffer[256];
	GetConsoleTitleA(buffer, 256);
	m_Window = FindWindowA("ConsoleWindowClass", buffer);
	return m_Window;
}

CConsole::~CConsole()
{
	// TODO assert != 0, -> MyAssert.h 编写
	CloseHandle(m_Thread);
	// 本来应该 fcolse(m_FX) 不过会卡死?
	// fclose(m_FO);
	// fclose(m_FI);
	// fclose(m_FE);
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

void CConsole::Topmost()
{
	::BringWindowToTop(GetHandle());
}

// 这个东西是在 GameLoop 里面调用的, 就是每帧调用的
// 用来在控制台中运行python的命令, 即控制台支持的输入测试, 这部分的工作要完成了 Python/C 接口才能继续
// TODO python - C 的流程
void CConsole::ConsoleCommand()
{
	char buf[256];// , buf1[256];
	if (!GetLine(buf)) return;

	// 把第一个 ' ', \t 变成 0，必定是空格? 还有这里的判断，不是太看得懂，而且感觉不是很必要 2018-4-7
	//char *argstr, *ptr;
	//strcpy(buf1, buf);
	//for (ptr = buf; *ptr != ' ' && *ptr != '\t' && *ptr != 0; ++ptr);
	//if (ptr == buf) return;
	//*ptr = 0;
	//ptr++;

	std::string str_gbk(buf);
	// 先直接输出作为测试
	Print(str_gbk.c_str());

	// misc 转换 TODO (对于特殊符号的处理, 不过也可以直接用 str_gbk, 在控制台的输入注意点就好了)
	// GBK2UTF16(str_gbk, str_utf16);
	// UTF162UTF8()

	PyObject* pInt = Py_BuildValue("i", 2003);
	Py_INTRCHECK_H
	assert(PyLong_AsLong(pInt));
	int i = PyLong_AsLong(pInt);
	Py_DECREF(pInt);

	PyRun_SimpleString(str_gbk.c_str());
}

CConsole& CConsole::Instance()
{
	static CConsole __inst("My Game");
	return __inst;
}

#endif //  _CONSOLE_

