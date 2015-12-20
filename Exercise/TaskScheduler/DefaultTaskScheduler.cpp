
#include "DefaultTaskScheduler.h"
#include <memory>
#include <assert.h>
#include <thread>
#include <algorithm>

using namespace std;

namespace BTSE //Best Task Scheduler Ever
{
	DefaultTaskScheduler::~DefaultTaskScheduler() noexcept
	{
		try
		{
			CancelAllTasks();
			ReleaseAllThreads();
			
		}
		catch (...)
		{
			assert(true);
		}
	}


	/* virtual */ void DefaultTaskScheduler::QueueTask(Task_ptr task) noexcept(false) /* override */
	{
		lock_guard<mutex> lock(m_taskQueueMutex);
		m_taskQueue.push(task);
		if (m_nThreads == 0) //A safe call -> under a lock
			CreateThreads();
		m_taskAvailable.notify_one();
	}

	//cancel all queued tasks
	/*virtual*/ void DefaultTaskScheduler::CancelAllTasks() noexcept /* final */
	{
		try
		{
			lock_guard<mutex> lock(m_taskQueueMutex);
			while (!m_taskQueue.empty())
			{
				auto task = m_taskQueue.top();
				m_taskQueue.pop();
			}
		}
		catch (...)
		{
			assert(true);
		}
	}

	void DefaultTaskScheduler::WorkerThread()
	{
		MakeCurrent();
		++m_nThreads;
		while (true)
		{
			// Wait until main() sends data
			std::unique_lock<std::mutex> workerThreadlock(m_workerThreadMutex);
			m_taskAvailable.wait(workerThreadlock, [this]
			{
				return m_exiting || (!m_taskQueue.empty() && m_running <= ConcurrencyLevel());
			});
			
			if (m_exiting)
				return;

			++m_running;
			Task_ptr task;
			{
				lock_guard<mutex> lock(m_taskQueueMutex);
				if (m_taskQueue.empty()) //no more tasks
				{
					workerThreadlock.unlock();
					return;
				}
				task = m_taskQueue.top();
				m_taskQueue.pop();

				if (!m_taskQueue.empty()) //more tasks, wake other
				{
					workerThreadlock.unlock();
					m_taskAvailable.notify_one();
				}
			}
			StartTask(task);
			--m_running;
		}
	}


	void DefaultTaskScheduler::ReleaseAllThreads() noexcept
	{
		try
		{
			m_exiting = true;
			m_taskAvailable.notify_all();
			for_each(begin(m_threads), end(m_threads), [this](thread &t)
			{
				t.join();
			});
		}
		catch (...)
		{
			assert(true);
		}
	}

	void DefaultTaskScheduler::CreateThreads() noexcept(false)
	{
		for (unsigned int i = 0; i < ConcurrencyLevel(); ++i)
		{
			thread t([this] {WorkerThread();});
			m_threads.push_back(move(t));
		}
	}
}