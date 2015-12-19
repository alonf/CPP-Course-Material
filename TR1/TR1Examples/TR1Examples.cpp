// TR1Examples.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <string>
#include <sstream>
#include <regex>
#include <type_traits>
#include <random>
#include <array>
#include <Windows.h>



using namespace std;
using namespace std::placeholders;

#include "TR1Examples.h"

typedef tuple<shared_ptr<const string>, function<void ()>> sample_entry;
vector<sample_entry> samples;

int register_sample::n_entry = 1;

register_sample::register_sample(const char *name, function<void ()> f)
{
	stringstream s;
	s << n_entry++ << ". " << name;

	shared_ptr<const string> s_name(new string(s.str())); //TR1
	//auto s_name(make_shared<const string>(string(s.str()))); //C++ 11
	
	samples.push_back(sample_entry(s_name, f));
}

shared_ptr<const string> get_name(const sample_entry &entry)
{
	return get<0>(entry);
}


int main()
{
	transform(samples.begin(), samples.end(), ostream_iterator<const char *>(cout, "\n"), bind(&string::c_str, bind(get_name, _1))); 

	unsigned int sample_entry;

	while (true)
	{
		cout << endl << "Choose sample:";
		cin >> sample_entry;

		if (sample_entry == 0 || sample_entry > samples.size())
			continue;

		get<1>(samples[sample_entry - 1])();
	}

	return 0;
}

//TR1 Containers
struct Hash
{
	int operator()(int key) const
	{
		return key % 10;
	}
};

template<class T>
void InsertElementsToSet(T &collection)
{
	for (int i = 0; i < 100; ++i)
	{
		collection.insert(i);
		collection.insert(i);
	}
}

void unordered_set_usage()
{
	unordered_set<int, Hash> numbers;
	InsertElementsToSet(numbers);

	auto iter = numbers.find(50);
	
	cout << *iter;
	++iter;
	cout << ' ' << *iter << " load factor: " << numbers.load_factor()<< endl;
}
register_sample unordered_set_usage_sample("unordered_set", &unordered_set_usage);



void unordered_multiset_usage()
{
	unordered_multiset<int, Hash> numbers;
	InsertElementsToSet(numbers);

	auto iter = numbers.find(50);
	
	cout << *iter;
	++iter;
	cout << ' ' << *iter << " load factor: " << numbers.load_factor()<< endl;
}

register_sample unordered_multiset_usage_sample("unordered_multiset", &unordered_multiset_usage);


template<class T>
void InsertElementsToMap(T &collection)
{
	for (char c = 'A'; c <= 'Z'; ++c)
	{
		pair<char, int> p(c,c);
		collection.insert(p);
		collection.insert(p);
	}
}


void unordered_map_usage()
{
	unordered_map<char,int> numbers;
	InsertElementsToMap(numbers);

	auto iter = numbers.find('G');
	
	cout << '(' << iter->first << ',' << iter->second << ')';
	++iter;
	cout << " (" << iter->first << ',' << iter->second << ')' << endl;
}
register_sample unordered_map_usage_sample("unordered_map", &unordered_map_usage);


void unordered_multimap_usage()
{
	unordered_multimap<char,int> numbers;
	InsertElementsToMap(numbers);

	auto iter = numbers.find('G');
	
	cout << '(' << iter->first << ',' << iter->second << ')';
	++iter;
	cout << " (" << iter->first << ',' << iter->second << ')' << endl;
}
register_sample unordered_multimap_usage_sample("unordered_multimap", &unordered_multimap_usage);



//basic shared_ptr
void basic_shared_ptr_useage()
{
	shared_ptr<const string> sp(new string("meow"));
	//auto sp(make_shared<const string>("meow"));

	cout << *sp << endl;
	cout << sp->size() << endl;

}
register_sample basic_shared_ptr_usage_sample("basic shared_ptr usage", &basic_shared_ptr_useage);


//return shared_ptr by value
shared_ptr<int> return_by_val(int n) 
{
	shared_ptr<int> r(new int(n));
	*r += 5;
	return r;
}

void return_by_val_usage() 
{
	shared_ptr<int> p = return_by_val(3);
	cout << *p << endl;
}
register_sample return_by_val_usage_sample("return by-val shared_ptr usage", &return_by_val_usage);



