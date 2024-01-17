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
		extern clipMap_t* cm;
		extern r_global_permanent_t* rgp;
		extern GfxBuffers* gfx_buf;

		extern CmdArgs* cmd_args;
		extern cmd_function_s** cmd_ptr;

		static DWORD* gfxCmdBufSourceState_ptr = (DWORD*)(0x829EA8);
		static DWORD* backEndDataOut_ptr = (DWORD*)(0x3DCB4CC);

		static utils::function<dvar_s* __cdecl (const char* name)> Dvar_FindVar = 0x5EDE30;
		static utils::function<Material* __fastcall (const char* name, int)> Material_RegisterHandle = 0x6E9C20;

		static utils::function<void __fastcall (int, GfxCmdBufSourceState*)> R_Set3D = 0x7244C0;

		typedef void(*Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
			extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

		typedef void(*Com_PrintMessage_t)(int, const char*, char);
			extern Com_PrintMessage_t Com_PrintMessage;

		typedef void(*Com_Error_t)(int type, const char* message, ...);
			extern Com_Error_t Com_Error;

		typedef void(*DB_EnumXAssets_FastFile_t)(XAssetType type, void(*)(XAssetHeader, void*), void* userdata, bool overrides);
			extern DB_EnumXAssets_FastFile_t DB_EnumXAssets_FastFile;

		typedef void(*DB_LoadXAssets_t)(XZoneInfo* zoneInfo, unsigned int zone_count, int sync);
			extern DB_LoadXAssets_t DB_LoadXAssets;

		extern scr_const_t* scr_const;

		void Cbuf_AddText(const char* text /*eax*/);
		void Vec2UnpackTexCoords(unsigned int packed, float* texcoord_out /*ecx*/);

		std::int16_t G_ModelIndex(const char* model_name /*eax*/); // ASM
		extern bool DB_FileExists(const char* file_name, game::DB_FILE_EXISTS_PATH);

		static utils::function<game::gentity_s* ()> G_Spawn = 0x54EAB0;
		static utils::function<bool(game::gentity_s*)> G_CallSpawnEntity = 0x5465A0;
		static utils::function<void(entityState_s*)> G_DObjUpdate = 0x54AC40;
	}

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace mp
	{
		extern DxGlobals* dx;
		extern clipMap_t* cm;

		static DWORD* gfxCmdBufSourceState_ptr = (DWORD*)(0x829B04);
		static DWORD* backEndDataOut_ptr = (DWORD*)(0x10CF9B5C);

		static utils::function<Material* __fastcall (const char* name, int)> Material_RegisterHandle = 0x6BBA90;
		static utils::function<dvar_s* __cdecl (const char* name)> Dvar_FindVar = 0x5C4170;
		static utils::function<dvar_s* __cdecl (dvar_s* dvar, const char* name, int dvar_type, int flags, const char* description, std::int64_t val, std::int64_t unk, float mins, float maxs)> Dvar_Reregister = 0x5C4DA0;
	}

	extern GfxCmdBufSourceState* get_cmdbufsourcestate();
	extern GfxBackEndData* get_backenddata();

	void Cmd_AddCommand(const char* name, void(*callback)(), cmd_function_s* data, char);
}
