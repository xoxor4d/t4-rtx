#include "std_include.hpp"
// :*

using namespace game::sp;
namespace components::sp
{
	std::unordered_map<std::string, scripting::scr_func> scripting::script_functions;
	std::unordered_map<std::string, scripting::scr_meth> scripting::script_methods;

	void scripting::add_function(const char* name, game::BuiltinFunction func, int type)
	{
		scr_func to_add;
		to_add.func = func;
		to_add.type = type;

		script_functions.insert_or_assign(utils::str_to_lower(name), to_add);
	}

	void scripting::add_method(const char* name, game::BuiltinMethod func, int type)
	{
		scr_meth to_add;
		to_add.func = func;
		to_add.type = type;

		script_methods.insert_or_assign(utils::str_to_lower(name), to_add);
	}

	game::BuiltinFunction scripting::scr_get_function_hook(const char** name, int* type)
	{
		if (const auto f = script_functions.find(*name); f != script_functions.end())
		{
			*type = f->second.type;
			return f->second.func;
		}

		return game::sp::Scr_GetFunction(name, type);
	}

	game::BuiltinMethod scripting::scr_get_method_hook(const char** name, int* type)
	{
		if (const auto m = script_methods.find(*name); m != script_methods.end())
		{
			*type = m->second.type;
			return m->second.func;
		}

		return game::sp::Scr_GetMethod(name, type);
	}

	__declspec(naked) void scripting::scr_get_method_stub()
	{
		__asm
		{
			push	edi;
			push	esi;
			call	scr_get_method_hook;
			add		esp, 8;
			retn;
		}
	}

	//

	scripting::scripting()
	{
		p_this = this;

		utils::hook(0x682DAF, scr_get_function_hook, HOOK_CALL).install()->quick();
		utils::hook(0x683043, scr_get_method_stub, HOOK_CALL).install()->quick();

		add_function("printConsole", []()
		{
			if (game::sp::Scr_GetNumParams(game::SCRIPTINSTANCE_SERVER) < 1)
			{
				return;
			}

			const char* to_print = game::sp::Scr_GetString(game::SCRIPTINSTANCE_SERVER, 0);
			if (!to_print)
			{
				return;
			}

			game::sp::Com_PrintMessage(0, to_print, 0);
		});

		// override og funcs


		// SetLightIntensity
		/*add_method("setLightIntensity", [](game::scr_entref_s ref)
		{
			int x = ref.entnum;
			DEBUG_PRINT("[GSC] SetLightIntensity called!\n")
		});*/

		add_function("lerpSunDirection", []()
		{
			game::vec3_t from = {};
			game::vec3_t to = {};

			game::sp::Scr_GetVector(game::SCRIPTINSTANCE_SERVER, from, 0);
			game::sp::Scr_GetVector(game::SCRIPTINSTANCE_SERVER, to, 1u);
			//const auto t = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 2u) * 1000.0f + static_cast<float>(*game::sp::level_time);
			const auto t = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 2u);

			// correct axis for remix
			game::vec3_t from_fwd = {};
			utils::vector_to_angles(from, from_fwd); // to angles first
			utils::angle_vectors(from_fwd, from_fwd, nullptr, nullptr); // forward vec of angles
			utils::scale3(from_fwd, -1.0f, from_fwd); // inv

			game::vec3_t to_fwd = {};
			utils::vector_to_angles(to, to_fwd); // to angles first
			utils::angle_vectors(to_fwd, to_fwd, nullptr, nullptr); // forward vec of angles
			utils::scale3(to_fwd, -1.0f, to_fwd); // inv

			utils::vector_normalize(from_fwd);
			utils::vector_normalize(to_fwd);
			
			const auto dir = map_settings::settings()->sun_direction;
			dir[0] = (to_fwd[0] - from_fwd[0]) * t + from_fwd[0];
			dir[1] = (to_fwd[1] - from_fwd[1]) * t + from_fwd[1];
			dir[2] = (to_fwd[2] - from_fwd[2]) * t + from_fwd[2];

		});

		add_function("resetSunDirection", []()
		{
			utils::copy(map_settings::setting_resets()->sun_direction, map_settings::settings()->sun_direction, 3);
		});

