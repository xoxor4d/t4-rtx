#include "std_include.hpp"
#include "remix_vars.hpp"

using namespace game::sp;

namespace components::sp
{
	// checks if str is made up of numbers only
	// ignores dot, comma, minus and whitespaces
	bool is_single_num_or_vector(const std::string& str)
	{
		return std::ranges::all_of(str.begin(), str.end(), [](const char c) 
		{
			return std::isdigit(c) || c == ',' || c == '.' || c == '-' || c == ' ';
		});
	}

	remix_vars::option_handle remix_vars::add_custom_option(const std::string& name, const option_s& o)
	{
		custom_options[name] = o;

		if (const auto it = custom_options.find(name); it != custom_options.end())
		{
			return &*it;
		}

		return nullptr;
	}

	remix_vars::option_handle remix_vars::get_custom_option(const char* o)
	{
		if (const auto it = custom_options.find(o); it != custom_options.end())
		{
			return &*it;
		}

		return nullptr;
	}

	remix_vars::option_handle remix_vars::get_custom_option(const std::string& o)
	{
		if (const auto it = custom_options.find(o); it != custom_options.end())
		{
			return &*it;
		}

		return nullptr;
	}

	/**
	 * Gets a handle of a variable from the options map
	 * @param o		remix variable name
	 * @return		handle (pointer to std::pair)
	 */
	remix_vars::option_handle remix_vars::get_option(const char* o)
	{
		if (const auto it = options.find(o); it != options.end())
		{
			return &*it;
		}

		return nullptr;
	}

	/**
	 * Gets a handle of a variable from the options map
	 * @param o		remix variable name
	 * @return		handle (pointer to std::pair)
	 */
	remix_vars::option_handle remix_vars::get_option(const std::string& o)
	{
		if (const auto it = options.find(o); it != options.end())
		{
			return &*it;
		}

		return nullptr;
	}

	/**
	 * Updates the given variable within the options map and sends it of to remix via the api
	 * @param o					handle into the options map
	 * @param v					variable will be set to this value 
	 * @param is_level_setting	update the reset_level value (used if reset_option() is called with reset_to_level_state)
	 * @return					true if successfull
	 */
	bool remix_vars::set_option(option_handle o, const option_value& v, const bool is_level_setting)
	{
		if (o && api::is_initialized())
		{
			o->second.current = v;

			if (is_level_setting)
			{
				o->second.reset_level = v;
			}

			std::string var_str;
			switch(o->second.type)
			{
			case OPTION_TYPE_BOOL:
				var_str = v.enabled ? "True" : "False";
				o->second.modified = o->second.current.enabled != o->second.reset.enabled;
				break;
			case OPTION_TYPE_INT:
				var_str = std::to_string(v.integer);
				o->second.modified = o->second.current.integer != o->second.reset.integer;
				break;
			case OPTION_TYPE_FLOAT:
				var_str = std::to_string(v.value);
				o->second.modified = o->second.current.value != o->second.reset.value;
				break;
			case OPTION_TYPE_VEC2:
				var_str = std::to_string(v.vector[0]) + ", " + std::to_string(v.vector[1]);
				o->second.modified = o->second.current.vector[0] != o->second.reset.vector[0] || o->second.current.vector[1] != o->second.reset.vector[1];
				break;
			case OPTION_TYPE_VEC3:
				var_str = std::to_string(v.vector[0]) + ", " + std::to_string(v.vector[1]) + ", " + std::to_string(v.vector[2]);
				o->second.modified = o->second.current.vector[0] != o->second.reset.vector[0] || o->second.current.vector[1] != o->second.reset.vector[1] || o->second.current.vector[2] != o->second.reset.vector[2];
				break;
			case OPTION_TYPE_NONE:
				return false;
			}

			if (!var_str.empty())
			{
				api::bridge.SetConfigVariable(o->first.c_str(), var_str.c_str());
				return true;
			}

			DEBUG_PRINT("[RTX-SET-OPTION] Skipping unknown option type %d of option %s \n", (uint32_t) o->second.type, o->first.c_str());
		}

		return false;
	}

	/**
	 * Resets a specified remix variable
	 * @param o						handle into the options map
	 * @param reset_to_level_state	\n
	 *								false => reset options to values stored in rtx.conf\n
	 *								true  => reset options to per level conf
	 * @return						
	 */
	bool remix_vars::reset_option(option_handle o, const bool reset_to_level_state)
	{
		if (o && api::is_initialized())
		{
			o->second.current = reset_to_level_state ? o->second.reset_level : o->second.reset;

			// should reset modified
			set_option(o, o->second.current);

			if (!o->second.modified)
			{
				return true;
			}

			DEBUG_PRINT("[RTX-RESET-OPTION] Failed to reset option %s \n", o->first.c_str());
		}

		return false;
	}

