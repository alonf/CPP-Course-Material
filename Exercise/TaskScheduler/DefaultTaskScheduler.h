#pragma once
#include "TaskScheduler.h"
#include <thread>
#include <atomic>
#include <stack>
#include <mutex>
#include <vector>

namespace BTSE //Best Task Scheduler Ever
{
	class DefaultTaskScheduler :
		public TaskScheduler
	{
	private:
		std::atomic<unsigned int> m_nQueued = 0;
		std::atomic<unsigned int> m_nThreads = 0;
		std::atomic<bool> m_exiting = false;
		std::stack<Task_ptr> m_taskQueue;
		std::mutex m_taskQueueMutex, m_workerThreadMutex;
		std::condition_variable m_taskAvailable;
		std::stack<std::thread> m_workerThreads;
		std::atomic<unsigned int> m_running = 0;
		std::vector<std::thread> m_threads;

		void ReleaseAllThreads() noexcept;
		void CreateThreads() noexcept(false);
		void WorkerThread() noexcept(false);

	protected:
		virtual void QueueTask(Task_ptr task) noexcept(false) override;

		//cancel all queued tasks
		virtual void DefaultTaskScheduler::CancelAllTasks() noexcept override final;

	public:
		//Cancel all waiting tasks, and destroy the task scheduler
		virtual ~DefaultTaskScheduler() noexcept;

		DefaultTaskScheduler(unsigned int concurrencyLevel = 0) noexcept : TaskScheduler(concurrencyLevel)  {};

	};
}
