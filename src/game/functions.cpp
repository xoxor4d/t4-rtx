#include "std_include.hpp"

namespace game
{
	std::vector<std::string> loaded_modules;

	game::DxGlobals* dx = reinterpret_cast<game::DxGlobals*>(0x1087DD04);

	game::GfxCmdBufSourceState* get_cmdbufsourcestate()
	{
		const auto out = reinterpret_cast<game::GfxCmdBufSourceState*>(*game::gfxCmdBufSourceState_ptr);
		return out;
	}

	game::GfxBackEndData* get_backenddata()
	{
		const auto out = reinterpret_cast<game::GfxBackEndData*>(*game::backEndDataOut_ptr);
		return out;
	}
}
