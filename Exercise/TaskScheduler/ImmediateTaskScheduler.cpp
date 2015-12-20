#include "ImmediateTaskScheduler.h"
#include <thread>

using namespace std;

namespace BTSE //Best Task Scheduler Ever
{
	ImmediateTaskScheduler::ImmediateTaskScheduler(int concurrencyLevel /*= 0*/) noexcept
	{
	}


	ImmediateTaskScheduler::~ImmediateTaskScheduler() noexcept
	{
	}

	/* virtual */ void ImmediateTaskScheduler::QueueTask(Task_ptr task) noexcept(false) /* override */
	{
		thread t([=]() {StartTask(task);});
		t.detach();
	}

}
