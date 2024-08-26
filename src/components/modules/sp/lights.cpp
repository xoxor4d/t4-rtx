#include "std_include.hpp"

using namespace game::sp;
namespace components::sp
{
	// > main_module::on_set_cgame_time()
	void lights::on_client_frame()
	{
		if (game::sp::clientUI->connectionState == game::CA_ACTIVE)
		{
			// #
			// fx omni lights

			// start of frame: flag is used do discard spawned fx omni lights if we fail to
			// assign previously spawned lights to lights created this frame
			for (auto& light : m_fx_omni_lights)
			{
				light.new_or_matched_previous_frame = false;
			}


			// #
			// primary lights

			// clear entity flag - we re-check and assign an entity to a primary light each frame
			for (auto& light : m_game_primary_lights)
			{
				light.has_entity = false;
			}

			// #
			auto update_light = [](const game::centity_s* e)
				{
					if (e && e->nextState.eType == game::ET_PRIMARY_LIGHT)
					{
						const auto pl_ent = &e->currentState.u.primaryLight;
						const auto pl_index = (std::uint32_t)e->nextState.index.primaryLight;

						if (pl_index < m_game_primary_lights.size())
						{
							auto& entry = m_game_primary_lights[pl_index];
							entry.has_entity = true;

							game::vec4_t unpacked_col = {};
							utils::byte4_unpack_rgba(pl_ent->colorAndExp, unpacked_col);

							entry.l.radiance =
							{
								unpacked_col[0] * pl_ent->radius * dvars::rtx_primarylight_radiance->current.value * pl_ent->intensity * 1.3f,
								unpacked_col[1] * pl_ent->radius * dvars::rtx_primarylight_radiance->current.value * pl_ent->intensity,
								unpacked_col[2] * pl_ent->radius * dvars::rtx_primarylight_radiance->current.value * pl_ent->intensity,
							};

							entry.s.position = { e->currentState.pos.trBase[0], e->currentState.pos.trBase[1], e->currentState.pos.trBase[2], };

							entry.s.shaping_hasvalue = pl_ent->radius != 0.0f;
							if (entry.s.shaping_hasvalue)
							{
								utils::angle_vectors(e->pose.angles, &entry.s.shaping_value.direction.x, nullptr, nullptr);

								const float phi = acos(pl_ent->cosHalfFovOuter);
								entry.s.shaping_value.coneAngleDegrees = phi * (180.0f / M_PI);

								const float theta = acos(pl_ent->cosHalfFovInner);
								entry.s.shaping_value.coneSoftness = std::cos(theta / 2.0f) - std::cos(phi);

								entry.s.shaping_value.focusExponent = dvars::rtx_primarylight_exp->current.value;
							}

							api::bridge.DestroyLight(entry.l.hash);
							if (api::bridge.CreateSphereLight(&entry.l, &entry.s))
							{
								if (!entry.tracked)
								{
									lights::light_hash_track_and_draw(entry.l.hash);
									entry.tracked = true;
								}
							}
						}
					}
				};

			// check if primary light is linked to any ent - update and spawn light
			for (auto i = 0u; i < 1024; i++)
			{
				update_light(&game::sp::cg_fakeEntitiesArray[i].cent);
			}

			for (auto i = 0u; i < 1024; i++)
			{
				update_light(&game::sp::cg_entitiesArray[i]);
			}

			// also do g_entities?

			// create lights that have no entity attached
			for (auto& light : m_game_primary_lights)
			{
				if (light.index && light.s.radius > 0.0f && !light.has_entity && !light.tracked)
				{
					if (api::bridge.CreateSphereLight(&light.l, &light.s))
					{
						lights::light_hash_track_and_draw(light.l.hash);
						light.tracked = true;
					}
				}
			}
		}
	}

