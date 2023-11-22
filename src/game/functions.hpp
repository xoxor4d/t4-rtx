#pragma once
#include "utils/function.hpp"

namespace game
{
	extern std::vector<std::string> loaded_modules;

	static inline float COLOR_WHITE[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static inline float COLOR_BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	extern DxGlobals* dx;

	static DWORD* gfxCmdBufSourceState_ptr = (DWORD*)(0x829B04);
	extern GfxCmdBufSourceState* get_cmdbufsourcestate();
	static DWORD* backEndDataOut_ptr = (DWORD*)(0x10CF9B5C);
	extern GfxBackEndData* get_backenddata();

	static utils::function<Material* __fastcall (const char* name, int)> Material_RegisterHandle = 0x6BBA90;

	static utils::function<dvar_s* __cdecl (const char* name)> Dvar_FindVar = 0x5C4170;
	static utils::function<dvar_s* __cdecl (dvar_s* dvar, const char* name, int dvar_type, int flags, const char* description, std::int64_t val, std::int64_t unk, float mins, float maxs)> Dvar_Reregister = 0x5C4DA0;
}