//sharing ownership
void sharing_ownership()
{
	shared_ptr<int> a(new int(1));
	shared_ptr<int> b = a;
	*a += 6;
	cout << *a << ", " << *b << endl;
	a.reset();
	cout << "a: " << (a ? "owns" : "empty") << endl;
	cout << "b: " << (b ? "owns" : "empty") << endl;
	cout << *b << endl;
}
register_sample sharing_ownership_sample("sharing ownership", &sharing_ownership);


//swap
void shared_pointer_swap()
{
	shared_ptr<string> a(new string("meow")); // meow: 1
	shared_ptr<string> b(new string("purr")); // purr: 1
	cout << *a << " " << *b << endl; 
	a.swap(b);  // meow: 1, purr: 1
	cout << *a << " " << *b << endl;
	swap(a, b); // meow: 1, purr: 1
	cout << *a << " " << *b << endl;
}
register_sample shared_pointer_swap_sample("shared_pointer swap", &shared_pointer_swap);

//waek_ptr
void observe(const weak_ptr<int>& wp)
{
	shared_ptr<int> t = wp.lock();
	cout << wp.use_count() << boolalpha << " " << wp.expired() << " " << (t ? *t : 2011) << endl;
}

void weak_ptr_usage()
{
	weak_ptr<int> wp;
	{
		shared_ptr<int> sp(new int(1969));
		wp = sp;
		observe(wp);
	}
	observe(wp);
}
register_sample weak_ptr_usage_sample("weak_ptr", &weak_ptr_usage);

//unique_ptr (C++ 11)
void MoveUniquePtr(unique_ptr<string> p)
{
	cout << *p << endl;
}
void unique_ptr_usage()
{
	unique_ptr<string> up(new string("I am unique!"));
	cout << *up << endl;
	MoveUniquePtr(move(up));

	if (!up)
		cout << "No unique!" << endl;
}
register_sample unique_ptr_usage_sample("unique_ptr", &unique_ptr_usage);

//function

class point {
public:
	point(double x, double y) : m_x(x), m_y(y) { }
	double mag() const {
		return sqrt(m_x * m_x + m_y * m_y); }
	double dist(const point& p) const {
		return sqrt(pow(m_x - p.m_x, 2)
			+ pow(m_y - p.m_y, 2)); }
private:
	double m_x;
	double m_y;
};

//mem_fn(): Member Function Pointer, Reference To Object
void reference_to_object()
{
	vector<point> v;
	v.push_back(point(3, 4));
	v.push_back(point(5, 12));
	v.push_back(point(8, 15));

	transform(v.begin(), v.end(),
		ostream_iterator<double>(cout, "\n"),
		mem_fn(&point::mag));
}
register_sample reference_to_object_sample("mem_fn() - reference to object", &reference_to_object);



//mem_fn(): Member Function Pointer, pointer To Object
void pointer_to_object()
{
	vector<point> v;
	v.push_back(point(3, 4));
	v.push_back(point(5, 12));
	v.push_back(point(8, 15));

	vector<const point *> p;

	for (vector<point>::const_iterator i = v.begin();
		i != v.end(); ++i) {
		p.push_back(&*i);
	}

	transform(p.begin(), p.end(),
		ostream_iterator<double>(cout, "\n"),
		mem_fn(&point::mag));
}
register_sample pointer_to_object_sample("mem_fn() - pointer to object", &pointer_to_object);



//mem_fn(): Member Function Pointer, shared_ptr To Object
shared_ptr<point> make_point(double x, double y)
{
	shared_ptr<point> p(new point(x, y));
	return p;
}


void shared_pointer_to_object()
{
	vector<shared_ptr<point> > s;
	s.push_back(make_point(3, 4));
	s.push_back(make_point(5, 12));
	s.push_back(make_point(8, 15));

	transform(s.begin(), s.end(),
		ostream_iterator<double>(cout, "\n"),
		mem_fn(&point::mag));
}
register_sample shared_pointer_to_object_sample("mem_fn(): Member Function Pointer, shared_ptr To Object", &shared_pointer_to_object);


