// Generators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <experimental/generator>
#include <algorithm>

using namespace std::experimental;
using namespace std;

generator<int> fib()
{
	int a = 0;
	int b = 1;
	for (;;) {
		co_yield a;
		auto next = a + b;
		a = b;
		b = next;
	}
}



int main()
{
	for (auto v : fib()) {
		if (v > 50)
			break;
		cout << v << endl;
	}

	auto v = fib();
	auto result = find_if(begin(v), end(v), [](auto x) {return x > 10; });
	cout << *result;
}