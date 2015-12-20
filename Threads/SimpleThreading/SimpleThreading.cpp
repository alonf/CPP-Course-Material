// SimpleThreading.cpp : Defines the entry point for the console application.
//

#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

void threadFunc1()
{
	cout << "threadFunc1 Thread " << this_thread::get_id() << " says hello from thread!" << endl;
}

int main()
{
	thread th1(&threadFunc1);
	th1.join();

	thread([]
	{
		cout << "Lambda thread " << this_thread::get_id() << " says hello from thread!" << endl;
	}).join();

	cout << "main() thread " << this_thread::get_id() << " says hello from main!" << endl;

	vector<thread> threadVector;

	for (int i = 0; i < 10; ++i)
	{
		auto t = thread([i]
		{
			stringstream text;
			text << "Hello from thread number " << i << " with thread id " << this_thread::get_id() << endl;
			cout << text.str();
			this_thread::sleep_for(chrono::seconds(1));
		});
		threadVector.push_back(move(t));
	}

	for (auto &t: threadVector)
	{
		t.join();
	}
	cout << "main() thread says bye bye!" << endl;
	return 0;
}

