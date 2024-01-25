#pragma once

namespace components
{
	class component
	{
	public:
		component() {};
		virtual ~component() {};
		virtual const char* get_name() { return "unknown"; };
	};

	class loader
	{
	public:
		static void initialize();
		static void uninitialize();
		static void _register(component* component);

		static utils::memory::allocator* get_alloctor();

	private:
		static std::vector<component*> components_;
		static utils::memory::allocator mem_allocator_;
	};
}

#include "modules/sp/main_module.hpp"
#include "modules/sp/fixed_function.hpp"
#include "modules/sp/fog.hpp"

#include "modules/mp/main_module.hpp"

#include "modules/flags.hpp"
#include "modules/command.hpp"
