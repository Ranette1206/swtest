#pragma once
#include "Core/Unit.h"

namespace sw::io
{
	struct SpawnHunter;
}

namespace sw::units
{
	class Hunter : public sw::core::Unit
	{
	public:
		static std::shared_ptr<Hunter> create(const sw::io::SpawnHunter& command);

	private:
		Hunter(uint32_t id);
	};
}
