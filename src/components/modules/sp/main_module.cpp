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
			filepath += "\\t4rtx-export\\"s;

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

	void spawn_light()
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_POINT;
		light.Diffuse.r = 2.0f;
		light.Diffuse.g = 0.0f;
		light.Diffuse.b = 0.0f;

		light.Position.x = 0.0;
		light.Position.y = 0.0;
		light.Position.z = 70.0;

		light.Range = 2000.0f;

		const auto dev = dx->device;
		dev->SetLight(0, &light);
		dev->LightEnable(0, TRUE);
		dev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	void setup_rtx()
	{
		const auto dev = dx->device;
		const auto data = game::get_backenddata();
		const auto src = game::get_cmdbufsourcestate();

		// populate viewParms3D because R_Set3D needs it
		src->viewParms3D = &data->viewInfo->viewParms;

		// update world matrix (R_Set3D)
		//utils::hook::call<void(__fastcall)(int, game::GfxCmdBufSourceState*)>(0x7244C0)(0, src); // updated SP
		R_Set3D(0, src);

		// directly set matrices on the device so that rtx-remix finds the camera
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));
		dev->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&src->viewParms.viewMatrix.m));
		dev->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&src->viewParms.projectionMatrix.m));

		//spawn_light();

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

	__declspec(naked) void r_set3d_stub()
	{
		const static uint32_t stock_func_addr = 0x7244C0;
		const static uint32_t retn_addr = 0x7246F5;
		__asm
		{
			call	stock_func_addr;

			pushad;
			push	esi; // bufsrcstate
			call	setup_rtx;
			add		esp, 4;
			popad;

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

		/*if (const auto var = Dvar_FindVar("r_fullbright"); var)
		{
			var->current.enabled = true;
			var->flags = game::dvar_flags::userinfo;
		}*/

		if (const auto var = Dvar_FindVar("r_skinCache"); var)
		{
			var->current.enabled = false;
			var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_fastSkin"); var)
		{
			var->current.enabled = false;
			var->flags = game::dvar_flags::userinfo;
		}

		/*if (const auto var = Dvar_FindVar("fx_enable"); var)
		{
			var->current.enabled = false;
			var->flags = game::dvar_flags::userinfo;
		}*/

		// enable via cfg when in-game
		if (const auto var = Dvar_FindVar("r_warm_static"); var)
		{
			var->current.enabled = false;
			var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("sv_cheats"); var)
		{
			var->current.enabled = true;
			var->flags = game::dvar_flags::userinfo;
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

	main_module::main_module()
	{
		// *
		// general

		

		// stuck in some loop 'Com_Quit_f'
		utils::hook::nop(0x5FEA01, 5);

		// don't play intro video
		utils::hook::nop(SELECT(0x564CB9, 0x59D68B), 5);

		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		utils::hook::nop(0x6E8B96, 8); utils::hook(0x6E8B96, rb_draw3d_internal_stub, HOOK_JUMP).install()->quick();

		//utils::hook(0x7246F0, sp::r_set3d_stub, HOOK_JUMP).install()->quick();

		// hook R_SetMaterial ...... 0x741F1E
		utils::hook(0x741F1E, r_set_material, HOOK_CALL).install()->quick();


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
		utils::hook::nop(0x6DD7DD, 5);

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
	}
}
