#include "std_include.hpp"

namespace components
{
	constexpr auto INI_MAPNAME_ARG = 0;
	constexpr auto INI_FOG_DIST = 1;
	constexpr auto INI_FOG_COLOR_BEGIN = 2;
	constexpr auto INI_SUN_DIR_BEGIN = 5;
	constexpr auto INI_SUN_COLOR_BEGIN = 8;
	constexpr auto INI_SUN_INTENSITY = 11;
	constexpr auto INI_SKY_INDEX = 12;
	constexpr auto INI_ARGS_TOTAL = 13;

	void map_settings::set_settings_for_loaded_map(bool reload_settings)
	{
		DEBUG_PRINT("[DEBUG] # Function: map_settings::set_settings_for_loaded_map()\n");

		if ((m_settings.empty() || reload_settings) && !map_settings::load_settings())
		{
			return;
		}

		if (const auto rgp = SELECT(game::mp::rgp, game::sp::rgp);
			rgp && rgp->world && rgp->world->name)
		{
			std::string map_name = rgp->world->name;
			utils::replace_all(map_name, std::string("maps/mp/"), "");	// if mp map
			utils::replace_all(map_name, std::string("maps/"), "");		// if sp map
			utils::replace_all(map_name, std::string(".d3dbsp"), "");

			bool found = false;
			for (const auto& s : m_settings)
			{
				if (s.mapname == map_name)
				{
					m_loaded_map_settings = s;

					// 0-255 to 0-1 range
					utils::scale3(m_loaded_map_settings.sun_color, (1.0f / 255.0f), m_loaded_map_settings.sun_color);

					MPSP_FUNC(main_module::skysphere_spawn(s.sky_index));

					// only sp rn
					if (game::is_sp)
					{
						// spawn map markers
						for (auto i = 0u; i < s.map_markers.size(); i++)
						{
							if (s.map_markers[i].active)
							{
								if (const auto fx = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_FX, utils::va("rtx/markers/rtx_marker_%02d", i)).fx; fx)
								{
									m_loaded_map_settings.map_markers[i].handle = game::sp::FX_SpawnOrientedEffect(game::IDENTITY_AXIS[0], fx, 0, &s.map_markers[i].origin[0]);
								}
							}
						}

						if (sp::api::is_initialized())
						{
							// reload rtx.conf without hash vars
							//open_and_set_var_config("rtx.conf", true, "");

							// resets all modified variables back to rtx.conf level
							sp::remix_vars::get()->reset_all_modified();

							// auto apply {map_name}.conf (if it exists)
							open_and_set_var_config(s.mapname + ".conf");

							// apply other manually defined configs
							for (const auto& f : s.api_var_configs)
							{
								open_and_set_var_config(f);
							}
						}

						found = true;
						break;
					}
				}
			}

			if (!found)
			{
				// only sp rn
				if (game::is_sp)
				{
					// grab map defaults
					if (const auto world = SELECT(nullptr, game::sp::gfx_world); world)
					{
						game::vec3_t sun_fwd = {};
						utils::angle_vectors(world->sunParse.angles, sun_fwd, nullptr, nullptr);
						utils::scale3(sun_fwd, -1.0f, sun_fwd);
						utils::copy(sun_fwd, m_loaded_map_settings.sun_direction, 3);

						utils::copy(world->sunParse.sunColor, m_loaded_map_settings.sun_color, 3);
						m_loaded_map_settings.sun_intensity = world->sunParse.sunLight;
					}

					if (sp::api::is_initialized())
					{
						// reload rtx.conf without hash vars
						//open_and_set_var_config("rtx.conf", true, "");

						// resets all modified variables back to rtx.conf level
						sp::remix_vars::get()->reset_all_modified();
					}
				}

				const bool is_skysphere_model_valid = game::is_sp ? sp::main_module::skysphere_is_model_valid() : mp::main_module::skysphere_is_model_valid();
				if (!flags::has_flag("no_default_sky") && !is_skysphere_model_valid)
				{
					// always spawn sunset
					if (game::is_sp)
					{
						sp::main_module::skysphere_spawn(4);
					}
					else
					{
						mp::main_module::skysphere_spawn(4);
					}
				}
			}

			utils::copy(settings()->sun_direction, setting_resets()->sun_direction, 3);
			utils::copy(settings()->sun_color, setting_resets()->sun_color, 3);
			setting_resets()->sun_intensity = settings()->sun_intensity;
		}
	}

	map_settings::map_settings_s* map_settings::get_or_create_settings(bool parse_mode, const char* map_name)
	{
		// check if map settings exist
		for (auto& e : m_settings)
		{
			if (e.mapname._Equal(parse_mode ? m_args[INI_MAPNAME_ARG] : map_name))
			{
				return &e;
			}
		}

		// create defaults if not
		m_settings.push_back(map_settings_s(parse_mode ? m_args[INI_MAPNAME_ARG] : map_name));
		return &m_settings.back();
	}

	void map_settings::parse_culling()
	{
		if (map_settings_s* s = get_or_create_settings(); s)
		{
			s->cell_settings.clear();
			s->cell_settings.resize(256);
			bool any_valid_overrides = false;

			// for each cell with its forced indices with format -> [cell](index index index)
			for (auto a = 1u; a < m_args.size(); a++)
			{
				const auto& str = m_args[a];

				if (str.empty())
				{
					// print msg here
					continue;
				}

				if (!utils::has_matching_symbols(str, '[', ']', true) || !utils::has_matching_symbols(str, '(', ')', true))
				{
					continue;
				}

				// which cell are we writing settings for?
				const auto cell_index = utils::try_stoi(utils::split_string_between_delims(str, '[', ']'), -1);
				if (cell_index >= 0)
				{
					// limit the vector to 1024 entries
					if (cell_index >= 1024)
					{
						continue;
					}

					// resize vector to up to 1024 entries
					if (static_cast<std::uint32_t>(cell_index) >= s->cell_settings.size())
					{
						s->cell_settings.resize(cell_index + 1);
					}

					// get cell
					const auto c = &s->cell_settings[cell_index];

					// ignore duplicate cells
					if (c->active)
					{
						continue;
					}

					// get inidices
					const auto indices_str = utils::split_string_between_delims(str, '(', ')');
					const auto split_indices = utils::split(indices_str, ' ');

					for (const auto& i : split_indices)
					{
						if (const auto n = utils::try_stoi(i, -1); n >= 0)
						{
							c->forced_cell_indices.push_back(n);
						}
					}

					c->active = true;
					any_valid_overrides = true;
				}
			}

			s->cell_overrides_exist = any_valid_overrides;
		}
	}

	void map_settings::parse_markers()
	{
		if (map_settings_s* s = get_or_create_settings(); s)
		{
			// only sp rn
			if (game::is_sp)
			{
				// kill active fx in-case we reload settings 
				for (auto i = 0u; i < m_loaded_map_settings.map_markers.size(); i++)
				{
					if (m_loaded_map_settings.map_markers[i].active && m_loaded_map_settings.map_markers[i].handle)
					{
						// uh .. yea

						auto v2 = 16 * HIWORD(m_loaded_map_settings.map_markers[i].handle);
						auto v4 = v2 + *(DWORD*)(0x16CFD68) - 16;

						if (*(DWORD*)v4)
						{
							game::FxEffect* v5 = *(game::FxEffect**)v4;
							game::sp::FX_KillEffect(v5);
						}
					}
				}
			}

			s->map_markers.clear();
			s->map_markers.resize(100);

			for (auto a = 1u; a < m_args.size(); a++)
			{
				const auto& str = m_args[a];

				if (str.empty())
				{
					// print msg here
					continue;
				}

				// which marker are we writing settings for?
				const auto marker_index = utils::try_stoi(utils::split_string_between_delims(str, '[', ']'), -1);
				if (marker_index >= 0)
				{
					// limit the vector to 100 entries
					if (marker_index >= 100)
					{
						continue;
					}

					// get marker
					const auto m = &s->map_markers[marker_index];

					// ignore duplicate markers
					if (m->active)
					{
						continue;
					}

					// get and assign origin
					const auto origin_str = utils::split_string_between_delims(str, '(', ')');
					if (const auto xyz = utils::split(origin_str, ' ');
						xyz.size() == 3u)
					{
						m->origin[0] = utils::try_stof(xyz[0], 0.0f);
						m->origin[1] = utils::try_stof(xyz[1], 0.0f);
						m->origin[2] = utils::try_stof(xyz[2], 0.0f);
						m->active = true;
					}
				}
			}
		}
	}

	void map_settings::open_and_set_var_config(const std::string& config, const bool ignore_hashes, const char* custom_path)
	{
		std::string path = "t4rtx\\map_configs";
		if (custom_path)
		{
			path = custom_path;
		}

		std::ifstream file;
		if (utils::fs::open_file_homepath(path, config, false, file))
		{
			std::string input;
			while (std::getline(file, input))
			{
				if (utils::starts_with(input, "#"))
				{
					continue;
				}

				if (auto pair = utils::split(input, '=');
					pair.size() == 2u)
				{
					utils::trim(pair[0]);
					utils::trim(pair[1]);

					if (ignore_hashes && pair[1].starts_with("0x"))
					{
						continue;
					}

					if (pair[1].empty())
					{
						continue;
					}

					//sp::api::bridge.DebugPrint(utils::va("Set config var: %s to: %s", pair[0].c_str(), pair[1].c_str()));
					//
					//DEBUG_PRINT(utils::va("[DEBUG] Set rtx var: %s to: %s\n", pair[0].c_str(), pair[1].c_str()));
					//sp::api::bridge.SetConfigVariable(pair[0].c_str(), pair[1].c_str());

					const auto& vars = sp::remix_vars::get();
					const auto o = vars->get_option(pair[0].c_str());

					if (o)
					{
						const auto& v = vars->string_to_option_value(o->second.type, pair[1]);
						vars->set_option(o, v, true);

						DEBUG_PRINT("[MAP-SETTINGS/OPEN-SET-CONF] Set rtx level var: %s to: %s\n", o->first.c_str(), pair[1].c_str());
					}
				}
			}

			file.close();
		}
	}

	void map_settings::parse_api_var_configs()
	{
		if (map_settings_s* s = get_or_create_settings(); s)
		{
			s->api_var_configs.clear();
			for (auto a = 1u; a < m_args.size(); a++)
			{
				auto str = m_args[a];
				if (str.empty())
				{
					// print msg here
					continue;
				}

				utils::trim(str);
				s->api_var_configs.emplace_back(str);
			}
		}
	}

	void map_settings::parse_settings()
	{
		if (m_args.size() == INI_ARGS_TOTAL)
		{
			m_settings.push_back(
				{
					m_args[INI_MAPNAME_ARG],
					true,
					utils::try_stof(m_args[INI_FOG_DIST], 5000.0f),
					D3DCOLOR_XRGB
					(
						utils::try_stoi(m_args[INI_FOG_COLOR_BEGIN + 0], 255),
						utils::try_stoi(m_args[INI_FOG_COLOR_BEGIN + 1], 255),
						utils::try_stoi(m_args[INI_FOG_COLOR_BEGIN + 2], 255)
					),
					{
						utils::try_stof(m_args[INI_SUN_DIR_BEGIN + 0], 75.0f),
						utils::try_stof(m_args[INI_SUN_DIR_BEGIN + 1], -15.0f),
						utils::try_stof(m_args[INI_SUN_DIR_BEGIN + 2], -35.0f)
					},
					{
						utils::try_stof(m_args[INI_SUN_COLOR_BEGIN + 0], 255),
						utils::try_stof(m_args[INI_SUN_COLOR_BEGIN + 1], 255),
						utils::try_stof(m_args[INI_SUN_COLOR_BEGIN + 2], 255)
					},
					utils::try_stof(m_args[INI_SUN_INTENSITY], 1.0f),
					utils::try_stoi(m_args[INI_SKY_INDEX], 2)
				});
		}
	}

	bool map_settings::load_settings()
	{
		DEBUG_PRINT("[DEBUG] # Function: map_settings::load_settings()\n");

		m_settings.clear();
		m_settings.reserve(32);

		std::ifstream file;
		if (utils::fs::open_file_homepath("t4rtx", "map_settings.ini", false, file))
		{
			std::string input;
			auto parse_mode = PARSE_MODE::SETTINGS;

			// read line by line
			while (std::getline(file, input))
			{
				// ignore comment
				if (input.starts_with("//"))
				{
					continue;
				}

				if (input.empty())
				{
					continue;
				}

				if (parse_mode == SETTINGS && input.starts_with("#CULL"))
				{
					parse_mode = CULL;
					continue;
				}

				if (parse_mode == CULL && input.starts_with("#MARKER"))
				{
					parse_mode = MARKER;
					continue;
				}

				if (parse_mode == MARKER && input.starts_with("#API_CONFIGVARS"))
				{
					parse_mode = API_VARS;
					continue;
				}

				// split string on ','
				m_args = utils::split(input, ',');

				switch (parse_mode)
				{
				case SETTINGS:
					parse_settings();
					break;
				case CULL:
					parse_culling();
					break;
				case MARKER:
					parse_markers();
					break;
				case API_VARS:
					parse_api_var_configs();
					break;
				}
			}

			file.close();
			return true;
		}

		return false;
	}

	void map_settings::on_map_load()
	{
		map_settings::get()->set_settings_for_loaded_map();
	}

	map_settings::map_settings()
	{
		command::add("mapsettings_update", [this](const command::params&)
		{
			map_settings::set_settings_for_loaded_map(true);
		});

		command::add("mapsettings_get_defaults",  [this](const command::params&)
		{
			if (const auto world = SELECT(nullptr, game::sp::gfx_world); world) 
			{
				game::vec3_t sun_fwd = {};
				utils::angle_vectors(world->sunParse.angles, sun_fwd, nullptr, nullptr);
				utils::scale3(sun_fwd, -1.0f, sun_fwd);

				game::vec3_t sun_color = {};
				utils::scale3(world->sunParse.sunColor, 255.0f, sun_color);

				const auto str = utils::va("SunDir - SunColor - SunIntensity [%.3f,%.3f,%.3f,   %.1f,%.1f,%.1f,   %.2f]\n",
					sun_fwd[0], sun_fwd[1], sun_fwd[2], sun_color[0], sun_color[1], sun_color[2], world->sunParse.sunLight);

				MPSP_GAME_FUNC(Com_PrintMessage(0, str, 0))
			}
			else
			{
				MPSP_GAME_FUNC(Com_PrintMessage(0, "No map loaded ..", 0));
			}
		});

		//command::add("mapsettings_reload_rtx_conf", [this](const command::params&)
		//{
		//	if (game::is_sp && sp::api::bridge.initialized)
		//	{
		//		// reload rtx.conf without hash vars
		//		open_and_set_var_config("rtx.conf", true, "");
		//	}
		//});
	}
}