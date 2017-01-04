// CPP11VS2015.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <tuple>
#include <functional>
#include <algorithm>
#include <thread>
#include "register_sample.h"

using namespace std;

//Make GCC take NULL as C++ 2003
#ifdef __GNUG__
#undef NULL
#define NULL 0
#endif

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

#pragma region nullptr
void nullptr_overload(int i)
{
	cout << "int" << endl;
}

void nullptr_overload(int *p)
{
	cout << "int *" << endl;
}

void nullptr_sample()
{
	nullptr_overload(0);
	nullptr_overload(NULL);
	nullptr_overload(nullptr);
}
REGISTER_SAMPLE("nullptr", nullptr_sample);
#pragma endregion


#pragma region explicit operator 

class Op
{
private:
	int _i;

public:
	explicit Op(int i) : _i(i)
	{
	}

	Op(string i) : _i(stoi(i))
	{
	}

	explicit operator int() { return _i; }
	operator string() { return to_string(_i); }

	int get_value() const { return _i; }
};

void UseOp(const Op &op)
{
	cout << op.get_value() << endl;
}

void explicit_operator_sample()
{
	//explicit:
	Op o(42);
	int i = static_cast<int>(o);
	UseOp(Op(i));
	UseOp(static_cast<Op>(42));

	//implicit
	//int x = o; //error
	string s = o; //ok
				  //UseOp(42); //error
	UseOp(string("42")); //ok

}

REGISTER_SAMPLE("Explicit Operator", explicit_operator_sample);
#pragma endregion


#pragma region enum

void enum_sample()
{
	enum old_enum
	{
		old_value
	};

	enum newer_enum : unsigned long long
	{
		newer_value = ~0ULL
	};

	cout << "Old size: " << sizeof(old_value) << endl;
	cout << "Newer size: " << sizeof(newer_value) << endl;

	cout << "Enum scope: " << static_cast<unsigned long long>(newer_enum::newer_value) << endl;
}
REGISTER_SAMPLE("Strongly type enums & backward old enum compatibility", enum_sample);


enum forward_declaration : unsigned char;
void foo(forward_declaration f);

enum forward_declaration : unsigned char
{
	val1 = 1,
	val2 = 2
};

void use_forward_declaration_enum(forward_declaration f)
{
	cout << f << endl;
}

void forward_declaration_enum_sample()
{
	use_forward_declaration_enum(forward_declaration::val2);
}
REGISTER_SAMPLE("forward declaration enum", forward_declaration_enum_sample);


enum class scope1
{
	value1 = 1,
	value2 = 2
};

enum class scope2
{
	value1 = 10,
	value2 = 20
};

ostream &operator<<(ostream &os, scope1 value)
{
	os << "Scope1: " << static_cast<int>(value) << ' ';
	return os;
}

ostream &operator<<(ostream &os, scope2 value)
{
	os << "Scope2: " << static_cast<int>(value) << ' ';
	return os;
}

void scope_enum_sample()
{
	cout << scope1::value1 << scope2::value2 << endl;
	// cout << value1 << value2 << endl; //<== Error
}
REGISTER_SAMPLE("scope enum sample", scope_enum_sample);

#pragma endregion

#pragma region exception_ptr


void execute_on_another_thread(function<void()> background_task)
{
	exception_ptr exp_ptr;
	thread t([&]()
	{
		try
		{
			background_task();
		}
		catch (...)
		{
			exp_ptr = current_exception();
		}
	});

	t.join();

	if (exp_ptr == exception_ptr())
		return;

	rethrow_exception(exp_ptr);
}

void exception_ptr_sample()
{
	string error;

	try
	{
		execute_on_another_thread([&]
		{
			stringstream msg;
			msg << "Error in thread id: " << this_thread::get_id();
			error = msg.str();
			throw runtime_error(error);
		});
	}
	catch (exception &e)
	{
		cout << "Current thread id: " << this_thread::get_id() << endl;
		cout << e.what() << endl;
	}
}
REGISTER_SAMPLE("Throwing exception between threads", exception_ptr_sample);
#pragma endregion


#pragma region noexcept

void Do() noexcept(false)
{
	throw "error...";
}
void Dont() noexcept {}

void noexcept_sample()
{
	cout << boolalpha << "noexcept(Do()):" << noexcept(Do()) << endl;
	cout << boolalpha << "noexcept(Dont()):" << noexcept(Dont()) << endl;
}

REGISTER_SAMPLE("noexcept", noexcept_sample);
#pragma endregion

#pragma region noexcept termination

void ExceptionFunction() noexcept
{
	Do();
}

REGISTER_SAMPLE("noexcept termination", ExceptionFunction);
#pragma endregion


#pragma region thread_local

thread_local int local = 0;
int global = 0;

