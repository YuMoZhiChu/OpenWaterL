#include "stdafx.h"
#include "task.h"

#include "../Core/CoreInclude.h"

//---------IO Task---------
std::list<std::unique_ptr<CIOTask>> CIOTaskManager::s_TaskList;
std::unique_ptr<std::thread> CIOTaskManager::s_Thread;
std::unique_ptr<std::mutex> CIOTaskManager::s_Mutex;
std::unique_ptr<std::condition_variable> CIOTaskManager::s_HasTask;
std::atomic<bool> CIOTaskManager::s_Quit;

void CIOTaskManager::AddTask(std::unique_ptr<CIOTask> task)
{
	std::unique_lock<std::mutex> lck(*s_Mutex);
	s_TaskList.push_back(std::move(task));
	s_HasTask->notify_one();
}

void CIOTaskManager::Initialize()
{
	Print("CIOTaskManager::Initialize");
	s_Quit = false;
	s_Mutex.reset(new std::mutex);
	s_HasTask.reset(new std::condition_variable);
	s_Thread.reset(new std::thread(Loop));
}

void CIOTaskManager::Release()
{
	s_Quit = true;
	s_HasTask->notify_one();
	s_Thread->join();
	s_Thread.reset();
	s_Mutex.reset();
	s_HasTask.reset();
	s_TaskList.clear();
	Print("CIOTaskManager::Release");
}

void CIOTaskManager::Loop()
{
	while (true)
	{
		std::unique_ptr<CIOTask> task;
		{
			std::unique_lock<std::mutex> lck(*s_Mutex);

			//// 等价于
			//while (!(s_Quit || !s_TaskList.empty()))
			//{
			//	s_HasTask->wait(lck);
			//}
			s_HasTask->wait(lck, [] {return s_Quit || !s_TaskList.empty(); });
			if (s_Quit)
			{
				return;
			}

			if (!s_TaskList.empty())
			{
				task.swap(s_TaskList.front());
				s_TaskList.pop_front();
			}
		}
		if (task)
		{
			task->Run();
		}
	}
}
