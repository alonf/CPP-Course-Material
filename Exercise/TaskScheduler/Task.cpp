

#include "GenericTask.h"

namespace BTSE //Best Task Scheduler Ever
{
	//template<typename TResult, typename ...Args>
	//static std::shared_ptr<Task> Task::Run(std::function<TResult(Args...)> func, Args... args) noexcept(false)
	//{
	//	//can't use std::make_shared, Task ctor is private
	//	auto task = std::shared_ptr<Task>(new GenericTask<TResult>(std::bind(func, args...)));
	//	TaskScheduler::Current()->QueueTask(task);
	//	return task;
	//}

	/*template<typename TResult>
	TResult Task::Get() noexcept(false)
	{
		static_assert(!std::is_void<TResult>::value, "Can't get the value of a void");
		return static_pointer_cast<GenericTask<TResult>>(shared_from_this())->GetResult();
	}*/
}