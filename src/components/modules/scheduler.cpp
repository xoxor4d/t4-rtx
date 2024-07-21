#include "std_include.hpp"

namespace components
{
	std::mutex scheduler::mutex_;
	std::queue<std::pair<std::string, int>> scheduler::errors_;
	utils::concurrent_list<std::pair<std::function<void()>, scheduler::thread>> scheduler::callbacks_;
	utils::concurrent_list<std::pair<std::function<void()>, scheduler::thread>> scheduler::single_callbacks_;
	utils::concurrent_list<std::pair<std::function<scheduler::evaluation()>, scheduler::thread>> scheduler::condition_callbacks_;

	void scheduler::on_frame(const std::function<void()>& callback, const thread thread)
	{
		callbacks_.add({ callback, thread });
	}

	void scheduler::delay(const std::function<void()>& callback, const std::chrono::milliseconds delay, const thread thread)
	{
		const auto begin = std::chrono::high_resolution_clock::now();

		until([=]()
		{
			if ((std::chrono::high_resolution_clock::now() - begin) > delay)
			{
				callback();
				return evaluation::remove;
			}

			return evaluation::reschedule;
		}, thread);
	}

	void scheduler::once(const std::function<void()>& callback, const thread thread)
	{
		single_callbacks_.add({ callback, thread });
	}

	void scheduler::until(const std::function<evaluation()>& callback, thread thread)
	{
		condition_callbacks_.add({ callback, thread });
	}

	void scheduler::error(const std::string& message, int level)
	{
		std::lock_guard _(mutex_);
		errors_.emplace(message, level);
	}

	__declspec(naked) void scheduler::main_frame_stub()
	{
		static const int execution_thread = thread::main;
		const static uint32_t retn_addr = 0x59DCF0;
		__asm
		{
			pushad;
			push	execution_thread;
			call	execute;
			pop		eax;
			popad;

			jmp		retn_addr;
		}
	}

	// -------

	__declspec(naked) void scheduler::renderer_frame_stub_stock()
	{
		static const int execution_thread = thread::renderer;
		const static uint32_t retn_addr = 0x46FA40;
		__asm
		{
			pushad;
			push	execution_thread;
			call	execute;
			pop		eax;
			popad;

			jmp		retn_addr;
		}
	}

	// -------

	__declspec(naked) void scheduler::execute(thread)
	{
		__asm
		{
			push	[esp + 0x4];

			call	execute_error;
			call	execute_safe;

			add		esp, 0x4;
			retn;
		}
	}

	void scheduler::execute_safe(const thread thread)
	{
		for (const auto callback : callbacks_)
		{
			if (callback->second == thread)
			{
				callback->first();
			}
		}

		for (auto callback : single_callbacks_)
		{
			if (callback->second == thread)
			{
				single_callbacks_.remove(callback);
				callback->first();
			}
		}

		for (auto callback : condition_callbacks_)
		{
			if (callback->second == thread)
			{
				if (callback->first() == evaluation::remove)
				{
					condition_callbacks_.remove(callback);
				}
			}
		}
	}

	void scheduler::execute_error([[maybe_unused]] thread thread)
	{
		const char* message;
		int level;

		if (get_next_error(&message, &level) && message)
		{
			MPSP_GAME_FUNC(Com_Error(level, "%s", message));
		}
	}

	bool scheduler::get_next_error(const char** error_message, int* error_level)
	{
		std::lock_guard _(mutex_);
		if (errors_.empty())
		{
			*error_message = nullptr;
			return false;
		}

		const auto error = errors_.front();
		errors_.pop();

		*error_level = error.second;
		*error_message = utils::va("%s", error.first.data());

		return true;
	}

	scheduler::scheduler()
	{
		utils::hook(0x59E4D7, main_frame_stub, HOOK_CALL).install()->quick();
		utils::hook(0x479290, renderer_frame_stub_stock, HOOK_CALL).install()->quick();
	}
}
