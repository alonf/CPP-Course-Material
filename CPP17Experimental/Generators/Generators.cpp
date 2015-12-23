// Generators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <experimental/generator>

using namespace std::experimental;
using namespace std;

generator<int> fib()
{
	int a = 0;
	int b = 1;
	for (;;) {
		__yield_value a;
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
}