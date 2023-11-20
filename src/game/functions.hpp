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

	static utils::function<Material* __fastcall (const char* name, int)>Material_RegisterHandle = 0x6BBA90;
}
