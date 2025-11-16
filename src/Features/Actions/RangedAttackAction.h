#pragma once

#include "Core/Point2D.h"
#include "Core/Action.h"
#include "Core/Property.h"

#include <cstdint>
#include <memory>

namespace sw::actions
{
	class RangedAttackAction : public sw::core::Action
	{
	public:
		explicit RangedAttackAction(const std::shared_ptr<core::Unit>& actor);

		bool tryExecute(const std::shared_ptr<sw::map::Map>& map) override;

	protected:
		std::shared_ptr<core::Unit> findTarget(const std::shared_ptr<sw::map::Map>& map);
		void                        attackTarget(const std::shared_ptr<core::Unit>& target);

	};
}
