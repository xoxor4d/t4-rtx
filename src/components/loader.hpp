#pragma once

namespace components
{
	class component
	{
	public:
		component() {};
		virtual ~component() {};
		virtual const char* get_name() { return "unknown"; }
	};

	class loader
	{
	public:
		static void initialize();
		static void uninitialize();
		static void _register(component* component);

		static utils::memory::allocator* get_alloctor();
		static std::vector<component*> components_;

	private:
		static utils::memory::allocator mem_allocator_;
	};
}

#include "modules/sp/main_module.hpp"
#include "modules/sp/fixed_function.hpp"
#include "modules/sp/api.hpp"
#include "modules/sp/remix_vars.hpp"
#include "modules/sp/scripting.hpp"
#include "modules/sp/lights.hpp"

#include "modules/mp/main_module.hpp"
#include "modules/mp/fixed_function.hpp"

#include "modules/scheduler.hpp"
#include "modules/map_settings.hpp"
#include "modules/flags.hpp"
#include "modules/command.hpp"
