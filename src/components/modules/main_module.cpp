#include "std_include.hpp"

#define HIDWORD(x)  (*((DWORD*)&(x)+1))

namespace components
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

		const auto dev = game::dx->device;
		dev->SetLight(0, &light);
		dev->LightEnable(0, TRUE);
		dev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	void setup_rtx()
	{
		const auto dev = game::dx->device;
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
		if (const auto	material = game::Material_RegisterHandle(material_name, 3);
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
		/*if (mat.switch_technique_type)
		{
			if (_renderer::is_valid_technique_for_type(mat.current_material, mat.technique_type))
			{
				_renderer::switch_technique(&mat, mat.current_material);
			}
		}*/

		state->material = mat.material;
		state->technique = mat.technique;

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

	main_module::main_module()
	{
		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		utils::hook::nop(0x6BAB26, 8); utils::hook(0x6BAB26, rb_draw3d_internal_stub, HOOK_JUMP).install()->quick();

		// hook R_SetMaterial
		utils::hook(0x717F9E, r_set_material, HOOK_CALL).install()->quick();

		// console string ;)
		utils::hook::set<const char*>(0x48AF32 + 1, "t4-rtx >");

		// no forward/backslash for console cmds
		utils::hook::nop(0x493DEF, 5);

		// un-cheat + saved flag for fx_enable
		utils::hook::set<BYTE>(0x4D1406 + 1, 0x01); // was 0x80

		// remove improper quit popup - nvapi is imported to late for this
		//utils::hook::set<BYTE>(0x5D03E6, 0xEB);
	}

	main_module::~main_module()
	{ }
}