//mem_fn(): Pointer to data member
void pointer_to_data_member()
{
	unordered_map<string, bool> m;
	m["cat"] = true;
	m["dog"] = false;
	m["kitty"] = true;
	cout << count_if(m.begin(), m.end(),
		mem_fn(&pair<const string, bool>::second)) << endl;
}
register_sample pointer_to_data_member_sample("mem_fn(): Pointer to data member", &pointer_to_data_member);


//bind
void bind_square()
{
	vector<int> v;
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	transform(v.begin(), v.end(),
		ostream_iterator<int>(cout, "\n"),
		bind(multiplies<int>(), _1, _1));
}
register_sample bind_square_sample("bind() - Square sample", &bind_square);



//bind(): It's A Copy Machine
void bind_copy()
{
	int n = 3;
	function<int ()> f = bind(multiplies<int>(), n, 5);
	cout << f() << endl;
	++n;
	cout << f() << endl;
}

register_sample bind_copy_sample("bind() - copy sample", &bind_copy);



//bind(): cref
void bind_cref()
{
	int n = 3;
	function<int ()> f = bind(multiplies<int>(), cref(n), 5);
	cout << f() << endl;
	++n;
	cout << f() << endl;
}

register_sample bind_cref_sample("bind() - cref sample", &bind_cref);




//bind(): nested
//
//void bind_nested()
//{
//	vector<point> v;
//	v.push_back(point(3, 4));
//	v.push_back(point(5, 12));
//	v.push_back(point(8, 15));
//
//	cout << count_if(v.begin(), v.end(),
//		bind(greater<double>(), bind(&point::mag, _1), 10)
//		) << endl;
//}
//
//register_sample bind_nested_sample("bind() - nesting sample", &bind_nested);
//
//


//function: implicitly converting constructor

void meow(const function<int (int)>& f) 
{
	cout << f(1701) << ", " << f(2161) << endl;
}

int plus_four(int n) 
{ 
	return n + 4; 
}


void function_implicitly_converting_constructor()
{
	meow(plus_four);
	meow(bind(minus<int>(), _1, 1000));
}

register_sample function_implicitly_converting_constructor_sample("function: implicitly converting constructor", &function_implicitly_converting_constructor);



//function: copyable and assignable
void function_copyable_and_assignable()
{
	function<int (int)> f = plus_four;
	cout << f(343) << endl;
	f = bind(minus<int>(), _1, 100);
	cout << f(343) << endl;
	function<int (int)> g = f;
	cout << g(2401) << endl;
}

register_sample function_copyable_and_assignable_sample("function: copyable and assignable", &function_copyable_and_assignable);



//function: empty vs. full
void function_empty_vs_full()
{
	function<int (int)> f;
	if (!f) { cout << "Empty!" << endl; }
	f = plus_four;
	if (f) { cout << "Full!" << endl; }
}

register_sample function_empty_vs_full_sample("function: empty vs. full", &function_empty_vs_full);



//function: vector of functions
void rev(string& s) { reverse(s.begin(), s.end()); }

class replacer {
public:
	replacer(const string& r, const string& fmt)
		: m_p(new regex(r)), m_fmt(fmt) { }
	void operator()(string& s) const {
		s = regex_replace(s, *m_p, m_fmt);
	}
private:
	shared_ptr<regex> m_p;
	string m_fmt;
};


void function_vector_of_functions()
{
	typedef vector<function<void (string&)> > vf_t;
	typedef vf_t::const_iterator vf_ci_t;

	string s("calcium");
	vf_t v;
	v.push_back(replacer("calc", "zircon"));
	v.push_back(rev);
	v.push_back(replacer("$", "!"));
	for (vf_ci_t i = v.begin(); i != v.end(); ++i)
	{
		(*i)(s);
	}
	cout << s << endl;
}

register_sample function_vector_of_functions_sample("function: vector of functions", &function_vector_of_functions);



//regex

//regex simple matching
void regex_simple_matching()
{
	const regex r("[1-9]\\d*x[1-9]\\d*");

	for (string s; getline(cin, s); ) 
	{
		if (s == "exit")
			return;

		cout << (regex_match(s, r) ? "Yes" : "No") << endl;
	}
}