	/**
	 * Resets all modified remix variables
	 * @param reset_to_level_state \n
	 *		false => reset options to values stored in rtx.conf\n
	 *		true  => reset options to per level conf
	 */
	void remix_vars::reset_all_modified(const bool reset_to_level_state)
	{
		if (api::is_initialized())
		{
			auto count = 0u;
			for (auto& o : options)
			{
				if (o.second.modified)
				{
					if (reset_option(&o, reset_to_level_state))
					{
						count++;
					}
				}
			}

			DEBUG_PRINT("[RTX-RESET-ALL-OPTIONS] Reset %d options \n", count);
		}
	}

	/**
	 * Tries to convert a string to <option_value>
	 * @param type	variable type
	 * @param str	string containing the value/s
	 * @return		returns a valid <option_value> even if conversion failed 
	 */
	remix_vars::option_value remix_vars::string_to_option_value(OPTION_TYPE type, const std::string& str)
	{
		option_value out = {};

		switch (type)
		{
		case OPTION_TYPE_NONE:
		case OPTION_TYPE_BOOL:
			out.enabled = str == "True";
			break;
		case OPTION_TYPE_INT:
			out.integer = utils::try_stoi(str);
			break;
		case OPTION_TYPE_FLOAT:
			out.value = utils::try_stof(str);
			break;
		case OPTION_TYPE_VEC2:
			if (const auto v = utils::split(str, ','); v.size() == 2)
			{
				out.vector[0] = utils::try_stof(v[0]);
				out.vector[1] = utils::try_stof(v[1]);
			}
			break;
		case OPTION_TYPE_VEC3:
			if (const auto v = utils::split(str, ','); v.size() == 3)
			{
				out.vector[0] = utils::try_stof(v[0]);
				out.vector[1] = utils::try_stof(v[1]);
				out.vector[2] = utils::try_stof(v[2]);
			}
			break;
		}

		return out;
	}

	/**
	 * Tries to convert a string to <option_s>
	 * @param str	string containing the value/s
	 * @return		option_s - type NONE if conversion failed
	 */
	remix_vars::option_s remix_vars::string_to_option(const std::string& str)
	{
		option_s out = {};

		if (str == "True" || str == "False")
		{
			// is bool
			out.type = OPTION_TYPE_BOOL;
			out.current.enabled = str == "True";
		}
		else if (is_single_num_or_vector(str))
		{
			if (const auto x = utils::split(str, ','); x.size() > 1)
			{
				// is vector
				out.type = OPTION_TYPE_VEC2;
				out.current.vector[0] = utils::try_stof(x[0]);
				out.current.vector[1] = utils::try_stof(x[1]);

				if (x.size() > 2)
				{
					out.type = OPTION_TYPE_VEC3;
					out.current.vector[2] = utils::try_stof(x[2]);
				}
			}
			else
			{
				// is single float
				out.type = OPTION_TYPE_FLOAT; // treat everything as float
				out.current.value = utils::try_stof(str);
			}
		}

		out.reset = out.current;
		out.reset_level = out.current;

		return out;
	}

	/**
	 * Parses the rtx.conf in the root directory and builds an unordered map \n
	 * with pairs made of: <variable name> (std::string) and <variable value/type/...> (option_s) 
	 */
	void remix_vars::parse_rtx_options()
	{
		std::ifstream file;
		if (utils::fs::open_file_homepath("", "rtx.conf", false, file))
		{
			std::string input;
			while (std::getline(file, input))
			{
				if (auto pair = utils::split(input, '='); pair.size() == 2u)
				{
					utils::trim(pair[0]);
					utils::trim(pair[1]);

					bool is_valid_option = false;

					if (!pair[1].starts_with("0x") && !pair[1].empty())
					{
						if (const auto o = string_to_option(pair[1]); o.type != OPTION_TYPE_NONE)
						{
							options[pair[0]] = o;
							is_valid_option = true;
						}
					}

					if (!is_valid_option)
					{
						DEBUG_PRINT("[RTX-PARSE-OPTIONS] Ignored: %s \n", pair[0].c_str());
					}
				}
			}

			file.close();
		}
	}

