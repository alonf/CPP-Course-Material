#include "register_sample.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <list>
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


#pragma region Raw string literal 

void raw_string_literal_sample()
{
	auto regular_string = "\\\\ \t\tYes it is simple \n\n";
	auto raw_string = R"(\\\\ \t\tYes it is simple \n\n)";

	cout << regular_string << endl;
	cout << raw_string << endl;
}

REGISTER_SAMPLE("Raw String Literal", raw_string_literal_sample);
#pragma endregion


#pragma region C++1'4 digit separators  

void Cpp14_digit_separators_sample()
{
	auto integer_literal = 1'000'000;
	auto floating_point_literal = 0.000'015'3;
	auto binary_literal = 0b0100'1100'0110;
	auto silly_example = 1'0'0'000'00;

	cout << integer_literal << endl;
	cout << floating_point_literal << endl;

}

REGISTER_SAMPLE("C++1'4 digit separators", Cpp14_digit_separators_sample);
#pragma endregion


#pragma region Auto Variables

class Base {};
class Derived1 : public Base {};
class Derived2 : public Base {};


//auto keyword: http://msdn.microsoft.com/en-us/library/dd293667(VS.100).aspx
void show_auto_variables(bool callAgain)
{
	int answer = 42;
	auto &question = answer;
	auto *p_question = &answer;
	auto d1(make_shared<Derived1>(Derived1()));
	auto d2 = make_shared<Derived2>(Derived2());
	auto ptr_base = answer > rand() ? shared_ptr<Base>(d1) : shared_ptr<Base>(d2);
	auto i(answer * 3);
	auto x = i, *p = &i;
	const auto y = x;
	auto *z = new auto(i + 20);
	auto func = show_auto_variables;
	if (callAgain)
		func(false);

	list<stack<map<vector<char>, long long>>> could_use_a_typedef;
	auto could_use_a_typedef_iterator = begin(could_use_a_typedef);
}

void auto_variables()
{
	show_auto_variables(true);
}
REGISTER_SAMPLE("auto variables", auto_variables);

#pragma endregion


#pragma region decltype

struct Complex
{
	double real;
	double imag;
	double abs_plus(double d) { return sqrt(real*real + imag*imag) + d; }
};
ostream& operator<<(ostream& os, Complex& c)
{
	os << c.real << ' ' << c.imag;
	return os;
}

//This can be more properly rewritten using std::forward<T> and r-value references. See below.
template <typename T, typename S>
auto add(const T& t, const S& s) -> decltype(t + s)
{
	return t + s;
}


//decltype: http://msdn.microsoft.com/en-us/library/dd537655(VS.100).aspx
void decl_type()
{
	float arr[15];
	decltype(arr) another_arr = { 1,2,3 };
	decltype(arr[0]) a_float_ref = arr[9];//float&
	decltype(arr[1] + 7) a_float = 42.0f;//float

	decltype(rand()) number = rand();

	Complex i = { 0.0, 1.0 };
	const Complex* j = &i;
	decltype(i.real) real_val = 16.0;//double
	decltype((i.real)) real_val_ref = i.real;//double&
	real_val_ref++;
	decltype((j->imag)) imag_val_ref = j->real;//const double&
	decltype(j->imag) imag_val = j->imag;//double
	imag_val++;
	cout << i << endl;

	decltype(add(1, 3.14)) sum = add(1, 3.14);
	auto str = add(string("Goodbye, old "), 'C');
	cout << sum << ' ' << str << endl;
}

REGISTER_SAMPLE("decltype", decl_type);

#pragma endregion

#pragma region Trailing return type
auto foo(int a) -> int
{
	if (a > 5)
		return a;
	return 'a';
}

template<class T, class U>
auto factory(T t, U u) -> function<decltype(t + u) (T, U)>
{
	return [](T t, U u) { return t + u; };
}

void trailing_return_type()
{
	cout << foo(3) << endl;
	cout << foo(6) << endl;

#pragma push_macro("max")
#undef max
	long long ll = numeric_limits<long long>::max();
#pragma pop_macro("max")

	auto fi = factory(1, 1);
	auto fl = factory(ll, 1);
#pragma warning (push)
#pragma warning (disable:4244)
	cout << fi(ll, 0) << endl;
	cout << fi(ll, 1) << endl;
#pragma warning (pop)
	cout << fl(ll, 0) << endl;
	cout << fl(ll, 1) << endl;
}
REGISTER_SAMPLE("trailing return type", trailing_return_type);
#pragma endregion


#pragma region Lambda Functions

void capture_by_value_and_by_ref()
{
	int i = 3;
	int j = 5;

	// The following lambda expression captures i by value and
	// j by reference.
	function<int(void)> f = [=, &j] { return i + j; };

	cout << f() << endl;

	// Change the values of i and j.
	i = 22;
	j = 44;

	// Call f and print its result.
	cout << f() << endl;
}
REGISTER_SAMPLE("Lambda: capture by value and by ref", capture_by_value_and_by_ref);


