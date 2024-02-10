#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define M_PI				3.14159265358979323846f

namespace utils
{
	#define VECTOR_SUBTRACT(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])

	int try_stoi(const std::string& str, const int& default_return_val = 0);
	float try_stof(const std::string& str, const float& default_return_val = 0.0f);

	static std::vector<std::string> split(const std::string& raw_input, const std::vector<char>& delims)
	{
		std::vector<std::string> strings;

		auto find_first_delim = [](const std::string_view& input, const std::vector<char>& delims) -> std::pair<char, std::size_t>
			{
				auto first_delim = 0;
				auto first_delim_index = static_cast<std::size_t>(-1);
				auto index = 0u;

				for (auto& delim : delims)
				{
					if ((index = input.find(delim)) != std::string::npos)
					{
						if (first_delim_index == -1 || index < first_delim_index)
						{
							first_delim = delim;
							first_delim_index = index;
						}
					}
				}

				return { (char)first_delim, first_delim_index };
			};

		std::string input = raw_input;

		while (!input.empty())
		{
			const auto split_delim = find_first_delim(input, delims);
			if (split_delim.first != 0)
			{
				strings.push_back(input.substr(0, split_delim.second));
				input = input.substr(split_delim.second + 1);
			}
			else
			{
				break;
			}
		}

		strings.push_back(input);
		return strings;
	}

	static std::vector<std::string> split(std::string& str, char delimiter)
	{
		return split(str, std::vector < char >({ delimiter }));
	}

	static std::vector<std::string> split(const std::string& str, char delimiter)
	{
		return split(str, std::vector < char >({ delimiter }));
	}

	bool starts_with(std::string_view haystack, std::string_view needle);
	bool string_contains(const std::string_view& s1, const std::string_view s2);
	void replace_all(std::string& source, const std::string_view& from, const std::string_view& to);
	bool erase_substring(std::string& base, const std::string& replace);
	std::string str_to_lower(std::string input);
	std::string convert_wstring(const std::wstring& wstr);
	int	q_stricmpn(const char* s1, const char* s2, int n);
	const char* va(const char* fmt, ...);
	void extract_integer_words(const std::string_view& str, std::vector<int>& integers, bool check_for_duplicates);

	void axis_to_angles(game::vec3_t axis[3], game::vec3_t angles);
	void axis_transpose(const float(*in)[3], float(*out)[3]);
	void unit_quat_to_axis(const float* quat, float(*axis)[3]);

	game::vec_t	length(const game::vec3_t v);
	game::vec_t	distance(const game::vec3_t p1, const game::vec3_t p2);

	void byte3_pack_rgba(const float* from, unsigned char* to);
	void byte4_pack_rgba(const float* from, char* to);

	namespace fs
	{
		bool open_file_homepath(const std::string& sub_dir, const std::string& file_name, bool print, std::ifstream& file);
	}
}
