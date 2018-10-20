
#include "stdafx.h"
#include "Engine.h"
#include "Console.h"
#include "SpdLogWrapper.h"

#include "FBXImportWrap.h"
#include "MyPython.h"

void StartGame(HINSTANCE hInstance, std::wstring pipe_name);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _CONSOLE_
	CConsoleInstance().Show(true);
#endif

	std::wstring pipe_name;
	StartGame(hInstance, pipe_name);

	return 0;
}

void StartGame(HINSTANCE hInstance, std::wstring pipe_name)
{

#ifdef _SPDLOG_
	SpdLogInit();
#endif
	InitPyHelloTest(); //C_Hello 样例，已经在 MyPython.h 中被注释了

	g_hInstance = hInstance;
	CEngine* c_engine = CEngine::Instance();
	c_engine->Main();
}

void __window_init()
{
	CEngine* c_engine = CEngine::Instance();
	c_engine->Start();
}

void __window_run_loop()
{
	CEngine* c_engine = CEngine::Instance();
	while (!g_QuitGame)
	{
		// 先直接卡死
		// 这个应该放在逻辑线程的,现在先放在这里
		CConsoleInstance().ConsoleCommand();
	}
}
