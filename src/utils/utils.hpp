#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define M_PI				3.14159265358979323846f

namespace utils
{
	bool starts_with(std::string haystack, std::string needle);
	bool string_contains(const std::string& s1, const std::string s2);
	bool erase_substring(std::string& base, const std::string& replace);
	std::string str_to_lower(std::string input);
	std::string convert_wstring(const std::wstring& wstr);
	const char* va(const char* fmt, ...);

	void axis_to_angles(game::vec3_t axis[3], game::vec3_t angles);
	void axis_transpose(const float(*in)[3], float(*out)[3]);
	void unit_quat_to_axis(const float* quat, float(*axis)[3]);
}
