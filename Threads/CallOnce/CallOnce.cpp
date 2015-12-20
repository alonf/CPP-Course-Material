// CallOnce.cpp : Defines the entry point for the console application.
//

#include <thread>
#include <mutex>
#include <random>
#include <iostream>

using namespace std;

once_flag flag;
thread::id first;

bool AmITheFirst()
{
	call_once(flag, [] {first = this_thread::get_id();});
	return (this_thread::get_id() == first);
}

int main()
{
	vector<thread> v;
	mt19937 eng;  // a core engine class    
	uniform_int_distribution<int> unif(0, 100);

	for (int i = 0; i < 10; ++i)
	{
		thread t([&] 
		{
			cout << "Thread " << this_thread::get_id() << " is running" << endl;
			this_thread::sleep_for(chrono::milliseconds(unif(eng)));
			if (AmITheFirst())
				cout << "Thread " << this_thread::get_id() << " is the first" << endl;
		});
		v.push_back(move(t));
	}

	for (auto &t : v)
		t.join();

	return 0;
}