	/**
	 * Parses a .conf within the map_configs folder lerps to contained values
	 * @param conf_name			config name without extension
	 * @param style				interpolation style
	 * @param duration_or_speed	duration in s or speed scalar if using progressive lerp
	 */
	void remix_vars::parse_and_apply_conf_with_lerp(const std::string& conf_name, INTERPOLATE_TYPE style, const float duration_or_speed)
	{
		std::ifstream file;
		if (utils::fs::open_file_homepath("t4rtx\\map_configs", conf_name + ".conf", false, file))
		{
			std::string input;
			while (std::getline(file, input))
			{
				if (utils::starts_with(input, "#") || input.empty())
				{
					continue;
				}

				if (auto pair = utils::split(input, '=');
					pair.size() == 2u)
				{
					utils::trim(pair[0]);
					utils::trim(pair[1]);

					if (pair[1].starts_with("0x") || pair[1].empty())
					{
						continue;
					}

					if (const auto o = get_option(pair[0].c_str()); o)
					{
						const auto& v = string_to_option_value(o->second.type, pair[1]);

						switch(style)
						{
						case INTERPOLATE_TYPE_LINEAR:
							add_linear_interpolate_entry(o, v, (uint32_t)game::sp::cgs->time, duration_or_speed);
							break;

						case INTERPOLATE_TYPE_SMOOTH:
							add_smooth_interpolate_entry(o, v, (uint32_t)game::sp::cgs->time, duration_or_speed);
							break;

						case INTERPOLATE_TYPE_PROGRESSIVE:
							add_progressive_interpolate_entry(o, v, duration_or_speed);
							break;
						}

						DEBUG_PRINT("[VAR-LERP] Start lerping var: %s to: %s\n", o->first.c_str(), pair[1].c_str());
					}
				}
			}

			file.close();
		}
	}


	// #
	// Interpolation

	/**
	 * Adds a remix var (option) to the interpolation stack and smoothly interpolates it
	 * @param handle				handle of remix var option in the options map (can be nullptr if 'remix_var_name' is used instead) 
	 * @param goal					transition goal
	 * @param interpolation_speed	speed at which to interpolate between start and goal
	 * @param remix_var_name		can be used to if handle = nullptr
	 * @return 
	 */
	bool remix_vars::add_interpolate_entry(option_handle handle, const option_value& goal, const std::uint32_t time_start, const std::uint32_t duration, const float interpolation_speed, INTERPOLATE_TYPE style, const std::string& remix_var_name)
	{
		option_handle h = handle;
		if (!h)
		{
			if (remix_var_name.empty())
			{
				return false;
			}

			h = remix_vars::get()->get_option(remix_var_name);
		}

		if (h)
		{
			// check if we are already interpolating the value
			bool exists = false;

			for (auto& ip : interpolate_stack)
			{
				if (ip.option == h)
				{
					// update
					ip.start = h->second.current;
					ip.goal = goal;
					ip.style = style;
					ip.interpolation_speed = interpolation_speed;
					ip.time_start = time_start;
					ip.duration = duration;

					exists = true;
					break;
				}
			}

			if (!exists)
			{
				interpolate_stack.emplace_back(interpolate_entry_s
					{ h, h->second.current ,goal, h->second.type, style, interpolation_speed, time_start, duration, false });
			}

			return true;
		}

		return false;
	}

	/**
	 * Adds a remix var (option) to the interpolation stack and linearly interpolates it
	 * @param handle				handle of remix var option in the options map (can be nullptr if 'remix_var_name' is used instead)
	 * @param goal					transition goal
	 * @param time_start			time when transition starts (usually cgs->time)
	 * @param duration				duration of transition in seconds
	 * @param remix_var_name		can be used to if handle = nullptr
	 * @return
	 */
	bool remix_vars::add_linear_interpolate_entry(option_handle handle, const option_value& goal, const std::uint32_t time_start, const float duration, const std::string& remix_var_name)
	{
		return remix_vars::get()->add_interpolate_entry(handle, goal, time_start, (uint32_t)(duration * 1000.0f), 0.0f, INTERPOLATE_TYPE_LINEAR, remix_var_name);
	}

