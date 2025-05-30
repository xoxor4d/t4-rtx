#pragma once
#include "utils/function.hpp"

#define SELECT(mp_addr, sp_addr) (game::is_mp ? mp_addr : sp_addr)

#define MPSP_FUNC(func) { \
		if (game::is_mp) { \
			mp::##func; \
		} else { \
			sp::##func; \
		} \
	}

#define MPSP_FUNC_RET(func, dest) { \
		if (game::is_mp) { \
			(dest) = mp::##func; \
		} else { \
			(dest) = sp::##func; \
		} \
	}

#define MPSP_GAME_FUNC(func) { \
		if (game::is_mp) { \
			game::mp::##func; \
		} else { \
			game::sp::##func; \
		} \
	}

namespace game
{
	extern bool is_sp;
	extern bool is_mp;

	extern std::vector<std::string> loaded_modules;
	extern HWND main_window;

	static inline float COLOR_WHITE[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static inline float COLOR_BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static inline float COLOR_RED[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	static inline float COLOR_GREEN[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	static inline float COLOR_BLUE[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	static inline float vec3_origin[3] = { 0.0f, 0.0f, 0.0f };
	static inline float IDENTITY_AXIS[3][3] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace sp
	{
		extern DxGlobals* dx;
		extern clipMap_t* cm;
		extern GfxWorld* gfx_world;
		extern ComWorld* com_world;

		extern r_global_permanent_t* rgp;
		extern GfxBuffers* gfx_buf;
		extern cg_s* cgs;

		extern clientUIActive_s* clientUI;
		extern clientActive_s* client;

		extern DpvsGlob* dpvsGlob;

		extern CmdArgs* cmd_args;
		extern cmd_function_s** cmd_ptr;

		static DWORD* gfxCmdBufSourceState_ptr = (DWORD*)(0x829EA8);
		static DWORD* frontEndDataOut_ptr = (DWORD*)(0x3DCB498);
		static DWORD* backEndDataOut_ptr = (DWORD*)(0x3DCB4CC);

		static utils::function<dvar_s* __cdecl (const char* name)> Dvar_FindVar = 0x5EDE30;
		static utils::function<Material* __fastcall (const char* name, int)> Material_RegisterHandle = 0x6E9C20;
		static utils::function<void __fastcall (int, GfxCmdBufSourceState*)> R_Set3D = 0x7244C0;
		static utils::function<Font_s* (const char* fontName)> R_RegisterFont = 0x6E8D80;
		static utils::function<void(game::DebugGlobals* debugGlobalsEntry, const float* origin, const float* color, float scale, const char* string)> R_AddDebugString = 0x70C140;

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

		extern game::weaponInfo_s* cg_weaponsArray;
		extern scr_const_t* scr_const;
		extern int* Scr_GetNumParamArray;
		extern int* Scr_GetTypeArray;
		extern int* gScrMemTreePub_mtbuffer;
		extern int* level_time;
		extern std::uint32_t* level_num_entities;
		extern game::gentity_s* g_entities;
		extern game::centity_s* cg_entitiesArray;
		extern game::fake_centity_s* cg_fakeEntitiesArray; // 339CC38

		static utils::function<BuiltinFunction(const char**, int*)> Scr_GetFunction = 0x52F0B0;
		extern game::BuiltinMethod Scr_GetMethod(const char** pName, int* typ);

		static utils::function<BuiltinFunction(const char**, int*)> CScr_GetFunction = 0x66EA30;
		static utils::function<BuiltinMethod(const char**, int*)> CScr_GetMethod = 0x671110;

		extern int Scr_GetNumParams(int scrInstance);
		extern int Scr_GetType(int scrInstance, unsigned int paramNum);
		extern const char* Scr_GetString(int scrInstance, unsigned int paramNum);
		extern int Scr_GetInt(int scrInstance, unsigned int paramNum);
		extern float Scr_GetFloat(int scrInstance, unsigned int paramNum);
		extern void Scr_GetVector(int scrInstance, float* vectorValue, unsigned int paramNum);

		void Cbuf_AddText(const char* text /*eax*/);
		void Vec2UnpackTexCoords(unsigned int packed, float* texcoord_out /*ecx*/);

		std::int16_t G_ModelIndex(const char* model_name /*eax*/); // ASM
		extern bool DB_FileExists(const char* file_name, game::DB_FILE_EXISTS_PATH);

		extern int FX_SpawnOrientedEffect(const float* axis, game::FxEffectDef* def, int msec_begin, const float* origin);
		extern void FX_KillEffect(game::FxEffect* def);

		extern void draw_text_with_engine(float x, float y, float scale_x, float scale_y, const char* font, const float* color, const char* text);
		extern void R_AddCmdDrawTextASM(const char* text, int max_chars, void* font, float x, float y, float x_scale, float y_scale, float rotation, const float* color, int style);

		extern int G_DObjGetWorldTagPos(int scr_const_tag, int ent_num, float* pos_out);
		extern int DObjGetBoneIndex(game::DObj_s* obj /*ecx*/, int tag_name, BYTE* bone_index);
		extern int CG_DObjGetWorldBoneMatrix(game::cpose_t* pose /*eax*/, int bone_index /*ecx*/, game::DObj_s* obj /*edi*/, float* axis, float* origin);

		extern void R_AddCellSurfacesAndCullGroupsInFrustumDelayed(GfxCell* cell /*eax*/, DpvsPlane* planes /*edi*/, int planeCount, int frustumPlaneCount); // ASM
		extern void R_VisitPortals(int plane_count /*eax*/, GfxCell* cell, DpvsPlane* parent_plane, DpvsPlane* planes); // ASM

		static utils::function<game::gentity_s* ()> G_Spawn = 0x54EAB0;
		static utils::function<bool(game::gentity_s*)> G_CallSpawnEntity = 0x5465A0;
		static utils::function<void(entityState_s*)> G_DObjUpdate = 0x54AC40;

		static utils::function<void(const char* dir, const char* path, int is_lang, int lang)> FS_AddGameDirectory = 0x5DD8F0;

		// db
		extern int** DB_XAssetPool;
		extern unsigned int* g_poolSize;

		typedef int(__cdecl* DB_GetXAssetSizeHandler_t)();
			extern DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandler;

		extern int DB_GetXAssetTypeSize(int);
		extern void* DB_ReallocXAssetPool(int, unsigned int);
	}

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace mp
	{
		extern DxGlobals* dx;
		extern clipMap_t* cm;
		extern r_global_permanent_t* rgp;
		extern GfxBuffers* gfx_buf;
		//extern game::mp::cg_s* cgs;

		extern CmdArgs* cmd_args;
		extern cmd_function_s** cmd_ptr;

		static DWORD* gfxCmdBufSourceState_ptr = (DWORD*)(0x829B04);
		static DWORD* frontEndDataOut_ptr = (DWORD*)(0x10882FB4);
		static DWORD* backEndDataOut_ptr = (DWORD*)(0x10CF9B5C);

		static utils::function<dvar_s* __cdecl (const char* name)> Dvar_FindVar = 0x5C4170;
		static utils::function<Material* __fastcall (const char* name, int)> Material_RegisterHandle = 0x6BBA90;
		static utils::function<dvar_s* __cdecl (dvar_s* dvar, const char* name, int dvar_type, int flags, const char* description, std::int64_t val, std::int64_t unk, float mins, float maxs)> Dvar_Reregister = 0x5C4DA0;

		
		static utils::function<void __fastcall (int, GfxCmdBufSourceState*)> R_Set3D = 0x6FEBB0;

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

		static utils::function<std::int16_t(const char* model_name)> G_ModelIndex = 0x541020;
		extern bool DB_FileExists(const char* file_name, game::DB_FILE_EXISTS_PATH);

		static utils::function<game::gentity_s* ()> G_Spawn = 0x543760;
		static utils::function<bool(game::gentity_s*)> G_CallSpawnEntity = 0x5399B0;
		static utils::function<void(entityState_s*)> G_DObjUpdate = 0x541310;

		// db
		extern int** DB_XAssetPool;
		extern unsigned int* g_poolSize;

		typedef int(__cdecl* DB_GetXAssetSizeHandler_t)();
		extern DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandler;

		extern int DB_GetXAssetTypeSize(int);
		extern void* DB_ReallocXAssetPool(int, unsigned int);
	}

	extern IDirect3DDevice9* get_device();
	extern GfxCmdBufSourceState* get_cmdbufsourcestate();
	extern GfxBackEndData* get_frontenddata();
	extern GfxBackEndData* get_backenddata();

	void Cmd_AddCommand(const char* name, void(*callback)(), cmd_function_s* data, char);
	dvar_s* Dvar_RegisterInt(const char* name, int value, int min, int max, int flags, const char* description);
	dvar_s* Dvar_RegisterFloat(const char* name, float value, float min, float max, game::dvar_flags flags, const char* description);
	dvar_s* Dvar_RegisterVec3(const char* name, float x, float y, float z, float min, float max, game::dvar_flags flags, const char* description);
	dvar_s* Dvar_RegisterEnum(const char* dvar_name, const char** values, std::uint32_t value_count, int default_value, int flags, const char* description);
	dvar_s* Dvar_RegisterBool(const char* name, const char* description, bool value, int flags);

	XAssetHeader DB_FindXAssetHeader(XAssetType type, const char* name);

	// mp-sp functions
	extern utils::function<dvar_s* (const char*, int, int, DvarValue, DvarLimits, const char*)> Dvar_RegisterVariant;
	void init_offsets();
}
