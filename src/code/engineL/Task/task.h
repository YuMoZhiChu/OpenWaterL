#pragma once
#include <atomic>
#include <memory>
#include <list>
#include <mutex>
#include <thread>


class CIOTask
{
public:
	virtual ~CIOTask() {}
	virtual void Run() = 0;
};

class CIOTaskManager
{
public:
	static void AddTask(std::unique_ptr<CIOTask> task);
	static void Initialize();
	static void Release();
private:
	~CIOTaskManager() = default;
	static void Loop();
	static std::list<std::unique_ptr<CIOTask>> s_TaskList;
	static std::unique_ptr<std::thread> s_Thread;
	static std::unique_ptr<std::mutex> s_Mutex;
	static std::unique_ptr<std::condition_variable> s_HasTask;
	static std::atomic<bool> s_Quit;
};
