// STLFileIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <ostream>
#include <fstream>

using namespace std;

int main()
{
	auto ofile = ofstream("Test.txt");
	if (ofile.bad())
		return -1;
	ofile << "Test file." << endl;
	ofile.close();

	auto ifile = ifstream("Test.txt");
	if (ifile.bad())
		return -2;
	cout << ifile.rdbuf();

	return 0;
}

