#pragma once

namespace components::sp
{
	class scripting : public component
	{
	public:
		scripting();
		~scripting() = default;
		const char* get_name() override { return "scripting_sp"; }

		static inline scripting* p_this = nullptr;
		static scripting* get() { return p_this; }

		static void add_function(const char* name, game::BuiltinFunction, int = 0);
		static void add_method(const char* name, game::BuiltinMethod, int = 0);
		static void add_client_function(const char* name, game::BuiltinFunction, int = 0);
		static void add_client_method(const char* name, game::BuiltinMethod, int = 0);

	private:
		struct scr_func
		{
			game::BuiltinFunction func;
			int type;
		};

		struct scr_meth
		{
			game::BuiltinMethod func;
			int type;
		};

		static std::unordered_map<std::string, scr_func> script_functions;
		static std::unordered_map<std::string, scr_meth> script_methods;
		static std::unordered_map<std::string, scr_func> script_client_functions;
		static std::unordered_map<std::string, scr_meth> script_client_methods;

		static void scr_get_method_stub();
		static game::BuiltinFunction scr_get_function_hook(const char**, int*);
		static game::BuiltinMethod scr_get_method_hook(const char**, int*);
		static game::BuiltinFunction cscr_get_function_hook(const char**, int*);
		static game::BuiltinMethod cscr_get_method_hook(const char**, int*);
	};
}
