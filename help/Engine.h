#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <condition_variable>
#include "CoreInclude.h"

class CEngine
{
public:
	CEngine();
	~CEngine();

	std::thread::id GetMainThreadId();

	void Main();
	void Start();
	void Stop();

	CApplication* GetApplication();
	CWindow* GetWindow();

	void Tick();
	
	static CEngine* Instance();
	static void ModuleInitialize();
	static void ModuleRelease();

private:
	static std::unique_ptr<CEngine> s_CEngine;

	std::unique_ptr<CApplication> m_App;
	std::unique_ptr<CWindow> m_Window;
	std::thread::id m_MainThreadId;
	bool m_Started;
};
