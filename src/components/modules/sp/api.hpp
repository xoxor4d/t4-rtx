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

		static BRIDGEAPI_ErrorCode init();

		static inline bridgeapi_Interface bridge = {};
	};
}
