#include "std_include.hpp"

#define HIDWORD(x)  (*((DWORD*)&(x)+1))

using namespace game::mp;

namespace components
{
	namespace mp
	{
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

			//// update world matrix (R_Set3D)
			utils::hook::call<void(__fastcall)(int, game::GfxCmdBufSourceState*)>(0x6FEBB0)(0, src);

			//// directly set matrices on the device so that rtx-remix finds the camera
			dev->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&src->matrices.matrix[0].m));
			dev->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&src->viewParms.viewMatrix.m));
			dev->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&src->viewParms.projectionMatrix.m));

			//spawn_light();
			//setup_dvars_rtx();
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

		game::MaterialTechnique* Material_RegisterTechnique(const char* name, int is_renderer_in_use /*esi*/)
		{
			const static uint32_t func_addr = 0x6EB220;
			__asm
			{
				mov		esi, is_renderer_in_use;
				push	name;
				add     esp, 4;
				call	func_addr;
			}
		}

		int r_set_material(game::MaterialTechniqueType type, game::GfxCmdBufSourceState* src, game::GfxCmdBufState* state, game::GfxDrawSurf drawSurf)
		{
			const auto rgp = reinterpret_cast<game::r_global_permanent_t*>(0x1087BA80);

			game::switch_material_t mat = {};

			mat.current_material = rgp->sortedMaterials[(HIDWORD(drawSurf.packed) >> 1) & 2047];
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

			if (const auto var = Dvar_FindVar("r_znear"); var)
			{
				var->current.value = 40.0f; // 10.0 works but still a little wobble at larger distances
				var->domain.value.min = 0.0f;
				var->domain.value.max = FLT_MAX;
				var->flags = game::dvar_flags::userinfo;
			}

			if (const auto var = Dvar_FindVar("r_znear_depthhack"); var)
			{
				var->current.value = 39.9805f; // - 9.99512f see ^
				var->domain.value.min = 0.0f;
				var->domain.value.max = FLT_MAX;
				var->flags = game::dvar_flags::userinfo;
			}

			if (const auto var = Dvar_FindVar("r_fullbright"); var)
			{
				var->current.enabled = true;
				var->flags = game::dvar_flags::userinfo;
			}

			if (const auto var = Dvar_FindVar("fx_enable"); var)
			{
				var->current.enabled = false;
				var->flags = game::dvar_flags::userinfo;
			}

			// enable via cfg when in-game
			if (const auto var = Dvar_FindVar("r_warm_static"); var)
			{
				var->current.enabled = false;
				var->flags = game::dvar_flags::userinfo;
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
				call	register_rtx_dvars;
				popad;

				jmp		retn_addr;
			}
		}

		// ------------------------------------------------------------------------

		__declspec(naked) void force_lod_stub01()
		{
			const static uint32_t retn_addr = 0x70ACF9;
			__asm
			{
				// esi = LOD for model
				// eax = LOD count for model?

				xor		esi, esi;	// clear
				mov		esi, 0;		// LOD

				jmp		retn_addr;
			}
		}

		__declspec(naked) void force_lod_stub02()
		{
			const static uint32_t retn_addr = 0x70A74D;
			__asm
			{
				// ebx = LOD for model
				// eax = overwritten with r_warm_static dvar pointer here

				xor		ebx, ebx;	// clear
				mov		ebx, 0;		// LOD

				jmp		retn_addr;
			}
		}
	}

	main_module_mp::main_module_mp()
	{
		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		utils::hook::nop(0x6BAB26, 8); utils::hook(0x6BAB26, mp::rb_draw3d_internal_stub, HOOK_JUMP).install()->quick();

		// hook R_SetMaterial
		utils::hook(0x717F9E, mp::r_set_material, HOOK_CALL).install()->quick();


		// ------------------------------------------------------------------------


		// stub after 'R_InitGraphicsApi' (NVAPI Entry) to re-register stock dvars
		utils::hook(0x6AC0A4, mp::register_dvars_stub, HOOK_JUMP).install()->quick();

		// un-cheat + userinfo flag for fx_enable
		//utils::hook::set<BYTE>(0x4D1406 + 1, 0x01); // was 0x80


		// ------------------------------------------------------------------------


		if (flags::has_flag("disable_culling"))
		{
			// R_AddWorldSurfacesPortalWalk :: less culling
			utils::hook::set<BYTE>(0x6A945E, 0xEB);

			// R_AddAabbTreeSurfacesInFrustum_r :: disable all surface culling (bad fps)
			utils::hook::nop(0x6F6F29, 6);
			utils::hook::set<BYTE>(0x6F6F65, 0xEB);

			// TODO: entities
		}

		// disable dynent drawing (could have stable hashes but no stable lods right now)
		utils::hook::nop(0x6B624D, 5);

		// force static model LOD via 'r_warm_static' dvar (R_AddAllStaticModelSurfacesRangeSunShadow)
		utils::hook::nop(0x70ACF3, 6);
		utils::hook(0x70ACF3, mp::force_lod_stub01, HOOK_JUMP).install()->quick();

		// ^ (R_AddAllStaticModelSurfacesCamera)
		utils::hook::nop(0x70A747, 6);
		utils::hook(0x70A747, mp::force_lod_stub02, HOOK_JUMP).install()->quick();

		// remove 'r_warm_static' check that adds all meshes to the scene no matter what 'r_lodScaleRigid' is set to (R_AddAllStaticModelSurfacesCamera)
		// jz (0F 84 AC 01 00 00) to jmp (E9 AD 01 00 00 + 0x90)
		utils::hook::set<BYTE>(0x70A5E5, 0xE9);
		utils::hook::set<BYTE>(0x70A5E5 + 1, 0xAD); // < yes
		utils::hook::set<BYTE>(0x70A5E5 + 2, 0x01);
		utils::hook::set<BYTE>(0x70A5E5 + 3, 0x00);
		utils::hook::set<BYTE>(0x70A5E5 + 4, 0x00);
		utils::hook::set<BYTE>(0x70A5E5 + 5, 0x90);

		// stop 'r_warm_dpvs' dvar from resetting itself je 0x74 -> jmp 0xEB
		// can be used as anti cull
		utils::hook::set<BYTE>(0x6B6988, 0xEB);

		// ------------------------------------------------------------------------

#ifdef GIT_DESCRIBE
		static auto version_str = std::string("t4-rtx-"s + GIT_DESCRIBE + " > "s);
#else
		static auto version_str = "t4-rtx > "s;
#endif

		// console string ;)
		utils::hook::set<const char*>(0x48AF32 + 1, version_str.c_str());

		// no forward/backslash for console cmds
		utils::hook::nop(0x493DEF, 5);

		// remove improper quit popup - nvapi is imported to late for this
		//utils::hook::set<BYTE>(0x5D03E6, 0xEB);
	}
}