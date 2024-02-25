#include "std_include.hpp"

namespace game
{
	bool is_sp = false;
	bool is_mp = false;

	std::vector<std::string> loaded_modules;
	HWND main_window = nullptr;

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace sp
	{
		DxGlobals* dx = reinterpret_cast<DxGlobals*>(0x3BF3B04);
		clipMap_t* cm = reinterpret_cast<clipMap_t*>(0x1F41A00);
		r_global_permanent_t* rgp = reinterpret_cast<r_global_permanent_t*>(0x3BF1880);
		game::GfxBuffers* gfx_buf = reinterpret_cast<game::GfxBuffers*>(0x42C2000);
		game::cg_s* cgs = reinterpret_cast<game::cg_s*>(0x34732B8);

		CmdArgs* cmd_args = reinterpret_cast<CmdArgs*>(0x1F41670);
		cmd_function_s** cmd_ptr = reinterpret_cast<cmd_function_s**>(0x1F416F4);

		Cmd_ExecuteSingleCommand_t	Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x594ED0);

		Com_PrintMessage_t Com_PrintMessage = Com_PrintMessage_t(0x59A170);
		Com_Error_t Com_Error = Com_Error_t(0x59AC50);
		DB_EnumXAssets_FastFile_t DB_EnumXAssets_FastFile = DB_EnumXAssets_FastFile_t(0x48D560);
		DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x48E7B0);

		scr_const_t* scr_const = reinterpret_cast<scr_const_t*>(0x1F33B90);

		void Cbuf_AddText(const char* text /*eax*/)
		{
			const static uint32_t Cbuf_AddText_func = 0x594200;
			__asm
			{
				mov		ecx, 0;
				mov		eax, text;
				call	Cbuf_AddText_func;
			}
		}

		void Vec2UnpackTexCoords(unsigned int packed, float* texcoord_out /*ecx*/)
		{
			const static uint32_t func_addr = 0x5E4D30;
			__asm
			{
				mov		ecx, texcoord_out;
				push	packed;
				call	func_addr;
				add		esp, 4;
			}
		}

		std::int16_t G_ModelIndex(const char* model_name /*edi*/)
		{
			const static uint32_t G_ModelIndex_func = 0x54A480;
			__asm
			{
				mov		edi, model_name;
				call	G_ModelIndex_func;
			}
		}

		bool DB_FileExists(const char* file_name, game::DB_FILE_EXISTS_PATH src)
		{
			const static uint32_t DB_FileExists_func = 0x48FC10;
			__asm
			{
				push	src;
				mov		eax, file_name;

				call	DB_FileExists_func;
				add     esp, 4;
			}
		}
	} // sp-end

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace mp
	{
		DxGlobals* dx = reinterpret_cast<DxGlobals*>(0x1087DD04);
		clipMap_t* cm = reinterpret_cast<clipMap_t*>(0x2223A80);

		r_global_permanent_t* rgp = reinterpret_cast<r_global_permanent_t*>(0x1087BA80);
		game::GfxBuffers* gfx_buf = reinterpret_cast<game::GfxBuffers*>(0x10F3A7C8);
		//game::mp::cg_s* cgs = reinterpret_cast<game::mp::cg_s*>(0x98FCE0);

		CmdArgs* cmd_args = reinterpret_cast<CmdArgs*>(0x22236F8);
		cmd_function_s** cmd_ptr = reinterpret_cast<cmd_function_s**>(0x222377C);

		Cmd_ExecuteSingleCommand_t	Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x55CD90);

		Com_PrintMessage_t Com_PrintMessage = Com_PrintMessage_t(0x5622B0);
		Com_Error_t Com_Error = Com_Error_t(0x562DA0);
		DB_EnumXAssets_FastFile_t DB_EnumXAssets_FastFile = DB_EnumXAssets_FastFile_t(0x4B9AE0);
		DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x4BAD70);

		scr_const_t* scr_const = reinterpret_cast<scr_const_t*>(0x221A910);

		void Cbuf_AddText(const char* text /*eax*/)
		{
			const static uint32_t Cbuf_AddText_func = 0x55C130;
			__asm
			{
				mov		ecx, 0;
				mov		eax, text;
				call	Cbuf_AddText_func;
			}
		}

		void Vec2UnpackTexCoords(unsigned int packed, float* texcoord_out /*ecx*/)
		{
			const static uint32_t func_addr = 0x5BB880;
			__asm
			{
				mov		ecx, texcoord_out;
				push	packed;
				call	func_addr;
				add		esp, 4;
			}
		}

		bool DB_FileExists(const char* file_name, game::DB_FILE_EXISTS_PATH src)
		{
			const static uint32_t DB_FileExists_func = 0x4BC460;
			__asm
			{
				push	src;
				mov		eax, file_name;

				call	DB_FileExists_func;
				add     esp, 4;
			}
		}
	} // mp-end

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	GfxCmdBufSourceState* get_cmdbufsourcestate()
	{
		const auto out = reinterpret_cast<GfxCmdBufSourceState*>(game::is_mp ? *game::mp::gfxCmdBufSourceState_ptr : *game::sp::gfxCmdBufSourceState_ptr);
		return out;
	}

	GfxBackEndData* get_frontenddata()
	{
		const auto out = reinterpret_cast<GfxBackEndData*>(game::is_mp ? *mp::frontEndDataOut_ptr : *sp::frontEndDataOut_ptr);
		return out;
	}

	GfxBackEndData* get_backenddata()
	{
		const auto out = reinterpret_cast<GfxBackEndData*>(game::is_mp ? *mp::backEndDataOut_ptr : *sp::backEndDataOut_ptr);
		return out;
	}

	void Cmd_AddCommand(const char* name, void(*callback)(), cmd_function_s* data, char)
	{
		data->name = name;
		data->function = callback;
		data->next = game::is_sp ? *sp::cmd_ptr : *mp::cmd_ptr;

		if (game::is_sp)
		{
			*game::sp::cmd_ptr = data;
		}
		else
		{
			*game::mp::cmd_ptr = data;
		}
	}

	// *
	// mp-sp functions

	utils::function<dvar_s* (const char* dvarName, int type, int flags, DvarValue value, DvarLimits domain, const char* description)> Dvar_RegisterVariant;

	dvar_s* Dvar_RegisterFloat(const char* name, float value, float min, float max, game::dvar_flags flags, const char* description)
	{
		game::DvarValue val = {};
		val.value = value;

		game::DvarLimits lim = {};
		lim.value.min = min;
		lim.value.max = max;

		return game::Dvar_RegisterVariant(name, game::dvar_type::value, flags, val, lim, description);
	}

	dvar_s* Dvar_RegisterEnum(const char* dvar_name, const char** values, std::uint32_t value_count, int default_value, int flags, const char* description)
	{
		game::DvarValue val = {};
		val.integer = default_value;

		game::DvarLimits lim = {};
		lim.enumeration.stringCount = static_cast<int>(value_count);
		lim.enumeration.strings = values;

		return game::Dvar_RegisterVariant(dvar_name, game::enumeration, flags, val, lim, description);
	}

	dvar_s* Dvar_RegisterBool(const char* name, const char* description, bool value, int flags)
	{
		game::DvarValue val = {};
		val.enabled = value;

		game::DvarLimits lim = {};
		lim.integer.min = 0;
		lim.integer.max = 1;

		return game::Dvar_RegisterVariant(name, game::boolean, flags, val, lim, description);
	}

	void init_offsets()
	{
		Dvar_RegisterVariant = SELECT(0x5C5100, 0x5EED90);
	}
}
