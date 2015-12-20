#pragma once
#include "TaskScheduler.h"
namespace BTSE //Best Task Scheduler Ever
{
	class ImmediateTaskScheduler :
		public TaskScheduler
	{
	protected:
		virtual void QueueTask(Task_ptr task) noexcept(false) override;

	public:
		ImmediateTaskScheduler(int concurrencyLevel = 0) noexcept;
		virtual ~ImmediateTaskScheduler() noexcept;
	};
}
