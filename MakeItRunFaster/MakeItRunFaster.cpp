#include "register_sample.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <chrono>
#include <cstring>
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
	register_sample FUNCTION##_sample(DESCRIPTION, &FUNCTION)


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


#pragma region Rvalue References

void print_reference(const string& str)
{
	cout << "const string& :" << str << endl;
}

void print_reference(string&& str)
{
	cout << "string&& :" << str << endl;
}

void rvalue_references_overload()
{
	string s("s: alon");
	print_reference(string("alon")); //What's happen if we used "alon with no casting to string. See here: http://www.eventhelix.com/realtimemantra/object_oriented/stl-tutorial-advanced.htm#Advanced_STL_4:_More_about_Rvalue_References
	print_reference(s);
}
REGISTER_SAMPLE("rvalue references overload", rvalue_references_overload);


class my_string_old
{
private:
	char* _string;
	unsigned int _length;
public:
	explicit my_string_old(const char* str)
	{
		if (str == nullptr)
		{
			_length = 0;
			_string = nullptr;
			return;
		}

		_length = static_cast<unsigned int>(strlen(str) + 1);
		_string = new char[_length];
#ifdef _WIN32
		strcpy_s(_string, _length, str);
#else
		strcpy(_string, str);
#endif
	}

	explicit my_string_old(int length)
	{
		_length = length;
		_string = new char[_length];
		_string[0] = '\0';
	}

	my_string_old(const my_string_old& other)
	{
		_length = other._length;
		if (other._string == nullptr)
		{
			_string = nullptr;
			return;
		}

		_string = new char[_length];
#ifdef _WIN32
		strcpy_s(_string, _length, other._string);
#else
		strcpy(_string, other._string);
#endif
	}

	my_string_old& operator=(const my_string_old& other)
	{
		if (this == &other)
			return *this;

		if (_string != nullptr)
			delete[] _string;

		_string = nullptr;
		_length = other._length;
		if (other._string != nullptr)
		{
			_string = new char[_length];

#ifdef _WIN32
		strcpy_s(_string, _length, other._string);
#else
		strcpy(_string, other._string);
#endif
		}
		return *this;
	}
	~my_string_old()
	{
		if (_string != nullptr)
		{
			delete[] _string;
			_string = nullptr;
		}
		_length = 0;
	}
};

class my_string_new
{
private:
	char* _string;
	int _length;
public:
	//standard functions:
	explicit my_string_new(const char* str)
	{
		if (str == nullptr)
		{
			_length = 0;
			_string = nullptr;
			return;
		}
		_length = static_cast<unsigned int>(strlen(str) + 1);
		_string = new char[_length];
#ifdef _WIN32
		strcpy_s(_string, _length, str);
#else
		strcpy(_string, str);
#endif
	}

	explicit my_string_new(int length)
	{
		_length = length;
		_string = new char[_length];
		_string[0] = '\0';
	}

	my_string_new(const my_string_new& other)
	{
		_length = other._length;
		if (other._string == nullptr)
		{
			_string = nullptr;
			return;
		}

		_string = new char[_length];
#ifdef _WIN32
		strcpy_s(_string, _length, other._string);
#else
		strcpy(_string, other._string);
#endif
	}

	my_string_new& operator=(const my_string_new& other)
	{
		if (this == &other)
			return *this;

		if (_string != nullptr)
			delete[] _string;

		_string = nullptr;
		_length = other._length;

		if (other._string != nullptr)
		{
			_string = new char[_length];
#ifdef _WIN32
			strcpy_s(_string, _length, other._string);
#else
			strcpy(_string, other._string);
#endif
		}
		return *this;
	}
	~my_string_new()
	{
		if (_string != nullptr)
		{
			delete[] _string;
			_string = nullptr;
		}
		_length = 0;
	}

	//move functions:
	my_string_new(my_string_new&& other)
	{
		*this = std::move(other); //alonf::move preserves && semantics so operator=(&&) is called
	}