		add_function("setSunlight", []()
		{
			if (game::sp::Scr_GetNumParams(game::SCRIPTINSTANCE_SERVER) < 3)
			{
				return;
			}

			if (api::is_initialized())
			{
				map_settings::settings()->sun_color[2] = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 2u);
				map_settings::settings()->sun_color[1] = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 1u);
				map_settings::settings()->sun_color[0] = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 0u);
				map_settings::settings()->sun_intensity = 2.0f;
			}
		});

		add_function("resetSunLight", []()
		{
			if (api::is_initialized())
			{
				utils::copy(map_settings::setting_resets()->sun_color, map_settings::settings()->sun_color, 3);
				map_settings::settings()->sun_intensity = map_settings::setting_resets()->sun_intensity;
			}
		});

		add_function("VisionSetNaked", []()
		{
			if (api::is_initialized())
			{
				if (game::sp::Scr_GetNumParams(game::SCRIPTINSTANCE_SERVER) < 2)
				{
					return;
				}

				const char* vision_name = game::sp::Scr_GetString(game::SCRIPTINSTANCE_SERVER, 0);
				if (!vision_name)
				{
					return;
				}

				const float transition_time = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 1u);
				remix_vars::parse_and_apply_conf_with_lerp(vision_name, remix_vars::INTERPOLATE_TYPE_SMOOTH, transition_time);
			}
		});

#if 0 // would need to replace cscr SetVolFog func and not the gsc variant for it to make sense (dog round) - cba
		add_function("SetVolFog", []()
		{
			if (api::is_initialized())
			{
				const float start_dist = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 0u);
				const float halfway_dist = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 1u);
				//const float halfway_height = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 2u);
				//const float base_height = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 3u);
				//const float red = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 4u);
				//const float green = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 5u);
				//const float blue = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 6u);
				const float transition_time = Scr_GetFloat(game::SCRIPTINSTANCE_SERVER, 7u);

				//remix_vars::option_value remap_dist_min = { .value = start_dist };
				//remix_vars::option_value remap_dist_max = { .value = halfway_dist /** 5.0f*/ };
				//remix_vars::add_smooth_interpolate_entry(nullptr, remap_dist_min, (uint32_t)game::sp::cgs->time, transition_time, "rtx.fogRemapMaxDistanceMin");
				//remix_vars::add_smooth_interpolate_entry(nullptr, remap_dist_max, (uint32_t)game::sp::cgs->time, transition_time, "rtx.fogRemapMaxDistanceMax");


				// # create custom option with start settings
				remix_vars::option_s v = { remix_vars::OPTION_TYPE_FLOAT, {} };

				// update start of custom option if it already exists
				if (auto o = remix_vars::get_custom_option("#FOG_DIST"); o)
				{
					v.current = remix_vars::option_value{ .value = o->second.current.value };
				}
				else // create custom option from fog values in map settings (first time script calls SetVolFog)
				{
					v.current.value = map_settings::settings()->fog_distance;
					v.reset = v.current;
					v.reset_level = v.current;
				}

				// let interpolation code know that this is not a remix variable
				v.not_a_remix_var = true;

				if (const auto o = remix_vars::add_custom_option("#FOG_DIST", v); o)
				{
					if (transition_time > 0.0f)
					{
						remix_vars::option_value goal = { .value = (halfway_dist * 2.0f - start_dist) * 3.0f };
						goal.value = goal.value < 1800.0f ? 1800.0f : goal.value;

						remix_vars::add_smooth_interpolate_entry(o, goal, (uint32_t)game::sp::cgs->time, transition_time);
					}
				}

#if 0 // fog color lerping (not so useful)
				// # create custom option with start settings
				remix_vars::option_s v = { remix_vars::OPTION_TYPE_VEC3, {} };

				// update start of custom option if it already exists
				if (auto o = remix_vars::get_custom_option("#FOG_COL"); o)
				{
					v.current = remix_vars::option_value{ .vector = { o->second.current.vector[0], o->second.current.vector[1], o->second.current.vector[2]}};
				}
				else // create custom option from fog values in map settings (first time script calls SetVolFog)
				{
					const auto m_fog_col_p = map_settings::settings()->fog_color;
					game::vec3_t unpacked = {};
					unpacked[0] = ((m_fog_col_p.packed >> 16) & 0xFF) / 255.0f;  // r
					unpacked[1] = ((m_fog_col_p.packed >> 8)  & 0xFF) / 255.0f;  // g
					unpacked[2] =  (m_fog_col_p.packed        & 0xFF) / 255.0f;  // b

					v.current = remix_vars::option_value{ .vector = { unpacked[0], unpacked[1], unpacked[2] } };
					v.reset = v.current;
					v.reset_level = v.current;
				}

				// let interpolation code know that this is not a remix variable
				v.not_a_remix_var = true;

				if (const auto o = remix_vars::add_custom_option("#FOG_COL", v); o)
				{
					if (transition_time > 0.0f)
					{
						const remix_vars::option_value goal = { .vector = { red, green, blue } };
						remix_vars::add_smooth_interpolate_entry(o, goal, (uint32_t)game::sp::cgs->time, transition_time);
					}
				}
#endif
			}
		});
#endif
	}
}