	/**
	 * Adds a remix var (option) to the interpolation stack and smoothly interpolates it
	 * @param handle				handle of remix var option in the options map (can be nullptr if 'remix_var_name' is used instead)
	 * @param goal					transition goal
	 * @param time_start			time when transition starts (usually cgs->time)
	 * @param duration				duration of transition in seconds
	 * @param remix_var_name		can be used to if handle = nullptr
	 * @return
	 */
	bool remix_vars::add_smooth_interpolate_entry(option_handle handle, const option_value& goal, const std::uint32_t time_start, const float duration, const std::string& remix_var_name)
	{
		return remix_vars::get()->add_interpolate_entry(handle, goal, time_start, (uint32_t)(duration * 1000.0f), 0.0f, INTERPOLATE_TYPE_LINEAR, remix_var_name);
	}

	/**
	 * Adds a remix var (option) to the interpolation stack and progressivly interpolates it
	 * @param handle				handle of remix var option in the options map (can be nullptr if 'remix_var_name' is used instead)
	 * @param goal					transition goal
	 * @param speed					speed at which to interpolate between start and goal
	 * @param remix_var_name		can be used to if handle = nullptr
	 * @return
	 */
	bool remix_vars::add_progressive_interpolate_entry(option_handle handle, const option_value& goal, const float speed, const std::string& remix_var_name)
	{
		return remix_vars::get()->add_interpolate_entry(handle, goal, 0u, 0u, speed, INTERPOLATE_TYPE_PROGRESSIVE, remix_var_name);
	}

	void lerp_float(float* current, const float from, const float to, float fraction, remix_vars::INTERPOLATE_TYPE style, const float delta = 0.0f, const float speed = 0.0f)
	{
		if (current)
		{
			switch (style)
			{
			case remix_vars::INTERPOLATE_TYPE_LINEAR:
				*current = (to - from) * fraction + from;
				return;

			case remix_vars::INTERPOLATE_TYPE_SMOOTH:
				fraction = fraction * M_PI * 0.5f;
				fraction = sinf(fraction);
				*current = (to - from) * fraction + from;
				return;

			case remix_vars::INTERPOLATE_TYPE_PROGRESSIVE:
				*current = utils::finterp_to(*current, to, delta, speed);
				return;
			}

			*current = to;
		}
	}

	void remix_vars::on_map_load()
	{
		remix_vars::custom_options.clear();
		remix_vars::interpolate_stack.clear();
	}

