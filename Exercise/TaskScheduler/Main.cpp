// Main.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include "Parallel.h"
#include "util.h"
#include "ImmediateTaskScheduler.h"
#include "TaskHelper.h"

using namespace std;
using namespace BTSE;

int main()
{
	auto scheduler = CreateScheduler<ImmediateTaskScheduler>();
	function<void(int, int)> add = [](int a, int b)
	{ cout << a << " + " << b << " = " << a + b << endl;};

	for (int i = 0; i < 10; ++i)
		RunTask(scheduler, add, i, i);

	this_thread::sleep_for(1s);

	auto task = RunTask(scheduler, function<int(int)>([](int a) { return a * 10;}), 5);
	cout << "Result: " << GetResult<int>(task) << endl;

	this_thread::sleep_for(1s);
	cout << endl << endl;

	function<thread::id()> f = []() { this_thread::sleep_for(chrono::milliseconds(10)); return this_thread::get_id();};
	vector<Task_ptr> v;
	for (int i = 0; i < 100; ++i)
	{
		auto t = RunTask(f);
		v.push_back(t);
	}
	for_each(begin(v), end(v), [](auto e) {cout << GetResult<thread::id>(e) << ", ";});
	cout << endl << endl;

	try
	{
		RunTask(FFL([] { throw runtime_error("Exception Demo...");}))->ThrowWait();
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
	cout << endl << endl;

	
	auto immediateScheduler = CreateScheduler<ImmediateTaskScheduler>(); //must hold a strong reference
	auto originalTaskScheduler = immediateScheduler->MakeCurrent();
	Parallel::For(0, 100, [](int i) { cout << i << ", ";});
	originalTaskScheduler->MakeCurrent(); //return to normal
	immediateScheduler.reset(); //free newScheduler resources
	cout << endl << endl;

	auto newScheduler = CreateScheduler(16); //must hold a strong reference
	originalTaskScheduler = newScheduler->MakeCurrent(); //run with 16 threas
	Parallel::For(0, 100, [](int i) { cout << i << ", ";});
	originalTaskScheduler->MakeCurrent(); //return to normal
	newScheduler.reset(); //free newScheduler resources
	cout << endl << endl;

	Parallel::Invoke(FFL([] {cout << "One" << endl;}),
					 FFL([]() {cout << "Another one" << endl;}),
					 FFL([]() {cout << "And another one" << endl;}));
	
	this_thread::sleep_for(chrono::milliseconds(100));


	Parallel::BlockedInvoke(FFL([] {cout << "Two" << endl;}),
						    FFL([]() {this_thread::sleep_for(chrono::milliseconds(100)); cout << "Another two" << endl;}),
							FFL([]() {cout << "And another two" << endl;}));


	return 0;
}

