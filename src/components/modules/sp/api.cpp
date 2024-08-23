#include "std_include.hpp"

using namespace game::sp;

namespace components::sp
{
	// called once per frame from main_module::setup_rtx
	void api::bullet_fire_frame_cb()
	{
		if (api::bridge.initialized)
		{
			for (auto& l : muzzleflash_lights)
			{
				if (l.hash && l.framecounter)
				{
					l.framecounter--;
					api::bridge.DrawLightInstance(l.hash);
				}
			}
		}
	}

	api::muzzleflash_light_s* api::get_avail_light()
	{
		muzzleflash_light_s* light = &muzzleflash_lights[0];
		for (uint8_t l = 0u; l < 12; l++)
		{
			auto e = &muzzleflash_lights[l];
			if (!e->framecounter)
			{
				if (!e->hash) // create hash on first use
				{
					e->hash = utils::fnv1a_hash("muzzleflash" + std::to_string(l));
				}

				light = e;
				break;
			}
		}

		return light;
	}

	void bullet_fire_event(const game::weaponParms* wp, const game::gentity_s* ent)
	{
		if (wp && api::bridge.initialized)
		{
			game::vec3_t muzzle_origin = { wp->muzzleTrace[0], wp->muzzleTrace[1], wp->muzzleTrace[2] };

			// get actual muzzleflash point (ignore npc's)
			if (ent && ent->client && !game::sp::cgs->renderingThirdPerson)
			{
				game::orientation_t orient = {};
				std::uint8_t bone = 254;

				std::uint32_t weapon_num = ent->s.weapon;
				if (weapon_num > 0)
				{
					const auto obj = game::sp::cg_weaponsArray[weapon_num].viewModelDObj;
					if (game::sp::DObjGetBoneIndex(obj, game::sp::scr_const->tag_flash, &bone))
					{
						// get orientation of tag_flash
						game::sp::CG_DObjGetWorldBoneMatrix(&game::sp::cgs->viewModelPose, bone, obj, orient.axis[0], orient.origin);
						utils::copy(orient.origin, muzzle_origin, 3);
					}
				}
			}

			//
			auto avail_light = api::get_avail_light();

			x86::remixapi_LightInfo l = {};
			{
				l.sType = REMIXAPI_STRUCT_TYPE_LIGHT_INFO;
				l.hash = avail_light->hash;

				if (dvars::rtx_muzzleflash_radiance)
				{
					l.radiance =
					{
						dvars::rtx_muzzleflash_radiance->current.vector[0],
						dvars::rtx_muzzleflash_radiance->current.vector[1],
						dvars::rtx_muzzleflash_radiance->current.vector[2]
					};
				}
				else
				{
					l.radiance = { 40.0f, 30.0f, 20.0f };
				}
			}

			x86::remixapi_LightInfoSphereEXT s = {};
			{
				s.sType = REMIXAPI_STRUCT_TYPE_LIGHT_INFO_SPHERE_EXT;
				s.position = { muzzle_origin[0], muzzle_origin[1], muzzle_origin[2], };
				s.radius = dvars::rtx_muzzleflash_radius ? dvars::rtx_muzzleflash_radius->current.value : 1.0f;
				s.shaping_hasvalue = FALSE;
			}

			avail_light->hash = api::bridge.CreateSphereLight(&l, &s);
			avail_light->framecounter = dvars::rtx_muzzleflash_duration ? (uint8_t) dvars::rtx_muzzleflash_duration->current.integer : 2u;
		}
	}

	__declspec(naked) void bullet_fire_stub()
	{
		const static uint32_t retn_addr = 0x4E6909;
		__asm
		{
			pushad;
			push	edi; // ent
			push	esi;
			call	bullet_fire_event;
			add		esp, 8;
			popad;

			// og code
			movss   xmm0, dword ptr[ebp + 0xC];
			jmp		retn_addr;
		}
	}


	// #
	// other api lights

	void api::track_and_draw_light_hash(std::uint64_t hash)
	{
		api_lights.insert(hash);
	}

	void api::remove_and_destroy_light(std::uint64_t hash)
	{
		api_lights.erase(hash);
		bridge.DestroyLight(hash);
	}

	void endscene_callback()
	{
		for (const auto& hash : api::api_lights)
		{
			api::bridge.DrawLightInstance(hash);
		}
	}

	// -----------------------------
	// -----------------------------

	// called from main_module::on_map_load()
	void api::on_map_load()
	{
		if (api::is_initialized())
		{
			api_lights.clear();
		}
	}

	bool api::is_initialized()
	{
		return bridge.initialized;
	}

	BRIDGEAPI_ErrorCode api::init()
	{
		const auto status = bridgeapi_initialize(&bridge);
		game::sp::Com_PrintMessage(0, utils::va("[BridgeApi] bridgeapi_initialize() : %s \n", !status ? "success" : utils::va("error : %d", status)), 0);

		if (status == BRIDGEAPI_ERROR_CODE_SUCCESS && bridge.initialized)
		{
			bridge.RegisterDevice();
			bridge.RegisterEndSceneCallback(endscene_callback);
		}

		return status;
	}

	api::api()
	{
		p_this = this;

		scheduler::once([]
		{
			api::init();
		}, scheduler::main);

		// hook bullet fire to spawn muzzleflash lights
		utils::hook(0x4E6904, bullet_fire_stub, HOOK_JUMP).install()->quick();

		dvars::rtx_primarylight_tweak_radiance = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_tweak_radiance",
			/* default	*/ 10.0f,
			/* minVal	*/ -10000.0f,
			/* maxVal	*/ 10000.0f,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Radiance scalar for rtx primary lights");

		dvars::rtx_primarylight_tweak_radius = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_tweak_radius",
			/* default	*/ 0.0f,
			/* minVal	*/ -10000.0f,
			/* maxVal	*/ 10000.0f,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Radius offset for rtx primary lights");

		dvars::rtx_primarylight_tweak_softness = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_tweak_softness",
			/* default	*/ 0.0f,
			/* minVal	*/ 0.2f,
			/* maxVal	*/ M_PI,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Softness for rtx primary lights");

		dvars::rtx_primarylight_tweak_exp = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_tweak_exp",
			/* default	*/ 0.0f,
			/* minVal	*/ 0.0f,
			/* maxVal	*/ 10.0f,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Expo for rtx primary lights");
	}
}
