#include "std_include.hpp"

#define HIDWORD(x)  (*((DWORD*)&(x)+1))

using namespace game::sp;

namespace components::sp
{
	void export_entity_string()
	{
		std::ofstream ents;

		if (const auto& fs_basepath = Dvar_FindVar("fs_basepath"); fs_basepath)
		{
			std::string filepath;
			filepath = fs_basepath->current.string;
			filepath += "\\t4rtx\\entity-export\\"s;

			std::filesystem::create_directories(filepath);

			std::string map_name = cm->mapEnts->name;
			utils::erase_substring(map_name, "maps/");
			utils::erase_substring(map_name, ".d3dbsp");

			ents.open(filepath + map_name + ".map");

			if (!ents.is_open())
			{
				return;
			}

			ents << cm->mapEnts->entityString << std::endl;

			for (auto i = 0u; i < cm->numStaticModels; i++)
			{
				game::vec3_t angles = {};

				float axis_inv[3][3] = {};
				utils::axis_transpose(cm->staticModelList[i].invScaledAxis, axis_inv);
				utils::axis_to_angles(axis_inv, angles);

				ents << "{" << std::endl;
				ents << R"("model" )" << "\""s + cm->staticModelList[i].xmodel->name << "\""s << std::endl;

				ents << R"("origin" )" << "\""s << cm->staticModelList[i].origin[0] << " "
					<< cm->staticModelList[i].origin[1] << " "
					<< cm->staticModelList[i].origin[2] << "\""s << std::endl;

				ents << R"("angles" )" << "\""s << angles[0] << " "
					<< angles[1] << " "
					<< angles[2] << "\""s << std::endl;

				ents << R"("classname" "misc_model")" << std::endl;
				ents << "}" << std::endl;
			}

			ents.close();
		}
	}
	
