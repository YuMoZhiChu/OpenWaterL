#include "Engine.h"

CEngine::CEngine() :
	m_Started(false)
{
}

CEngine::~CEngine()
{
}

std::unique_ptr<CEngine> CEngine::s_CEngine;

CEngine* CEngine::Instance()
{
	if (!s_CEngine)
	{
		s_CEngine.reset(new CEngine());
	}
	return s_CEngine.get();
}

void CEngine::Tick()
{

}

void CEngine::Main()
{
	m_MainThreadId = std::this_thread::get_id();
	m_App.reset(new CApplication);
	m_Window.reset(new CWindow);
	m_App->Run();
}

void CEngine::Start()
{
	// Begin Calculate Time
	Print("CEngine Start!");
	if (m_Started) {
		return;
	}

	m_Started = true;
	m_MainThreadId = std::this_thread::get_id();

	// Module Initlize

	// Start Logic Threads

	// End Calculate Time
}

void CEngine::Stop()
{

}

CApplication* CEngine::GetApplication()
{
	return m_App.get();
}

CWindow* CEngine::GetWindow()
{
	return m_Window.get();
}

std::thread::id CEngine::GetMainThreadId()
{
	return m_MainThreadId;
}

void CEngine::ModuleInitialize()
{

}

void CEngine::ModuleRelease()
{

}
