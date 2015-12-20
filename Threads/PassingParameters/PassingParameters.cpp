// PassingParameters.cpp : Defines the entry point for the console application.
//

#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

void threadFunc1(string id)
{
	stringstream text;
	text << "(string) Hello from thread number " << id << " with thread id " << this_thread::get_id() << endl;
	cout << text.str();
	this_thread::sleep_for(chrono::seconds(1));
}

void threadFunc2(string &id)
{
	stringstream text;
	text << "(string &) Hello from thread number " << id << " with thread id " << this_thread::get_id() << endl;
	cout << text.str();
	this_thread::sleep_for(chrono::seconds(1));
}


void threadFunc3(const string &id)
{
	stringstream text;
	text << "(const string &) Hello from thread number " << id << " with thread id " << this_thread::get_id() << endl;
	cout << text.str();
	this_thread::sleep_for(chrono::seconds(1));
}

void threadFunc4(string &&id)
{
	stringstream text;
	text << "(string &&id) Hello from thread number " << id << " with thread id " << this_thread::get_id() << endl;
	cout << text.str();
	this_thread::sleep_for(chrono::seconds(1));
}

void threadFunc5(vector<int> v, int n)
{
	cout << "n + v.size() = " << n + v.size() << endl;
}

int main()
{
	vector<thread> threadVector;
	string id;

	for (int i = 1; i <= 10; ++i)
	{
		id = to_string(i);
		auto t = thread(&threadFunc1, id);
		threadVector.push_back(move(t));
	}

	for (auto &t: threadVector)
	{
		t.join();
	}
	threadVector.clear();

	//sharing ref variable (bug!!!)
	for (int i = 1; i <= 10; ++i)
	{
		id = to_string(i);
		auto t = thread(&threadFunc2, ref(id));
		threadVector.push_back(move(t));
	}

	for (auto &t: threadVector)
	{
		t.join();
	}
	threadVector.clear();

	//sharing const variable (still bug!!!)
	for (int i = 1; i <= 10; ++i)
	{
		id = to_string(i);
		auto t = thread(&threadFunc3, cref(id));
		threadVector.push_back(move(t));
	}

	for (auto &t: threadVector)
	{
		t.join();
	}
	threadVector.clear();

	//Moving variable (Safe!)
	for (int i = 1; i <= 10; ++i)
	{
		id = to_string(i);
		auto t = thread(&threadFunc4, move(id));
		threadVector.push_back(move(t));
	}

	for (auto &t : threadVector)
	{
		t.join();
	}
	threadVector.clear();

	//fast and safe
	vector<int> v;
	v.push_back(1);

	thread th(&threadFunc5, std::move(v), 4);
	th.join();
	
	return 0;
}

