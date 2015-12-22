#pragma once
#include "DefaultTaskScheduler.h"
#include "GenericTask.h"
#include <memory>

namespace BTSE //Best Task Scheduler Ever
{
	template<typename TScheduler = DefaultTaskScheduler>
	TaskScheduler_ptr CreateScheduler(unsigned int concurrencyLevel = 0) noexcept
	{
		return std::static_pointer_cast<TaskScheduler>(std::make_shared<TScheduler>(concurrencyLevel));
	}

	template<typename TResult, typename ...Args>
	std::shared_ptr<Task> RunTask(TaskScheduler_ptr taskScheduler, std::function<TResult(Args...)> func, Args... args) noexcept(false)
	{
		//can't use std::make_shared, Task ctor is private
		auto task = std::shared_ptr<Task>(new GenericTask<TResult>(std::bind(func, args...)));
		taskScheduler->QueueTask(task);
		return task;
	}

	template<typename TResult, typename ...Args>
	std::shared_ptr<Task> RunTask(std::function<TResult(Args...)> func, Args... args) noexcept(false)
	{
		//can't use std::make_shared, Task ctor is private
		auto task = std::shared_ptr<Task>(new GenericTask<TResult>(std::bind(func, args...)));
		TaskScheduler::Current()->QueueTask(task);
		return task;
	}

	template<typename TResult>
	TResult GetResult(Task_ptr task)
	{
		return std::static_pointer_cast<GenericTask<TResult>>(task)->GetResult();
	}
}
