// StringExample.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

bool check_palindrom1(string s)
{
	auto second_half = s.substr(s.length() / 2);
	reverse(begin(second_half), end(second_half));
	return s.substr(0, s.length()/2) == second_half;
}

bool check_palindrom2(string s)
{
	string temp;
	auto length = s.length() / 2;
	for (unsigned int i = 0; i < length; ++i)
	{
		temp.push_back(s.back());
		s.pop_back();
	}

	return s == temp;
}


int main()
{
	string s;
	cout << "Enter a string: ";
	cin >> s;

	string s1 = s;
	reverse(begin(s1), end(s1));

	auto s2 = s + s1;
	cout << s2 << endl;

	cout << boolalpha << check_palindrom1(s2) << endl;
	cout << boolalpha << check_palindrom2(s2) << endl;

	return 0;
}

