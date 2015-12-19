//TR1Examples.h
#pragma once
#include <functional>

using namespace std;

class register_sample
{
public:
	register_sample(const char *name, function<void (void)> f);

private:
	static int n_entry;
};
