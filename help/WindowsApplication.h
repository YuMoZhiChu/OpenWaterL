#pragma once

#include "stdafx.h"

extern HINSTANCE g_hInstance;

class WindowsApplication
{
public:
	WindowsApplication();
	~WindowsApplication();

	virtual void Run();
	
	void PumpMessage();

private:

};

typedef WindowsApplication CApplication;
