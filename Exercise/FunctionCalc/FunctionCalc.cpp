// FunctionCalc.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <functional>
#include <map>


using namespace std;

int main()
{
	double a, b;
	char op;

	//C++ 11:
	/*map<char, function<double(double, double)>> operations
	{
		{ '+', plus<double>() },
		{ '-', minus<double>() },
		{ '*', multiplies<double>() },
		{ '/', divides<double>() }
	};*/
	
	map<char, function<double(double, double)>> operations;
	operations['+'] = plus<double>();
	operations['-'] = minus<double>();
	operations['*'] = multiplies<double>();
	operations['/'] = divides<double>();

	while (true)
	{
		cout << "enter binary expression like 4 + 5: ";
		cin >> a >> op >> b;
		cout << a << op << b << "=" << operations[op](a, b) << endl;
	}
	return 0;
}