void mutable_keyword()
{
	int i = 3;
	int j = 5;

	//delete the mutable: error C3491: 'j': a by-value capture cannot be modified in a non-mutable lambda
	[=, &i]() mutable { i = 4; j = 5; };

	// Call f and print its result.
	cout << i << " " << j << endl;
}
REGISTER_SAMPLE("Lambda: the mutable keyword", mutable_keyword);


void lambda_find_if()
{
	// Create a list of integers with a few initial elements.
	list<int> numbers;
	numbers.push_back(13);
	numbers.push_back(17);
	numbers.push_back(42);
	numbers.push_back(46);
	numbers.push_back(99);

	// Use the find_if function and a lambda expression to find the 
	// first even number in the list.
	const list<int>::const_iterator result =
		find_if(numbers.begin(), numbers.end(),
			[](int n) { return (n % 2) == 0; });

	// Print the result.
	if (result != numbers.end())
	{
		cout << "The first even number in the list is "
			<< (*result)
			<< "."
			<< endl;
	}
	else
	{
		cout << "The list contains no even numbers."
			<< endl;
	}
}
REGISTER_SAMPLE("Lambda: find_if", lambda_find_if);


void nesting_lambda_expressions()
{

	// The following lambda expression contains a nested lambda
	// expression.
	int m = [](int x)
	{ return [](int y) { return y * 2; }(x)+3; }(5);

	// Print the result.
	cout << m << endl;
}
REGISTER_SAMPLE("Lambda: nesting expressions", nesting_lambda_expressions);


void higher_order_lambda_expression()
{
	// The following code declares a lambda expression that returns 
	// another lambda expression that adds two numbers. 
	// The returned lambda expression captures parameter x by value.
	auto g = [](int x) -> function<int(int)>
	{ return [=](int y) { return x + y; }; };

	// The following code declares a lambda expression that takes another
	// lambda expression as its argument.
	// The lambda expression applies the argument z to the function f
	// and adds 1.
	auto h = [](const function<int(int)>& f, int z)  -> int
	{ return f(z) + 1; };

	// Call the lambda expression that is bound to h. 
	auto a = h(g(7), 8);

	// Print the result.
	cout << a << endl;
}
REGISTER_SAMPLE("Lambda: higher order expression", higher_order_lambda_expression);


template <typename T, typename Iterator>
struct enumerable
{
	Iterator begin_, end_;
	enumerable(Iterator begin, Iterator end) : begin_(begin), end_(end) {}
	operator bool() { return begin_ != end_; }
	enumerable& operator++() { if (*this) { ++begin_; } return *this; }
	T& operator*() { return *begin_; }

	template <typename Ftor> void apply(Ftor ftor)
	{
		while (*this)
		{
			ftor(**this);
			++(*this);
		}
	}
	template <typename Ftor, typename Filter> void apply(Ftor ftor, Filter flt)
	{
		while (*this)
		{
			if (flt(**this))
				ftor(**this);
			++(*this);
		}
	}
};

template <typename _Iterator>
static auto make_enumerable(_Iterator begin, _Iterator end)
-> enumerable<decltype(*begin), _Iterator>
{
	return enumerable<decltype(*begin), _Iterator>(begin, end);
}

template <typename G>
auto generator(G next) -> list<decltype(next(nullptr))>
{
	list<decltype(next(nullptr))> lst;
	bool has_more;
	decltype(next(nullptr)) val = next(&has_more);
	while (has_more)
	{
		lst.push_back(val);
		val = next(&has_more);
	}
	return lst;
}

template <typename Iterator, typename Combiner>
auto aggregate(
	Iterator begin,
	Iterator end,
	Combiner comb,
	decltype(comb(*begin, *begin)) initial)
	-> decltype(comb(*begin, *begin))
{
	decltype(comb(*begin, *begin)) result = initial;
	for (; begin != end; ++begin)
	{
		result = comb(result, *begin);
	}
	return result;
}

function<int(void)> prime_number_generator(int start)
{
	return [start]() mutable -> int
	{
		while (true)
		{
			if (start == 2)
				return start++;

			if (start % 2 != 0)
			{
				int root = (int)std::sqrt((double)start) + 1;
				bool stop = false;
				for (int n = 3; n <= root && !stop; n += 2)
				{
					if (start % n == 0)
						stop = true;
				}
				if (!stop)
					return start++;
			}
			++start;
		}
	};
}

//Note: 'this' can be captured implicitly or explicitly like any other variable.

