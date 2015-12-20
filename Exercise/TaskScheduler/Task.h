#pragma once

namespace BTSE //Best Task Scheduler Ever
{
	class Task abstract 
	{
		friend class TaskScheduler;
		
	protected:
		virtual void Start() noexcept(false) abstract;

		Task(const Task &o) = delete;
		Task &operator=(const Task &o) = delete;
		Task() = default;

	public:
		virtual ~Task()
		{
		}

		virtual void Wait() noexcept abstract;
		virtual void ThrowWait() noexcept(false) abstract;
	};
}