register_sample regex_simple_matching_sample("regex: simple matching", &regex_simple_matching);



//regex using match results
void regex_using_match_results()
{
	const regex r("([1-9]\\d*)x([1-9]\\d*)");

	for (string s; getline(cin, s); ) 
	{
		if (s == "exit")
			return;

		smatch m;

		if (regex_match(s, m, r)) 
		{
			cout << m[1] << " by " << m[2] << endl;
		}
	}
}

register_sample regex_using_match_results_sample("regex: using match results", &regex_using_match_results);



//regex reg_search
void regex_reg_search()
{
	const regex r("#\\s*(\\S+(\\s+\\S+)*)");

	for (string s; getline(cin, s); ) 
	{
		if (s == "exit")
			return;

		smatch m;
		if (regex_search(s, m, r)) 
		{
			cout << "*** Comment: " << m[1] << endl;
		}
	}
}

register_sample regex_reg_search_sample("regex: reg_search", &regex_reg_search);



//regex reg_replace
void regex_reg_replace()
{
	const regex r("(\\w+)( \\w+\\.?)? (\\w+)");
	const string fmt("$3, $1$2");
	for (string s; getline(cin, s); ) 
	{
		if (s == "exit")
			return;

		cout << "*** " << regex_replace(s, r, fmt) << endl;
	}
}

register_sample regex_reg_replace_sample("regex: reg_replace", &regex_reg_replace);



//regex regex_iterator
void regex_reg_iterator()
{
	const regex r("[^ ]\\w*day");
	string s("Sunday is a nice day.  Monday comes before Tuesday ");
	
	const sregex_iterator end;
	for (sregex_iterator i(s.begin(), s.end(), r); i != end; ++i) 
	{
		cout << (*i)[0] << endl;
	}
}

register_sample regex_reg_iterator_sample("regex: regex_iterator", &regex_reg_iterator);


//random uniform_int
void random_uniform_int()
{
	 mt19937 eng;  // a core engine class    
	 vector<int> v(10);

	 uniform_int_distribution<int> unif(0,9);

	 for (unsigned int i = 0; i < v.size(); ++i)
		 v[i] = 0;

	 for (int i = 0; i < 1000000; ++i)
	 {
		 auto n = unif(eng);
		 v[n]++;    
	 }

	 for (unsigned int i = 0; i < v.size(); ++i)
		 cout << v[i] << endl;
}

register_sample random_uniform_int_sample("random: uniform_int_distribution", &random_uniform_int);


//random normal_distribution
void random_normal_distribution()
{
	 mt19937 eng;  // a core engine class    
	 normal_distribution<double> dist;
	 vector<int> v(21);


	 for (unsigned int i = 0; i < v.size(); ++i)
		 v[i] = 0;

	 for (int i = 0; i < 1000000; ++i)
	 {
		 auto d = dist(eng);
		 int val = (int)(d*2) + 10;
		 v[val]++;    
	 }

	 for (unsigned int i = 0; i < v.size(); ++i)
		 cout << v[i] << endl;
}

register_sample random_normal_distribution_sample("random: normal_distribution", &random_normal_distribution);


//type traits
class TypeTraitDemoBase
{
};

class TypeTraitDemo : public TypeTraitDemoBase
{
public:
	TypeTraitDemo(int x) {}
};

template<typename Ty>
void Show()
{
	cout << typeid(Ty).name() << " is" << (is_integral<Ty>::value ? " " : " not") << " integral type." << endl;
	cout << typeid(Ty).name() << " is" << (is_floating_point<Ty>::value ? " " : " not") << " floating Point type." << endl;
	cout << typeid(Ty).name() << " is" << (is_base_of<TypeTraitDemoBase, Ty>::value ? " " : " not") << " derived from TypeTraitDemoBase." << endl;
	cout << typeid(Ty).name() << " has" <<(std::is_trivially_constructible<Ty>::value ? " " : " not") << " trivial ctor." << endl;
}

void type_traits()
{
	Show<int>();
	Show<double>();
	Show<TypeTraitDemoBase>();
	Show<TypeTraitDemo>();
}

register_sample type_traits_sample("type traits", &type_traits);



