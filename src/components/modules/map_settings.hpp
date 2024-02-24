#pragma once

namespace components
{
	class map_settings : public component
	{
	public:
		map_settings();
		~map_settings() = default;
		const char* get_name() override { return "map_settings"; }

		static map_settings* p_this;
		static map_settings* get();

		static inline float m_max_distance = 5000.0f;
		static inline game::GfxColor m_color = { 0x00FFFFFF };

		void set_settings_for_loaded_map(bool reload_settings = false);

	private:
		struct settings_s
		{
			std::string mapname;
			int skybox;
			float max_distance = 5000.0f;
			game::GfxColor m_color = { 0x00FFFFFF };
		};

		static inline std::vector<settings_s> m_settings;
		bool load_settings();
	};
}