	// Interpolates all variables on the 'interpolate_stack' and removes them once they reach their goal. \n
	// Called once per client frame (main_module::on_set_cgame_time()) (CL_Frame - after updating the delta time).
	void remix_vars::on_client_frame()
	{
		if (game::sp::clientUI->connectionState == game::CA_ACTIVE)
		{
			if (!remix_vars::interpolate_stack.empty())
			{
				// remove completed transitions - we do that in-front of the loop so that the final values (complete) can be used for the entire frame
				auto completed_condition = [](const remix_vars::interpolate_entry_s& ip)
					{
						if (ip.complete)
						{
							DEBUG_PRINT("[VAR-LERP] Complete: %s\n", ip.option->first.c_str());
						}

						return ip.complete;
					};

				const auto it = std::remove_if(remix_vars::interpolate_stack.begin(), remix_vars::interpolate_stack.end(), completed_condition);
				remix_vars::interpolate_stack.erase(it, remix_vars::interpolate_stack.end());

				// #

				const auto delta = static_cast<float>(game::sp::client->serverTimeDelta) * 0.0001f;
				const auto delta_abs = fabsf(delta);
				const auto time = game::sp::cgs->time;

				for (auto& ip : remix_vars::interpolate_stack)
				{
					const bool transition_time_exceeded = ip.time_start + ip.duration < (uint32_t)time && ip.style != remix_vars::INTERPOLATE_TYPE_PROGRESSIVE;

					const auto f = (float)(time - ip.time_start) / (float)ip.duration;
					float fraction = f;

					if ((f - 1.0f) >= 0.0f)
					{
						fraction = 1.0f;
					}

					if ((0.0f - f) >= 0.0f)
					{
						fraction = 0.0f;
					}

					switch (ip.type)
					{
						case remix_vars::OPTION_TYPE_INT:
						{
							if (!transition_time_exceeded)
							{
								float temp = (float)ip.option->second.current.integer;
								lerp_float(&temp, (float)ip.start.integer, (float)ip.goal.integer, fraction, ip.style, delta_abs, ip.interpolation_speed);
								ip.option->second.current.integer = (int)temp;

								ip.complete = ip.option->second.current.integer == ip.goal.integer;
							}
							else
							{
								ip.option->second.current.integer = ip.goal.integer;
								ip.complete = true;
							}
							break;
						}
							
						case remix_vars::OPTION_TYPE_FLOAT:
						{
							if (!transition_time_exceeded)
							{
								lerp_float(&ip.option->second.current.value, ip.start.value, ip.goal.value, fraction, ip.style, delta_abs, ip.interpolation_speed);
								ip.complete = utils::float_equal(ip.option->second.current.value, ip.goal.value);
							}
							else
							{
								ip.option->second.current.value = ip.goal.value;
								ip.complete = true;
							}
							break;
						}

						case remix_vars::OPTION_TYPE_VEC2:
						{
							if (!transition_time_exceeded)
							{
								lerp_float(&ip.option->second.current.vector[0], ip.start.vector[0], ip.goal.vector[0], fraction, ip.style, delta_abs, ip.interpolation_speed);
								lerp_float(&ip.option->second.current.vector[1], ip.start.vector[1], ip.goal.vector[1], fraction, ip.style, delta_abs, ip.interpolation_speed);
								ip.complete =  utils::float_equal(ip.option->second.current.vector[0], ip.goal.vector[0])
											&& utils::float_equal(ip.option->second.current.vector[1], ip.goal.vector[1]);
							}
							else
							{
								ip.option->second.current.vector[0] = ip.goal.vector[0];
								ip.option->second.current.vector[1] = ip.goal.vector[1];
								ip.complete = true;
							}
							break;
						}

						case remix_vars::OPTION_TYPE_VEC3:
						{
							if (!transition_time_exceeded)
							{
								lerp_float(&ip.option->second.current.vector[0], ip.start.vector[0], ip.goal.vector[0], fraction, ip.style, delta_abs, ip.interpolation_speed);
								lerp_float(&ip.option->second.current.vector[1], ip.start.vector[1], ip.goal.vector[1], fraction, ip.style, delta_abs, ip.interpolation_speed);
								lerp_float(&ip.option->second.current.vector[2], ip.start.vector[2], ip.goal.vector[2], fraction, ip.style, delta_abs, ip.interpolation_speed);
								ip.complete =  utils::float_equal(ip.option->second.current.vector[0], ip.goal.vector[0])
											&& utils::float_equal(ip.option->second.current.vector[1], ip.goal.vector[1])
											&& utils::float_equal(ip.option->second.current.vector[2], ip.goal.vector[2]);
							}
							else
							{
								ip.option->second.current.vector[0] = ip.goal.vector[0];
								ip.option->second.current.vector[1] = ip.goal.vector[1];
								ip.option->second.current.vector[2] = ip.goal.vector[2];
								ip.complete = true;
							}
							break;
						}

						case remix_vars::OPTION_TYPE_BOOL:
						{
							ip.option->second.current.enabled = ip.goal.enabled;
							ip.complete = true;
							break;
						}

						case remix_vars::OPTION_TYPE_NONE:
							ip.complete = true; // remove none type
							continue;
					}

					if (!ip.option->second.not_a_remix_var)
					{
						remix_vars::get()->set_option(ip.option, ip.option->second.current, false);
					}
				}
			}
		}
	}

