#pragma once

namespace components::sp
{
	class main_module : public component
	{
	public:
		main_module();
		~main_module() = default;
		const char* get_name() override { return "main_module_sp"; }
	
	private:
	};
}