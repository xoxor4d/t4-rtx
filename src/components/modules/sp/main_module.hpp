#pragma once

namespace components::sp
{
	class main_module : public component
	{
	public:
		main_module();
		~main_module() = default;
		const char* get_name() override { return "main_module_sp"; }

		static inline auto loc_culling_tweak_mins = 0u;
		static inline auto loc_culling_tweak_maxs = 0u;
		static inline auto loc_culling_tweak_frustum = 0u;
		static inline auto loc_culling_tweak_smodel = 0u;

		static constexpr auto FRUSTUM_PLANE_OFFSETS_COUNT = 7;
		static inline float m_frustum_plane_offsets[FRUSTUM_PLANE_OFFSETS_COUNT] = { 1200.0f, 1200.0f, 1200.0f, 1200.0f, 1200.0f, 1200.0f, 1200.0f };

		// old
		static inline bool OLD_CULLING_ACTIVE = false;
		static inline auto loc_disable_world_culling = 0u;
		static inline auto loc_disable_entity_culling = 0u;
		static inline std::vector rtx_disable_world_culling_enum = { "default", "less", "all", "all-but-models" };

		static bool skysphere_is_model_valid();
		static void skysphere_change_model(int variant);
		static void skysphere_update_pos(const float* pos);
		static void skysphere_spawn(int variant = 0);

		static void force_dvars_on_frame();
		static void rb_show_tess(game::GfxCmdBufSourceState* source, game::GfxCmdBufState* state, const float* center, const char* name, const float* color, game::DebugGlobals* manual_debug_glob = nullptr);

		static void on_map_load();
		static void on_map_shutdown();

	private:
		static const char* skysphere_get_name_for_variant(int variant);
		static inline bool skysphere_spawned = false;
		static inline int skysphere_variant = 0;
		static inline game::gentity_s* skysphere_model = nullptr;

		static inline bool noborder_active = false;
		static inline int noborder_titlebar_height = 0;
	};
}
