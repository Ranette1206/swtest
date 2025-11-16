#pragma once

#include "Core/Point2D.h"
#include "Core/Action.h"
#include <cstdint>
#include <memory>

namespace sw::core
{
	class Unit;
}

namespace sw::actions
{
  class MeleeAttackAction : public sw::core::Action
  {
  public:
    explicit MeleeAttackAction(const std::shared_ptr<sw::core::Unit>& actor);

	bool tryExecute(const std::shared_ptr<sw::map::Map>& map) override;

  protected:
    std::shared_ptr<sw::core::Unit> findTarget(const std::shared_ptr<sw::map::Map>& map);
    void attackTarget(const std::shared_ptr<sw::core::Unit>& target);
  };
}
