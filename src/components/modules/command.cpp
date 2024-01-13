#include "std_include.hpp"

namespace components
{
	std::vector<game::cmd_function_s*> command::functions;
	std::map<std::string, std::function<command::callback>> command::function_map;

	const char* command::params::operator[](size_t index)
	{
		if (index >= this->length()) 
		{
			return "";
		}

		return game::is_sp ? game::sp::cmd_args->argv[this->command_id][index]: nullptr; // TODO
	}

	size_t command::params::length()
	{
		return game::is_sp ? game::sp::cmd_args->argc[this->command_id] : 0; // TODO
	}

	void command::add(const char* name, std::function<command::callback> callback)
	{
		command::function_map[utils::str_to_lower(name)] = callback;

		game::Cmd_AddCommand(name, command::main_callback, command::allocate(), 0);
	}

	void command::execute(std::string command, bool sync)
	{
		command.append("\n");

		if (sync)
		{
			game::is_sp ? game::sp::Cmd_ExecuteSingleCommand(0, 0, command.data()) : __debugbreak(); // TODO
		}
		else
		{
			game::is_sp ? game::sp::Cbuf_AddText(command.data()) : __debugbreak(); // TODO
		}
	}

	game::cmd_function_s* command::allocate()
	{
		const auto cmd = new game::cmd_function_s;
		command::functions.push_back(cmd);

		return cmd;
	}

	void command::main_callback()
	{
		command::params params(game::is_sp ? game::sp::cmd_args->nesting : NULL); // TODO
		const std::string command = utils::str_to_lower(params[0]);

		if (command::function_map.contains(command))
		{
			command::function_map[command](params);
		}
	}

	command::~command()
	{
		for (const auto command : command::functions)
		{
			delete command;
		}

		command::functions.clear();
	}
}
