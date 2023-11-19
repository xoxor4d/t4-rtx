#include "std_include.hpp"

namespace components
{
	std::vector<component*> loader::components_;
	utils::memory::allocator loader::mem_allocator_;

	void loader::initialize()
	{
		loader::mem_allocator_.clear();

		loader::_register(new main_module());
		//loader::_register(new patches());
		//loader::_register(new renderer());

		/* if(const auto	con = GET_GUI(ggui::console_dialog);
						con)
		{
			game::printf_to_console("[Modules] ---------------------\n");

			for (const auto& str : game::glob::loadedModules)
			{
				game::printf_to_console(str.c_str());
			}

			game::printf_to_console("\n");
		} */
	}

	void loader::uninitialize()
	{
		std::ranges::reverse(loader::components_.begin(), loader::components_.end());
		for (auto component : loader::components_)
		{
			delete component;
		}

		loader::components_.clear();
		loader::mem_allocator_.clear();
		fflush(stdout);
		fflush(stderr);
	}

	void loader::_register(component* component)
	{
		if (component)
		{
			game::loaded_modules.push_back("component registered: "s + component->get_name() + "\n");
			loader::components_.push_back(component);
		}
	}

	utils::memory::allocator* loader::get_alloctor()
	{
		return &loader::mem_allocator_;
	}
}
