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
		GfxWorld* gfx_world = reinterpret_cast<GfxWorld*>(0x3DCB4E0);
		ComWorld* com_world = reinterpret_cast<ComWorld*>(0x1F551F0);

		// scene_sceneDObjCount - 0x3D6988C
		// scene_sceneDObj - 0x3D698FE
		// scene_sceneModelCount - 0x3D8A490
		// scene_sceneModel - 0x3D8A4B0


		r_global_permanent_t* rgp = reinterpret_cast<r_global_permanent_t*>(0x3BF1880);
		game::GfxBuffers* gfx_buf = reinterpret_cast<game::GfxBuffers*>(0x42C2000);
		game::cg_s* cgs = reinterpret_cast<game::cg_s*>(0x34732B8);

		game::clientUIActive_s* clientUI = reinterpret_cast<game::clientUIActive_s*>(0x305841C);
		game::clientActive_s* client = reinterpret_cast<game::clientActive_s*>(0x3058528);

		game::DpvsGlob* dpvsGlob = reinterpret_cast<game::DpvsGlob*>(0x3DA8D80);

		CmdArgs* cmd_args = reinterpret_cast<CmdArgs*>(0x1F41670);
		cmd_function_s** cmd_ptr = reinterpret_cast<cmd_function_s**>(0x1F416F4);

		Cmd_ExecuteSingleCommand_t	Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x594ED0);

		Com_PrintMessage_t Com_PrintMessage = Com_PrintMessage_t(0x59A170);
		Com_Error_t Com_Error = Com_Error_t(0x59AC50);
		DB_EnumXAssets_FastFile_t DB_EnumXAssets_FastFile = DB_EnumXAssets_FastFile_t(0x48D560);
		DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x48E7B0);

		game::weaponInfo_s* cg_weaponsArray = reinterpret_cast<game::weaponInfo_s*>(0x3463C40);
		scr_const_t* scr_const = reinterpret_cast<scr_const_t*>(0x1F33B90);

		int* Scr_GetNumParamArray = reinterpret_cast<int*>(0x3BD471C);
		int* Scr_GetTypeArray = reinterpret_cast<int*>(0x3BD4710);
		int* gScrMemTreePub_mtbuffer = reinterpret_cast <int*>(0x3702390);
		int* level_time = reinterpret_cast<int*>(0x18F5D88 + 0x1040); // level_locals_s time
		std::uint32_t* level_num_entities = reinterpret_cast<std::uint32_t*>(0x18F5D88 + 0xC); // level_locals_s num_entities
		game::gentity_s* g_entities = reinterpret_cast<game::gentity_s*>(0x176C6F0);
		game::centity_s* cg_entitiesArray = reinterpret_cast<game::centity_s*>(0x35D39F0);
		game::fake_centity_s* cg_fakeEntitiesArray = reinterpret_cast<game::fake_centity_s*>(0x339CC38);

		BuiltinMethod Scr_GetMethod(const char** pName, int* typ)
		{
			BuiltinMethod answer;
			const static uint32_t Scr_GetMethod_func = 0x530630;
			__asm
			{
				mov		edi, typ;
				mov		esi, pName;
				call	Scr_GetMethod_func;
				mov		answer, eax;
			}

			return answer;
		}

		int Scr_GetNumParams(int scrInstance)
		{
			return Scr_GetNumParamArray[4296 * scrInstance];
		}

		int Scr_GetType(int scrInstance, unsigned int paramNum)
		{
			return *reinterpret_cast<int*>(Scr_GetTypeArray[4296 * scrInstance] - 8 * paramNum + 4);
		}

		const char* Scr_GetString(int scrInstance, unsigned int paramNum)
		{
			char* result = nullptr;
			unsigned int str_pos_in_glob = 0;
			const static uint32_t Scr_GetString_func = 0x699F30;
			__asm
			{
				push	paramNum;
				mov		eax, scrInstance;
				call	Scr_GetString_func;
				add		esp, 4;
				mov		str_pos_in_glob, eax;
			}

			if (str_pos_in_glob)
			{
				result = reinterpret_cast<char*>(gScrMemTreePub_mtbuffer[scrInstance] + 12 * str_pos_in_glob + 4);
			}

			return const_cast<const char*>(result);
		}

		int Scr_GetInt(int scrInstance, unsigned int paramNum)
		{
			int result;
			const static uint32_t Scr_GetInt_func = 0x699C50;
			__asm
			{
				mov		ecx, paramNum;
				mov		eax, scrInstance;
				call	Scr_GetInt_func;
				mov		result, eax;
			}

			return result;
		}

		float Scr_GetFloat(int scrInstance, unsigned int paramNum)
		{
			float result;
			const static uint32_t Scr_GetFloat_func = 0x699E90;
			__asm
			{
				mov		 ecx, paramNum;
				mov		 eax, scrInstance;
				call	 Scr_GetFloat_func;

				// for double
				//cvtss2sd xmm0, xmm0; // to double precision
				//movsd	 result, xmm0; // move scalar double precision

				movss result, xmm0;
			}

			return result;
		}

		void Scr_GetVector(int scrInstance, float* vectorValue, unsigned int paramNum)
		{
			const static uint32_t Scr_GetVector_func = 0x69A220;
			__asm
			{
				push	paramNum;
				mov		ecx, vectorValue;
				mov		eax, scrInstance;
				call	Scr_GetVector_func;
				add		esp, 4;
			}
		}

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

		// some wrapper func for FX_SpawnEffect (FX_SpawnEffect is not returning a ptr but rather a handle (uint16))
		int FX_SpawnOrientedEffect(const float* axis, game::FxEffectDef* def, int msec_begin, const float* origin)
		{
			const static uint32_t func_addr = 0x4AD6B0;
			__asm
			{
				push	origin;
				push	msec_begin;
				push	def;
				mov		ecx, 1023; // bone index?
				mov		edx, axis;
				call	func_addr;
				add		esp, 12;
			}
		}

		void FX_KillEffect(game::FxEffect* def)
		{
			const static uint32_t func_addr = 0x4ADFB0;
			__asm
			{
				pushad;
				push	def;
				mov     esi, [0x16D7100]; // fx_systemPool
				call	func_addr;
				add		esp, 4;
				popad;
			}
		}

		void draw_text_with_engine(float x, float y, float scale_x, float scale_y, const char* font, const float* color, const char* text)
		{
			void* fontHandle = R_RegisterFont(font);
			R_AddCmdDrawTextASM(text, 0x7FFFFFFF, fontHandle, x, y, scale_x, scale_y, 0.0f, color, 0);
		}

		void R_AddCmdDrawTextASM(const char* text, int max_chars, void* font, float x, float y, float x_scale, float y_scale, float rotation, const float* color, int style)
		{
			const static uint32_t R_AddCmdDrawText_func = 0x6F5F10;
			__asm
			{
				push	style;
				sub     esp, 0x14;

				fld		rotation;
				fstp	[esp + 0x10];

				fld		y_scale;
				fstp	[esp + 0x0C];

				fld		x_scale;
				fstp	[esp + 0x8];

				fld		y;
				fstp	[esp + 0x4];

				fld		x;
				fstp	[esp];

				push	font;
				push	max_chars;
				push	text;
				mov		ecx, [color];

				call	R_AddCmdDrawText_func;
				add		esp, 0x24;
			}
		}

		int G_DObjGetWorldTagPos(int scr_const_tag, int ent_num, float* pos_out)
		{
			const static uint32_t G_DObjGetWorldTagPos_func = 0x54E6A0;
			__asm
			{
				push	pos_out;
				push	ent_num;
				mov		eax, scr_const_tag;
				call	G_DObjGetWorldTagPos_func;
				add		esp, 8;
			}
		}

		int DObjGetBoneIndex(DObj_s* obj /*ecx*/, int tag_name, BYTE* bone_index)
		{
			const static uint32_t func_addr = 0x60C420;
			int result = 0;
			__asm
			{
				push	bone_index;
				push	tag_name;
				mov		ecx, obj;
				call	func_addr;
				add		esp, 8;
				mov		result, eax;
			}

			return result;
		}

		int CG_DObjGetWorldBoneMatrix(cpose_t* pose /*eax*/, int bone_index /*ecx*/, DObj_s* obj /*edi*/, float* axis, float* origin)
		{
			const static uint32_t func_addr = 0x442F10;
			int result = 0;
			__asm
			{
				push	origin;
				push	axis;
				mov		edi, obj;
				mov		ecx, bone_index;
				mov		eax, pose;
				call	func_addr;
				add		esp, 8;
				mov		result, eax;
			}

			return result;
		}

		// #
		// portal related

		void R_AddCellSurfacesAndCullGroupsInFrustumDelayed(GfxCell* cell /*eax*/, DpvsPlane* planes /*edi*/, int planeCount, int frustumPlaneCount)
		{
			const static uint32_t func_addr = 0x6E3BF0;
			__asm
			{
				push	frustumPlaneCount;
				push	planeCount;
				mov		edi, planes;
				mov		eax, cell;
				call	func_addr;
				add		esp, 8;
			}
		}

		void R_VisitPortals(int plane_count /*eax*/, GfxCell* cell, DpvsPlane* parent_plane, DpvsPlane* planes)
		{
			const static uint32_t func_addr = 0x6E4690;
			__asm
			{
				push	planes;
				push	parent_plane;
				push	cell;
				mov		eax, plane_count;
				call	func_addr;
				add		esp, 12;
			}
		}

		// db
		int** DB_XAssetPool = reinterpret_cast<int**>(0x8DC828); // mp: 0x8D0958
		unsigned int* g_poolSize = reinterpret_cast<unsigned int*>(0x8DC5D0); // mp: 0x8D06E8
		DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandler = reinterpret_cast<DB_GetXAssetSizeHandler_t*>(0x8DCC18); // mp: 0x8D0D48

		int DB_GetXAssetTypeSize(int type)
		{
			return DB_GetXAssetSizeHandler[type]();
		}

		void* DB_ReallocXAssetPool(int type, unsigned int size)
		{
			const auto s = DB_GetXAssetTypeSize(type);
			const auto pool_entry = utils::memory::allocate(size * s);

			DB_XAssetPool[type] = static_cast<int*>(pool_entry);
			g_poolSize[type] = size;

			return pool_entry;
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

		// db
		int** DB_XAssetPool = reinterpret_cast<int**>(0x8D0958);
		unsigned int* g_poolSize = reinterpret_cast<unsigned int*>(0x8D06E8);
		DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandler = reinterpret_cast<DB_GetXAssetSizeHandler_t*>(0x8D0D48);

		int DB_GetXAssetTypeSize(int type)
		{
			return DB_GetXAssetSizeHandler[type]();
		}

		void* DB_ReallocXAssetPool(int type, unsigned int size)
		{
			const auto s = DB_GetXAssetTypeSize(type);
			const auto pool_entry = utils::memory::allocate(size * s);

			DB_XAssetPool[type] = static_cast<int*>(pool_entry);
			g_poolSize[type] = size;

			return pool_entry;
		}
	} // mp-end

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	IDirect3DDevice9* get_device()
	{
		return game::is_mp ? game::mp::dx->device : game::sp::dx->device;
	}

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

	dvar_s* Dvar_RegisterInt(const char* name, int value, int min, int max, int flags, const char* description)
	{
		game::DvarValue val;
		val.integer = value;

		game::DvarLimits lim;
		lim.integer.min = min;
		lim.integer.max = max;

		return game::Dvar_RegisterVariant(name, game::dvar_type::integer, flags, val, lim, description);
	}

	dvar_s* Dvar_RegisterFloat(const char* name, float value, float min, float max, game::dvar_flags flags, const char* description)
	{
		game::DvarValue val = {};
		val.value = value;

		game::DvarLimits lim = {};
		lim.value.min = min;
		lim.value.max = max;

		return game::Dvar_RegisterVariant(name, game::dvar_type::value, flags, val, lim, description);
	}

	dvar_s* Dvar_RegisterVec3(const char* name, float x, float y, float z, float min, float max, game::dvar_flags flags, const char* description)
	{
		game::DvarValue val = {};
		val.vector[0] = x;
		val.vector[1] = y;
		val.vector[2] = z;

		game::DvarLimits lim = {};
		lim.vector.min = min;
		lim.vector.max = max;

		return game::Dvar_RegisterVariant(name, game::dvar_type::vec3, flags, val, lim, description);
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

	XAssetHeader DB_FindXAssetHeader(XAssetType type, const char* name)
	{
		typedef XAssetHeader(*DB_FindXAssetHeader_t)(XAssetType, const char*, int, int);
		if (game::is_sp)
		{
			auto sp_func = reinterpret_cast<DB_FindXAssetHeader_t>(0x48DA30);
			return sp_func(type, name, 1, -1);
		}
		else
		{
			return (XAssetHeader) nullptr;
		}
	}

	void init_offsets()
	{
		Dvar_RegisterVariant = SELECT(0x5C5100, 0x5EED90);
	}
}
