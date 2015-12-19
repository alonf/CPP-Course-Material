#pragma once
#include <functional>

class register_sample
{
public:
	register_sample(const char *name, std::function<void(void)> f);

private:
	static int n_entry;
};
