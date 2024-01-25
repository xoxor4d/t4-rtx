#include "std_include.hpp"
using namespace game::sp;

namespace components::sp
{
	fog* fog::p_this = nullptr;
	fog* fog::get() { return p_this; }

	void fog::set_settings_for_loaded_map(bool reload_settings)
	{
		if ((m_settings.empty() || reload_settings) && !fog::load_settings())
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

					found = true;
					break;
				}
			}

			if (!found)
			{
				m_max_distance = 5000.0f;
				m_color.packed = D3DCOLOR_XRGB(200, 200, 220);
			}
		}
	}

	bool fog::load_settings()
	{
		m_settings.clear();
		m_settings.reserve(32);

		std::ifstream file;
		if (utils::fs::open_file_homepath("t4rtx", "fog.ini", false, file))
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

				if (args.size() == 5)
				{
					const DWORD color = D3DCOLOR_XRGB(utils::try_stoi(args[2], 255), utils::try_stoi(args[3], 255), utils::try_stoi(args[4], 255));
					m_settings.push_back({ args[0], utils::try_stof(args[1], 5000.0f), color });
				}
			}

			file.close();
			return true;
		}

		return false;
	}

	fog::fog()
	{
		command::add("fog_update", [this](command::params)
		{
			fog::set_settings_for_loaded_map(true);
		});
	}
}