#include "std_include.hpp"

namespace dvars
{
	game::dvar_s* fx_cull_elem_draw_radius = nullptr;
	game::dvar_s* rtx_disable_world_culling = nullptr;
	game::dvar_s* rtx_disable_entity_culling = nullptr;
	game::dvar_s* rtx_sky_follow_player = nullptr;

	game::dvar_s* rtx_culling_tweak_mins = nullptr;
	game::dvar_s* rtx_culling_tweak_maxs = nullptr;
	game::dvar_s* rtx_culling_tweak_frustum = nullptr;
	game::dvar_s* rtx_culling_tweak_smodel = nullptr;
	game::dvar_s* rtx_culling_plane_dist = nullptr;

	game::dvar_s* r_showTess = nullptr;
	game::dvar_s* r_showTessSkin = nullptr;
	game::dvar_s* r_showTessDist = nullptr;
	game::dvar_s* r_showCellIndex = nullptr;

	game::dvar_s* rtx_water_uv_scale = nullptr;
	game::dvar_s* rtx_muzzleflash_duration = nullptr;
	game::dvar_s* rtx_muzzleflash_radiance = nullptr;
	game::dvar_s* rtx_muzzleflash_radius = nullptr;

	game::dvar_s* rtx_primarylight_tweak_radiance = nullptr;
	game::dvar_s* rtx_primarylight_tweak_radius = nullptr;
	game::dvar_s* rtx_primarylight_tweak_softness = nullptr;
	game::dvar_s* rtx_primarylight_tweak_exp = nullptr;
}