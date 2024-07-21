#pragma once

namespace components
{
	class scheduler final : public component
	{
	public:
		enum thread
		{
			main,
			renderer,
		};

		enum evaluation : bool
		{
			reschedule = false,
			remove = true,
		};

		static void on_frame(const std::function<void()>& callback, thread thread = main);
		static void delay(const std::function<void()>& callback, std::chrono::milliseconds delay, thread thread = main);
		static void once(const std::function<void()>& callback, thread thread = main);
		static void until(const std::function<evaluation()>& callback, thread thread = main);

		static void error(const std::string& message, int level);

		scheduler();
		const char* get_name() override { return "scheduler"; };

	private:
		static std::mutex mutex_;
		static std::queue<std::pair<std::string, int>> errors_;
		static utils::concurrent_list<std::pair<std::function<void()>, thread>> callbacks_;
		static utils::concurrent_list<std::pair<std::function<void()>, thread>> single_callbacks_;
		static utils::concurrent_list<std::pair<std::function<scheduler::evaluation()>, thread>> condition_callbacks_;

		static void main_frame_stub();
		static void renderer_frame_stub_stock();
	
		static void execute(thread thread);
		static void execute_safe(thread thread);
		static void execute_error(thread thread);
		static bool get_next_error(const char** error_message, int* error_level);
	};
}
