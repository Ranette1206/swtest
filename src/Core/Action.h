#pragma once

#include <memory>
#include "Features/Actions/Actions.h"

namespace sw::map
{
	class Map;
}

namespace sw::core
{
	class Unit;
}

namespace sw::core
{
	class Action
	{
	public:
		virtual ~Action() = default;

		sw::actions::ActionType getType() const
		{
			return m_ActionType;
		}

		virtual bool tryExecute(const std::shared_ptr<sw::map::Map>& map) = 0;

	protected:
		explicit Action(const std::shared_ptr<Unit>& actor, sw::actions::ActionType type) :
				m_Actor(actor),
				m_ActionType(type)
		{}

		std::weak_ptr<Unit> m_Actor;
		sw::actions::ActionType m_ActionType;
	};
}
