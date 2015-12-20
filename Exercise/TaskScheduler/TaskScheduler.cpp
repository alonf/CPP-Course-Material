#include "TaskScheduler.h"
#include "DefaultTaskScheduler.h"
#include "Task.h"
#include <memory>

using namespace std;

namespace BTSE //Best Task Scheduler Ever
{
	/*static*/ thread_local std::weak_ptr<TaskScheduler> TaskScheduler::s_current;
	/*static*/ TaskScheduler_ptr TaskScheduler::m_defualtScheduler;
	/*static*/ once_flag TaskScheduler::m_defualtSchedulerOneTimeInitFlag;

	//The current ambient task scheduler. 
	//If there is no one, create one and return it to the caller
	/* static */ TaskScheduler_ptr TaskScheduler::Current() noexcept(false)
	{
		//This is a global, need to initate only once
		call_once(m_defualtSchedulerOneTimeInitFlag, [] 
			{m_defualtScheduler = make_shared<DefaultTaskScheduler>(); });

		//this is a TLS, no need to synchronize
		auto current = s_current.lock();
		if (!current)
		{
			current = m_defualtScheduler;
			s_current = current;
		}
		return current;
	}

	//Make this Task Scheduler the current ambient task scheduler and return the previous
	TaskScheduler_ptr TaskScheduler::MakeCurrent() noexcept(false)
	{
		auto current = s_current.lock();
		//same scheduler
		if (current && current.get() == this)
			return current;
		
		auto previous = current;
		s_current = shared_from_this();

		return current;
	}

	void TaskScheduler::StartTask(Task_ptr task) noexcept(false)
	{
		task->Start();
	}
}
