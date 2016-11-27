#include "register_sample.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <array>
#include <numeric>
#include <vector>
#include <functional>
#include <list>
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


//TODO: C99 Func
//TODO: Extended sizeof

#pragma region Static Assert

template <typename T>
void Swap(T& a, T& b)
{
	static_assert(std::is_copy_constructible<T>::value,
		"Swap requires copying");
	static_assert(std::is_nothrow_move_constructible<T>::value
		&& std::is_nothrow_move_assignable<T>::value,
		"Swap may throw");
	auto c = b;
	b = a;
	a = c;
}

template <class T>
struct data_structure
{
	static_assert(std::is_default_constructible<T>::value,
		"Data Structure requires default-constructible elements");
};

struct no_copy
{
	no_copy(const no_copy&) = delete;
	no_copy() = default;
};

struct no_default
{
	no_default() = delete;
};



template <int N>
struct fibo
{
	enum { value = fibo<N - 1>::value + fibo<N - 2>::value };
};
template <>
struct fibo<1>
{
	enum { value = 1 };
};
template <>
struct fibo<0>
{
	enum { value = 1 };
};


class just_some_type
{
public:
	just_some_type() {}
};

template <typename T>
void write_out(ostream& os, const T& value)
{
	static_assert(
		(is_array<T>::value && is_arithmetic<typename remove_extent<T>::type>::value) ||
		is_arithmetic<T>::value, "T must be an arithmetic type or a 1-dimensional array of such");

	//Proceed to write out the value in some way
}

//static_assert declaration: http://msdn.microsoft.com/en-us/library/dd293588(VS.100).aspx

int static_assertions()
{
	int a, b;
	Swap(a, b);
#ifdef _WIN32
#pragma warning (disable: 4101)
#else
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
	// ReSharper disable once CppEntityNeverUsed
	data_structure<int> ds_ok;
	

	int arr[] = { 42,42,43,0,17 };
	write_out(cout, arr);
	
	//The following won't compile because of the static_assert.
	int staticAssertLineNumber = __LINE__;
	/*
	no_copy nc_a, nc_b;
	Swap(nc_a, nc_b); // 1

	//data_structure<no_default> ds_error; // 2

	int arr2[10];
	write_out(cout, arr2);
	write_out(cout, just_some_type());

	*/
	

	static_assert(fibo<0>::value == 1, "Definition for 0 is missing or wrong");
	static_assert(fibo<1>::value == 1, "Definition for 1 is missing or wrong");
	static_assert(fibo<2>::value == 2, "Definition for 2 is wrong");
	static_assert(fibo<3>::value == 3, "Definition for 3 is wrong");
	static_assert(fibo<4>::value == 5, "Definition for 4 is wrong");
	static_assert(fibo<5>::value == 8, "Definition for 5 is wrong");

#ifdef _WIN32
	static_assert(sizeof(long) == sizeof(int), "What compiler are you using?? Your ints aren't long!");
#else
	static_assert(sizeof(long) != sizeof(int), "What compiler are you using?? Your ints aren't long!");
#endif

	return staticAssertLineNumber;
	
}
void static_assertion_description()
{
	auto staticAssertLineNumber = static_assertions();
	cout << "Uncomment lines after " << staticAssertLineNumber << " to see how it finds error during compilation" << endl;
}

REGISTER_SAMPLE("Static assertion description", static_assertion_description);
#pragma endregion


#pragma region Local & Unnamed Types as Template Arguments
// ReSharper disable once CppEntityUsedOnlyInUnevaluatedContext
template <class T> void typeid_of(T t)
{
	cout << "Typeid: " << typeid(t).name() << endl;
}

void local_unamed_types()
{
	struct Local {};
	enum { No = 0, Yes = 1 };

	typeid_of(Local());
	typeid_of(Yes);
}
REGISTER_SAMPLE("Local & Unnamed Types as Template Arguments", local_unamed_types);
#pragma endregion


#pragma region Extern Template
#include "print.h"

extern template extern_template <Print>;

void extern_template_sample()
{
	extern_template<Print>().act();

}
REGISTER_SAMPLE("Extern Taemplate", extern_template_sample);
#pragma endregion


#pragma region Template Alias

