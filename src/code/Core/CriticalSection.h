#pragma once
// RAII 的临界区

#include "../Core/CoreInclude.h"

class CCriticalSection
{
public:
	CCriticalSection();
	~CCriticalSection();

	void Lock();
	void UnLock();
	bool TryLock();

private:
	CRITICAL_SECTION m_CS;
};

class CCriticalSectionGuard
{
public:
	CCriticalSectionGuard(CCriticalSection& critical_section) : m_CriticalSection(critical_section)
	{
		m_CriticalSection.Lock();
	}
	~CCriticalSectionGuard()
	{
		m_CriticalSection.UnLock();
	}
private:
	CCriticalSection & m_CriticalSection;
};
