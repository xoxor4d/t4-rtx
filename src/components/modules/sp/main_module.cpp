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

	void main_module::force_dvars_on_frame()
	{
		// update culling vars at the end of a frame (so we don't change culling behaviour mid-frame -> not safe)
		{
			//if (dvars::rtx_culling_tweak_mins) loc_culling_tweak_mins = dvars::rtx_culling_tweak_mins->current.enabled;
			//if (dvars::rtx_culling_tweak_maxs) loc_culling_tweak_maxs = dvars::rtx_culling_tweak_maxs->current.enabled;
			if (dvars::rtx_culling_tweak_frustum) loc_culling_tweak_frustum = dvars::rtx_culling_tweak_frustum->current.enabled;
			if (dvars::rtx_culling_tweak_smodel) loc_culling_tweak_smodel = dvars::rtx_culling_tweak_smodel->current.enabled;

			// #
			// old anti culling - activ if flag 'old_anti_culling' is set

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

		if (const auto var = Dvar_FindVar("r_depthprepass"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_blur_allowed"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_dof_enable"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_motionblur_enable"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_multiGpu"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("sc_enable"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("sm_enable"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_smc_enable"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_smp_backend"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_smp_worker"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_drawWater"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_watersim_enabled"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_skinCache"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_fastSkin"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("r_distortion"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
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

		// update world matrix (R_Set3D)
		R_Set3D(0, src); //utils::hook::call<void(__fastcall)(int, game::GfxCmdBufSourceState*)>(0x7244C0)(0, src); // updated SP

		// directly set matrices on the device so that rtx-remix finds the camera
		dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));
		dev->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&src->viewParms.viewMatrix.m));
		dev->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&src->viewParms.projectionMatrix.m));

		// needed for skysphere
		dev->SetRenderState(D3DRS_LIGHTING, FALSE);

		main_module::force_dvars_on_frame();
		api::bullet_fire_frame_cb();

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

		if (!flags::has_flag("no_sun"))
		{
			{
				const auto s = map_settings::settings();

				D3DLIGHT9 light = {};
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = s->sun_color[0] * s->sun_intensity;
				light.Diffuse.g = s->sun_color[1] * s->sun_intensity;
				light.Diffuse.b = s->sun_color[2] * s->sun_intensity;

				D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, (const D3DXVECTOR3*)&s->sun_direction);

				dev->SetLight(0, &light);
				dev->LightEnable(0, TRUE);
			}
		}

		if (dvars::rtx_sky_follow_player && dvars::rtx_sky_follow_player->current.enabled)
		{
			main_module::skysphere_update_pos(game::sp::cgs->predictedPlayerState.origin);
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

	// model:	[1] techset - [2] material
	// bsp:		[3] techset - [4] material
	// bmodel:	[5] techset - [6] material
	void main_module::rb_show_tess(game::GfxCmdBufSourceState* source, game::GfxCmdBufState* state, const float* center, const char* name, const float* color, game::DebugGlobals* manual_debug_glob)
	{
		const auto debug_glob = manual_debug_glob ? manual_debug_glob : &source->data->debugGlobals;

		float offset_center[3];
		offset_center[0] = center[0];
		offset_center[1] = center[1];
		offset_center[2] = center[2];

		const auto dist_to_str = utils::distance(source->eyeOffset, offset_center);

		if (dvars::r_showTessDist)
		{
			if (dist_to_str > dvars::r_showTessDist->current.value && dvars::r_showTessDist->current.value != 0.0f)
			{
				return;
			}
		}

		bool viewmodel_string = false;
		auto font_scale = 0.25f;
		if (dist_to_str < 25.0f)
		{
			viewmodel_string = true;
			font_scale = 0.025f;
		}

		const game::MaterialTechnique* tech = nullptr;
		if (state->material && state->material->techniqueSet->techniques[static_cast<std::uint8_t>(state->techType)])
		{
			tech = state->material->techniqueSet->techniques[static_cast<std::uint8_t>(state->techType)];
		}

		if (const auto r_showTess = game::sp::Dvar_FindVar("r_showTess"); r_showTess && tech)
		{
			switch (r_showTess->current.integer)
			{
			case 1: // techset model
			case 3: // techset bsp
			case 5: // techset bmodel
			{
				// offset_center[2] = (((float)state->techType - 16.0f) * 0.3f) + offset_center[2];
				// header
				game::sp::R_AddDebugString(debug_glob, offset_center, color, font_scale, utils::va("%s: %s", name, tech->name));
				font_scale *= 0.5f;

				offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;
				game::sp::R_AddDebugString(debug_glob, offset_center, color, font_scale, utils::va("> [TQ]: %s", state->material->techniqueSet->name));

				offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;
				game::sp::R_AddDebugString(debug_glob, offset_center, color, font_scale, utils::va("> [VS] %s", tech->passArray[0].vertexShader ? tech->passArray[0].vertexShader->name : "<NONE>"));

				offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;
				game::sp::R_AddDebugString(debug_glob, offset_center, color, font_scale, utils::va("> [PS] %s", tech->passArray[0].pixelShader ? tech->passArray[0].pixelShader->name : "<NONE>"));
				break;
			}

			case 2: // material model
			case 4: // material bsp
			case 6: // material bmodel
			{
				// header
				game::sp::R_AddDebugString(debug_glob, offset_center, color, font_scale, utils::va("%s: %s", name, state->material->info.name));
				font_scale *= 0.5f;

				for (auto i = 0; i < state->material->textureCount; i++)
				{
					if (&state->material->textureTable[i] && state->material->textureTable[i].u.image && state->material->textureTable[i].u.image->name)
					{
						const auto img = state->material->textureTable[i].u.image;
						offset_center[2] -= viewmodel_string ? 0.25f : 2.5f;

						const char* semantic_str;
						switch (static_cast<std::uint8_t>(img->semantic))
						{
						case 0: semantic_str = "2D"; break;
						case 1: semantic_str = "F"; break;
						case 2: semantic_str = "C"; break;
						case 5: semantic_str = "N"; break;
						case 8: semantic_str = "S"; break;
						case 11: semantic_str = "W"; break;
						default: semantic_str = "C+"; break;
						}

						game::sp::R_AddDebugString(debug_glob, offset_center, color, font_scale, utils::va("> [%s] %s", semantic_str, img->name));
					}
				}
				break;
			}

			default:
				break;
			}
		}
	}

	// ------------------------

	bool is_valid_technique_for_type(const game::Material* mat, const game::MaterialTechniqueType type)
	{
		if (mat
			&& mat->techniqueSet
			&& mat->techniqueSet->remappedTechniqueSet
			&& mat->techniqueSet->remappedTechniqueSet->techniques[type])
		{
			return true;
		}

		return false;
	}

	bool switch_material(game::switch_material_t* swm, const char* material_name)
	{
		if (const auto	material = Material_RegisterHandle(material_name, 3);
			material)
		{
			for (auto t = 0u; t < (uint32_t)material->textureCount; t++)
			{
				if (material->textureTable[t].semantic == 2 && material->textureTable[t].u.image && material->textureTable[t].u.image->name)
				{
					if (std::string_view(material->textureTable[t].u.image->name) == "default")
					{
						return false;
					}
				}
			}

			swm->material = material;
			swm->technique = nullptr;

			if (is_valid_technique_for_type(material, swm->technique_type))
			{
				swm->technique = material->techniqueSet->remappedTechniqueSet->techniques[swm->technique_type];
			}

			swm->switch_material = true;
			return true;
		}

		// return stock material if the above failed
		swm->material = swm->current_material;
		return false;
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

		mat.current_material = game::sp::rgp->sortedMaterials[(HIDWORD(drawSurf.packed) >> 1) & 2047];
		mat.current_technique = mat.current_material->techniqueSet->remappedTechniqueSet->techniques[type];

		mat.material = mat.current_material;
		mat.technique = mat.current_technique;
		mat.technique_type = type;

		if (std::string_view(mat.current_material->info.name).starts_with("wc/sky_"))
		{
			mat.technique_type = game::TECHNIQUE_UNLIT;
			switch_material(&mat, "mc/mtl_test_sphere_silver");
		}

		// remove skybox model
		if (std::string_view(mat.current_material->info.name).starts_with("mc/mtl_skybox"))
		{
			return 0;
		}

		// fix water from switching to random textures
		if (std::string_view(mat.current_material->info.name) == "wc/water_dynamic")
		{
			if (!switch_material(&mat, "wc/temp_water"))
			{
				if (!switch_material(&mat, "wc/temp_muddy_water"))
				{
					switch_material(&mat, "wc/_default_water");
				}
			}
		}
		else if (std::string_view(mat.current_material->info.name).starts_with("wc/water_dynamic_"))
		{
			// case64blue
			std::string replacement = mat.current_material->info.name;
			utils::replace_all(replacement, "water_dynamic_", "water_");
			switch_material(&mat, replacement.c_str());
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

		if (const auto var = Dvar_FindVar("fx_drawClouds"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (const auto var = Dvar_FindVar("fx_cull_elem_draw"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		// r_warm_static hinders static models from reaching the lowest lod, thus always drawing all models
		// ^ model draw distances are not affected by 'r_lodScaleRigid' with it enabled
		// ^ note: now disabled by default because 'r_forceLod' is now used instead
		if (const auto var = Dvar_FindVar("r_warm_static"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		if (!flags::has_flag("no_forced_lod"))
		{
			// force lod to LOD0 (doesn't work on some models for unkown reasons)
			// max draw distances of models are unaffected by this and can be tweaked using 'r_lodScaleRigid'
			if (const auto var = Dvar_FindVar("r_forceLod"); var)
			{
				var->current.integer = 0; var->flags = game::dvar_flags::userinfo;
				var->latched.integer = var->current.integer;
			}
		}

		if (const auto var = Dvar_FindVar("r_fovScaleThresholdRigid"); var)
		{
			var->current.value = 10000.0f; var->flags = game::dvar_flags::userinfo;
			var->latched.value = var->current.value;
		}

		// batch surfaces (def. needed)
		if (const auto var = Dvar_FindVar("r_pretess"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		// #

		// enable cheats
		if (const auto var = Dvar_FindVar("sv_cheats"); var)
		{
			var->current.enabled = true; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
		}

		// enable console
		if (const auto var = Dvar_FindVar("monkeytoy"); var)
		{
			var->current.enabled = false; var->flags = game::dvar_flags::userinfo;
			var->latched.enabled = var->current.enabled;
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

		if (game::sp::DB_FileExists("xcommon_rtx", game::DB_FILE_EXISTS_PATH::DB_PATH_ZONE))
		{
			xzone_info_stack[i].name = "xcommon_rtx";
			xzone_info_stack[i].allocFlags = 512;
			xzone_info_stack[i].freeFlags = 0;
			++i;
		}
#if DEBUG
		else
		{
			DEBUG_PRINT("[T4RTX-DEBUG] # ERR: could not find 'xcommon_rtx.ff'\n");
		}
#endif

		// ------------------------------------

		game::sp::DB_LoadXAssets(&xzone_info_stack[0], i, 0);
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

	const char* main_module::skysphere_get_name_for_variant(std::uint32_t variant)
	{
		if (variant >= SKY::COUNT)
		{
			return SKY_STRINGS[SKY::SUNSET];
		}

		return SKY_STRINGS[variant];
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

	void main_module::skysphere_update_pos(const float* pos)
	{
		if (skysphere_spawned)
		{
			skysphere_model->r.svFlags = 0x04;
			skysphere_model->s.lerp.pos.trBase[0] = pos[0];
			skysphere_model->s.lerp.pos.trBase[1] = pos[1];
			skysphere_model->s.lerp.pos.trBase[2] = pos[2];
			skysphere_model->s.lerp.pos.trType = game::trType_t::TR_STATIONARY;
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


	// *
	// LOD

	int forcelod_get_lod(const int lod_count)
	{
		const auto& r_forceLod = game::sp::Dvar_FindVar("r_forceLod");
		//const auto& r_warm_static = game::sp::Dvar_FindVar("r_warm_static");

		if (r_forceLod->current.integer > lod_count // force lowest possible LOD
			|| (r_forceLod->current.integer >= lod_count)) // force second lowest possible LOD
		{
			return lod_count - 1 >= 0 ? lod_count - 1 : 0;
		}

		return r_forceLod->current.integer;
	}

	int forcelod_is_enabled()
	{
		const auto& r_forceLod = game::sp::Dvar_FindVar("r_forceLod");

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
		const static uint32_t break_addr = 0x6D9C4D;
		const static uint32_t og_logic_addr = 0x6D9C17;
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
		const static uint32_t break_addr = 0x732CEC;
		const static uint32_t og_logic_addr = 0x732CDD;
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



	// return true if material / surface / ... is invalid - HACK!
	int r_add_bmodel_surfaces_camera_check(game::GfxSurface* bsp_surf)
	{
		const auto mem_x = (DWORD)game::get_frontenddata();
		const auto mem_mat = (DWORD)bsp_surf->material;
		const auto mem_surf = (DWORD) & *bsp_surf;

		// HACK 1
		if (mem_x == 0xcccccccc || mem_surf < 0x400000 || mem_mat < 0x400000)
		{
			return 1;
		}

		// HACK 2
		if (!bsp_surf || !bsp_surf->flags || (!bsp_surf->tris.triCount && !bsp_surf->pad) || !bsp_surf->material)
		{
			return 1;
		}

		return 0;
	}

	__declspec(naked) void r_add_bmodel_surfaces_camera_stub()
	{
		const static uint32_t stock_addr = 0x6DACB6;
		const static uint32_t skip_surface_addr = 0x6DAD97; // retn
		__asm
		{
			// og code
			mov     edx, [ecx - 4];

			pushad;
			push	edx; // gfxsurf
			call	r_add_bmodel_surfaces_camera_check;
			add		esp, 4;
			cmp		eax, 1;
			je		SKIP_SURF;				// skip if material invalid
			popad;

			// og code
			mov     edx, [ecx - 4];
			mov     eax, [edx + 0x14];
			jmp		stock_addr;

		SKIP_SURF:
			popad;
			jmp		skip_surface_addr;
		}
	}



	// returns true if inside radius
	int fx_cullsphere_radius_check(const float* camera_pos,const float* fx_world_pos)
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
		const static uint32_t og_continue_addr = 0x4B4126;
		const static uint32_t og_retn_addr = 0x4B4185;
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
			je		loc_4B4185;				// do not cull if within radius

			// OG_LOGIC
			xor		eax, eax;
			test    edx, edx;
			jbe		loc_4B4185;
			jmp		og_continue_addr;

		loc_4B4185:
			jmp		og_retn_addr;
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
		const static uint32_t og_retn_addr = 0x59A225;
		__asm
		{
			pushad;
			push	edi;
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

	namespace cull
	{
		int _last_active_valid_cell = 0; //-1;
		void R_AddWorldSurfacesPortalWalk_hk(int camera_cell_index, game::DpvsView* dpvs)
		{
			const auto glob = game::sp::dpvsGlob;

			// never show the complete map, only the last valid cell
			if (camera_cell_index < 0)
			{
				const auto cell = &game::sp::rgp->world->cells[_last_active_valid_cell];
				const auto cell_index = cell - game::sp::rgp->world->cells;
				game::sp::R_AddCellSurfacesAndCullGroupsInFrustumDelayed(cell, dpvs->frustumPlanes, dpvs->frustumPlaneCount, dpvs->frustumPlaneCount);
				glob->cellVisibleBits[(cell_index >> 5) + 3] |= (1 << (cell_index & 0x1F)); // is the +3 correct here?
			}
			else
			{
				_last_active_valid_cell = camera_cell_index;

				// hack - disable most frustum culling
				glob->views[0][0].frustumPlanes[0].coeffs[3] += main_module::m_frustum_plane_offsets[0]; //5000.0f;
				glob->views[0][0].frustumPlanes[1].coeffs[3] += main_module::m_frustum_plane_offsets[1];
				glob->views[0][0].frustumPlanes[2].coeffs[3] += main_module::m_frustum_plane_offsets[2];
				glob->views[0][0].frustumPlanes[3].coeffs[3] += main_module::m_frustum_plane_offsets[3];
				glob->views[0][0].frustumPlanes[4].coeffs[3] += main_module::m_frustum_plane_offsets[4];
				glob->views[0][0].frustumPlanes[5].coeffs[3] += main_module::m_frustum_plane_offsets[5];
				glob->viewPlane.coeffs[3] += main_module::m_frustum_plane_offsets[6]; //5000.0f;
				// ^ needs to be viewPlane here?

				const auto& var = game::sp::Dvar_FindVar("r_singleCell");
				const bool single_cell = (var && var->current.enabled) ? true : false;

				// #
				// always add full cell the player is in (same as r_singlecell)
				const auto cell = &game::sp::rgp->world->cells[camera_cell_index];
				const auto cell_index = cell - game::sp::rgp->world->cells;

				if (single_cell)
				{
					glob->farPlane = nullptr;
				}

				game::sp::R_AddCellSurfacesAndCullGroupsInFrustumDelayed(cell, dpvs->frustumPlanes, dpvs->frustumPlaneCount, dpvs->frustumPlaneCount); // dpvs->frustumPlaneCount
				glob->cellVisibleBits[(cell_index >> 5) + 3] |= (1 << (cell_index & 0x1F));


				// #
				// draw cell index at the center of the current cell
				if (dvars::r_showCellIndex && dvars::r_showCellIndex->current.enabled)
				{
					const game::vec4_t col = { 1.0f, 1.0f, 1.0f, 1.0f };
					game::sp::draw_text_with_engine(
						/* x	*/ 50.0f,
						/* y	*/ 500.0f,
						/* scaX */ 1,
						/* scaY */ 1,
						/* font */ "fonts/boldFont",
						/* colr */ col,
						/* txt	*/ utils::va("Current Cell: %d", camera_cell_index));
				}


				// #
				// force cells defined in map_settings.ini

				if (!single_cell && map_settings::settings()->cell_overrides_exist && camera_cell_index < 1024)
				{
					const auto& c_ow = map_settings::settings()->cell_settings[camera_cell_index];
					if (c_ow.active)
					{
						for (const auto& i : c_ow.forced_cell_indices)
						{
							const auto forced_cell = &game::sp::rgp->world->cells[i];
							const auto c_index = forced_cell - game::sp::rgp->world->cells;
							game::sp::R_AddCellSurfacesAndCullGroupsInFrustumDelayed(forced_cell, dpvs->frustumPlanes, dpvs->frustumPlaneCount, dpvs->frustumPlaneCount);
							glob->cellVisibleBits[(c_index >> 5) + 3] |= (1 << (c_index & 0x1F));
						}
					}
				}

				if (!single_cell)
				{
					// R_VisitPortals
					game::sp::R_VisitPortals(dpvs->frustumPlaneCount, cell, &glob->viewPlane, dpvs->frustumPlanes); // viewplane here .. or is that the nearplane?
				}
			}
		}

		__declspec(naked) void cell_stub()
		{
			const static uint32_t retn_addr = 0x6E557F;
			__asm
			{
				// ebx = world
				// esi = cameraCellIndex
				// eax = DpvsView

				push	eax;
				push	esi;
				call	R_AddWorldSurfacesPortalWalk_hk;
				add		esp, 8;
				jmp		retn_addr;
			}
		}

		// #
		// # tweakable culling dvars

		//__declspec(naked) void disable_mins_culling_stub()
		//{
		//	const static uint32_t stock_addr = 0x643B08;
		//	const static uint32_t disable_culling_addr = 0x643B0E;
		//	__asm
		//	{
		//		// jump if culling is less or disabled
		//		cmp		rtx::loc_culling_tweak_mins, 1;
		//		je		SKIP;

		//		fnstsw  ax;
		//		test    ah, 0x41;
		//		jmp		stock_addr;

		//	SKIP:
		//		jmp		disable_culling_addr;
		//	}
		//}

		//__declspec(naked) void disable_maxs_culling_stub()
		//{
		//	const static uint32_t stock_addr = 0x643B39;
		//	const static uint32_t disable_culling_addr = 0x643B48;
		//	__asm
		//	{
		//		// jump if culling is less or disabled
		//		cmp		rtx::loc_culling_tweak_maxs, 1;
		//		je		SKIP;

		//		fnstsw  ax;
		//		test    ah, 1;
		//		jmp		stock_addr;

		//	SKIP:
		//		jmp		disable_culling_addr;
		//	}
		//}

		//__declspec(naked) void disable_frustum_culling_stub()
		//{
		//	//const static uint32_t stock_addr = 0x643D44;
		//	const static uint32_t disable_culling_addr = 0x643D80;
		//	__asm
		//	{
		//		// og
		//		xor		ecx, ecx;

		//		// jump if culling is less or disabled
		//		//cmp		rtx::loc_culling_tweak_frustum, 1;
		//		//je		SKIP;
		//		//
		//		//cmp		[esp + 0x54], ecx;
		//		//jmp		stock_addr;

		//	//SKIP:
		//		jmp		disable_culling_addr;
		//	}
		//}

		//__declspec(naked) void disable_smodel_culling_stub()
		//{
		//	const static uint32_t stock_addr = 0x643C79;
		//	const static uint32_t disable_culling_addr = 0x643CB5;
		//	__asm
		//	{
		//		// og
		//		xor ecx, ecx;

		//		// jump if culling is less or disabled
		//		cmp		rtx::loc_culling_tweak_smodel, 1;
		//		je		SKIP;

		//		cmp[esp + 0x54], ecx;
		//		jmp		stock_addr;

		//	SKIP:
		//		jmp		disable_culling_addr;
		//	}
		//}

#pragma region OLD_CULLING
		// #
		// old anti culling - active if flag 'old_anti_culling' is set

		// R_AddWorldSurfacesPortalWalk
		__declspec(naked) void world_stub_01()
		{
			const static uint32_t retn_skip = 0x6E5585;
			const static uint32_t retn_stock = 0x6E551E;
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
			const static uint32_t retn_stock = 0x739D18;
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
			const static uint32_t retn_stock = 0x7399B3;
			const static uint32_t retn_stock_jumped = 0x7399DB;
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
				jz		loc_7399DB;
				mov     esi, [ecx + 0x20];
				jmp		retn_stock;

			loc_7399DB: // og code
				jmp		retn_stock_jumped;


			SKIP:		// skip static model rendering
				mov     esi, [ecx + 0x20];
				jmp		retn_stock_jumped;
			}
		}

		// R_AddAabbTreeSurfacesInFrustum_r
		__declspec(naked) void world_stub_02()
		{
			const static uint32_t retn_skip = 0x73991F;
			const static uint32_t retn_stock = 0x739919;
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
			const static uint32_t retn_skip = 0x739988;
			const static uint32_t retn_stock = 0x739955;
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
			const static uint32_t retn_skip = 0x74A31B;
			const static uint32_t retn_stock = 0x74A315;
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
#pragma endregion
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
		const static uint32_t skip_img_addr = 0x73C6B3;
		const static uint32_t og_logic_addr = 0x73C6AB;
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
		utils::hook::nop(0x59A21F, 6);
		utils::hook(0x59A21F, Com_PrintMessage_stub, HOOK_JUMP).install()->quick();
#endif

		// *
		// general

		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		utils::hook::nop(0x6E8B96, 8); utils::hook(0x6E8B96, rb_draw3d_internal_stub, HOOK_JUMP).install()->quick();

		// hook R_SetMaterial - material/technique replacing
		utils::hook(0x741F1E, r_set_material, HOOK_CALL).install()->quick();

		// precaching xmodels beyond level load (dynamic skysphere spawning)
		utils::hook::set<BYTE>(0x54A4D6, 0xEB);

		// disable loading of specular and normalmaps (de-clutter remix ui)
		if (!flags::has_flag("load_normal_spec"))
		{
			utils::hook::nop(0x70F85C, 5);
			utils::hook::nop(0x73C6A4, 7); utils::hook(0x73C6A4, load_image_stub, HOOK_JUMP).install()->quick();
		}

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

		// change default value of r_fullscreen to 0
		utils::hook::set<BYTE>(0x707AA2 + 1, 0x0); 

		// ------------------------------------------------------------------------

		// disable dynent drawing (could have stable hashes but no stable lods right now)
		//utils::hook::nop(0x6DD7DD, 5);

		// implement r_forcelod logic for skinned models (R_SkinXModel)
		utils::hook::nop(0x6D9C10, 7);  utils::hook(0x6D9C10, skinned_xmodel_get_lod_for_dist_inlined, HOOK_JUMP).install()->quick();

		// implement r_forcelod logic for all other static models (R_AddAllStaticModelSurfacesCamera)
		utils::hook::nop(0x732CD6, 7);  utils::hook(0x732CD6, rigid_xmodel_get_lod_for_dist_inlined, HOOK_JUMP).install()->quick();

		// note: dvar 'r_fovScaleThresholdRigid' can be used to stop fov related lod changes


		// dvar 'r_warm_dpvs' can be used to disable all culling (no longer needed > use dvar 'rtx_disable_world_culling')
		// DISABLE CULLING :: stop 'r_warm_dpvs' dvar from resetting itself -- 0x6E5D21 adds static models per cell -- 0x7398B4 frustum culling
		utils::hook::set<BYTE>(0x6DDED8, 0xEB);

		// ^ do not enable 'r_warm_dpvs' by default
		utils::hook::nop(0x6F69EE, 5);


		// *
		// culling

		if (flags::has_flag("old_anti_culling"))
		{
			OLD_CULLING_ACTIVE = true;

			// R_AddWorldSurfacesPortalWalk :: less culling
			utils::hook::nop(0x6E5518, 6); utils::hook(0x6E5518, cull::world_stub_01, HOOK_JUMP).install()->quick();

			{
				// note: using 'rtx_disable_world_culling' = 'less' results in unstable world geometry hashes (but stable static model hashes)
				// -> using 'all-but-models' leaves culling logic for static models at 'less' but does not cull world geometry

				// R_AddCellStaticSurfacesInFrustumCmd :: stub used to reset the skip model check from the stub below
				utils::hook::nop(0x739D12, 6); utils::hook(0x739D12, cull::world_stub_02_reset_helper, HOOK_JUMP).install()->quick();
				// R_AddAabbTreeSurfacesInFrustum_r :: check if culling mode 'all-but-models' is active - check note above
				utils::hook(0x7399AE, cull::world_stub_02_skip_static_model, HOOK_JUMP).install()->quick();

				// R_AddAabbTreeSurfacesInFrustum_r :: mins check
				utils::hook::nop(0x739912, 7); utils::hook(0x739912, cull::world_stub_02, HOOK_JUMP).install()->quick();
			}

			// R_AddAabbTreeSurfacesInFrustum_r :: maxs check
			utils::hook::nop(0x73994E, 7); utils::hook(0x73994E, cull::world_stub_03, HOOK_JUMP).install()->quick();

			// R_AddCellSceneEntSurfacesInFrustumCmd :: active ents like destructible cars / players
			utils::hook::nop(0x74A30D, 8); utils::hook(0x74A30D, cull::entities_stub, HOOK_JUMP).install()->quick();

			// R_AddCellSceneEntSurfacesInFrustumCmd :: brushmodels
			utils::hook::nop(0x74A50A, 2);

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
		else // new way of culling
		{
			// rewrite some logic in 'R_AddWorldSurfacesPortalWalk'
			utils::hook::nop(0x6E552F, 9); utils::hook(0x6E552F, cull::cell_stub, HOOK_JUMP).install()->quick();

			// ^ - never show all cells at once when the camera cell index is < 0, we handle that in the func above
			utils::hook::nop(0x6E551E, 2);


			// #
			// tweakable culling via dvars

			// only frustum and static

			// disable mins culling - 0x643B08 nop 6
			//utils::hook(0x643B03, cull::disable_mins_culling_stub, HOOK_JUMP).install()->quick();

			// disable maxs culling - 0x643B39 to jmp
			//utils::hook(0x643B34, cull::disable_maxs_culling_stub, HOOK_JUMP).install()->quick();

			// disable (most) frustum culling - 0x643D44 to jmp
			//utils::hook::nop(0x7399F6, 6); utils::hook(0x7399F6, cull::disable_frustum_culling_stub, HOOK_JUMP).install()->quick();
			utils::hook::nop(0x739BBA, 2); // func is not inlined and a little different compared to iw3

			// ^ for smodels - 0x643C79 to jmp
			//utils::hook::nop(0x643C73, 6); utils::hook(0x643C73, cull::disable_smodel_culling_stub, HOOK_JUMP).install()->quick();
			utils::hook::nop(0x739B4F, 2); // ^ same

			//dvars::rtx_culling_tweak_mins = game::Dvar_RegisterBool(
			//	/* name		*/ "rtx_culling_tweak_mins",
			//	/* desc		*/ "Disable dpvs mins check (reduces culling)",
			//	/* default	*/ false,
			//	/* flags	*/ game::dvar_flags::saved);

			//dvars::rtx_culling_tweak_maxs = game::Dvar_RegisterBool(
			//	/* name		*/ "rtx_culling_tweak_maxs",
			//	/* desc		*/ "Disable dpvs maxs check (reduces culling)",
			//	/* default	*/ false,
			//	/* flags	*/ game::dvar_flags::saved);

			//dvars::rtx_culling_tweak_frustum = game::Dvar_RegisterBool(
			//	/* name		*/ "rtx_culling_tweak_frustum",
			//	/* desc		*/ "Disable (most) frustum culling (reduces culling)",
			//	/* default	*/ true,
			//	/* flags	*/ game::dvar_flags::saved);

			//dvars::rtx_culling_tweak_smodel = game::Dvar_RegisterBool(
			//	/* name		*/ "rtx_culling_tweak_smodel",
			//	/* desc		*/ "Disable static model frustum culling (reduces culling)",
			//	/* default	*/ true,
			//	/* flags	*/ game::dvar_flags::saved);

			dvars::rtx_culling_plane_dist = game::Dvar_RegisterFloat(
				/* name		*/ "rtx_culling_plane_dist",
				/* default	*/ 1250.0f,
				/* minVal	*/ -50000.0f,
				/* maxVal	*/ 50000.0f,
				/* flags	*/ game::dvar_flags::saved,
				/* desc		*/ "Distance of dpvs culling-planes"
			);

			// never cull brushmodels via dpvs
			//utils::hook::nop(0x64D35B, 2);
			//utils::hook::set<BYTE>(0x64D368, 0xEB); // ^
			utils::hook::nop(0x74A50A, 2);
			utils::hook::nop(0x74A524, 2);
			
			// ^ scene ents (spawned map markers (script models))
			//utils::hook::nop(0x64D17A, 2);
			//utils::hook::set<BYTE>(0x64D1A9, 0xEB); // ^
			utils::hook::nop(0x74A319, 2);
			utils::hook::set<BYTE>(0x74A357, 0xEB); // ^
		}

		// ignore "too many static models ..." msg - still add model to the scene
		utils::hook::nop(0x732D25, 6); utils::hook::jump(0x732D25, 0x732E22);

		// do not add dynent bmodels
		//utils::hook::nop(0x6DD7BE, 5);

		// fix nullptr access (gfxsurface->material ptr) that can occur when culling is disabled (certain sp maps)
		utils::hook::nop(0x6DACB0, 6); utils::hook(0x6DACB0, r_add_bmodel_surfaces_camera_stub, HOOK_JUMP).install()->quick();

		// hook FX_CullSphere to implement an additional radius check
		utils::hook::nop(0x4B4120, 6); utils::hook(0x4B4120, fx_cullsphere_stub, HOOK_JUMP).install()->quick();

		// debug visualizations: disable need for enabled developer dvar 
		utils::hook::nop(0x6E6528, 2);

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

		static const char* r_showTess_enum[] = { "off", "model_techset", "model_material", "bsp_techset", "bsp_material", "bmodel_techset", "bmodel_material" };
		dvars::r_showTess = game::Dvar_RegisterEnum(
			/* name		*/ "r_showTess",
			/* enumData */ r_showTess_enum,
			/* enumSize */ 7,
			/* default	*/ 0,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "surface data info"
		);

		dvars::r_showTessSkin = game::Dvar_RegisterBool(
			/* name		*/ "r_showTessSkin",
			/* desc		*/ "show skinned surface info",
			/* default	*/ false,
			/* flags	*/ game::dvar_flags::none);

		dvars::r_showTessDist = game::Dvar_RegisterFloat(
			/* name		*/ "r_showTessDist",
			/* default	*/ 400.0f,
			/* min		*/ 0.0f,
			/* max		*/ 10000.0f,
			/* flags	*/ game::dvar_flags::saved,
			/* desc		*/ "radius in which to draw r_showTess debug strings"
		);

		dvars::r_showCellIndex = game::Dvar_RegisterBool(
			/* name		*/ "r_showCellIndex",
			/* desc		*/ "draw cell index at the center of current cell (useful for map_settings)",
			/* default	*/ false,
			/* flags	*/ game::dvar_flags::none);

		dvars::rtx_water_uv_scale = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_water_uv_scale",
			/* default	*/ 0.25f,
			/* min		*/ 0.001f,
			/* max		*/ 100.0f,
			/* flags	*/ game::dvar_flags::saved,
			/* desc		*/ "UV scale of water surfaces (global scalar)");

		dvars::rtx_muzzleflash_duration = game::Dvar_RegisterInt(
			/* name		*/ "rtx_muzzleflash_duration",
			/* default	*/ 1,
			/* min		*/ 0,
			/* max		*/ 60,
			/* flags	*/ game::dvar_flags::saved,
			/* desc		*/ "lifetime of muzzleflash sphere lights");

		dvars::rtx_muzzleflash_radiance = game::Dvar_RegisterVec3(
			/* name		*/ "rtx_muzzleflash_radiance",
			/* x		*/ 50.0f,
			/* y		*/ 40.0f,
			/* z		*/ 30.0f,
			/* min		*/ 0.01f,
			/* max		*/ 1000.0f,
			/* flags	*/ game::dvar_flags::saved,
			/* desc		*/ "radiance of muzzleflash sphere light");

		dvars::rtx_muzzleflash_radius = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_muzzleflash_radius",
			/* default	*/ 8.0f,
			/* min		*/ 0.01f,
			/* max		*/ 100.0f,
			/* flags	*/ game::dvar_flags::saved,
			/* desc		*/ "radius of muzzleflash sphere light");

		// ------------------------------------------------------------------------

#ifdef GIT_DESCRIBE
		static auto version_str = std::string("t4-rtx-"s + GIT_DESCRIBE + " > "s);
#else
		static auto version_str = "t4-rtx > "s;
#endif

		

		// console string ;)
		utils::hook::set<const char*>(0x472E13 + 1, version_str.c_str());

		// solo scoreboard
		utils::hook::nop(0x437ACC, 5);

		// t4m
		game::sp::DB_ReallocXAssetPool(game::ASSET_TYPE_FX, 600);
		game::sp::DB_ReallocXAssetPool(game::ASSET_TYPE_IMAGE, 4096);
		game::sp::DB_ReallocXAssetPool(game::ASSET_TYPE_LOADED_SOUND, 2400);
		game::sp::DB_ReallocXAssetPool(game::ASSET_TYPE_MATERIAL, 4096);
		game::sp::DB_ReallocXAssetPool(game::ASSET_TYPE_WEAPON, 320);
		game::sp::DB_ReallocXAssetPool(game::ASSET_TYPE_XMODEL, 1500);

		// increase g_mem
		utils::hook::set<DWORD>(0x5F5492, 0x19600000);
		utils::hook::set<DWORD>(0x5F54D1, 0x19600000);
		utils::hook::set<DWORD>(0x5F54DB, 0x19600000);

		// no forward/backslash for console cmds
		//utils::hook::nop(0x493DEF, 5);

		command::add("rtx_sky", [](command::params p)
		{
			if (p.length() > 1)
			{
				main_module::skysphere_spawn(utils::try_stoi(p[1]));
			}
			else
			{
				game::sp::Com_PrintMessage(0, "Usage: rtx_sky <index>\n", 0);
			}
			
		});

		command::add("export_entities", [this](command::params)
		{
			export_entity_string();
		});

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

		command::add("unlockall", [this](command::params)
		{
			game::sp::Cmd_ExecuteSingleCommand(0, 0, "seta mis_01 50");
		});
	}
}
