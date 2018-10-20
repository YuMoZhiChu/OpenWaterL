
#include "stdafx.h"
#include "Engine.h"
#include "Console.h"

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
	while (true)
	{
		// œ»÷±Ω”ø®À¿
	}
}
