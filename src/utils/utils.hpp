#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define M_PI				3.14159265358979323846f

namespace utils
{
	#define VECTOR_SUBTRACT(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])

	std::uint64_t fnv1a_hash(const std::string& str);

	int try_stoi(const std::string& str, const int& default_return_val = 0);
	float try_stof(const std::string& str, const float& default_return_val = 0.0f);

	std::string split_string_between_delims(const std::string& str, const char delim_start, const char delim_end);

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

	bool has_matching_symbols(const std::string& str, char opening_symbol = '(', char closing_symbol = ')', bool single_only = false);

	bool starts_with(std::string_view haystack, std::string_view needle);
	bool string_contains(const std::string_view& s1, const std::string_view s2);
	void replace_all(std::string& source, const std::string_view& from, const std::string_view& to);
	bool erase_substring(std::string& base, const std::string& replace);
	std::string str_to_lower(std::string input);
	std::string convert_wstring(const std::wstring& wstr);
	std::string& ltrim(std::string& s);
	std::string& rtrim(std::string& s);
	std::string& trim(std::string& s);
	int	q_stricmpn(const char* s1, const char* s2, int n);
	const char* va(const char* fmt, ...);
	void extract_integer_words(const std::string_view& str, std::vector<int>& integers, bool check_for_duplicates);

	void axis_to_angles(game::vec3_t axis[3], game::vec3_t angles);
	void axis_transpose(const float(*in)[3], float(*out)[3]);
	void unit_quat_to_axis(const float* quat, float(*axis)[3]);

	void copy(const game::vec_t* in, game::vec_t* out, int size);
	void angle_vectors(const game::vec3_t angles, game::vec3_t forward, game::vec3_t right, game::vec3_t up);

	game::vec_t	length(const game::vec3_t v);
	game::vec_t	distance(const game::vec3_t p1, const game::vec3_t p2);
	void scale3(const game::vec3_t v1, float scalar, game::vec3_t out);

	void vinterp_to(float* output, int vector_size, const float* current, const float* target, const float delta_time, const float interpolation_speed);
	float finterp_to(const float current, const float target, const float delta_time, const float interpolation_speed);

	void byte3_pack_rgba(const float* from, unsigned char* to);
	void byte4_pack_rgba(const float* from, char* to);

	namespace fs
	{
		bool open_file_homepath(const std::string& sub_dir, const std::string& file_name, bool print, std::ifstream& file);
	}

	template <typename T>
	class concurrent_list final
	{
	public:
		class element final
		{
		public:
			explicit element(std::recursive_mutex* mutex, std::shared_ptr<T> entry = {},
				std::shared_ptr<element> next = {}) :
				mutex_(mutex),
				entry_(std::move(entry)),
				next_(std::move(next))
			{
			}

			void remove(const std::shared_ptr<T>& element)
			{
				std::lock_guard _(*this->mutex_);
				if (!this->next_) return;

				if (this->next_->entry_.get() == element.get())
				{
					this->next_ = this->next_->next_;
				}
				else
				{
					this->next_->remove(element);
				}
			}

			std::shared_ptr<element> get_next() const
			{
				std::lock_guard _(*this->mutex_);
				return this->next_;
			}

			std::shared_ptr<T> operator*() const
			{
				std::lock_guard _(*this->mutex_);
				return this->entry_;
			}

			element& operator++()
			{
				std::lock_guard _(*this->mutex_);
				*this = this->next_ ? *this->next_ : element(this->mutex_);
				return *this;
			}

			element operator++(int)
			{
				std::lock_guard _(*this->mutex_);
				auto result = *this;
				this->operator++();
				return result;
			}

			bool operator==(const element& other)
			{
				std::lock_guard _(*this->mutex_);
				return this->entry_.get() == other.entry_.get();
			}

			bool operator!=(const element& other)
			{
				std::lock_guard _(*this->mutex_);
				return !(*this == other);
			}

		private:
			std::recursive_mutex* mutex_;
			std::shared_ptr<T> entry_;
			std::shared_ptr<element> next_;
		};

		element begin()
		{
			std::lock_guard _(this->mutex_);
			return this->entry_ ? *this->entry_ : this->end();
		}

		element end()
		{
			std::lock_guard _(this->mutex_);
			return element(&this->mutex_);
		}

		void remove(const element& entry)
		{
			std::lock_guard _(this->mutex_);
			this->remove(*entry);
		}

		void remove(const std::shared_ptr<T>& element)
		{
			std::lock_guard _(this->mutex_);
			if (!this->entry_) return;

			if ((**this->entry_).get() == element.get())
			{
				this->entry_ = this->entry_->get_next();
			}
			else
			{
				this->entry_->remove(element);
			}
		}

		void add(const T& object)
		{
			std::lock_guard _(this->mutex_);

			const auto object_ptr = std::make_shared<T>(object);
			this->entry_ = std::make_shared<element>(&this->mutex_, object_ptr, this->entry_);
		}

		void clear()
		{
			std::lock_guard _(this->mutex_);
			this->entry_ = {};
		}

	private:
		std::recursive_mutex mutex_;
		std::shared_ptr<element> entry_;
	};
}
