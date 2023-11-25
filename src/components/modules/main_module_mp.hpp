#pragma once

namespace components
{
	class main_module_mp : public component
	{
	public:
		main_module_mp();
		~main_module_mp() = default;
		const char* get_name() override { return "main_module_mp"; }
	
	private:
	};
}
