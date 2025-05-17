#include "std_include.hpp"
#include "bridge_remix_api.h"

using namespace game::sp;

namespace components::sp
{
	// > main_module::on_map_load()
	void api::on_map_load()
	{ }

	// > d3d9::device::EndScene (callback registered via bridge api)
	void endscene_callback()
	{
		lights::on_api_callback();
	}

	bool api::is_initialized()
	{
		return get()->m_initialized;
	}


	remixapi_ErrorCode api::init()
	{
		const auto status = remixapi::bridge_initRemixApi(&bridge);
		game::sp::Com_PrintMessage(0, utils::va("[BridgeApi] bridgeapi_initialize() : %s \n", !status ? "success" : utils::va("error : %d", status)), 0);

		if (status == REMIXAPI_ERROR_CODE_SUCCESS) 
		{
			remixapi::bridge_setRemixApiCallbacks(nullptr, endscene_callback, nullptr);
			get()->m_initialized = true;
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

		dvars::rtx_primarylight_radiance = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_radiance",
			/* default	*/ 5.0f,
			/* minVal	*/ -10000.0f,
			/* maxVal	*/ 10000.0f,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Radiance scalar for rtx primary lights");

		dvars::rtx_primarylight_radius = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_radius",
			/* default	*/ 0.0f,
			/* minVal	*/ -10000.0f,
			/* maxVal	*/ 10000.0f,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Radius offset for rtx primary lights");

		dvars::rtx_primarylight_softness = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_softness",
			/* default	*/ 0.2f,
			/* minVal	*/ 0.0f,
			/* maxVal	*/ M_PI,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Softness for rtx primary lights");

		dvars::rtx_primarylight_exp = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_primarylight_exp",
			/* default	*/ 0.0f,
			/* minVal	*/ 0.0f,
			/* maxVal	*/ 10.0f,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Expo for rtx primary lights");

		dvars::rtx_fxlight_hash_dist = game::Dvar_RegisterFloat(
			/* name		*/ "rtx_fxlight_hash_dist",
			/* default	*/ 30.0f,
			/* minVal	*/ 0.0f,
			/* maxVal	*/ 2000.0f,
			/* flags	*/ game::dvar_flags::none,
			/* desc		*/ "Distance between new and old pos of fx lights to try and match remix api lights with the previous frame");
	}
}
