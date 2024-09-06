#pragma once

namespace dvars
{
	extern game::dvar_s* fx_cull_elem_draw_radius;
	extern game::dvar_s* rtx_disable_world_culling;
	extern game::dvar_s* rtx_disable_entity_culling;
	extern game::dvar_s* rtx_sky_follow_player;

	extern game::dvar_s* rtx_culling_tweak_mins;
	extern game::dvar_s* rtx_culling_tweak_maxs;
	extern game::dvar_s* rtx_culling_tweak_frustum;
	extern game::dvar_s* rtx_culling_tweak_smodel;
	extern game::dvar_s* rtx_culling_plane_dist;

	extern game::dvar_s* r_showTess;
	extern game::dvar_s* r_showTessSkin;
	extern game::dvar_s* r_showTessDist;
	extern game::dvar_s* r_showCellIndex;
	extern game::dvar_s* r_showModelNames;

	extern game::dvar_s* rtx_water_uv_scale;
	extern game::dvar_s* rtx_muzzleflash_duration;
	extern game::dvar_s* rtx_muzzleflash_radiance;
	extern game::dvar_s* rtx_muzzleflash_radius;
	extern game::dvar_s* rtx_fog_start;

	extern game::dvar_s* rtx_primarylight_radiance;
	extern game::dvar_s* rtx_primarylight_radius;
	extern game::dvar_s* rtx_primarylight_softness;
	extern game::dvar_s* rtx_primarylight_exp;
	extern game::dvar_s* rtx_fxlight_hash_dist;
}