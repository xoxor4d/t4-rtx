#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

namespace utils
{
	bool starts_with(std::string haystack, std::string needle);
	std::string str_to_lower(std::string input);
	std::string convert_wstring(const std::wstring& wstr);
}
