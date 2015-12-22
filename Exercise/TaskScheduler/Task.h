#pragma once

namespace BTSE //Best Task Scheduler Ever
{
	class Task
	{
		friend class TaskScheduler;
		
	protected:
		virtual void Start() noexcept(false) = 0;

		Task(const Task &o) = delete;
		Task &operator=(const Task &o) = delete;
		Task() = default;

	public:
		virtual ~Task()
		{
		}

		virtual void Wait() noexcept = 0;
		virtual void ThrowWait() noexcept(false) = 0;
	};
}
