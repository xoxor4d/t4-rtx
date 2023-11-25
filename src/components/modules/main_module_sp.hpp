#pragma once

namespace components
{
	class main_module_sp : public component
	{
	public:
		main_module_sp();
		~main_module_sp() = default;
		const char* get_name() override { return "main_module_sp"; }
	
	private:
	};
}