template<class V, class L>
class double_container
{
	vector<V> m_v;
	list<L> m_l;

public:
	double_container(initializer_list<tuple<V&&,L&&>> &&list)
	{
		for (auto &&p : list)
		{
			m_v.push_back(forward<V>(get<0>(p)));
			m_l.push_back(forward<L>(get<1>(p)));
		}
	}

	//...
};

template<>
using string_int_container = double_container<string, int>;

template<class V>
using same_type_container = double_container<V, V>;

void template_alias_sample()
{
	string_int_container<> d{ make_tuple("A"s, 65), make_tuple("B"s, 66) };
	same_type_container<int> s{ make_tuple(1,1), make_tuple(2,3) };

}
REGISTER_SAMPLE("Template Alias", template_alias_sample);
#pragma endregion

//TODO: Template aliases


#pragma region Default template arguments for function templates

template<class T = int, int I = 42>
void default_template_sample(T t)
{
	cout << "I: " << I << " T: " << t << endl;
}

void default_template_arguments_for_function_templates_sample()
{
	default_template_sample(10);
	default_template_sample(1.3);
	default_template_sample<double, 43>(1.3);
}

REGISTER_SAMPLE("Default template arguments for function templates", default_template_arguments_for_function_templates_sample);
#pragma endregion


#pragma region Variadic Template

template<typename T>
T Add(T a, T b)
{
	return a + b;
}

template<typename T>
T Add(T a, T b, T c)
{
	return a + b + c;
}

template<typename T>
T Add(T a, T b, T c, T d)
{
	return a + b + c + d;
}


template <typename... Args>
void variadic_template_demo(Args... args)
{
	cout << "The number of parameters is: " << sizeof...(Args) << endl;
	cout << Add(args...) << endl << endl;
};

void variadic_template_sample()
{
	variadic_template_demo(1, 2);
	variadic_template_demo(string("1"), string("2"), string("3"));
	variadic_template_demo(1.1, 2.2, 3.3, 4.4);
}

REGISTER_SAMPLE("Variadic Template", variadic_template_sample);


template <int ...I>
int Sum()
{
	array <int, sizeof...(I)> numbers = { I... };
	int result = accumulate(begin(numbers), end(numbers), 0);
	return result;
}

void variadic_template_sum()
{
	cout << Sum<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>() << endl;
}

REGISTER_SAMPLE("Variadic Template Sum", variadic_template_sum);
#pragma endregion


#pragma region constexpr

constexpr int Add(int a, int b)
{
	return a + b;
}
int RTAdd(int a, int b)
{
	return a + b;
}
#ifndef _WIN32
constexpr int extended_constexpr_sum(int reminder)
{
	int sum = 0;
	for (int i = 1; i <= 100; ++i)
		sum += (i % 2 == reminder) ? i : 0;
	return sum;
}
#endif
void constexpr_sample()
{
	int a = Add(10, 32);
	cout << a << endl;
	a = RTAdd(10, 32);
	cout << a << endl;

	int b, c;
	cin >> b >> c;
	a = Add(b, c);
	cout << a << endl;

#ifndef _WIN32
	cout << "The sum of all odds is: " << extended_constexpr_sum(1) << endl;
	cout << "The sum of all evens is: " << extended_constexpr_sum(0) << endl;;
#endif
}

REGISTER_SAMPLE("constexpr", constexpr_sample);
#pragma endregion


#pragma region variable template

template<typename T>
	constexpr T pi = T(3.14159265358979323856);

template<typename T>
constexpr T area_of_circle_with_radius(T r)
{
	return pi<T> * r * r;
}

void variable_template()
{
	cout.precision(10);
	cout << "int: " << area_of_circle_with_radius<int>(10) << endl;
	cout << "float: " << area_of_circle_with_radius<float>(10) << endl;
	cout << "double: " << area_of_circle_with_radius<double>(10) << endl;
}

REGISTER_SAMPLE("Variable Template", variable_template);

#pragma endregion


#pragma region attributes

[[ noreturn ]] void just_throws()
{
	while (true)
		;
	throw "error...";
}



[[ deprecated ]] void consider_using_another_function()
{
	cout << "There must be a better newer way to do that." << endl;
	cout << "It also generates a compilation warning." << endl;
}

void attributes_sample()
{
	try
	{
		just_throws();
	}
	catch (...)
	{

	}
	consider_using_another_function();
}

REGISTER_SAMPLE("C++ attributes", attributes_sample);
#pragma endregion

