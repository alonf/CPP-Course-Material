#pragma once
#include "Task.h"
#include "TaskScheduler.h"
#include <functional>
#include <future>

namespace BTSE //Best Task Scheduler Ever
{
	template<typename TResult>
	class GenericTask : public Task
	{
		friend class Task;
		friend class DefaultTaskScheduler;
		
		template<typename FResult>
		friend FResult GetResult(Task_ptr task);

		template<typename FResult, typename ...Args>
		friend std::shared_ptr<Task> RunTask(TaskScheduler_ptr taskScheduler, std::function<FResult(Args...)> func, Args... args) noexcept(false);

		template<typename FResult, typename ...Args>
		friend std::shared_ptr<Task> RunTask(std::function<FResult(Args...)> func, Args... args) noexcept(false);

	private:
		std::packaged_task<TResult()> m_func;
		std::future<TResult> m_future;

		GenericTask(const std::function<TResult()> &func) noexcept(false) : m_func(func), m_future(m_func.get_future()) {}

		GenericTask(GenericTask &&o) noexcept { *this = std::move(o); }
		GenericTask &operator=(GenericTask &&o) noexcept
		{
			if (&o == this)
				return *this;
			
			m_func = std::move(o.m_func);
			m_future = std::move(o.m_future);
			return *this;
		}

		virtual void Start() noexcept(false) override
		{
			m_func();
		}
		
		GenericTask(const GenericTask &o) = delete;
		GenericTask &operator=(const GenericTask &o) = delete;
		virtual void Wait() noexcept final { m_future.wait(); }
		virtual void ThrowWait() noexcept(false) final { m_future.get(); }
		TResult GetResult() { return m_future.get(); }
	};
}
