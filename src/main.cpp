#include "std_include.hpp"

// rename window title to get rid of the trademark (R) which currently causes issues with the remix toolkit
DWORD WINAPI find_window_loop(LPVOID)
{
	const char* hwnd_title = "Call of Duty\xAE";
	std::uint32_t _time = 0;

	// wait for window creation
	while (!game::main_window)
	{
		// get main window hwnd
		game::main_window = FindWindowExA(nullptr, nullptr, "CoD-WaW", hwnd_title);

		Sleep(1000); _time += 500;
		if (_time >= 30000)
		{
			return TRUE;
		}
	}

#ifdef GIT_DESCRIBE
	static auto version_str = std::string("t4-rtx-"s + GIT_DESCRIBE);
	SetWindowTextA(game::main_window, version_str.c_str());

#else
	SetWindowTextA(game::main_window, "t4-rtx");
#endif

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
#if DEBUG
		// disable stdout buffering
		setvbuf(stdout, nullptr, _IONBF, 0);

		// create an external console
		if (AllocConsole())
		{
			FILE* file = nullptr;
			freopen_s(&file, "CONIN$", "r", stdin);
			freopen_s(&file, "CONOUT$", "w", stdout);
			freopen_s(&file, "CONOUT$", "w", stderr);
			SetConsoleTitleA("t4-rtx console");
		}
#endif

		char filename[MAX_PATH];
		GetModuleFileNameA(nullptr,filename, MAX_PATH);

		if (const auto pos = std::string_view(filename).find_last_of('\\'); 
			pos != std::string::npos)
		{
			const auto exe_name = std::string_view(filename).substr(pos + 1) ;
			game::is_sp = exe_name == "CoDWaW.exe";
			game::is_mp = exe_name == "CoDWaWmp.exe";

			if (!game::is_sp && !game::is_mp)
			{
				return FALSE;
			}

			if (const auto version_str = reinterpret_cast<const char*>(SELECT(0x11510400, 0x4E55B20)); *version_str)
			{
				ASSERT(utils::string_contains(version_str, "1.7.1263 CL(350073)"), "Wrong game version!\n-> t4-rtx is only compatible with '1.7.1263' but found: %s", version_str);
			}
		}

		CreateThread(nullptr, 0, find_window_loop, nullptr, 0, nullptr);
		game::init_offsets();
		components::loader::initialize();

		// cookie 0x7A8256 --- sp == 0x7AF316
		// nvapi.dll is imported when 'R_PreCreateWindow' in 'R_Init' is called so hooks we place before that will be ignored
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		components::loader::uninitialize();
	}

	return TRUE;
}
