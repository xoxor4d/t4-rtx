#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

namespace utils
{
	bool starts_with(std::string haystack, std::string needle);
}