	void setup_rtx()
	{
		const auto dev = dx->device;
		const auto data = game::get_backenddata();
		const auto src = game::get_cmdbufsourcestate();

		// populate viewParms3D because R_Set3D needs it
		src->viewParms3D = &data->viewInfo->viewParms;

		// update world matrix (R_Set3D)
		R_Set3D(0, src); //utils::hook::call<void(__fastcall)(int, game::GfxCmdBufSourceState*)>(0x7244C0)(0, src); // updated SP

		// directly set matrices on the device so that rtx-remix finds the camera
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));
		dev->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&src->viewParms.viewMatrix.m));
		dev->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&src->viewParms.projectionMatrix.m));

		// needed for skysphere
		dev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (!flags::has_flag("no_fog"))
		{
			const float fog_start = 1.0f;
			dev->SetRenderState(D3DRS_FOGENABLE, TRUE);
			dev->SetRenderState(D3DRS_FOGCOLOR, fog::m_color.packed);
			dev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			dev->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fog_start));
			dev->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fog::m_max_distance));
		}

		if (!flags::has_flag("no_default_sky") && !main_module::skysphere_is_model_valid())
		{
			main_module::skysphere_spawn(4); // always spawn sunset
		}

		if (const auto& export_entities = Dvar_FindVar("export_entities");
			export_entities && std::string_view(export_entities->current.string) == "1")
		{
			export_entities->current.string = "0";
			export_entities->latched.string = "0";
			export_entities->modified = true;
			export_entity_string();
		}
	}

	__declspec(naked) void rb_draw3d_internal_stub()
	{
		const static uint32_t retn_addr = 0x6E8B9E;
		__asm
		{
			pushad;
			call	setup_rtx;
			popad;

			// og instructions
			sub     eax, 0;
			push    ebp;
			mov     ebp, [esp + 0xC];
			jmp		retn_addr;
		}
	}

	// ------------------------

	bool is_valid_technique_for_type(const game::Material* mat, const game::MaterialTechniqueType type)
	{
		if (mat
			&& mat->techniqueSet
			&& mat->techniqueSet->remappedTechniqueSet
			&& mat->techniqueSet->remappedTechniqueSet->techniques
			&& mat->techniqueSet->remappedTechniqueSet->techniques[type])
		{
			return true;
		}

		return false;
	}

	void switch_material(game::switch_material_t* swm, const char* material_name)
	{
		if (const auto	material = Material_RegisterHandle(material_name, 3);
			material)
		{
			swm->material = material;
			swm->technique = nullptr;

			if (is_valid_technique_for_type(material, swm->technique_type))
			{
				swm->technique = material->techniqueSet->remappedTechniqueSet->techniques[swm->technique_type];
			}

			swm->switch_material = true;
			return;
		}

		// return stock material if the above failed
		swm->material = swm->current_material;
	}

	void switch_technique(game::switch_material_t* swm, game::Material* material)
	{
		if (material)
		{
			swm->technique = nullptr;

			if (is_valid_technique_for_type(material, swm->technique_type))
			{
				swm->technique = material->techniqueSet->remappedTechniqueSet->techniques[swm->technique_type];
			}

			swm->switch_technique = true;
			return;
		}

		// return stock technique if the above failed
		swm->technique = swm->current_technique;
	}

	int r_set_material(game::MaterialTechniqueType type, game::GfxCmdBufSourceState* src, game::GfxCmdBufState* state, game::GfxDrawSurf drawSurf)
	{
		//const auto rgp = reinterpret_cast<game::r_global_permanent_t*>(0x3BF1880); // updated SP

		game::switch_material_t mat = {};

		mat.current_material = game::sp::rgp->sortedMaterials[(HIDWORD(drawSurf.packed) >> 1) & 2047];
		mat.current_technique = mat.current_material->techniqueSet->remappedTechniqueSet->techniques[type];

		mat.material = mat.current_material;
		mat.technique = mat.current_technique;
		mat.technique_type = type;


		if (utils::starts_with(mat.current_material->info.name, "wc/sky_"))
		{
			mat.technique_type = game::TECHNIQUE_UNLIT;
			switch_material(&mat, "mc/mtl_test_sphere_silver");
		}

		// remove skybox model
		if (utils::starts_with(mat.current_material->info.name, "mc/mtl_skybox"))
		{
			return 0;
		}

		/*if (utils::starts_with(mat.current_material->info.name, "mc/mtl_rtx_"))
		{
			int x = 1;
		}*/

		/*if (std::string_view(mat.current_material->techniqueSet->name) == std::string_view("wc_water"))
		{
			mat.switch_technique_type = true;
			mat.technique_type = TECHNIQUE_LIT;
		}*/

		if (!mat.switch_material && !mat.switch_technique && !mat.switch_technique_type)
		{
			if (state->origMaterial)
			{
				state->material = state->origMaterial;
			}
			if (state->origTechType)
			{
				state->techType = state->origTechType;
			}
		}

		// save the original material
		state->origMaterial = state->material;

		// only switch to a different technique_type
		if (mat.switch_technique_type)
		{
			switch_technique(&mat, mat.current_material);
		}

		state->material = mat.material;
		state->technique = mat.technique;

		if (!state->technique)
		{
			return 0;
		}

		if (!mat.switch_material && !mat.switch_technique && !mat.switch_technique_type)
		{
			if ((mat.technique_type == game::TECHNIQUE_EMISSIVE || mat.technique_type == game::TECHNIQUE_UNLIT) && (state->technique->flags & 0x10) != 0 && !*&src->pad00[0x48])
			{
				return 0;
			}
		}

		/*if ((drawSurf.packed & 0xF0000) != 0)
		{
			float v10 = ((drawSurf.packed >> 16) & 0xF) * *&dword_82A3D4;
			*&src->pad0[560] = v10;
			*&src->pad0[564] = v10;
			*&src->pad0[568] = v10;
			*&src->pad0[572] = v10;
			++*&src->pad00[102];
		}

		if ((&dword_3C00000 & HIDWORD(drawSurf.packed)) == 0 && (current_material->info.gameFlags & 8) != 0)
		{
			return 0;
		}*/

		state->origTechType = state->techType;
		state->techType = mat.technique_type;

		return 1;
	}

	void register_rtx_dvars()
	{
		if (const auto var = Dvar_FindVar("r_lodScaleRigid"); var)
		{
			var->current.value = 1.0f;
			var->domain.value.min = 0.0f;
			var->domain.value.max = FLT_MAX;
			var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_lodScaleSkinned"); var)
		{
			var->domain.value.min = 0.0f;
			var->domain.value.max = FLT_MAX;
			var->flags = game::dvar_flags::userinfo;
		}

		//if (const auto var = Dvar_FindVar("r_znear"); var)
		//{
		//	var->current.value = 40.0f; // 10.0 works but still a little wobble at larger distances
		//	var->domain.value.min = 0.0f;
		//	var->domain.value.max = FLT_MAX;
		//	var->flags = game::dvar_flags::userinfo;
		//}

		//if (const auto var = Dvar_FindVar("r_znear_depthhack"); var)
		//{
		//	var->current.value = 39.9805f; // - 9.99512f see ^
		//	var->domain.value.min = 0.0f;
		//	var->domain.value.max = FLT_MAX;
		//	var->flags = game::dvar_flags::userinfo;
		//}

		if (const auto var = Dvar_FindVar("r_skinCache"); var)
		{
			var->current.enabled = false;
		}

		if (const auto var = Dvar_FindVar("r_fastSkin"); var)
		{
			var->current.enabled = false;
		}

		if (const auto var = Dvar_FindVar("r_distortion"); var)
		{
			var->current.enabled = false;
		}

		if (const auto var = Dvar_FindVar("r_depthprepass"); var)
		{
			var->current.enabled = false;
		}

		if (const auto var = Dvar_FindVar("r_smc_enable"); var)
		{
			var->current.enabled = false;
		}

		if (const auto var = Dvar_FindVar("r_smp_backend"); var)
		{
			var->current.enabled = false;
		}

		if (const auto var = Dvar_FindVar("r_smp_worker"); var)
		{
			var->current.enabled = false;
		}

		// force static models to lod 0
		if (const auto var = Dvar_FindVar("r_warm_static"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_fovScaleThresholdRigid"); var)
		{
			var->current.value = 10000.0f;
		}

		//if (const auto var = Dvar_FindVar("r_fovScaleThresholdSkinned"); var)
		//{
		//	var->current.value = 2.0f;
		//}

		// disable culling
		if (const auto var = Dvar_FindVar("r_warm_dpvs"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
		}

		// batch surfaces (def. needed)
		if (const auto var = Dvar_FindVar("r_pretess"); var)
		{
			var->current.enabled = true;
		}

		// #

		// enable cheats
		if (const auto var = Dvar_FindVar("sv_cheats"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
		}

		// enable console
		if (const auto var = Dvar_FindVar("monkeytoy"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}
	}

	__declspec(naked) void register_dvars_stub()
	{
		const static uint32_t stock_func = 0x70E8F0;
		const static uint32_t retn_addr = 0x6D6B45;
		__asm
		{
			call	stock_func;

			pushad;
			call	register_rtx_dvars;
			popad;

			jmp		retn_addr;
		}
	}


	// *
	// load custom fastfile containing required custom assets

	void load_common_fast_files()
	{
		const char** zone_code_post_gfx = reinterpret_cast<const char**>(0x3BF6800);
		const char** zone_patch = reinterpret_cast<const char**>(0x3BF6804);
		const char** zone_ui = reinterpret_cast<const char**>(0x3BF6808);
		const char** zone_common = reinterpret_cast<const char**>(0x3BF680C);
		const char** zone_localized_common = reinterpret_cast<const char**>(0x3BF6814);
		const char** zone_mod = reinterpret_cast<const char**>(0x3BF6818);

		int i = 0;
		game::XZoneInfo xzone_info_stack[8];

		// ------------------------------------

		xzone_info_stack[i].name = *zone_code_post_gfx;
		xzone_info_stack[i].allocFlags = 4;
		xzone_info_stack[i].freeFlags = 0;
		++i;

		// ------------------------------------

		// unused in sp
		/*if (zone_localized_code_post_gfx)
		{
			xzone_info_stack[i].name = *game::zone_localized_code_post_gfx_mp;
			xzone_info_stack[i].allocFlags = game::XZONE_FLAGS::XZONE_LOC_POST_GFX;
			xzone_info_stack[i].freeFlags = game::XZONE_FLAGS::XZONE_LOC_POST_GFX_FREE;
			++i;
		}*/

		if (*zone_mod)
		{
			xzone_info_stack[i].name = *zone_mod;
			xzone_info_stack[i].allocFlags = 0x800;
			xzone_info_stack[i].freeFlags = 0;
			++i;

			game::sp::DB_LoadXAssets(&xzone_info_stack[0], i, 0);

			utils::hook::call<void(__cdecl)()>(0x6F6CE0)(); // R_BeginRemoteScreenUpdate
			utils::hook::call<void(__cdecl)()>(0x5A3320)(); // no clue
			utils::hook::call<void(__cdecl)()>(0x6F6D60)(); // R_EndRemoteScreenUpdate
			utils::hook::call<void(__cdecl)()>(0x5FDBF0)(); // no clue
			utils::hook::call<void(__cdecl)()>(0x48E560)(); // DB_SyncXAssets

			// start a new zone stack
			i = 0;
		}

		if (game::sp::DB_FileExists("xcommon_rtx", game::DB_FILE_EXISTS_PATH::DB_PATH_ZONE))
		{
			xzone_info_stack[i].name = "xcommon_rtx";
			xzone_info_stack[i].allocFlags = 4;
			xzone_info_stack[i].freeFlags = 0;
			++i;
		}

		// ---------------------------------------------------------------------------------------------------------

		if (*zone_ui) // not loaded when starting dedicated servers
		{
			xzone_info_stack[i].name = *zone_ui;
			xzone_info_stack[i].allocFlags = 16;
			xzone_info_stack[i].freeFlags = 0;
			++i;
		}

		if (*zone_localized_common) // not loaded on when starting dedicated servers
		{
			xzone_info_stack[i].name = *zone_localized_common;
			xzone_info_stack[i].allocFlags = 1;
			xzone_info_stack[i].freeFlags = 0;
			++i;
		}

		xzone_info_stack[i].name = *zone_common;
		xzone_info_stack[i].allocFlags = 8;
		xzone_info_stack[i].freeFlags = 0;
		++i;

		if (*zone_patch) // not loaded on when starting dedicated servers
		{
			xzone_info_stack[i].name = *zone_patch;
			xzone_info_stack[i].allocFlags = 512;
			xzone_info_stack[i].freeFlags = 0;
			++i;
		}

		// ------------------------------------

		game::sp::DB_LoadXAssets(&xzone_info_stack[0], i, 0);
	}


	// *
	// fix resolution issues by removing duplicates returned by EnumAdapterModes

	namespace resolution
	{
		auto hash = [](const _D3DDISPLAYMODE& d) { return d.Width + 10 * d.Height + d.RefreshRate; };
		auto equal = [](const _D3DDISPLAYMODE& d1, const _D3DDISPLAYMODE& d2) { return d1.Width == d2.Width && d1.Height == d2.Height && d1.RefreshRate == d2.RefreshRate; };
		std::unordered_set<_D3DDISPLAYMODE, decltype(hash), decltype(equal)> modes(256, hash, equal);

		int enum_adapter_modes_intercept(std::uint32_t adapter_index, std::uint32_t mode_index)
		{
			_D3DDISPLAYMODE current = {};
			const auto hr = game::sp::dx->d3d9->EnumAdapterModes(adapter_index, D3DFMT_X8R8G8B8, mode_index, &current) < 0;
			modes.emplace(current);
			return hr;
		}

		__declspec(naked) void R_EnumDisplayModes_stub()
		{
			const static uint32_t retn_addr = 0x6D5CF2;
			__asm
			{
				push	esi; // mode index
				push	ebx; // adapter index
				call	enum_adapter_modes_intercept;
				add		esp, 8;
				jmp		retn_addr;
			}
		}

		void enum_adapter_modes_write_array()
		{
			std::uint32_t idx = 0;
			for (auto& m : modes)
			{
				if (idx >= 256)
				{
					game::sp::Com_PrintMessage(0, "EnumAdapterModes : Failed to grab all possible resolutions. Array to small!\n", 0);
					break;
				}

				memcpy(&game::sp::dx->displayModes[idx], &m, sizeof(_D3DDISPLAYMODE));
				idx++;
			}
		}

		__declspec(naked) void R_EnumDisplayModes_stub2()
		{
			const static uint32_t R_CompareDisplayModes_addr = 0x6D5C70;
			const static uint32_t retn_addr = 0x6D5D2E;
			__asm
			{
				pushad;
				call	enum_adapter_modes_write_array;
				popad;

				push	R_CompareDisplayModes_addr;
				jmp		retn_addr;
			}
		}
	}

	void fix_aspect_ratio(int* window_parms)
	{
		*reinterpret_cast<float*>(0x3BED844) = static_cast<float>(window_parms[7]) / static_cast<float>(window_parms[8]);
	}

	void __declspec(naked) fix_aspect_ratio_stub()
	{
		const static uint32_t retn_addr = 0x6D504E;
		__asm
		{
			pop		eax;
			pushad;
			push	eax;
			call	fix_aspect_ratio;
			add		esp, 4;
			popad;
			push	eax;

			jmp		retn_addr;
		}
	}


	// *
	// skysphere

	const char* main_module::skysphere_get_name_for_variant(int variant)
	{
		switch (variant)
		{
		default:
		case 0: return "rtx_skysphere_oceanrock";
		case 1: return "rtx_skysphere_desert";
		case 2: return "rtx_skysphere_night";
		case 3: return "rtx_skysphere_overcast";
		case 4: return "rtx_skysphere_sunset_clouds";
		}
	}

	bool main_module::skysphere_is_model_valid()
	{
		// if not spawned an entity yet
		if (!skysphere_spawned)
		{
			return false;
		}

		// check if the entity is valid (player changed level etc.)
		if (skysphere_model == nullptr || skysphere_model->classname == 0
			|| skysphere_model->model != game::sp::G_ModelIndex(skysphere_get_name_for_variant(skysphere_variant)))
		{
			return false;
		}

		return true;
	}

	void main_module::skysphere_change_model(int variant)
	{
		const std::int16_t model_index = game::sp::G_ModelIndex(skysphere_get_name_for_variant(variant));

		skysphere_model->model = model_index;
		skysphere_model->s.index.xmodel = model_index;
		skysphere_model->classname = game::sp::scr_const->script_model;
		game::sp::G_DObjUpdate(&skysphere_model->s);

		skysphere_variant = variant;
	}

	void main_module::skysphere_spawn(int variant)
	{
		if (skysphere_is_model_valid())
		{
			skysphere_change_model(variant);
			return;
		}

		// needs :: 
		// s->index = modelIndex
		// linked = 0x1;
		// svFlags = 0x04; // even = visible, uneven = hidden

		const std::int16_t model_index = game::sp::G_ModelIndex(skysphere_get_name_for_variant(variant));

		skysphere_model = game::sp::G_Spawn();
		skysphere_model->classname = game::sp::scr_const->script_model;
		skysphere_model->model = model_index;
		skysphere_model->s.index.xmodel = model_index;
		skysphere_model->r.svFlags = 0x04;
		skysphere_model->r.linked = 0x1;

		//Game::G_SetOrigin(skysphere_model, skysphere_model_origin);
		skysphere_model->s.lerp.pos.trBase[0] = 0.0f;
		skysphere_model->s.lerp.pos.trBase[1] = 0.0f;
		skysphere_model->s.lerp.pos.trBase[2] = 0.0f;
		skysphere_model->s.lerp.pos.trType = game::TR_STATIONARY;
		skysphere_model->s.lerp.pos.trTime = 0;
		skysphere_model->s.lerp.pos.trDuration = 0;
		skysphere_model->s.lerp.pos.trDelta[0] = 0.0;
		skysphere_model->s.lerp.pos.trDelta[1] = 0.0;
		skysphere_model->s.lerp.pos.trDelta[2] = 0.0;
		skysphere_model->r.currentOrigin[0] = 0.0f;
		skysphere_model->r.currentOrigin[1] = 0.0f;
		skysphere_model->r.currentOrigin[2] = 0.0f;

		//Game::G_SetAngles(skysphere_model, skysphere_model_rotation);
		skysphere_model->s.lerp.apos.trBase[0] = 0.0f;
		skysphere_model->s.lerp.apos.trBase[1] = 0.0f;
		skysphere_model->s.lerp.apos.trBase[2] = 0.0f;
		skysphere_model->s.lerp.apos.trType = game::TR_STATIONARY;
		skysphere_model->s.lerp.apos.trTime = 0;
		skysphere_model->s.lerp.apos.trDuration = 0;
		skysphere_model->s.lerp.apos.trDelta[0] = 0.0;
		skysphere_model->s.lerp.apos.trDelta[1] = 0.0;
		skysphere_model->s.lerp.apos.trDelta[2] = 0.0;
		skysphere_model->r.currentAngles[0] = 0.0f;
		skysphere_model->r.currentAngles[1] = 0.0f;
		skysphere_model->r.currentAngles[2] = 0.0f;

		game::sp::G_CallSpawnEntity(skysphere_model);

		skysphere_spawned = true;
		skysphere_variant = variant;
	}

	// ------------------------------------------------------------------------

	__declspec(naked) void force_lod_stub01()
	{
		const static uint32_t retn_addr = 0x732CFD;
		__asm
		{
			xor		ebx, ebx;	// clear
			mov		ebx, 0;		// LOD

			jmp		retn_addr;
		}
	}

	__declspec(naked) void force_lod_stub02()
	{
		const static uint32_t retn_addr = 0x7332A9;
		__asm
		{
			xor		esi, esi;	// clear
			mov		esi, 0;		// LOD

			jmp		retn_addr;
		}
	}

	// > fixed_function::init_fixed_function_buffers_stub
	void main_module::on_map_load()
	{
		fog::get()->set_settings_for_loaded_map();
	}

	// > fixed_function::free_fixed_function_buffers_stub
	void main_module::on_map_shutdown()
	{
	}

	main_module::main_module()
	{
		// *
		// general

		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		utils::hook::nop(0x6E8B96, 8); utils::hook(0x6E8B96, rb_draw3d_internal_stub, HOOK_JUMP).install()->quick();

		// hook R_SetMaterial - material/technique replacing
		utils::hook(0x741F1E, r_set_material, HOOK_CALL).install()->quick();

		// precaching xmodels beyond level load (dynamic skysphere spawning)
		utils::hook::set<BYTE>(0x54A4D6, 0xEB);

		// load custom fastfile containing required assets
		utils::hook(0x6D63A5, load_common_fast_files, HOOK_CALL).install()->quick();

		// dxvk's 'EnumAdapterModes' returns a lot of duplicates and the games array only has a capacity of 256 which is not enough depending on max res. and refreshrate
		// fix resolution issues by removing duplicates returned by EnumAdapterModes - then write the array ourselfs
		utils::hook(0x6D5CE2, sp::resolution::R_EnumDisplayModes_stub, HOOK_JUMP).install()->quick();
		utils::hook(0x6D5D29, sp::resolution::R_EnumDisplayModes_stub2, HOOK_JUMP).install()->quick();
		utils::hook::set<BYTE>(0x6D5CD0 + 2, 0x04); // set max array size check to 1024 (check within loop)

		// Remove Impure client (iwd) check - needed?
		utils::hook::nop(0x5DBC7F, 30);

		// stuck in some loop 'Com_Quit_f'
		utils::hook::nop(0x5FEA01, 5);

		// don't play intro video - allows to devmap into a map via commandline
		utils::hook::nop(0x59D68B, 5); // mp: 0x564CB9

		// :*
		utils::hook(0x6D4FB3, fix_aspect_ratio_stub, HOOK_JUMP).install()->quick();

		// ------------------------------------------------------------------------

		// stub after 'R_InitGraphicsApi' (NVAPI Entry) to re-register stock dvars
		utils::hook(0x6D6B40, register_dvars_stub, HOOK_JUMP).install()->quick();

		// un-cheat + userinfo flag for fx_enable
		utils::hook::set<BYTE>(0x4A4D16 + 1, 0x01); // was 0x80

		// un-cheat + userinfo flag for sv_cheats
		utils::hook::set<BYTE>(0x70B92D + 1, 0x01); // was 0x48
		utils::hook::set<BYTE>(0x70B92D + 1, 0x01);

		// ------------------------------------------------------------------------


		//if (flags::has_flag("disable_culling"))
		//{
		//	// R_AddWorldSurfacesPortalWalk :: less culling
		//	utils::hook::set<BYTE>(0x6A945E, 0xEB);

		//	// R_AddAabbTreeSurfacesInFrustum_r :: disable all surface culling (bad fps)
		//	utils::hook::nop(0x6F6F29, 6);
		//	utils::hook::set<BYTE>(0x6F6F65, 0xEB);

		//	// TODO: entities
		//}

		// disable dynent drawing (could have stable hashes but no stable lods right now)
		//utils::hook::nop(0x6DD7DD, 5);

		// force static model LOD via 'r_warm_static' dvar (R_AddAllStaticModelSurfacesCamera)
		utils::hook::nop(0x732CF7, 6);
		utils::hook(0x732CF7, force_lod_stub01, HOOK_JUMP).install()->quick();

		// ^ (R_AddAllStaticModelSurfacesRangeSunShadow)
		utils::hook::nop(0x7332A3, 6);
		utils::hook(0x7332A3, force_lod_stub02, HOOK_JUMP).install()->quick();

		// remove 'r_warm_static' check that adds all meshes to the scene no matter what 'r_lodScaleRigid' is set to (R_AddAllStaticModelSurfacesCamera)
		// jz (0F 84 AC 01 00 00) to jmp (E9 AD 01 00 00 + 0x90)
		utils::hook::set<BYTE>(0x732B95, 0xE9);
		utils::hook::set<BYTE>(0x732B95 + 1, 0xAD); // < yes
		utils::hook::set<BYTE>(0x732B95 + 2, 0x01);
		utils::hook::set<BYTE>(0x732B95 + 3, 0x00);
		utils::hook::set<BYTE>(0x732B95 + 4, 0x00);
		utils::hook::set<BYTE>(0x732B95 + 5, 0x90);

		// stop 'r_warm_dpvs' dvar from resetting itself je 0x74 -> jmp 0xEB
		// can be used as anti cull
		utils::hook::set<BYTE>(0x6DDED8, 0xEB);

		// now using dvar 'r_fovScaleThresholdRigid'
		//utils::hook::nop(0x6DEA6F, 4); // do not scale lodScaleRigid with fov
		//utils::hook::nop(0x6DEA78, 4); // do not scale lodBiasRigid with fov
		//utils::hook::nop(0x6DEAC2, 4); // do not scale lodScaleSkinned with fov
		//utils::hook::nop(0x6DEACB, 4); // do not scale lodBiasSkinned with fov

		// ------------------------------------------------------------------------

#ifdef GIT_DESCRIBE
		static auto version_str = std::string("t4-rtx-"s + GIT_DESCRIBE + " > "s);
#else
		static auto version_str = "t4-rtx > "s;
#endif

		// console string ;)
		utils::hook::set<const char*>(0x472E13 + 1, version_str.c_str());

		// no forward/backslash for console cmds
		//utils::hook::nop(0x493DEF, 5);

		command::add("rtx_sky_clear", [](command::params) { main_module::skysphere_spawn(0); });
		command::add("rtx_sky_desert", [](command::params) { main_module::skysphere_spawn(1); });
		command::add("rtx_sky_night", [](command::params) { main_module::skysphere_spawn(2); });
		command::add("rtx_sky_overcast", [](command::params) { main_module::skysphere_spawn(3); });
		command::add("rtx_sky_sunset", [](command::params) { main_module::skysphere_spawn(4); });

		/*command::add("fog_update", [this](command::params)
		{
			update_fog_settings();
		});*/

		command::add("noborder", [this](command::params)
		{
			const auto hwnd = game::sp::dx->windows->hwnd ? game::sp::dx->windows->hwnd : FindWindow(nullptr, L"Call of Duty®");

			// calculate titlebar height
			RECT w, c; GetWindowRect(hwnd, &w); GetClientRect(hwnd, &c);
			sp::main_module::noborder_titlebar_height = (w.bottom - w.top) - (c.bottom - c.top);

			SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(hwnd, nullptr, 0, 0, game::sp::dx->windows->width, game::sp::dx->windows->height, SWP_SHOWWINDOW | SWP_NOACTIVATE);
		});

		command::add("windowed", [this](command::params)
		{
			if (sp::main_module::noborder_titlebar_height)
			{
				const auto hwnd = game::sp::dx->windows->hwnd ? game::sp::dx->windows->hwnd : FindWindow(nullptr, L"Call of Duty®");
				SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
				SetWindowPos(hwnd, nullptr, 0, 0, game::sp::dx->windows->width, game::sp::dx->windows->height + sp::main_module::noborder_titlebar_height, SWP_SHOWWINDOW | SWP_NOACTIVATE);
			}
		});
	}
}
