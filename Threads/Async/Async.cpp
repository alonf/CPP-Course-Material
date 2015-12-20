// Async.cpp : Defines the entry point for the console application.
//

#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

void async_launch_policy(launch policy)
{
	cout << "Launch policy: " << static_cast<int>(policy) << endl;

	vector<future<string>> v;

	for (int i = 0; i < 10; ++i)
	{
		v.push_back(async(policy, []
		{
			stringstream text;
			text << "In Loop: Hello from thread number " << this_thread::get_id() << endl;
			this_thread::sleep_for(chrono::microseconds(15));
			return text.str();
		}));
	}

	for (auto && ftr : v)
	{
		cout << ftr.get();
	}
}

int main()
{
	future<string> result = async([]
	{
		stringstream text;
		text << "Hello from thread number " << this_thread::get_id() << endl;
		return text.str();
	});

	cout << "Hello from main" << endl;
	cout << result.get();

	async_launch_policy(launch::async);
	async_launch_policy(launch::deferred);

	return 0;
}

