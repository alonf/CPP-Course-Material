// Promises.cpp : Defines the entry point for the console application.
//

#include <thread>
#include <future>
#include <iostream>
#include <functional>
#include <exception>

using namespace std;

void calculate(int a, int b, function<int (int,int)> func, promise<int> &&prms)
{
	cout << "Calculating in thread " << this_thread::get_id() << endl;

	try
	{
		prms.set_value(func(a,b));
	}
	catch (...)
	{
		prms.set_exception(current_exception());
	}
}

int main()
{
	promise<int> prms;
	auto ftr = prms.get_future();

	int a = 1;
	int b = 0;
	auto f = [](int x, int y)
	{ 
		if (y == 0)
			throw exception("Divide by zero");
		return x / y;
	};

	thread th(&calculate, a, b, f, move(prms));
	
	try
	{
		cout << "Result:" << ftr.get() << endl;
	}
	catch (exception &exp)
	{
		cout << "Exception:" << exp.what() << endl;
	}
	
	th.join();

	return 0;
}

