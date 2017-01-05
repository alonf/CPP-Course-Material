// ThreadSafeStrTok.cpp : Defines the entry point for the console application.
//
#include <string>
#include <algorithm>
#include <iostream>
#include <thread>
#include <array>
#include <sstream>
#include <mutex>

using namespace std;

template<typename T>
using string_t = basic_string<T, char_traits<T>, allocator<T>>;

template<typename T>
string_t<T> StrTok(const string_t<T> &text, const string_t<T> &delimiters)
{
	typedef string_t<T> str_t;
	typedef typename string_t<T>::iterator iter_t;

	thread_local str_t original;
	thread_local iter_t current_offset = begin(original);
	
	//Start new string 
	if (!text.empty())
	{
		original = text;
		current_offset = begin(original);
	}
	
	//Internal function to find the first character that
	//appears after the delimeter, or the last character 
	//before the delimiter
	function<iter_t(iter_t, bool)> find_first = 
		[&](iter_t start, bool is_before_delimiters)
	{
		start = find_if(start, end(original),
			[&](T element)
		{
			auto f = find(begin(delimiters), end(delimiters), element);
			return is_before_delimiters ?
				f != end(delimiters) : 
				f == end(delimiters);
		});
		return start;
	};

	auto word_start = find_first(current_offset, false);
	current_offset = find_first(word_start, true);

	return str_t(word_start, current_offset);
}

int main()
{
	mutex cout_mutex;
	function<void(string, string)> tester = [&cout_mutex](string word, string delimiter)
	{
		stringstream result;
		word = StrTok(word, delimiter);
		result << "thread id:" << hex << this_thread::get_id() << ":\t|";
		while (!word.empty())
		{
			{
				result << word << "|";
			}
			word = StrTok(string(), delimiter);
		}
		{
			lock_guard<mutex> lock(cout_mutex);
			cout << result.str() << endl;
		}
	};

	array<thread, 10> threads;

	array<tuple<string,string>, 10> data =
	{
		make_tuple("A,B,C", ","),
		make_tuple(",,....--", ",.-"),
		make_tuple("alon",""),
		make_tuple("",""),
		make_tuple("ab,cd,x",","),
		make_tuple("A|B|CD|","|||"),
		make_tuple("","abc"),
		make_tuple("lskfjlskfjsldkgjsldf",","),
		make_tuple("@#$%^&*()","$"),
		make_tuple("A","A")
	};

	for(int i = 0; i < threads.size(); ++i)
	{
		auto t(thread([&data,&tester](int x)
		{
			tester(get<0>(data[x]), get<1>(data[x]));
			//apply(tester, data[i]);
		},i));
		threads[i] = move(t);
	}
	for (thread &x : threads)
		x.join();

    return 0;
}

