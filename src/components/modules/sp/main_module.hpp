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
		static void on_set_cgame_time();

		enum SKY : std::uint32_t
		{
			SUNSET,
			NIGHT,
			BERLIN1,
			BERLIN2,
			BERLIN3,
			BGATE,
			COURT,
			FLYING,
			FLYING_DAY,
			HOLLAND1,
			HOLLAND3,
			KNEE,
			MAKIN,
			NIGHTFIRE,
			OKI1,
			OKI2,
			OKI3,
			PELELIU1A,
			PELELIU1B,
			PELELIU2,
			PROLOGUE,
			RHINE2,
			RHINE3,
			SEELOW1,
			SEELOW2,
			SNIPER,
			STAL,
			VODKA,
			ZMB_ASYLUM,
			ZMB_FACTORY,
			ZMB_FARM,
			ZMB_PROTOTYPE,
			COUNT,
		};

		static inline constexpr const char* SKY_STRINGS[] =
		{
			"rtx_skysphere_sunset_clouds",
			"rtx_skysphere_night",
			"rtx_polarsky_berlin1",
			"rtx_polarsky_berlin2",
			"rtx_polarsky_berlin3",
			"rtx_polarsky_bgate",
			"rtx_polarsky_court",
			"rtx_polarsky_flying",
			"rtx_polarsky_flying_day",
			"rtx_polarsky_holland1",
			"rtx_polarsky_holland3",
			"rtx_polarsky_knee",
			"rtx_polarsky_makin",
			"rtx_polarsky_nightfire",
			"rtx_polarsky_oki1",
			"rtx_polarsky_oki2",
			"rtx_polarsky_oki3",
			"rtx_polarsky_peleliu_1a",
			"rtx_polarsky_peleliu_1b",
			"rtx_polarsky_peleliu2",
			"rtx_polarsky_prologue",
			"rtx_polarsky_rhine2",
			"rtx_polarsky_rhine3",
			"rtx_polarsky_seelow1",
			"rtx_polarsky_seelow2",
			"rtx_polarsky_sniper",
			"rtx_polarsky_stal",
			"rtx_polarsky_vodka",
			"rtx_polarsky_zmb_asylum",
			"rtx_polarsky_zmb_factory",
			"rtx_polarsky_zmb_farm",
			"rtx_polarsky_zmb_proto",
		};

	private:
		static const char* skysphere_get_name_for_variant(std::uint32_t variant);
		static inline bool skysphere_spawned = false;
		static inline int skysphere_variant = 0;
		static inline game::gentity_s* skysphere_model = nullptr;

		static inline bool noborder_active = false;
		static inline int noborder_titlebar_height = 0;
	};
}
