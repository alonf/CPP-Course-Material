// PromiseTree.cpp : Defines the entry point for the console application.
//

#include "Tree.h"
#include <iostream>

using namespace std;

int main()
{
	//will create 1024 threads...
	Tree tree(10);
	auto sum = tree.Sum();
	cout << "Sum: " << sum << endl;
	return 0;
}

