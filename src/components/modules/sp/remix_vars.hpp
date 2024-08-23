#pragma once

namespace components::sp
{
	class remix_vars : public component
	{
	public:
		remix_vars();
		~remix_vars() = default;
		const char* get_name() override { return "remix_vars_sp"; }

		static inline remix_vars* p_this = nullptr;
		static remix_vars* get() { return p_this; }

		enum INTERPOLATE_TYPE
		{
			INTERPOLATE_TYPE_LINEAR = 0,
			INTERPOLATE_TYPE_SMOOTH = 1,
			INTERPOLATE_TYPE_PROGRESSIVE = 2,
		};

		enum OPTION_TYPE : uint8_t
		{
			OPTION_TYPE_BOOL,
			OPTION_TYPE_INT,
			OPTION_TYPE_FLOAT,
			OPTION_TYPE_VEC2,
			OPTION_TYPE_VEC3,
			OPTION_TYPE_NONE,
		};

		union option_value
		{
			bool enabled;
			int integer;
			float value;
			float vector[4];
		};

		struct option_s
		{
			
			option_s(const OPTION_TYPE& _type, const option_value& _current)
			{
				current = _current;
				reset = current;
				reset_level = current;
				type = _type;
				not_a_remix_var = false;
				modified = false;
			}

			option_s()
			{
				current = { false };
				reset = current;
				reset_level = current;
				type = OPTION_TYPE_NONE;
				not_a_remix_var = false;
				modified = false;
			}

			option_value current;
			option_value reset;
			option_value reset_level;
			OPTION_TYPE type;
			bool not_a_remix_var;
			bool modified;
		};

		typedef std::pair<const std::string, option_s>* option_handle;
		static inline std::unordered_map<std::string, option_s> options;
		static inline std::unordered_map<std::string, option_s> custom_options;

		static option_handle	add_custom_option(const std::string& name, const option_s& o);
		static option_handle	get_custom_option(const char* o);
		static option_handle	get_custom_option(const std::string& o);

		static option_handle	get_option(const char*);
		static option_handle	get_option(const std::string& o);
		static bool				set_option(option_handle o, const option_value& v, bool is_level_setting = false);
		static bool				reset_option(option_handle o, bool reset_to_level_state = false);
		static void				reset_all_modified(bool reset_to_level_state = false);
		static option_value		string_to_option_value(OPTION_TYPE type, const std::string& str);
		static option_s			string_to_option(const std::string& str);
		static void				parse_rtx_options();
		static void				parse_and_apply_conf_with_lerp(const std::string& conf_name, INTERPOLATE_TYPE style, float duration_or_speed);

		struct interpolate_entry_s
		{
			option_handle option;
			option_value start;
			option_value goal;
			OPTION_TYPE type;
			INTERPOLATE_TYPE style;
			float interpolation_speed;
			std::uint32_t time_start;
			std::uint32_t duration;
			bool complete;
		};

		static inline std::vector<interpolate_entry_s> interpolate_stack;

		static bool add_linear_interpolate_entry(option_handle handle, const option_value& goal, std::uint32_t time_start, float duration, const std::string& remix_var_name = "");
		static bool add_smooth_interpolate_entry(option_handle handle, const option_value& goal, std::uint32_t time_start, float duration, const std::string& remix_var_name = "");
		static bool add_progressive_interpolate_entry(option_handle handle, const option_value& goal, float speed, const std::string& remix_var_name = "");

	private:
		bool add_interpolate_entry(option_handle handle, const option_value& goal, std::uint32_t time_start, std::uint32_t duration, float interpolation_speed, INTERPOLATE_TYPE style, const std::string& remix_var_name = "");
	};
}
