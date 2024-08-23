#pragma once

namespace components::sp
{
	class api : public component
	{
	public:
		api();
		~api() = default;
		const char* get_name() override { return "api_sp"; }

		static inline api* p_this = nullptr;
		static api* get() { return p_this; }

		static void on_map_load();
		static bool is_initialized();
		static BRIDGEAPI_ErrorCode init();
		static inline bridgeapi_Interface bridge = {};


		// muzzleflash lights
		struct muzzleflash_light_s
		{
			std::uint64_t hash;
			std::uint8_t framecounter;
		};

		static inline muzzleflash_light_s muzzleflash_lights[12] = {};
		static void bullet_fire_frame_cb();
		static muzzleflash_light_s* get_avail_light();


		// other api lights
		static void track_and_draw_light_hash(std::uint64_t hash);
		static void remove_and_destroy_light(std::uint64_t hash);

		static inline std::unordered_set<std::uint64_t> api_lights = {};
	};
}
