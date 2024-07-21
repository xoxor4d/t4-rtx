#pragma once

namespace components::mp
{
	class main_module : public component
	{
	public:
		main_module();
		~main_module() = default;
		const char* get_name() override { return "main_module_mp"; }

		static inline auto loc_disable_world_culling = 0u;
		static inline auto loc_disable_entity_culling = 0u;
		static inline std::vector rtx_disable_world_culling_enum = { "default", "less", "all", "all-but-models" };

		static void force_dvars_on_frame();

		static bool skysphere_is_model_valid();
		static void skysphere_change_model(int variant);
		static void skysphere_update_pos(const float* pos);
		static void skysphere_spawn(int variant = 0);

		static void on_map_load();
		static void on_map_shutdown();

	private:
		static const char* skysphere_get_name_for_variant(int variant);
		static inline bool skysphere_spawned = false;
		static inline int skysphere_variant = 0;
		static inline game::mp::gentity_s* skysphere_model = nullptr;

		static inline bool noborder_active = false;
		static inline int noborder_titlebar_height = 0;
	};
}
