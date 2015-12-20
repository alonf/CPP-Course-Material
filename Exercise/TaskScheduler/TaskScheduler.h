#pragma once
#include <memory>
#include <thread>
#include <mutex>

namespace BTSE //Best Task Scheduler Ever
{
	class TaskScheduler;
	class Task;

	typedef std::shared_ptr<Task> Task_ptr;
	typedef std::shared_ptr<TaskScheduler> TaskScheduler_ptr;

	class TaskScheduler abstract : public std::enable_shared_from_this<TaskScheduler>
	{
		friend class Task;
		static std::once_flag m_defualtSchedulerOneTimeInitFlag;

	private:
		static thread_local std::weak_ptr<TaskScheduler> s_current;
		static TaskScheduler_ptr m_defualtScheduler;
		unsigned int m_concurrncyLevel;

	public:
		//Concurrency Level of 0 ==> the hardware maximum true concurrency 
		TaskScheduler(unsigned int concurrencyLevel = 0) noexcept : 
			m_concurrncyLevel(concurrencyLevel != 0 ? concurrencyLevel : std::thread::hardware_concurrency()) {}

		//Cancel all waiting tasks, and destroy the task scheduler
		virtual ~TaskScheduler() noexcept {};
		
		unsigned int ConcurrencyLevel() const noexcept { return m_concurrncyLevel; }

		//The current ambient task scheduler. 
		//If there is no one, create one and return it to the caller
		static TaskScheduler_ptr Current() noexcept(false);

		//Make this Task Scheduler the current ambient task scheduler and return the previous
		TaskScheduler_ptr MakeCurrent() noexcept(false);

		virtual void QueueTask(Task_ptr task) noexcept(false) abstract;

		//cancel all queued tasks
		virtual void CancelAllTasks() noexcept {};


	protected:

		void StartTask(Task_ptr task) noexcept(false);
		TaskScheduler_ptr This() noexcept { return shared_from_this(); }
	};

}
