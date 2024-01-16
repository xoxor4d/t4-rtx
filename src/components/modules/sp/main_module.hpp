#pragma once

namespace components::sp
{
	class main_module : public component
	{
	public:
		main_module();
		~main_module() = default;
		const char* get_name() override { return "main_module_sp"; }
	
		static bool skysphere_is_model_valid();
		static void skysphere_change_model(int variant);
		static void skysphere_spawn(int variant = 0);

	private:
		static const char* skysphere_get_name_for_variant(int variant);
		static inline bool skysphere_spawned = false;
		static inline int skysphere_variant = 0;
		static inline game::gentity_s* skysphere_model = nullptr;

		static inline bool noborder_active = false;
		static inline int noborder_titlebar_height = 0;
	};
}
