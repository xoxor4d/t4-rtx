#include "std_include.hpp"

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
	 * @param is_level_setting	update the reset_level value (used if <b>reset_option()</b> is called with <b>reset_to_level_state</b>)
	 * @return					true if successfull
	 */
	bool remix_vars::set_option(option_handle o, const option_value& v, const bool is_level_setting)
	{
		if (o && api::bridge.initialized)
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
		if (o && api::bridge.initialized)
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
		if (api::bridge.initialized)
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
	 * with pairs made of: <b><variable name></b> (std::string) and <b><variable value/type/...></b> (option_s) 
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


	// #
	// Interpolation

	bool remix_vars::add_interpolate_entry(option_handle handle, const option_value& goal, const float interpolation_speed, const std::string& remix_var_name)
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
					// update goal and speed if we are
					ip.goal = goal;
					ip.interpolation_speed = interpolation_speed;

					exists = true;
					break;
				}
			}

			if (!exists)
			{
				interpolate_stack.emplace_back(interpolate_entry_s
					{ h, goal, h->second.type, interpolation_speed, false });
			}

			return true;
		}

		return false;
	}

	/**
	 * Interpolates all variables on the <b>interpolate_stack</b> and removes them once they reach their goal. \n
	 * Called once per client frame (CL_Frame - after updating the delta time).
	 */
	void on_set_cgame_time()
	{
		if (game::sp::clientUI->connectionState == game::CA_ACTIVE)
		{
			if (!remix_vars::interpolate_stack.empty())
			{
				const auto delta = static_cast<float>(game::sp::client->serverTimeDelta) * 0.0001f;
				const auto delta_abs = fabsf(delta);

				for (auto& ip : remix_vars::interpolate_stack)
				{
					switch (ip.type)
					{
						case remix_vars::OPTION_TYPE_INT:
						{
							ip.option->second.current.integer = (int)utils::finterp_to((float)ip.option->second.current.integer, (float)ip.goal.integer, delta_abs, ip.interpolation_speed);
							ip.complete = ip.option->second.current.integer == ip.goal.integer;
							break;
						}
							
						case remix_vars::OPTION_TYPE_FLOAT:
						{
							ip.option->second.current.value = utils::finterp_to(ip.option->second.current.value, ip.goal.value, delta_abs, ip.interpolation_speed);
							ip.complete = ip.option->second.current.value == ip.goal.value;
							break;
						}

						case remix_vars::OPTION_TYPE_VEC2:
						{
							utils::vinterp_to(ip.option->second.current.vector, 2, ip.option->second.current.vector, ip.goal.vector, delta_abs, ip.interpolation_speed);
							ip.complete =  ip.option->second.current.vector[0] == ip.goal.vector[0]
										&& ip.option->second.current.vector[1] == ip.goal.vector[1];
							break;
						}

						case remix_vars::OPTION_TYPE_VEC3:
						{
							utils::vinterp_to(ip.option->second.current.vector, 3, ip.option->second.current.vector, ip.goal.vector, delta_abs, ip.interpolation_speed);
							ip.complete =  ip.option->second.current.vector[0] == ip.goal.vector[0]
										&& ip.option->second.current.vector[1] == ip.goal.vector[1]
										&& ip.option->second.current.vector[2] == ip.goal.vector[2];
							break;
						}

						case remix_vars::OPTION_TYPE_BOOL:
						case remix_vars::OPTION_TYPE_NONE:
							continue;
					}

					remix_vars::get()->set_option(ip.option, ip.option->second.current, false);
				}

				auto completed_condition = [](const remix_vars::interpolate_entry_s& ip)
				{
					return ip.complete;
				};

				const auto it = std::remove_if(remix_vars::interpolate_stack.begin(), remix_vars::interpolate_stack.end(), completed_condition);
				remix_vars::interpolate_stack.erase(it, remix_vars::interpolate_stack.end());
			}
		}
	}

	__declspec(naked) void on_set_cgame_time_stub()
	{
		const static uint32_t CL_SetCGameTime_func = 0x63C6C0;
		const static uint32_t retn_addr = 0x644BA5;
		__asm
		{
			// stock func
			call	CL_SetCGameTime_func;

			pushad;
			call	on_set_cgame_time;
			popad;

			jmp		retn_addr;
		}
	}

	remix_vars::remix_vars()
	{
		p_this = this;

		scheduler::once([]
		{
			remix_vars::get()->parse_rtx_options();
		}, scheduler::main);

		// CL_SetCGameTime, called every client frame
		utils::hook(0x644BA0, on_set_cgame_time_stub, HOOK_JUMP).install()->quick();

		// --------

#if DEBUG
		command::add("rtx_parse_options", [this](const command::params&)
		{
			if (game::is_sp)
			{
				options.clear();
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
#endif

		command::add("rtx_set_option", [this](command::params p)
		{
			if (game::is_sp)
			{
				bool valid_value = false;

				if (p.length() > 2)
				{
					const auto type = std::string_view(p[2]);
					const auto val0 = std::string_view(p[3]);
					//const std::string_view val1 = p.length() > 3 ? std::string_view(p[4]) : "";
					//const std::string_view val2 = p.length() > 4 ? std::string_view(p[5]) : "";
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

		command::add("rtx_clear_transitions", [this](command::params p)
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
				// 3	- 4		- speed
				// 4	- 5		- goal x
				// 5	- 6		- goal y
				// 6	- 7		- goal z

				if (p.length() >= 5)
				{
					const auto type = std::string_view(p[2]);
					const auto speed = utils::try_stof(p[3], 1.0f);
					auto v = option_value{};

					if (type.contains("int"))
					{
						v.integer = utils::try_stoi(p[4]);
						valid_value = true;
					}
					else if (type.contains("float"))
					{
						v.value = utils::try_stof(p[4]);
						valid_value = true;
					}
					else if (type.contains("vec2"))
					{
						v.vector[0] = utils::try_stof(p[4]);
						v.vector[1] = utils::try_stof(p[5]);
						valid_value = true;
					}
					else if (type.contains("vec3"))
					{
						v.vector[0] = utils::try_stof(p[4]);
						v.vector[1] = utils::try_stof(p[5]);
						v.vector[2] = utils::try_stof(p[6]);
						valid_value = true;
					}

					if (valid_value)
					{
						if (!remix_vars::get()->add_interpolate_entry(nullptr, v, speed, p[1]))
						{
							game::sp::Com_PrintMessage(0, "rtx_transition: option not found! \n", 0);
						}
					}
				}

				if (p.length() < 5 || !valid_value)
				{
					game::sp::Com_PrintMessage(0, "rtx_transition   <option name>   <option type>   <transition speed>   <goal value/s> (true|false|0|1|2.5|1 2 3)\n", 0);
				}
			}
		});
	}
}
