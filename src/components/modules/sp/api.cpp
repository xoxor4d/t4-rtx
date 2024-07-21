#include "std_include.hpp"

using namespace game::sp;

namespace components::sp
{
	BRIDGEAPI_ErrorCode api::init()
	{
		const auto status = bridgeapi_initialize(&bridge);
		game::sp::Com_PrintMessage(0, utils::va("[BridgeApi] bridgeapi_initialize() : %s \n", !status ? "success" : utils::va("error : %d", status)), 0);

		if (status == BRIDGEAPI_ERROR_CODE_SUCCESS && bridge.initialized)
		{
			bridge.RegisterDevice();
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
	}
}
