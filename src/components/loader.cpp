#include "std_include.hpp"

namespace components
{
	std::vector<component*> loader::components_;
	utils::memory::allocator loader::mem_allocator_;

	void loader::initialize()
	{
		mem_allocator_.clear();

		_register(new scheduler());
		_register(new command());
		_register(new map_settings());

		if (game::is_sp)
		{
			_register(new sp::main_module());
			_register(new sp::fixed_function());
			_register(new sp::api());
			
		}

		if (game::is_mp)
		{
			_register(new mp::main_module());
			_register(new mp::fixed_function());
		}

#if DEBUG
		for (const auto& str : game::loaded_modules)
		{
			printf(str.c_str());
		}
#endif
	}

	void loader::uninitialize()
	{
		std::ranges::reverse(components_.begin(), components_.end());
		for (auto component : components_)
		{
			delete component;
		}

		components_.clear();
		mem_allocator_.clear();
		fflush(stdout);
		fflush(stderr);
	}

	void loader::_register(component* component)
	{
		if (component)
		{
			game::loaded_modules.push_back("component registered: "s + component->get_name() + "\n");
			components_.push_back(component);
		}
	}

	utils::memory::allocator* loader::get_alloctor()
	{
		return &loader::mem_allocator_;
	}
}
