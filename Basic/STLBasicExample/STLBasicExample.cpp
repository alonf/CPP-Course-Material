// STLBasicExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#ifdef WIN32
#include <Windows.h>
#include "VS/concrt_extras.h"
using namespace Concurrency::samples;
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;


int main()
{
#ifdef WIN32
	vector<int, concrt_suballocator<int>> v;
#else
	vector<int> v;
#endif
	v.push_back(3);
	v.push_back(2);
	v.push_back(1);

	do
	{
		cout << '(' << v[0] << ',' << v[1] << ',' << v[2] << ')' << endl;
	} while (next_permutation(begin(v), end(v), greater<int>()));

	return 0;
}

