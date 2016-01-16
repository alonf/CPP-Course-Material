#pragma once
#include <ostream>
#include <iostream>

template <class T>
class extern_template
{
public:
	void act()
	{
		T::run();
	}
};

class Print
{
public:
	static void run()
	{
		std::cout << "Running..." << std::endl;
	}
};
