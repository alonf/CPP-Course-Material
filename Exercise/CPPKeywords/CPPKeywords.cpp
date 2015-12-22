#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <regex>
#include <iterator>

using namespace std;

int main()
{
	auto file = ifstream(__FILE__);

	if (file.bad())
		return -1;

	const regex keywords("[^0-9_A-Za-z|\"\'](alignas|alignof|auto|bool|break|case|catch|char|char16_t|char32_t|class|const|constexpr|const_cast|continue|decltype|default|delete|do|double|dynamic_cast|else|enum|explicit|export|extern|false|float|for|friend|goto|if|inline|int|long|mutable|namespace|new|noexcept|nullptr|operator|private|protected|public|register|reinterpret_cast|return|short|signed|sizeof|static|static_assert|static_cast|struct|switch|template|this|thread_local|throw|true|try|typedef|typeid|typename|union|unsigned|using|virtual|void|volatile|wchar_t|while|override|final)[^0-9_A-Za-z|\"\']");

	map<string, int> wordCount;

	string line;
	while (!getline(file, line).eof())
	{
		for (sregex_iterator i(begin(line), end(line), keywords); i != sregex_iterator(); ++i)
		{
			++wordCount[(*i)[1]];
		}
	}

	for_each(begin(wordCount), end(wordCount), [](pair<string, int> entry) { cout << entry.first << ":" << entry.second << endl; });

	return 0;
}

