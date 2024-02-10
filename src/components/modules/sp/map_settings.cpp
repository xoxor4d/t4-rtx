#include "std_include.hpp"
using namespace game::sp;

namespace components::sp
{
	map_settings* map_settings::p_this = nullptr;
	map_settings* map_settings::get() { return p_this; }

	void map_settings::set_settings_for_loaded_map(bool reload_settings)
	{
		DEBUG_PRINT("[T4RTX-DEBUG] # Function: map_settings::set_settings_for_loaded_map()\n");

		if ((m_settings.empty() || reload_settings) && !map_settings::load_settings())
		{
			return;
		}

		if (game::sp::rgp->world && game::sp::rgp->world->name)
		{
			std::string map_name = game::sp::rgp->world->name;
			utils::replace_all(map_name, std::string("maps/mp/"), "");	// if mp map
			utils::replace_all(map_name, std::string("maps/"), "");		// if sp map
			utils::replace_all(map_name, std::string(".d3dbsp"), "");

			bool found = false;
			for (const auto& s : m_settings)
			{
				if (s.mapname == map_name)
				{
					m_max_distance = s.max_distance;
					m_color = s.m_color;

					main_module::skysphere_spawn(s.skybox);

					found = true;
					break;
				}
			}

			if (!found)
			{
				m_max_distance = 5000.0f;
				m_color.packed = D3DCOLOR_XRGB(200, 200, 220);

				if (!flags::has_flag("no_default_sky") && !main_module::skysphere_is_model_valid())
				{
					main_module::skysphere_spawn(4); // always spawn sunset
				}
			}
		}
	}

	constexpr auto INI_MAPNAME_ARG = 0;
	constexpr auto INI_SKYBOX_ARG = 1;
	constexpr auto INI_FOG_MAX_ARG = 2;
	constexpr auto INI_FOG_COLOR_ARG_BEGIN = 3;

	bool map_settings::load_settings()
	{
		DEBUG_PRINT("[T4RTX-DEBUG] # Function: map_settings::load_settings()\n");

		m_settings.clear();
		m_settings.reserve(32);

		std::ifstream file;
		if (utils::fs::open_file_homepath("t4rtx", "map_settings.ini", false, file))
		{
			std::string input;
			std::vector<std::string> args;

			// read line by line
			while (std::getline(file, input))
			{
				// ignore comment
				if (utils::starts_with(input, "//"))
				{
					continue;
				}

				// split string on ','
				args = utils::split(input, ',');

				if (args.size() == INI_FOG_COLOR_ARG_BEGIN+3) // fog colors rgb are last
				{
					const DWORD color = D3DCOLOR_XRGB(
						utils::try_stoi(args[INI_FOG_COLOR_ARG_BEGIN+0], 255), 
						utils::try_stoi(args[INI_FOG_COLOR_ARG_BEGIN+1], 255), 
						utils::try_stoi(args[INI_FOG_COLOR_ARG_BEGIN+2], 255));

					m_settings.push_back(
						{
							args[INI_MAPNAME_ARG],
							utils::try_stoi(args[INI_SKYBOX_ARG], 0),
							utils::try_stof(args[INI_FOG_MAX_ARG], 5000.0f),
							color
						});
				}
			}

			file.close();
			return true;
		}

		return false;
	}

	map_settings::map_settings()
	{
		command::add("mapsettings_update", [this](command::params)
		{
			map_settings::set_settings_for_loaded_map(true);
		});
	}
}