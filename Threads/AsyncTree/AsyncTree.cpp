// AsyncTree.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "Tree.h"

using namespace std;

int main()
{
	//will not create 256 threads 
	Tree tree(8);
	auto sum = tree.Sum();
	cout << "Sum: " << sum << endl;

	return 0;
}

