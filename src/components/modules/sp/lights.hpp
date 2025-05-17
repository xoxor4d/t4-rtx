#pragma once

namespace components::sp
{
	class lights : public component
	{
	public:
		lights();
		~lights() = default;
		const char* get_name() override { return "lights_sp"; }

		static inline lights* p_this = nullptr;
		static lights* get() { return p_this; }

		static void		on_client_frame();
		static void		on_map_load();
		static void		on_api_callback();

		static void		create_primary_lights_for_map();
		static void		check_added_fx_omnilights();

		struct game_primary_light_s
		{
			remixapi_LightInfoSphereEXT ext = {};
			remixapi_LightInfo info = {};
			remixapi_LightHandle handle = nullptr;
			game::ComPrimaryLight* og_light;
			std::uint32_t index;
			bool tracked;
			bool has_entity;
		};
		static inline std::vector<game_primary_light_s> m_game_primary_lights = {};

		struct fx_omni_light_s
		{
			remixapi_LightInfoSphereEXT ext = {};
			remixapi_LightInfo info = {};
			remixapi_LightHandle handle = nullptr;
			bool new_or_matched_previous_frame;
		};
		static inline std::vector<fx_omni_light_s> m_fx_omni_lights = {};


		// #
		// muzzleflash lights
#if 0
		struct muzzleflash_light_s
		{
			std::uint64_t hash;
			std::uint8_t framecounter;
		};

		static inline muzzleflash_light_s muzzleflash_lights[12] = {};
		static void bullet_fire_frame_cb();
		static muzzleflash_light_s* muzzleflash_get_avail_light();
#endif


		// #
		// api light hash tracking and drawing

		static void light_hash_track_and_draw(remixapi_LightHandle handle);
		static void light_hash_untrack_and_destroy(remixapi_LightHandle handle);

		static inline std::unordered_set<remixapi_LightHandle> m_api_light_handles = {};
	};
}
