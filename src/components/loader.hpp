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

#include "modules/main_module.hpp"
//#include "modules/patches.hpp"
//#include "modules/renderer.hpp"
