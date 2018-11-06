#include "CriticalSection.h"

CCriticalSection::CCriticalSection()
{
	// 设定线程自旋时间, 4000 比较合适
	InitializeCriticalSectionAndSpinCount(&m_CS, 4000);
}

CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection(&m_CS);
}

void CCriticalSection::Lock()
{
	EnterCriticalSection(&m_CS);
}

void CCriticalSection::UnLock()
{
	LeaveCriticalSection(&m_CS);
}

bool CCriticalSection::TryLock()
{
	if (TryEnterCriticalSection(&m_CS))
		return true;
	return false;
}
