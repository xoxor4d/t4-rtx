#include "std_include.hpp"

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

	main_module::main_module()
	{
		// hook beginning of 'RB_Draw3DInternal' to setup general stuff required for rtx-remix
		utils::hook::nop(0x6BAB26, 8); utils::hook(0x6BAB26, rb_draw3d_internal_stub, HOOK_JUMP).install()->quick();

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
