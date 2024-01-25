#pragma once

namespace components::sp
{
	class fog : public component
	{
	public:
		fog();
		~fog() = default;
		const char* get_name() override { return "fog_sp"; }

		static fog* p_this;
		static fog* get();

		static inline float m_max_distance = 5000.0f;
		static inline game::GfxColor m_color = { 0x00FFFFFF };

		void set_settings_for_loaded_map(bool reload_settings = false);

	private:
		struct fog_settings
		{
			std::string mapname;
			float max_distance = 5000.0f;
			game::GfxColor m_color = { 0x00FFFFFF };
		};

		static inline std::vector<fog_settings> m_settings;

		bool load_settings();
	};
}
