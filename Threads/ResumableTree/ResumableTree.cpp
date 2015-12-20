// ResumableTree.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "Tree.h"

using namespace std;

int main()
{
	//will not create million threads 
	Tree tree(20);
	auto sum = tree.Sum().get();
	cout << "Sum: " << sum << endl;

	return 0;
}

