#include "std_include.hpp"

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
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
		}

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