	/**
	 * Converts all primary lights (ComWorld) to remix lights. Should only be called once on map start.
	 */
	void lights::create_primary_lights_for_map()
	{
		m_game_primary_lights.clear();
		m_game_primary_lights.emplace_back(); // always ignore light 0 (sun)

		for (auto i = 1u; i < game::sp::com_world->primaryLightCount; i++)
		{
			const auto pl = &game::sp::com_world->primaryLights[i];

			game_primary_light_s entry = {};
			entry.index = i;
			entry.og_light = pl;

			entry.l.sType = REMIXAPI_STRUCT_TYPE_LIGHT_INFO;
			entry.l.hash = utils::fnv1a_hash("primary_light" + std::to_string(i));
			entry.l.radiance =
			{
				pl->color[0] * pl->radius * dvars::rtx_primarylight_radiance->current.value * 1.3f,
				pl->color[1] * pl->radius * dvars::rtx_primarylight_radiance->current.value,
				pl->color[2] * pl->radius * dvars::rtx_primarylight_radiance->current.value,
			};

			entry.s.sType = REMIXAPI_STRUCT_TYPE_LIGHT_INFO_SPHERE_EXT;
			entry.s.position = { pl->origin[0], pl->origin[1], pl->origin[2], };
			entry.s.radius = 1.0f + dvars::rtx_primarylight_radius->current.value;

			entry.s.shaping_hasvalue = pl->radius != 0.0f;
			if (entry.s.shaping_hasvalue)
			{
				entry.s.shaping_value.direction.x = pl->dir[0];
				entry.s.shaping_value.direction.y = pl->dir[1];
				entry.s.shaping_value.direction.z = pl->dir[2];
				utils::scale3(&entry.s.shaping_value.direction.x, -1.0f, &entry.s.shaping_value.direction.x);

				const float phi = acos(pl->cosHalfFovOuter);
				entry.s.shaping_value.coneAngleDegrees = phi * (180.0f / M_PI);

				const float theta = acos(pl->cosHalfFovInner);
				entry.s.shaping_value.coneSoftness = std::cos(theta / 2.0f) - std::cos(phi);

				//s.shaping_value.coneSoftness = dvars::rtx_primarylight_tweak_softness->current.value;
				entry.s.shaping_value.focusExponent = dvars::rtx_primarylight_exp->current.value;
			}

			m_game_primary_lights.emplace_back(entry);
		}
	}

	// > main_module::on_map_load()
	void lights::on_map_load()
	{
		if (api::is_initialized())
		{
			for (const auto hash : m_api_light_hashes)
			{
				api::bridge.DestroyLight(hash);
			}
			m_api_light_hashes.clear();
		}

		lights::create_primary_lights_for_map();
	}

	// > main_module::endscene_callback()
	void lights::on_api_callback()
	{
		lights::check_added_fx_omnilights();

		for (const auto& hash : m_api_light_hashes)
		{
			api::bridge.DrawLightInstance(hash);
		}
	}

	/**
	 * Submit light for drawing
	 * @param hash	Hash of light
	 */
	void lights::light_hash_track_and_draw(const std::uint64_t hash)
	{
		m_api_light_hashes.insert(hash);
	}

	/**
	 * Disable light drawing and destroy light
	 * @param hash	Hash of light
	 */
	void lights::light_hash_untrack_and_destroy(const std::uint64_t hash)
	{
		m_api_light_hashes.erase(hash);
		api::bridge.DestroyLight(hash);
	}

	// #
	// fx omni lights

