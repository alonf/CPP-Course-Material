#include "register_sample.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
using namespace std;


#pragma region Menu

typedef tuple<shared_ptr<const string>, function<void()>> sample_entry;
vector<sample_entry> samples;

register_sample::register_sample(const char *name, function<void(void)> f)
{
	stringstream s;
	s << n_entry++ << ". " << name;
	auto sample_name(make_shared<const string>(s.str()));

	samples.push_back(sample_entry(sample_name, f));
}

int register_sample::n_entry = 1;

#define REGISTER_SAMPLE(DESCRIPTION, FUNCTION) \
	register_sample FUNCTION##_sample(DESCRIPTION, &##FUNCTION)


int main()
{
	for_each(begin(samples), end(samples), [&](sample_entry entry) { cout << *get<0>(entry) << endl; });

	unsigned int sample_entry;

	while (true)
	{
		cout << endl << "Choose sample:";
		cin >> sample_entry;

		if (sample_entry == 0 || sample_entry > samples.size())
			continue;

		get<1>(samples[sample_entry - 1])();
	}
}

#pragma endregion
