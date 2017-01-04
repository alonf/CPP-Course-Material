// ThreadSafeStrTok.cpp : Defines the entry point for the console application.
//
#include <string>
#include <algorithm>
#include <iostream>
#include <thread>
#include <array>
#include <allocators>
using namespace std;

template<typename T>
using string_t = basic_string<T, char_traits<T>, allocator<T>>;

template<typename T>
string_t<T> StrTok(string_t<T> text, string_t<T> delimiters)
{
	thread_local string_t<T> original;
	thread_local typename string_t<T>::iterator current_offset = begin(original);
	if (!text.empty())
	{
		original = text;
		current_offset = begin(original);
	}
	
	auto word_start = find_if(current_offset, end(original),
		[&](T element)
		{
			return find(begin(delimiters), end(delimiters), element)
				== end(delimiters);
		}
	);

	current_offset = find_if(word_start, end(original),
		[&](T element)
	{
		return find(begin(delimiters), end(delimiters), element)
			!= end(delimiters);
	}
	);

	return string_t<T>(word_start, current_offset);

}

int main()
{
	function<void(string, string)> tester = [](string word, string delimiter)
	{
		word = StrTok(word, delimiter);
		cout << "thread id:" << this_thread::get_id() << endl;
		while (!word.empty())
		{
			{
				cout << word << "-";
			}
			word = StrTok(string(), delimiter);
		}
	};

	array<thread, 10> threads;

	array<tuple<string,string>, 10> data =
	{
		make_tuple("A,B,C"s, ","s),
		make_tuple(",,....--"s, ",.-"s),
		make_tuple("alon"s,""s),
		make_tuple(""s,""s),
		make_tuple("ab,cd,x"s,","s),
		make_tuple("A|B|CD|"s,"|||"s),
		make_tuple(""s,"abc"s),
		make_tuple("lskfjlskfjsldkgjsldf"s,","s),
		make_tuple("@#$%^&*()"s,"$"s),
		make_tuple("A","A")
	};

	for(int i = 0; i < threads.size(); ++i)
	{
		auto t(thread([&]()
		{
			tester(get<0>(data[i]), get<1>(data[i]));
			//apply(tester, data[i]);
		}));
		threads[i] = move(t);
	}
	for (thread &x : threads)
		x.join();

	

    return 0;
}

