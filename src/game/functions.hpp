#pragma once
#include "utils/function.hpp"

#define SELECT(mp_addr, sp_addr) (game::is_mp ? mp_addr : sp_addr)

namespace game
{
	extern bool is_sp;
	extern bool is_mp;

	extern std::vector<std::string> loaded_modules;

	static inline float COLOR_WHITE[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static inline float COLOR_BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace sp
	{
		extern DxGlobals* dx;
		static utils::function<dvar_s* __cdecl (const char* name)> Dvar_FindVar = 0x5EDE30;
		static utils::function<Material* __fastcall (const char* name, int)> Material_RegisterHandle = 0x6E9C20;

		static DWORD* gfxCmdBufSourceState_ptr = (DWORD*)(0x829EA8);
		static DWORD * backEndDataOut_ptr = (DWORD*)(0x3DCB4CC);
	}

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace mp
	{
		extern DxGlobals* dx;

		static DWORD* gfxCmdBufSourceState_ptr = (DWORD*)(0x829B04);
		static DWORD* backEndDataOut_ptr = (DWORD*)(0x10CF9B5C);

		static utils::function<Material* __fastcall (const char* name, int)> Material_RegisterHandle = 0x6BBA90;
		static utils::function<dvar_s* __cdecl (const char* name)> Dvar_FindVar = 0x5C4170;
		static utils::function<dvar_s* __cdecl (dvar_s* dvar, const char* name, int dvar_type, int flags, const char* description, std::int64_t val, std::int64_t unk, float mins, float maxs)> Dvar_Reregister = 0x5C4DA0;
	}

	extern GfxCmdBufSourceState* get_cmdbufsourcestate();
	extern GfxBackEndData* get_backenddata();
}
