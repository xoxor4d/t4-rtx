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
				modified = false;
			}

			option_s()
			{
				current = { false };
				reset = current;
				reset_level = current;
				type = OPTION_TYPE_NONE;
				modified = false;
			}

			option_value current;
			option_value reset;
			option_value reset_level;
			OPTION_TYPE type;
			bool modified;
		};

		typedef std::pair<const std::string, option_s>* option_handle;
		static inline std::unordered_map<std::string, option_s> options;

		option_handle	get_option(const char*);
		option_handle	get_option(const std::string& o);
		bool			set_option(option_handle o, const option_value& v, bool is_level_setting = false);
		bool			reset_option(option_handle o, bool reset_to_level_state = false);
		void			reset_all_modified(bool reset_to_level_state = false);
		option_value	string_to_option_value(OPTION_TYPE type, const std::string& str);
		option_s		string_to_option(const std::string& str);
		void			parse_rtx_options();


		/*union interpolate_value
		{
			int integer;
			float value;
			float vector[4];
		};*/

		struct interpolate_entry_s
		{
			option_handle option;
			option_value goal;
			OPTION_TYPE type;
			float interpolation_speed;
			bool complete;
		};

		static inline std::vector<interpolate_entry_s> interpolate_stack;

		bool add_interpolate_entry(option_handle handle, const option_value& goal, float interpolation_speed, const std::string& remix_var_name = "");
	};
}