//lambda functions: http://msdn.microsoft.com/en-us/library/dd293608(VS.100).aspx
void lambda_functions()
{
	int generated = 0;//captured by reference and mutated inside the lambda
	list<int> numbers = generator([&generated](bool* has_next)->int { *has_next = ++generated < 100; return generated; });
	for_each(begin(numbers), end(numbers), [](int n) { if (n % 7 == 0) cout << n << ' '; });
	cout << endl;

	int result = aggregate(begin(numbers), end(numbers), [](int curr, int next) { return curr - next; }, 100);
	cout << result << endl;

	auto my_enum = make_enumerable(begin(numbers), end(numbers));
	my_enum.apply(
		[](int n) { cout << n << ' '; },//functor
		[](int n) { return n % 13 == 0; }); //filter
	cout << endl;

	auto add_f = [](int a, int b) { return a + b; };
	auto something = aggregate(numbers.begin(), numbers.end(), add_f, 0);
	cout << something << endl;

	//the following is a lambda that takes a lambda and returns a lambda. it's a high-order function.
	auto curry = [](const function<int(int, int)>& f, int a) ->function<int(int)> { return [=](int b) { return f(b, a); }; };
	auto add_5 = curry(add_f, 5);
	cout << "Curry? " << add_5(13) << endl;

	int x = 12;
	auto checker = [x](int n) { return n != 0 && n != x && n%x == 0; };
	auto divisible_by = *find_if(numbers.begin(), numbers.end(), checker);
	cout << divisible_by << endl;
	x = 13;
	divisible_by = *find_if(numbers.begin(), numbers.end(), checker);//nothing changes - x is captured by value
	cout << divisible_by << endl;

	auto checker2 = [&x](int n) { return n != 0 && n != x && n%x == 0; };
	divisible_by = *find_if(numbers.begin(), numbers.end(), checker2);
	cout << divisible_by << endl;
	x = 17;
	divisible_by = *find_if(numbers.begin(), numbers.end(), checker2);//works - x is captured by reference
	cout << divisible_by << endl;

	int fib1 = 1, fib2 = 1;
	auto next_step = [&]() { int temp = fib2; fib2 = fib2 + fib1; fib1 = temp; };//capture by reference
	for (int i = 0; i < 20; ++i)
		next_step();
	cout << fib2 << endl;

	int howmany = 25;
	auto next_num = [=](int prev) mutable ->int		//mutable lambda
	{ return (--howmany > 0) ? prev + 1 : 0; };
	int curr = 1;
	while (curr != 0)
	{
		curr = next_num(curr);
		cout << curr << ' ';
	}
	cout << endl;

	auto primes_from_100 = prime_number_generator(100);
	for (int i = 0; i < 10; ++i)
		std::cout << primes_from_100() << ' ';
	std::cout << endl;
}

REGISTER_SAMPLE("Lambda: in depth sample", lambda_functions);

#pragma endregion


#pragma region Uniform Initializers 

class UniformInitializers
{
private:
	int _i;
	string _s;

public:
	UniformInitializers(int i, string s) : _i(i), _s(s)
	{
	}

	void Print()
	{
		cout << _s << " " << _i << endl;
	}
};


void uniform_initializers_sample()
{
	UniformInitializers t = { 43, "Alon" };
	t.Print();
}

REGISTER_SAMPLE("Uniform Initializers", uniform_initializers_sample);
#pragma endregion

#pragma region Initializer list

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

void initializer_list_sample()
{
	InitializerlistAverage avg({ 1, 2, 3, 4 });
	cout << "Average({1,2,3,4}) = " << avg << endl;

	auto list = { 1.0, 2.0, 3.0, 5.0, 7.0, 11.0, 13.0, 17.0, 19.0 };
	cout << "Average(" << list << ") = " << InitializerlistAverage(list) << endl;
}

REGISTER_SAMPLE("Initializer List", initializer_list_sample);
#pragma endregion



#pragma region Range-based for-loop 

void range_based_loop_sample()
{
	vector<string> names = { "Alon"s, "Erez"s, "Ariel"s, "Shay"s, "Mor"s, "Liat"s };
	for (auto name : names)
	{
		cout << "My name is " + name << endl;
	}
}

REGISTER_SAMPLE("Range-based for-loop", range_based_loop_sample);
#pragma endregion


#pragma region user defined literals

// used as conversion
constexpr long double operator"" _deg(long double deg)
{
	return deg*3.141592 / 180;
}

// used with custom type
struct mytype
{
	mytype(unsigned long long m) :m(m) {}
	unsigned long long m;
};

mytype operator"" _mytype(unsigned long long n)
{
	return mytype(n);
}

// used for side-effects
void operator"" _print(const char* str)
{
	std::cout << str;
}

void user_defined_literals()
{
	double x = 90.0_deg;
	std::cout << std::fixed << x << '\n';
	mytype y = 123_mytype;
	std::cout << y.m << '\n';
	0x123ABC_print;
}

REGISTER_SAMPLE("user defined literals", user_defined_literals);
#pragma endregion