	my_string_new& operator=(my_string_new&& other)
	{
		if (this == &other)
			return *this;//handle self-assignment

		_string = other._string;
		_length = other._length;

		other._string = nullptr;
		other._length = 0;

		return *this;
	}
};



double measure_time(int times, function<void(void)> action)
{
	chrono::steady_clock::duration total = std::chrono::steady_clock::duration::zero();
	
	action();//discard first measurement to warm up
	for (int i = 0; i < times; ++i)
	{
		auto const start_time = std::chrono::steady_clock::now();
		action();
		auto const end_time = std::chrono::steady_clock::now();
		total +=  end_time - start_time;
	}
	return chrono::duration_cast<std::chrono::milliseconds>(total).count() 
		/ static_cast<double>(times);
}

void rvalue_references_move()
{
	const int OUTER_ITERATIONS = 5;
	const int INNER_ITERATIONS = 100000;

	double time = measure_time(OUTER_ITERATIONS, [=]()
	{
		vector<my_string_new> vec;
		for (int i = 0; i < INNER_ITERATIONS; ++i)
			vec.push_back(my_string_new("hello"));
	});
	cout << "with move ctor: " << time << " ms" << endl;
	time = measure_time(OUTER_ITERATIONS, [=]()
	{
		vector<my_string_old> vec;
		for (int i = 0; i < INNER_ITERATIONS; ++i)
			vec.push_back(my_string_old("hello"));
	});
	cout << "without move ctor: " << time << " ms" << endl;
}
REGISTER_SAMPLE("rvalue references move", rvalue_references_move);


template<typename T>
class Target
{

public:
	void Action(const typename remove_reference<T>::type &) { cout << "Action(T&)" << endl; };
	void Action(typename remove_reference<T>::type &&) { cout << "Action(T&&)" << endl; };
};

template <typename T>
void old_forward(const T& value)
{
	cout << "old_forward(T& value) ==> ";
	Target<T> target;
	target.Action(value);
}

template <typename T>
void wrong_new_forward(T&& value)
{
	cout << "new_forward(T&& value) ==> ";
	Target<T> target;
	target.Action(value);
}

template <typename T>
void new_forward(T&& value)
{
	cout << "new_forward(T&& value) ==> ";
	Target<T> target;
	target.Action(std::forward<T>(value));
}

void rvalue_references_forward()
{
	string s("Alon");
	cout << "old forward:" << endl;
	cout << "------------" << endl;
	cout << "string s(\"Alon\"); ==> ";
	old_forward(s);
	cout << "old_forward(string(\"Alon\")) ==> ";
	old_forward(string("Alon"));
	cout << endl;

	cout << "wrong new forward - not a perfect forwarding:" << endl;
	cout << "---------------------------------------------" << endl;
	cout << "string s(\"Alon\"); ==> ";
	wrong_new_forward(s);
	cout << "new_forward(string(\"Alon\")) ==> ";
	wrong_new_forward(string("Alon"));
	cout << endl;

	cout << "new perfect forwarding:" << endl;
	cout << "-----------------------" << endl;
	cout << "string s(\"Alon\"); ==> ";
	new_forward(s);
	cout << "new_forward(string(\"Alon\")) ==> ";
	new_forward(string("Alon"));
}

REGISTER_SAMPLE("rvalue references forward", rvalue_references_forward);
#pragma endregion


#pragma region ref qualifiers on member functions
class RefQualifierClass
{
public:
	void Do() &
	{
		cout << "Do &" << endl;
	}

	void Do() const &
	{
		cout << "Do Const &" << endl;
	}

	void Do() &&
	{
		cout << "Do &&" << endl;
	}
};

void ref_qualifiers_on_member_functions()
{
	RefQualifierClass m;
	m.Do();
	const auto x = m;
	x.Do();
	RefQualifierClass().Do();

}

REGISTER_SAMPLE("ref qualifiers on member functions", ref_qualifiers_on_member_functions);
#pragma endregion