	/**
	 * Erases all unmatched fx omni lights
	 */
	void lights::check_added_fx_omnilights()
	{
		for (auto it = m_fx_omni_lights.begin(); it != m_fx_omni_lights.end(); ) 
		{
			// this light is at least a frame old and we failed to match it to any previous lights this frame
			// so delete this light as it was either created with a new hash or simply not spawned by the game
			if (!it->new_or_matched_previous_frame)
			{
				lights::light_hash_untrack_and_destroy(it->l.hash);
				it = m_fx_omni_lights.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// # Stub function only #
	// Memory address of posworld and other struct members around it are used (will probably crash if called outside the asm stub)
	// 
	// > Creates a remix light from omni fx lights. Tries to match "new" lights to lights created in prior frames
	// > by checking if the "new" origin is within a bounding box around the previous origin
	void add_fx_omnilight(const game::GfxLight* gfxlight, const float* posworld)
	{
		//const int light_num = *reinterpret_cast<int*>(0x3D52674);
		game::orientation_t* orient = reinterpret_cast<game::orientation_t*>(((std::uint32_t)posworld) - 0x50);

		/*struct FxElem
		{
			char defIndex;
			char sequence;
			char atRestFraction;
			char emitResidual;
			std::uint16_t nextElemHandleInEffect;
			std::uint16_t prevElemHandleInEffect;
			int msecBegin;
			float baseVel[3];
		};*/

		//FxElem* elem = reinterpret_cast<FxElem*>(*(DWORD*)((std::uint32_t)posworld - 0x50 - 0xC));
		//std::uint32_t elem_addr = (std::uint32_t)&elem->defIndex;

		if (gfxlight->radius > 0.0f)
		{
			bool matched_light = false;
			for (auto& light : lights::m_fx_omni_lights)
			{
				const float sdist = dvars::rtx_fxlight_hash_dist ? dvars::rtx_fxlight_hash_dist->current.value : 30.0f;

				// bounds from previous position
				const game::vec3_t mins = { light.s.position.x - sdist, light.s.position.y - sdist, light.s.position.z - sdist };
				const game::vec3_t maxs = { light.s.position.x + sdist, light.s.position.y + sdist, light.s.position.z + sdist };

				if (utils::point_in_bounds(gfxlight->origin, mins, maxs))
				{
					// update light properties

					light.l.radiance =
					{
						gfxlight->color[0] * gfxlight->radius * dvars::rtx_primarylight_radiance->current.value * 1.3f,
						gfxlight->color[1] * gfxlight->radius * dvars::rtx_primarylight_radiance->current.value,
						gfxlight->color[2] * gfxlight->radius * dvars::rtx_primarylight_radiance->current.value,
					};

					light.s.position = { gfxlight->origin[0], gfxlight->origin[1], gfxlight->origin[2], };
					light.s.radius = 1.0f + dvars::rtx_primarylight_radius->current.value;

					// update remix light
					api::bridge.DestroyLight(light.l.hash);
					if (api::bridge.CreateSphereLight(&light.l, &light.s))
					{
						light.new_or_matched_previous_frame = true;
						matched_light = true;
						break;
					}
				}
			}

			// new light
			if (!matched_light)
			{
				lights::fx_omni_light_s entry = {};

				entry.l.sType = REMIXAPI_STRUCT_TYPE_LIGHT_INFO;

				// works okay
				entry.l.hash = utils::fnv1a_hash("fxlight" + std::to_string((int)orient->origin[0]) + std::to_string((int)orient->origin[1]) + std::to_string((int)orient->origin[2]));

				//entry.l.hash = utils::fnv1a_hash("fx_omni_light" + std::to_string(elem_addr));
				//entry.l.hash = utils::fnv1a_hash("fx_omni_light" + std::to_string(light_num));

				entry.l.radiance =
				{
					gfxlight->color[0] * gfxlight->radius * dvars::rtx_primarylight_radiance->current.value * 1.3f,
					gfxlight->color[1] * gfxlight->radius * dvars::rtx_primarylight_radiance->current.value,
					gfxlight->color[2] * gfxlight->radius * dvars::rtx_primarylight_radiance->current.value,
				};

				entry.s.sType = REMIXAPI_STRUCT_TYPE_LIGHT_INFO_SPHERE_EXT;
				entry.s.position = { gfxlight->origin[0], gfxlight->origin[1], gfxlight->origin[2], };
				entry.s.radius = 1.0f /*+ (gfxlight->radius * 0.01f)*/ + dvars::rtx_primarylight_radius->current.value;

				entry.new_or_matched_previous_frame = true;

				if (api::bridge.CreateSphereLight(&entry.l, &entry.s))
				{
					lights::light_hash_track_and_draw(entry.l.hash);
					lights::m_fx_omni_lights.emplace_back(entry);
				}
			}
		}
	}

	__declspec(naked) void R_AddOmniLightToScene_stub()
	{
		const static uint32_t retn_addr = 0x6DA768;
		__asm
		{
			// og code
			mov     dword ptr[esi + 0x38], 0xFFFFFFFF;

			pushad;
			push	edi; // FxDrawState->posWorld
			push	esi; // GfxLight
			call	add_fx_omnilight;
			add		esp, 8;
			popad;
			jmp		retn_addr;
		}
	}


	// #
	// Unused bullet muzzleflash logic (game fx omni lights handle that now)

#if 0
	// called once per frame from main_module::setup_rtx
	void lights::bullet_fire_frame_cb()
	{
		if (api::is_initialized())
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

	lights::muzzleflash_light_s* lights::muzzleflash_get_avail_light()
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
			auto avail_light = lights::muzzleflash_get_avail_light();

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
			avail_light->framecounter = dvars::rtx_muzzleflash_duration ? (uint8_t)dvars::rtx_muzzleflash_duration->current.integer : 2u;
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
#endif

	// --------------------------------

	lights::lights()
	{
		p_this = this;

		// #
		// primary lights

		m_game_primary_lights.reserve(64);

		command::add("rtx_primarylight_update", [this](const command::params&)
		{
			if (api::is_initialized())
			{
				for (const auto& light : m_game_primary_lights)
				{
					lights::light_hash_untrack_and_destroy(light.l.hash);
				}

				lights::create_primary_lights_for_map();
			}
		});

		// #
		// fx omni lights

		utils::hook::nop(0x6DA761, 7);
			 utils::hook(0x6DA761, R_AddOmniLightToScene_stub, HOOK_JUMP).install()->quick();

		// #
		// muzzleflash lights

		// hook bullet fire to spawn muzzleflash lights
		// utils::hook(0x4E6904, bullet_fire_stub, HOOK_JUMP).install()->quick();
	}
}
