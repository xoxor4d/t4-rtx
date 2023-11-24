#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

namespace utils
{
	bool starts_with(std::string haystack, std::string needle);
	bool string_contains(const std::string& s1, const std::string s2);
	std::string str_to_lower(std::string input);
	std::string convert_wstring(const std::wstring& wstr);
}
