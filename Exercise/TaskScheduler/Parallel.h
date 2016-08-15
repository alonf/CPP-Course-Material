//http://en.wikipedia.org/wiki/Variadic_template

#pragma once
#include <vector>
#include <functional>
#include "DefaultTaskScheduler.h"
#include "TaskHelper.h"

namespace BTSE //Best Task Scheduler Ever
{
	class Parallel
	{
	private:
		struct Pass
		{
			template<typename ...T> Pass(T...) {}
		};

	public:

		template<typename F>
		static void Invoke(F f)
		{
			RunTask(f);
		}

		template<typename F, typename ...Args>
		static void Invoke(F f, Args... args)
		{
			Invoke(f);
			Invoke(args...);
		}
		

		template<typename ...Args>
		static void BlockedInvoke(Args&&... args)
		{
			std::vector<Task_ptr> tasks;
			Pass{ (tasks.push_back(RunTask(args)), 1)... };

			for (auto task : tasks)
				task->Wait();
		}

		template<typename ...Args>
		static void For(int start, int end, std::function<void(int)> f)
		{
			auto partSize = (end - start) / TaskScheduler::Current()->ConcurrencyLevel();
			if (partSize <= 0)
				partSize = 1;

			std::function<void(int, int)> runLoop = [=] (int from, int to) 
			{
				while (from < to)
					f(from++);
			};

			std::vector<Task_ptr> tasks;
			for (int i = start; i < end; i += partSize)
			{
				tasks.push_back(RunTask(runLoop, i, std::min<int>(i + partSize, end)));
			}
			
			//block...
			for (auto task : tasks)
				task->Wait();
		}
	};
}