void thread_local_sample()
{
	vector<thread> threadVector;

	for (int i = 0; i < 10; ++i)
	{
		auto t = thread([i]
		{
			++global;
			++local;
			stringstream text;
			text << "Hello from thread number " << i << " with thread id " << this_thread::get_id() << "  global: " << global << " local: " << local << endl;
			cout << text.str();
			this_thread::sleep_for(chrono::seconds(1));
		});
		threadVector.push_back(move(t));
	}

	for (auto &t : threadVector)
	{
		t.join();
	}
	cout << "main() thread says bye bye!" << endl;
}

REGISTER_SAMPLE("thread_local", thread_local_sample);
#pragma endregion


#pragma region alignof and alignas

struct Foo { int f2;    float f1;    char c; };
class Empty {};

// every object of type struct sse_t will be aligned to 16-byte boundary
struct Sse_t
{
	alignas(16) float sse_data[4];
};

// every object of type struct data will be aligned to 128-byte boundary
struct Data {
	char x;
	alignas(128) char cacheline[128]; // over-aligned array of char, 
									  // not array of over-aligned chars
};


void alignof_and_alignas_sample()
{
	cout << "alignment of empty class: " << alignof(Empty) << endl;
	cout << "alignment of pointer : " << alignof(int*) << endl;
	cout << "alignment of char : " << alignof(char) << endl;
	cout << "alignment of Foo : " << alignof(Foo) << endl;
	cout << "alignment of sse_t : " << alignof(Sse_t) << endl;
	cout << "sizeof Data: " << sizeof(Data) << endl;

}

REGISTER_SAMPLE("alignof and alignas", alignof_and_alignas_sample);
#pragma endregion


#pragma region non restricted union

class NonTrivial
{
private:
	string m_s;
public:
	NonTrivial(string s) : m_s(s) {}
	void Print() const { cout << m_s << endl; }
};

union NonRestricted
{
	NonRestricted(string s) : m_nonTrivial(s) {}
	NonRestricted(int x) : m_c(x) {}
	~NonRestricted() {} //has to have it, it is a deleted function
	NonTrivial m_nonTrivial;
	char m_c;
};

void non_restricted_union()
{
	NonRestricted u1(string("Test"));
	u1.m_nonTrivial.Print();
	u1.m_c = 10; //very dangerous
	u1.m_nonTrivial.Print();
	NonRestricted u2(65);
	cout << u2.m_c << endl;
}

REGISTER_SAMPLE("non restricted union", non_restricted_union);
#pragma endregion


//TODO: Inline namespaces

#if _MSC_VER >= 1910 
#include <variant>
#include <optional>
#include <any>

void varaint_sample()
{
	variant<int, float> v, w;
	v = 12; // v contains int
	int i = std::get<int>(v);
	cout << i << endl;

	w = std::get<int>(v);
	cout << std::get<int>(w) << endl;
	w = std::get<0>(v); // same effect as the previous line
	w = v; // same effect as the previous line
	//  std::get<double>(v); // error: no double in [int, float]
	//  std::get<3>(v);      // error: valid index values are 0 and 1
	try
	{
		std::get<float>(w); // w contains int, not float: will throw
	}
	catch (std::bad_variant_access& ex)
	{
		cerr << ex.what() << endl;
	}

	vector<variant<int, string>> vv = { "A", 1, "B", 2, "C", 3 };
	for (auto &&e : vv)
	{
		visit([](auto &&arg) { cout << arg << ","; }, e);
	}
	cout << endl;

	class Selector
	{
	public:
		void operator()(int i) const { cout << "Int: " << i << endl; } 
		void operator()(const string &s) const { cout << "String: " << s << endl; } 
	};

	for (auto &&e : vv)
	{
		visit(Selector(), e);
	}
}
REGISTER_SAMPLE("variant", varaint_sample);

void any_sample()
{    
	any value {42};// int 42
	cout << any_cast<int>(value) << endl;
	value = 3.14;// double 3.14
	cout << any_cast<double>(value) << endl;
	value = "Alon"s;// string "Alon"
	cout << any_cast<string>(value) << endl;
	
	if(value.has_value()) 
	{ 
		if (value.type() == typeid(std::string)) 
		{
			std::cout << "value=" << std::any_cast<std::string>(value) << std::endl;
		}
	}

	try
	{
		cout << any_cast<double>(value) << endl; // value contains string, not double: will throw
	}
	catch (std::bad_any_cast& ex)
	{
		cerr << ex.what() << endl;
	}
}
REGISTER_SAMPLE("any", any_sample);



void print_value(optional<string> v)
{
	if (v.has_value())
	{
		cout << "The value is " << v.value() << endl;
	}
	else
	{
		cout << "The value is empty" << endl;
	}

	cout << "The value is " << v.value_or("empty") << endl;
}

void optional_sample()
{
	optional<string> v;
	print_value(v);
	v = "Alon"s;
	print_value(v);
	v = nullopt;
	print_value(v);
	print_value("Liat"s);
	print_value({});
	
}
REGISTER_SAMPLE("optional", optional_sample);
#endif

