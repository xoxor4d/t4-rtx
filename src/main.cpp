#include "std_include.hpp"

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		components::loader::initialize();

		// cookie 0x7A8256
		// nvapi.dll is imported when 'R_PreCreateWindow' in 'R_Init' is called so hooks we place before that will be ignored
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		components::loader::uninitialize();
	}

	return TRUE;
}
