#pragma once

namespace components::mp
{
	class fixed_function : public component
	{
	public:
		fixed_function();
		~fixed_function() = default;
		const char* get_name() override { return "fixed_function_mp"; }
	
	private:
	};
}
