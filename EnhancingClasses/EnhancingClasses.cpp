#include "register_sample.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <numeric>
#include <iterator>
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


//TODO: Abstract, override and final demo

//TODO: Non-Static data member initialization

//TODO: Implicit move ctor


#pragma region Delegate Constructor

class DelegateCtor
{
private:
	int _i;
	string _s;

public:

	DelegateCtor(int i, string s) : _i(i), _s(s)
	{
	}

	DelegateCtor(int i) : DelegateCtor(i, "")
	{
	}

	DelegateCtor() : DelegateCtor(0, "")
	{
	}

	void Print()
	{
		cout << _s << " " << _i << endl;
	}
};


void delegate_constructor_sample()
{
	DelegateCtor t = { 43, "Alon" };
	t.Print();

	DelegateCtor t1 = { 43 };
	t1.Print();

	DelegateCtor t2;
	t2.Print();
}
REGISTER_SAMPLE("Delegate Constructor", delegate_constructor_sample);
#pragma endregion


#pragma region Inheriting Constructor

class Base
{
private:
	int _a;
public:
	Base(int a) : _a(a) {}
	void Print()
	{
		cout << _a << endl;
	}
};

class Derived : public Base
{
public:
	using Base::Base;
};


void inheriting_constructor_sample()
{
	Derived d(10);
	d.Print();
}

REGISTER_SAMPLE("Inheriting Constructor", inheriting_constructor_sample);
#pragma endregion


#pragma region Defaulted and deleted functions 

class Derived2 : public Base
{
public:
	using Base::Base;
	Derived2(const Derived2 &o) = delete;
	Derived2 &operator=(const Derived &) = delete;
};

class DefSample
{
	int _a = 42;
public:
	DefSample(int a) : _a(a) {}
	DefSample() = default;
	void Print() { cout << _a << endl; }
};

void defaulted_and_deleted_functions()
{
	Derived2 d(42);
	//Derived2 o(d);
	//d = o;

	DefSample d2(21);
	d2.Print();

	DefSample().Print();


}

REGISTER_SAMPLE("defaulted and deleted functions", defaulted_and_deleted_functions);
#pragma endregion


#pragma region Initializer list ctor

class InitializerlistAverage
{
private:
	double m_result;

public:
	InitializerlistAverage(initializer_list<double> list)
	{
		m_result = accumulate(begin(list), end(list), 0.0) / list.size();
	}

	operator double() const
	{
		return m_result;
	}
};

template<typename T>
ostream &operator<<(ostream &os, initializer_list<T> list)
{
	stringstream s;
	copy(cbegin(list), cend(list), ostream_iterator<double>(s, ", "));
	auto result(s.str());
	result.erase(result.length() - 2);
	os << "{ " << result << " }";
	return os;
}

void initializer_list_ctor_sample()
{
	InitializerlistAverage avg({ 1, 2, 3, 4 });
	cout << "Average({1,2,3,4}) = " << avg << endl;

	auto list = { 1.0, 2.0, 3.0, 5.0, 7.0, 11.0, 13.0, 17.0, 19.0 };
	cout << "Average(" << list << ") = " << InitializerlistAverage(list) << endl;
}

REGISTER_SAMPLE("Initializer List ctor", initializer_list_ctor_sample);
#pragma endregion