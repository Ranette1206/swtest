#pragma once

#include "Core/Point2D.h"
#include "Core/Action.h"

#include <memory>
#include <optional>

namespace sw::actions
{
	class MoveAction : public sw::core::Action
	{
	public:
		explicit MoveAction(const std::shared_ptr<core::Unit>& actor);

		void setTarget(const core::Point2DInt& target);

		bool tryExecute(const std::shared_ptr<sw::map::Map>& map) override;

	protected:
		std::optional<core::Point2DInt> m_Target;
	};
}
