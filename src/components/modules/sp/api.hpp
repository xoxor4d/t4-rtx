#pragma once
#include "remix/remix_c.h"

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
		static remixapi_ErrorCode init();
		static inline remixapi_Interface bridge = {};
		bool m_initialized;
	};
}
