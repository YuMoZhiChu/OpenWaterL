
#include "WindowsApplication.h"

HINSTANCE g_hInstance = nullptr;

void __window_init();
void __window_run_loop();

WindowsApplication::WindowsApplication()
{
}

WindowsApplication::~WindowsApplication()
{
}

void WindowsApplication::Run()
{
	__window_init();
	__window_run_loop();
}

void WindowsApplication::PumpMessage()
{

}
