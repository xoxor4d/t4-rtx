#include "std_include.hpp"

namespace game
{
	bool is_sp = false;
	bool is_mp = false;

	std::vector<std::string> loaded_modules;

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace sp
	{
		DxGlobals* dx = reinterpret_cast<DxGlobals*>(0x3BF3B04);
		clipMap_t* cm = reinterpret_cast<clipMap_t*>(0x1F41A00);
	}

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	namespace mp
	{
		DxGlobals* dx = reinterpret_cast<DxGlobals*>(0x1087DD04);
		clipMap_t* cm = reinterpret_cast<clipMap_t*>(0x2223A80);
	}

	// ------------------------------------------------------------------------------------------------------------
	// ############################################################################################################
	// ------------------------------------------------------------------------------------------------------------

	GfxCmdBufSourceState* get_cmdbufsourcestate()
	{
		const auto out = reinterpret_cast<GfxCmdBufSourceState*>(game::is_mp ? *game::mp::gfxCmdBufSourceState_ptr : *game::sp::gfxCmdBufSourceState_ptr);
		return out;
	}

	GfxBackEndData* get_backenddata()
	{
		const auto out = reinterpret_cast<GfxBackEndData*>(game::is_mp ? *mp::backEndDataOut_ptr : *sp::backEndDataOut_ptr);
		return out;
	}
}