	remix_vars::remix_vars()
	{
		p_this = this;

		scheduler::once([]
		{
			remix_vars::get()->parse_rtx_options();
		}, scheduler::main);

		// --------

//#if DEBUG
		command::add("rtx_parse_options", [this](const command::params&)
		{
			if (game::is_sp)
			{
				options.clear();
				custom_options.clear();
				remix_vars::parse_rtx_options();
			}
		});

		command::add("rtx_reset_option", [this](command::params p)
		{
			if (game::is_sp)
			{
				if (p.length() > 1)
				{
					remix_vars::reset_option(get_option(p[1]));
				}
			}
		});

		command::add("rtx_reset_all_options", [this](const command::params&)
		{
			if (game::is_sp)
			{
				remix_vars::reset_all_modified(false);
			}
		});
//#endif

		command::add("rtx_set_option", [this](command::params p)
		{
			if (game::is_sp)
			{
				bool valid_value = false;

				if (p.length() > 2)
				{
					const auto type = std::string_view(p[2]);
					const auto val0 = std::string_view(p[3]);
					auto v = option_value {};

					if (type.contains("bool"))
					{
						v.enabled = val0.contains("true") || val0.contains("1");
						valid_value = true;
					}
					else if (type.contains("int"))
					{
						v.integer = utils::try_stoi(p[3]);
						valid_value = true;
					}
					else if (type.contains("float"))
					{
						v.value = utils::try_stof(p[3]);
						valid_value = true;
					}
					else if (type.contains("vec2"))
					{
						v.vector[0] = utils::try_stof(p[3]);
						v.vector[1] = utils::try_stof(p[4]);
						valid_value = true;
					}
					else if (type.contains("vec3"))
					{
						v.vector[0] = utils::try_stof(p[3]);
						v.vector[1] = utils::try_stof(p[4]);
						v.vector[2] = utils::try_stof(p[5]);
						valid_value = true;
					}

					if (valid_value)
					{
						if (!remix_vars::set_option(get_option(p[1]), v))
						{
							game::sp::Com_PrintMessage(0, "rtx_set_option: option not found! \n", 0);
						}
					}
				}

				if (p.length() <= 2 || !valid_value)
				{
					game::sp::Com_PrintMessage(0, "rtx_set_option <option name> <type> (bool|int|float|vec2|vec3) <value/s> (true|false|0|1|2.5|1 2 3) \n", 0);
				}
			}
		});

		command::add("rtx_clear_transitions", [this](command::params)
		{
			if (game::is_sp)
			{
				interpolate_stack.clear();
			}
		});

		command::add("rtx_transition", [this](command::params p)
		{
			if (game::is_sp)
			{
				bool valid_value = false;

				// idx	- size	- arg
				// 0	- 1		- cmd name
				// 1	- 2		- name
				// 2	- 3		- type
				// 3	- 4		- style
				// 4	- 5		- speed
				// 5	- 6		- goal x
				// 6	- 7		- goal y
				// 7	- 8		- goal z

				if (p.length() >= 5)
				{
					const int START_VALUE_IDX = 5;
					const auto type = std::string_view(p[2]);

					auto style = (INTERPOLATE_TYPE) utils::try_stoi(p[3], 1);
					style = style < INTERPOLATE_TYPE_LINEAR ? INTERPOLATE_TYPE_SMOOTH : style > INTERPOLATE_TYPE_PROGRESSIVE ? INTERPOLATE_TYPE_SMOOTH : style;

					const auto speed = utils::try_stof(p[4], 1.0f);
					auto v = option_value{};


					if (type.contains("int"))
					{
						v.integer = utils::try_stoi(p[START_VALUE_IDX + 0]);
						valid_value = true;
					}
					else if (type.contains("float"))
					{
						v.value = utils::try_stof(p[START_VALUE_IDX + 0]);
						valid_value = true;
					}
					else if (type.contains("vec2"))
					{
						v.vector[0] = utils::try_stof(p[START_VALUE_IDX + 0]);
						v.vector[1] = utils::try_stof(p[START_VALUE_IDX + 1]);
						valid_value = true;
					}
					else if (type.contains("vec3"))
					{
						v.vector[0] = utils::try_stof(p[START_VALUE_IDX + 0]);
						v.vector[1] = utils::try_stof(p[START_VALUE_IDX + 1]);
						v.vector[2] = utils::try_stof(p[START_VALUE_IDX + 2]);
						valid_value = true;
					}

					if (valid_value)
					{
						bool valid = false;
						switch (style)
						{
						case INTERPOLATE_TYPE_LINEAR:
							valid = remix_vars::get()->add_linear_interpolate_entry(nullptr, v, (uint32_t)game::sp::cgs->time, speed, p[1]);
							break;

						case INTERPOLATE_TYPE_SMOOTH:
							valid = remix_vars::get()->add_smooth_interpolate_entry(nullptr, v, (uint32_t)game::sp::cgs->time, speed, p[1]);
							break;

						case INTERPOLATE_TYPE_PROGRESSIVE:
							valid = remix_vars::get()->add_progressive_interpolate_entry(nullptr, v, speed, p[1]);
							break;
						}

						if (!valid)
						{
							game::sp::Com_PrintMessage(0, "rtx_transition: option not found! \n", 0);
						}
						else
						{
							DEBUG_PRINT("[VAR-LERP] Start lerping rtx vision var: %s\n", p[1]);
						}
					}
				}

				if (p.length() < 6 || !valid_value)
				{
					game::sp::Com_PrintMessage(0, "rtx_transition   <option name>   <option type> (float|int|vec2|vec3)   <option style> (0:linear|1:smooth|2:progressive)   <transition speed>   <goal value/s> (true|false|0|1|2.5|1 2 3)\n", 0);
				}
			}
		});
	}
}
