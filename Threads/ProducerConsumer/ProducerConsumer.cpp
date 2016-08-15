// ProducerConsumer.cpp : Defines the entry point for the console application.
//

#include <thread>
#include <future>
#include <iostream>
#include <queue>
#include <atomic>

using namespace std;

atomic_bool exit_flag;

queue<int> q;
condition_variable queue_has_data;
mutex queue_mutex;


void producer()
{
	for (int i = 1; i <= 1000; ++i)
	{
		lock_guard<mutex> lock(queue_mutex);
		q.push(i);
		cout << "Push Data:" << i << endl;
		queue_has_data.notify_one();
	}
	exit_flag= true;
}

void consumer()
{
	bool end;

	do
	{
		unique_lock<mutex> lock(queue_mutex);
		queue_has_data.wait(lock, []{return !q.empty();});
		int data = q.front();
		q.pop();
		end = q.empty() && exit_flag;
		cout << "Consume data: " << data << endl;
	} while (!end);
}

int main()
{
	thread consumer_thread(&consumer);
	thread producer_thread(&producer);

	consumer_thread.join();
	producer_thread.join();

	return 0;
}

