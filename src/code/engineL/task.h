#pragma once
//#include <atomic>
#include <memory>
#include <list>
#include <mutex>


class CIOTask
{
public:
	virtual ~CIOTask() {}
	virtual void Run() = 0;
public:
	static void AddTask(std::unique_ptr<CIOTask> task);
	static void Initialize();
	static void Release();
private:
	static void Loop();
	// static std::list<> s_TaskList;

};
