#include "std_include.hpp"
#define HIDWORD(x)  (*((DWORD*)&(x)+1))

using namespace game::mp;

namespace components::mp
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
			utils::erase_substring(map_name, "maps/mp/");
			utils::erase_substring(map_name, ".d3dbsp");

			ents.open(filepath + map_name + ".map");

			if (!ents.is_open())
			{
				return;
			}

			ents << cm->mapEnts->entityString << std::endl;;

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
#if DEBUG
		if (static bool p_once = false; !p_once)
		{
			DEBUG_PRINT("[T4RTX-DEBUG] # Function: setup_rtx()\n");
			p_once = true;
		}
#endif

		const auto dev = dx->device;
		const auto data = game::get_backenddata();
		const auto src = game::get_cmdbufsourcestate();

		// populate viewParms3D because R_Set3D needs it
		src->viewParms3D = &data->viewInfo->viewParms;

		// update world matrix
		R_Set3D(0, src);

		// directly set matrices on the device so that rtx-remix finds the camera
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));
		dev->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&src->viewParms.viewMatrix.m));
		dev->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&src->viewParms.projectionMatrix.m));

		// needed for skysphere
		dev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (!flags::has_flag("no_fog"))
		{
			const auto s = map_settings::settings();
			const float fog_start = 1.0f;

			dev->SetRenderState(D3DRS_FOGENABLE, TRUE);
			dev->SetRenderState(D3DRS_FOGCOLOR, s->fog_color.packed);
			dev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			dev->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&fog_start);
			dev->SetRenderState(D3DRS_FOGEND, *(DWORD*)&s->fog_distance);
		}

		if (dvars::rtx_sky_follow_player && dvars::rtx_sky_follow_player->current.enabled)
		{
			const float* predicted_origin = reinterpret_cast<const float*>(0x9E2AD4);
			main_module::skysphere_update_pos(predicted_origin);
		}

		// update culling vars at the end of a frame (so we don't change culling behaviour mid-frame -> not safe)
		{
			// update world culling
			if (dvars::rtx_disable_world_culling)
			{
				main_module::loc_disable_world_culling = dvars::rtx_disable_world_culling->current.integer;
				main_module::loc_disable_world_culling = main_module::loc_disable_world_culling < 0 ? 0 :
					main_module::loc_disable_world_culling > 3 ? 3 : main_module::loc_disable_world_culling;
			}

			// update entity culling
			if (dvars::rtx_disable_entity_culling)
			{
				main_module::loc_disable_entity_culling = dvars::rtx_disable_entity_culling->current.enabled ? 1 : 0;
			}
		}
	}

	__declspec(naked) void rb_draw3d_internal_stub()
	{
		const static uint32_t retn_addr = 0x6BAB2E;
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
		if (   mat
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
		game::switch_material_t mat = {};

		mat.current_material = game::mp::rgp->sortedMaterials[(HIDWORD(drawSurf.packed) >> 1) & 2047];
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

		state->origTechType = state->techType;
		state->techType = mat.technique_type;

		return 1;
	}

	void set_dvar_defaults()
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

		// reduce wobble on objects rendered using shaders (more wobble the further from 0 0 0 the player is)
		if (const auto var = Dvar_FindVar("r_znear"); var)
		{
			var->current.value = 40.0f;
			var->domain.value.min = 0.0f;
			var->domain.value.max = FLT_MAX;
			var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_skinCache"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_fastSkin"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_distortion"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("fx_drawClouds"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("fx_cull_elem_draw"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_depthprepass"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_smc_enable"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_smp_backend"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (const auto var = Dvar_FindVar("r_smp_worker"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		// r_warm_static hinders static models from reaching the lowest lod, thus always drawing all models
		// ^ model draw distances are not affected by 'r_lodScaleRigid' with it enabled
		// ^ note: now disabled by default because 'r_forceLod' is now used instead
		if (const auto var = Dvar_FindVar("r_warm_static"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
		}

		if (!flags::has_flag("no_forced_lod"))
		{
			// force lod to LOD0 (doesn't work on some models for unkown reasons)
			// max draw distances of models are unaffected by this and can be tweaked using 'r_lodScaleRigid'
			if (const auto var = Dvar_FindVar("r_forceLod"); var)
			{
				var->current.integer = 0; var->flags = game::dvar_flags::userinfo;
			}
		}

		if (const auto var = Dvar_FindVar("r_fovScaleThresholdRigid"); var)
		{
			var->current.value = 10000.0f; var->flags = game::dvar_flags::userinfo;
		}

		//if (const auto var = Dvar_FindVar("r_fovScaleThresholdSkinned"); var)
		//{
		//	var->current.value = 2.0f;
		//}

		// disable culling
		/*if (const auto var = Dvar_FindVar("r_warm_dpvs"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
		}*/

		// batch surfaces (def. needed)
		if (const auto var = Dvar_FindVar("r_pretess"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
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
		const static uint32_t stock_func = 0x6E13F0;
		const static uint32_t retn_addr = 0x6AC0A9;
		__asm
		{
			call	stock_func;

			pushad;
			call	set_dvar_defaults;
			popad;

			jmp		retn_addr;
		}
	}


	// *
	// load custom fastfile containing required custom assets

	void load_common_fast_files()
	{
		DEBUG_PRINT("[T4RTX-DEBUG] # Function: load_common_fast_files()\n");

		const char** zone_code_post_gfx = reinterpret_cast<const char**>(0x10880A34);
		const char** zone_patch = reinterpret_cast<const char**>(0x10880A38);
		const char** zone_ui = reinterpret_cast<const char**>(0x10880A3C);
		const char** zone_common = reinterpret_cast<const char**>(0x10880A40);
		const char** localized_code_post_gfx = reinterpret_cast<const char**>(0x10880A44);
		const char** zone_localized_common = reinterpret_cast<const char**>(0x10880A48);
		const char** zone_mod = reinterpret_cast<const char**>(0x10880A4C);

		int i = 0;
		game::XZoneInfo xzone_info_stack[8];

		// ------------------------------------

		xzone_info_stack[i].name = *zone_code_post_gfx;
		xzone_info_stack[i].allocFlags = 4;
		xzone_info_stack[i].freeFlags = 0;
		++i;

		if (*localized_code_post_gfx)
		{
			xzone_info_stack[i].name = *localized_code_post_gfx;
			xzone_info_stack[i].allocFlags = 0;
			xzone_info_stack[i].freeFlags = 0;
			++i;
		}

		// ------------------------------------

		if (*zone_mod)
		{
			xzone_info_stack[i].name = *zone_mod;
			xzone_info_stack[i].allocFlags = 0x800;
			xzone_info_stack[i].freeFlags = 0;
			++i;

			DB_LoadXAssets(&xzone_info_stack[0], i, 0);

			utils::hook::call<void(__cdecl)()>(0x6BED30)(); // R_BeginRemoteScreenUpdate
			utils::hook::call<void(__cdecl)()>(0x572410)(); // no clue
			utils::hook::call<void(__cdecl)()>(0x6BEDB0)(); // R_EndRemoteScreenUpdate
			utils::hook::call<void(__cdecl)()>(0x5CEE40)(); // no clue
			utils::hook::call<void(__cdecl)()>(0x4BAAD0)(); // DB_SyncXAssets

			// start a new zone stack
			i = 0;
		}

		if (DB_FileExists("xcommon_rtx", game::DB_FILE_EXISTS_PATH::DB_PATH_ZONE))
		{
			xzone_info_stack[i].name = "xcommon_rtx";
			xzone_info_stack[i].allocFlags = 4;
			xzone_info_stack[i].freeFlags = 0;
			++i;
		}
#if DEBUG
		else
		{
			DEBUG_PRINT("[T4RTX-DEBUG] # ERR: could not find 'xcommon_rtx.ff'\n");
		}
#endif

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

		DB_LoadXAssets(&xzone_info_stack[0], i, 0);
	}


	// *
	// fix resolution issues by removing duplicates returned by EnumAdapterModes
	// ^ this was fixed on the dxvk branch - TODO: remove when latest dxvk changes were merged into dxvk-remix

	namespace resolution
	{
		auto hash = [](const _D3DDISPLAYMODE& d) { return d.Width + 10 * d.Height + d.RefreshRate; };
		auto equal = [](const _D3DDISPLAYMODE& d1, const _D3DDISPLAYMODE& d2) { return d1.Width == d2.Width && d1.Height == d2.Height && d1.RefreshRate == d2.RefreshRate; };
		std::unordered_set<_D3DDISPLAYMODE, decltype(hash), decltype(equal)> modes(256, hash, equal);

		int enum_adapter_modes_intercept(std::uint32_t adapter_index, std::uint32_t mode_index)
		{
			_D3DDISPLAYMODE current = {};
			const auto hr = game::mp::dx->d3d9->EnumAdapterModes(adapter_index, D3DFMT_X8R8G8B8, mode_index, &current) < 0;
			modes.emplace(current);
			return hr;
		}

		__declspec(naked) void R_EnumDisplayModes_stub()
		{
			const static uint32_t retn_addr = 0x6AB232;
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
					Com_PrintMessage(0, "EnumAdapterModes : Failed to grab all possible resolutions. Array to small!\n", 0);
					break;
				}

				memcpy(&game::mp::dx->displayModes[idx], &m, sizeof(_D3DDISPLAYMODE));
				idx++;
			}
		}

		__declspec(naked) void R_EnumDisplayModes_stub2()
		{
			const static uint32_t R_CompareDisplayModes_addr = 0x6AB1B0;
			const static uint32_t retn_addr = 0x6AB26E;
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
		*reinterpret_cast<float*>(0x10880A04) = static_cast<float>(window_parms[7]) / static_cast<float>(window_parms[8]);
	}

	void __declspec(naked) fix_aspect_ratio_stub()
	{
		const static uint32_t retn_addr = 0x6AA53E;
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
			|| skysphere_model->model != G_ModelIndex(skysphere_get_name_for_variant(skysphere_variant)))
		{
			return false;
		}

		return true;
	}

	void main_module::skysphere_change_model(int variant)
	{
		const std::int16_t model_index = G_ModelIndex(skysphere_get_name_for_variant(variant));

		skysphere_model->model = model_index;
		skysphere_model->s.index.xmodel = model_index;
		skysphere_model->classname = game::mp::scr_const->script_model;
		G_DObjUpdate(&skysphere_model->s);

		skysphere_variant = variant;
	}

	void main_module::skysphere_update_pos(const float* pos)
	{
		if (skysphere_spawned)
		{
			skysphere_model->r.svFlags = 0x04;
			skysphere_model->s.lerp.pos.trBase[0] = pos[0];
			skysphere_model->s.lerp.pos.trBase[1] = pos[1];
			skysphere_model->s.lerp.pos.trBase[2] = pos[2];
			skysphere_model->s.lerp.pos.trType = TR_STATIONARY;
			skysphere_model->s.lerp.pos.trTime = 0;
			skysphere_model->s.lerp.pos.trDuration = 0;
			skysphere_model->s.lerp.pos.trDelta[0] = 0.0f;
			skysphere_model->s.lerp.pos.trDelta[1] = 0.0f;
			skysphere_model->s.lerp.pos.trDelta[2] = 0.0f;
			skysphere_model->r.currentOrigin[0] = pos[0];
			skysphere_model->r.currentOrigin[1] = pos[1];
			skysphere_model->r.currentOrigin[2] = pos[2];
		}
	}

	void main_module::skysphere_spawn(int variant)
	{
		if (skysphere_is_model_valid())
		{
			skysphere_change_model(variant);
			return;
		}

		const std::int16_t model_index = G_ModelIndex(skysphere_get_name_for_variant(variant));

		skysphere_model = reinterpret_cast<game::mp::gentity_s*>(G_Spawn());
		skysphere_model->classname = game::mp::scr_const->script_model;
		skysphere_model->model = model_index;
		skysphere_model->s.index.xmodel = model_index;
		skysphere_model->r.svFlags = 0x04;
		skysphere_model->r.linked = 0x1;

		//Game::G_SetOrigin(skysphere_model, skysphere_model_origin);
		skysphere_model->s.lerp.pos.trBase[0] = 0.0f;
		skysphere_model->s.lerp.pos.trBase[1] = 0.0f;
		skysphere_model->s.lerp.pos.trBase[2] = 0.0f;
		skysphere_model->s.lerp.pos.trType = TR_STATIONARY;
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
		skysphere_model->s.lerp.apos.trType = TR_STATIONARY;
		skysphere_model->s.lerp.apos.trTime = 0;
		skysphere_model->s.lerp.apos.trDuration = 0;
		skysphere_model->s.lerp.apos.trDelta[0] = 0.0;
		skysphere_model->s.lerp.apos.trDelta[1] = 0.0;
		skysphere_model->s.lerp.apos.trDelta[2] = 0.0;
		skysphere_model->r.currentAngles[0] = 0.0f;
		skysphere_model->r.currentAngles[1] = 0.0f;
		skysphere_model->r.currentAngles[2] = 0.0f;

		G_CallSpawnEntity(reinterpret_cast<game::gentity_s*>(skysphere_model));

		skysphere_spawned = true;
		skysphere_variant = variant;
	}

	// *
	// LOD

	
	int forcelod_get_lod(const int lod_count)
	{
		const auto& r_forceLod = Dvar_FindVar("r_forceLod");

		if (r_forceLod->current.integer > lod_count // force lowest possible LOD
			|| (r_forceLod->current.integer >= lod_count)) // force second lowest possible LOD
		{
			return lod_count - 1 >= 0 ? lod_count - 1 : 0;
		}

		return r_forceLod->current.integer;
	}

	int forcelod_is_enabled()
	{
		const auto& r_forceLod = Dvar_FindVar("r_forceLod");

		// 4 = none - disabled
		if (r_forceLod->current.integer == r_forceLod->reset.integer)
		{
			return 0;
		}

		return 1;
	}

	int xmodel_get_lod_for_dist_global_1 = 0;
	__declspec(naked) void skinned_xmodel_get_lod_for_dist_inlined()
	{
		const static uint32_t break_addr = 0x6B7BFD;
		const static uint32_t og_logic_addr = 0x6B7BC7;
		__asm
		{
			pushad;
			push	ecx;					// save ecx
			call	forcelod_is_enabled;
			cmp		eax, 1;
			pop		ecx;					// restore ecx
			jne		OG_LOGIC;				// if r_forceLod != 1

			push	ecx;					// holds model->numLods
			call	forcelod_get_lod;
			add		esp, 4;
			mov		xmodel_get_lod_for_dist_global_1, eax;
			popad;

			mov		eax, xmodel_get_lod_for_dist_global_1; // move returned lodindex into the register the game expects it to be
			jmp		break_addr;


		OG_LOGIC:
			popad;
			movss   xmm1, dword ptr[edx];
			comiss  xmm1, xmm0;
			jmp		og_logic_addr;
		}
	}

	int xmodel_get_lod_for_dist_global_2 = 0;
	__declspec(naked) void rigid_xmodel_get_lod_for_dist_inlined()
	{
		const static uint32_t break_addr = 0x70A73C;
		const static uint32_t og_logic_addr = 0x70A72D;
		__asm
		{
			pushad;

			push	eax;					// save eax
			call	forcelod_is_enabled;
			cmp		eax, 1;
			jne		OG_LOGIC;				// if r_forceLod != 1

			pop		eax;					// restore eax
			push	eax;					// holds model->numLods
			call	forcelod_get_lod;
			add		esp, 4;
			mov		xmodel_get_lod_for_dist_global_2, eax;
			popad;

			mov		ebx, xmodel_get_lod_for_dist_global_2; // move returned lodindex into the register the game expects it to be
			jmp		break_addr;


		OG_LOGIC:
			pop		eax;					// restore eax
			popad;
			movss   xmm1, dword ptr[ecx];
			comiss  xmm1, xmm0;
			jmp		og_logic_addr;
		}
	}

	namespace cull
	{
		// R_AddWorldSurfacesPortalWalk
		__declspec(naked) void world_stub_01()
		{
			const static uint32_t retn_skip = 0x6A94C5;
			const static uint32_t retn_stock = 0x6A945E;
			__asm
			{
				// jump if culling is less or disabled
				cmp		main_module::loc_disable_world_culling, 1;
				jge		SKIP;

				// og code
				cmp     esi, ebp;
				mov		[esp + 0x10], eax;
				jmp		retn_stock;

			SKIP:
				mov		[esp + 0x10], eax;
				jmp		retn_skip;
			}
		}

		int _skipped_cull = 0; // helper var
		__declspec(naked) void world_stub_02_reset_helper()
		{
			const static uint32_t retn_stock = 0x6F7328;
			__asm
			{
				mov		_skipped_cull, 0;
				mov		[ecx + 0xC], edi;
				mov		[ecx + 8], edx;
				jmp		retn_stock;
			}
		}

		__declspec(naked) void world_stub_02_skip_static_model()
		{
			const static uint32_t retn_stock = 0x6F6FC3;
			const static uint32_t retn_stock_jumped = 0x6F6FEB;
			__asm
			{
				// do we want to cull static models the way geo would be culled?
				cmp		main_module::loc_disable_world_culling, 3;
				jl		STOCK;

				// did we skip the culling check in 'r_cull_world_stub_02'?
				cmp		_skipped_cull, 1;
				je		SKIP;

			STOCK:		// og code
				test    edx, edx;
				jz		loc_6F6FEB;
				mov     esi, [ecx + 0x20];
				jmp		retn_stock;

			loc_6F6FEB: // og code
				jmp		retn_stock_jumped;


			SKIP:		// skip static model rendering
				mov     esi, [ecx + 0x20];
				jmp		retn_stock_jumped;
			}
		}

		// R_AddAabbTreeSurfacesInFrustum_r
		__declspec(naked) void world_stub_02()
		{
			const static uint32_t retn_skip = 0x6F6F2F;
			const static uint32_t retn_stock = 0x6F6F29;
			__asm
			{
				// jump if culling is disabled
				cmp		main_module::loc_disable_world_culling, 2;
				jge		SKIP;

				// og code
				addss   xmm5, xmm6;
				comiss  xmm4, xmm5;
				jmp		retn_stock;

			SKIP: // jumped here because culling is disabled 
				addss   xmm5, xmm6;
				comiss  xmm4, xmm5;
				jnb		HACKED_CULLING;
				jmp		retn_skip;

			HACKED_CULLING: // jumped here because the game would have culled this object
				mov		_skipped_cull, 1;
				jmp		retn_skip;
			}
		}

		// R_AddAabbTreeSurfacesInFrustum_r
		__declspec(naked) void world_stub_03()
		{
			const static uint32_t retn_skip = 0x6F6F98;
			const static uint32_t retn_stock = 0x6F6F65;
			__asm
			{
				// jump if culling is less or disabled
				cmp		main_module::loc_disable_world_culling, 1;
				jge		SKIP;

				// og code
				addss   xmm5, xmm0;
				comiss  xmm5, xmm4;
				jmp		retn_stock;

			SKIP:
				addss   xmm5, xmm0;
				comiss  xmm5, xmm4;
				jmp		retn_skip;
			}
		}

		// R_AddCellSceneEntSurfacesInFrustumCmd
		__declspec(naked) void entities_stub()
		{
			const static uint32_t retn_skip = 0x71E2DB;
			const static uint32_t retn_stock = 0x71E2D5;
			__asm
			{
				cmp		main_module::loc_disable_entity_culling, 1;
				je		SKIP;

				// stock op's
				and		[esp + 0xC], esi;
				mov     esi, [esp + 0x4C];
				jmp		retn_stock;

			SKIP:
				and		[esp + 0xC], esi;
				mov     esi, [esp + 0x4C];
				jmp		retn_skip;
			}
		}
	}

	// returns true if inside radius
	int fx_cullsphere_radius_check(const float* camera_pos, const float* fx_world_pos)
	{
		if (const auto var = Dvar_FindVar("fx_cull_elem_draw");
			var && var->current.enabled)
		{
			const auto dist = utils::distance(fx_world_pos, camera_pos);

			if (dvars::fx_cull_elem_draw_radius &&
				dist < dvars::fx_cull_elem_draw_radius->current.value)
			{
				return 1;
			}
		}

		return 0;
	}

	int fx_cullsphere_global_helper = 0;
	__declspec(naked) void fx_cullsphere_stub()
	{
		const static uint32_t og_continue_addr = 0x4E1076;
		const static uint32_t og_retn_addr = 0x4E10D5;
		__asm
		{
			mov     eax, [esp + 4];			// FxCamera*
			pushad;
			push	ecx;					// float* fx worldpos
			push	eax;					// FxCamera*
			call	fx_cullsphere_radius_check;
			add		esp, 8;
			mov		fx_cullsphere_global_helper, eax;
			popad;
			xor		eax, eax;

			cmp		fx_cullsphere_global_helper, 1;
			je		loc_4E10D5;				// do not cull if within radius

			// OG_LOGIC
			xor		eax, eax;
			test    edx, edx;
			jbe		loc_4E10D5;
			jmp		og_continue_addr;

		loc_4E10D5:
			jmp		og_retn_addr;
		}
	}

	int skip_image_load(game::GfxImage* img)
	{
		// 0x2 = color, 0x5 = normal, 0x8 = spec
		if (img->semantic == 0x5 || img->semantic == 0x8)
		{
			return 1;
		}

		return 0;
	}

	__declspec(naked) void load_image_stub()
	{
		const static uint32_t skip_img_addr = 0x712D83;
		const static uint32_t og_logic_addr = 0x712D7B;
		__asm
		{
			pushad;
			push	ebx;					// img
			call	skip_image_load;
			pop		ebx;
			cmp		eax, 1;
			jne		OG_LOGIC;

			popad;
			jmp		skip_img_addr;

			// og code
		OG_LOGIC:
			popad;
			push    ebx;
			mov     edx, edi;
			lea     eax, [esp + 0x10];
			jmp		og_logic_addr;
		}
	}

#if DEBUG
	void Com_PrintMessage_intercept(const char* msg)
	{
		if (msg && !utils::string_contains(msg, "dvar set"))
		{
			printf(msg);
		}
	}

	__declspec(naked) void Com_PrintMessage_stub()
	{
		const static uint32_t og_retn_addr = 0x5623E9;
		__asm
		{
			pushad;
			push	ebx;
			call	Com_PrintMessage_intercept;
			add		esp, 4;
			popad;

			// og code
			mov     esi, [ebp + 8];
			cmp     esi, 6;
			jmp		og_retn_addr;
		}
	}
#endif


	// *
	// Event stubs

	// > fixed_function::init_fixed_function_buffers_stub
	void main_module::on_map_load()
	{
		map_settings::get()->set_settings_for_loaded_map();
	}

	// > fixed_function::free_fixed_function_buffers_stub
	void main_module::on_map_shutdown()
	{
	}

	main_module::main_module()
	{
#if DEBUG
		utils::hook::nop(0x5623E3, 6);
		utils::hook(0x5623E3, Com_PrintMessage_stub, HOOK_JUMP).install()->quick();
#endif

		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		utils::hook::nop(0x6BAB26, 8); utils::hook(0x6BAB26, rb_draw3d_internal_stub, HOOK_JUMP).install()->quick();

		// hook R_SetMaterial :: material/technique replacing
		utils::hook(0x717F9E, r_set_material, HOOK_CALL).install()->quick();

		// precaching xmodels beyond level load (dynamic skysphere spawning)
		utils::hook::set<BYTE>(0x5410B2, 0xEB);

		// disable loading of specular and normalmaps (de-clutter remix ui)
		if (!flags::has_flag("load_normal_spec"))
		{
			utils::hook::nop(0x6E23FC, 5);
			utils::hook::nop(0x712D74, 7); utils::hook(0x712D74, load_image_stub, HOOK_JUMP).install()->quick();
		}

		// load custom fastfile containing required assets
		utils::hook(0x6AB8CF, load_common_fast_files, HOOK_CALL).install()->quick();

		// dxvk's 'EnumAdapterModes' returns a lot of duplicates and the games array only has a capacity of 256 which is not enough depending on max res. and refreshrate
		// fix resolution issues by removing duplicates returned by EnumAdapterModes - then write the array ourselfs
		utils::hook(0x6AB222, mp::resolution::R_EnumDisplayModes_stub, HOOK_JUMP).install()->quick();
		utils::hook(0x6AB269, mp::resolution::R_EnumDisplayModes_stub2, HOOK_JUMP).install()->quick();
		utils::hook::set<BYTE>(0x6AB210 + 2, 0x04); // set max array size check to 1024 (check within loop)

		// Remove Impure client (iwd) check - needed?
		utils::hook::nop(0x5B260F, 30);

		// stuck in some loop 'Com_Quit_f'
		utils::hook::nop(0x5CFAC1, 5);

		// don't play intro video - allows to devmap into a map via commandline
		utils::hook::nop(0x564CB9, 5);

		// :*
		utils::hook(0x6AA4A3, fix_aspect_ratio_stub, HOOK_JUMP).install()->quick();

		// ------------------------------------------------------------------------

		// stub after 'R_InitGraphicsApi' (NVAPI Entry) to re-register stock dvars
		utils::hook(0x6AC0A4, register_dvars_stub, HOOK_JUMP).install()->quick();

		// un-cheat + userinfo flag for fx_enable
		utils::hook::set<BYTE>(0x4D1406 + 1, 0x01); // was 0x80

		// un-cheat + userinfo flag for sv_cheats
		utils::hook::set<BYTE>(0x6DBF95 + 1, 0x01); // was 0x48

		// change default value of r_fullscreen to 0
		utils::hook::set<BYTE>(0x6D7E45 + 1, 0x0);

		// ------------------------------------------------------------------------

		// implement r_forcelod logic for skinned models (R_SkinXModel)
		utils::hook::nop(0x6B7BC0, 7);  utils::hook(0x6B7BC0, skinned_xmodel_get_lod_for_dist_inlined, HOOK_JUMP).install()->quick();

		// implement r_forcelod logic for all other static models (R_AddAllStaticModelSurfacesCamera)
		utils::hook::nop(0x70A726, 7);  utils::hook(0x70A726, rigid_xmodel_get_lod_for_dist_inlined, HOOK_JUMP).install()->quick();

		// note: dvar 'r_fovScaleThresholdRigid' can be used to stop fov related lod changes


		// dvar 'r_warm_dpvs' can be used to disable all culling (no longer needed > use dvar 'rtx_disable_world_culling')
		// DISABLE CULLING :: stop 'r_warm_dpvs' dvar from resetting itself -- 0x6E5D21 adds static models per cell -- 0x7398B4 frustum culling
		utils::hook::set<BYTE>(0x6B6988, 0xEB);

		// ^ do not enable 'r_warm_dpvs' by default
		utils::hook::nop(0x6BEA0E, 5);


		// *
		// culling
		{
			// R_AddWorldSurfacesPortalWalk :: less culling
			utils::hook::nop(0x6A9458, 6); utils::hook(0x6A9458, cull::world_stub_01, HOOK_JUMP).install()->quick();

			{
				// note: using 'rtx_disable_world_culling' = 'less' results in unstable world geometry hashes (but stable static model hashes)
				// -> using 'all-but-models' leaves culling logic for static models at 'less' but does not cull world geometry

				// R_AddCellStaticSurfacesInFrustumCmd :: stub used to reset the skip model check from the stub below
				utils::hook::nop(0x6F7322, 6); utils::hook(0x6F7322, cull::world_stub_02_reset_helper, HOOK_JUMP).install()->quick();
				// R_AddAabbTreeSurfacesInFrustum_r :: check if culling mode 'all-but-models' is active - check note above
				utils::hook(0x6F6FBE, cull::world_stub_02_skip_static_model, HOOK_JUMP).install()->quick();

				// R_AddAabbTreeSurfacesInFrustum_r :: mins check
				utils::hook::nop(0x6F6F22, 7); utils::hook(0x6F6F22, cull::world_stub_02, HOOK_JUMP).install()->quick();
			}

			// R_AddAabbTreeSurfacesInFrustum_r :: maxs check
			utils::hook::nop(0x6F6F5E, 7); utils::hook(0x6F6F5E, cull::world_stub_03, HOOK_JUMP).install()->quick();

			// R_AddCellSceneEntSurfacesInFrustumCmd :: active ents like destructible cars / players
			utils::hook::nop(0x71E2CD, 8); utils::hook(0x71E2CD, cull::entities_stub, HOOK_JUMP).install()->quick();

			// R_AddCellSceneEntSurfacesInFrustumCmd :: brushmodels
			utils::hook::nop(0x71E4CA, 2);

			dvars::rtx_disable_world_culling = game::Dvar_RegisterEnum(
				/* name		*/ "rtx_disable_world_culling",
				/* enumData */ main_module::rtx_disable_world_culling_enum.data(),
				/* enumSize	*/ main_module::rtx_disable_world_culling_enum.size(),
				/* default	*/ 1,
				/* flags	*/ game::dvar_flags::saved,
				/* desc		*/ "Disable world culling. 'all' is needed for stable geometry hashes!\n"
				"- less: reduces culling to portals only (unstable world geo hashes!)\n"
				"- all: disable culling of all surfaces including models\n"
				"- all-but-models: disable culling of all surfaces excluding models");

			dvars::rtx_disable_entity_culling = game::Dvar_RegisterBool(
				/* name		*/ "rtx_disable_entity_culling",
				/* desc		*/ "Disable culling of game entities (script objects/destructible cars ...)",
				/* default	*/ true,
				/* flags	*/ game::dvar_flags::saved);
		}

		// ignore "too many static models ..." msg - still add model to the scene
		utils::hook::nop(0x70A775, 6); utils::hook::jump(0x70A775, 0x70A872);

		// disable dynent drawing (could have stable hashes but no stable lods right now)
		//utils::hook::nop(0x6B624D, 5);

		// hook FX_CullSphere to implement an additional radius check
		utils::hook::nop(0x4E1070, 6); utils::hook(0x4E1070, fx_cullsphere_stub, HOOK_JUMP).install()->quick();

		dvars::fx_cull_elem_draw_radius = game::Dvar_RegisterFloat(
			"fx_cull_elem_draw_radius",
			1200.0f, 0.0f, 100000.0f,
			game::dvar_flags::archive,
			"fx elements inside this radius are not affected by culling (fx_cull_elem_draw)");

		dvars::rtx_sky_follow_player = game::Dvar_RegisterBool(
			/* name		*/ "rtx_sky_follow_player",
			/* desc		*/ "Sky will follow the player (can help with culling issues)",
			/* default	*/ true,
			/* flags	*/ game::dvar_flags::saved);

#ifdef GIT_DESCRIBE
		static auto version_str = std::string("t4-rtx-"s + GIT_DESCRIBE + " > "s);
#else
		static auto version_str = "t4-rtx > "s;
#endif

		// console string ;)
		utils::hook::set<const char*>(0x48AF32 + 1, version_str.c_str());

		// no forward/backslash for console cmds
		utils::hook::nop(0x493DEF, 5);

		command::add("rtx_sky_clear", [](command::params) { main_module::skysphere_spawn(0); });
		command::add("rtx_sky_desert", [](command::params) { main_module::skysphere_spawn(1); });
		command::add("rtx_sky_night", [](command::params) { main_module::skysphere_spawn(2); });
		command::add("rtx_sky_overcast", [](command::params) { main_module::skysphere_spawn(3); });
		command::add("rtx_sky_sunset", [](command::params) { main_module::skysphere_spawn(4); });

		command::add("export_entities", [this](command::params)
		{
			export_entity_string();
		});

		command::add("noborder", [this](command::params)
		{
			const auto hwnd = game::mp::dx->windows->hwnd ? game::mp::dx->windows->hwnd : FindWindow(nullptr, L"Call of Duty®");

			// calculate titlebar height
			RECT w, c; GetWindowRect(hwnd, &w); GetClientRect(hwnd, &c);
			mp::main_module::noborder_titlebar_height = (w.bottom - w.top) - (c.bottom - c.top);

			SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(hwnd, nullptr, 0, 0, game::mp::dx->windows->width, game::mp::dx->windows->height, SWP_SHOWWINDOW | SWP_NOACTIVATE);
		});

		command::add("windowed", [this](command::params)
		{
			if (mp::main_module::noborder_titlebar_height)
			{
				const auto hwnd = game::mp::dx->windows->hwnd ? game::mp::dx->windows->hwnd : FindWindow(nullptr, L"Call of Duty®");
				SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
				SetWindowPos(hwnd, nullptr, 0, 0, game::mp::dx->windows->width, game::mp::dx->windows->height + mp::main_module::noborder_titlebar_height, SWP_SHOWWINDOW | SWP_NOACTIVATE);
			}
		});
	}
}
