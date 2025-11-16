#pragma once
#include "Core/Unit.h"

namespace sw::io
{
	struct SpawnSwordsman;
}

namespace sw::units
{
	class SwordsMan : public sw::core::Unit
	{
	public:
		static std::shared_ptr<SwordsMan> create(const sw::io::SpawnSwordsman & command);

	private:
		SwordsMan(uint32_t id);
	};
}
