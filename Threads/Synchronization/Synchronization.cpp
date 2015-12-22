// Synchronization.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <mutex>
#include <future>
#include <vector>
#include <atomic>

using namespace std;

int main()
{
	
	int counter = 0;
	vector<future<void>> ftr;

	//Buggy:
	for (int i = 0; i < 100000; ++i)
	{
		ftr.push_back(async([&]
		{
			++counter;
		}));
	}
	
	for (auto && f : ftr)
		f.wait();

	cout << "Non locked: " << counter << endl;


	//with mutex
	counter = 0;
	ftr.clear();
	mutex m;
	
	for (int i = 0; i < 100000; ++i)
	{
		ftr.push_back(async([&]
		{
			m.lock();
			++counter;
			m.unlock();
		}));
	}

	for (auto && f : ftr)
		f.wait();
	cout << "With mutex: " << counter << endl;


	//with RAII (lock_gaurd)
	counter = 0;
	ftr.clear();
	
	for (int i = 0; i < 100000; ++i)
	{
		ftr.push_back(async([&]
		{
			lock_guard<mutex> lock(m);
			++counter;
		}));
	}

	for (auto && f : ftr)
		f.wait();
	cout << "With lock_guard mutex: " << counter << endl;


	//with atomic operation
	atomic<int> atomic_counter(0);
	ftr.clear();

	for (int i = 0; i < 100000; ++i)
	{
		ftr.push_back(async([&]
		{
			++atomic_counter;
		}));
	}

	for (auto && f : ftr)
		f.wait();
	cout << "With atomic<int>: " << atomic_counter << endl;

	return 0;
}

