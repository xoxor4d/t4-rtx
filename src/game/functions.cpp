#include "std_include.hpp"

namespace game
{
	bool is_sp = false;
	bool is_mp = false;

	std::vector<std::string> loaded_modules;

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace sp
	{
		DxGlobals* dx = reinterpret_cast<DxGlobals*>(0x3BF3B04);
		clipMap_t* cm = reinterpret_cast<clipMap_t*>(0x1F41A00);
		r_global_permanent_t* rgp = reinterpret_cast<r_global_permanent_t*>(0x3BF1880);
		game::GfxBuffers* gfx_buf = reinterpret_cast<game::GfxBuffers*>(0x42C2000);

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
				add     esp, 4h;
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
	} // mp-end

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	GfxCmdBufSourceState* get_cmdbufsourcestate()
	{
		const auto out = reinterpret_cast<GfxCmdBufSourceState*>(game::is_mp ? *game::mp::gfxCmdBufSourceState_ptr : *game::sp::gfxCmdBufSourceState_ptr);
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
		data->next = game::is_sp ? *sp::cmd_ptr : nullptr;

		if (game::is_sp)
		{
			*game::sp::cmd_ptr = data;
		}
		else
		{
			__debugbreak(); // TODO
		}
	}
}
