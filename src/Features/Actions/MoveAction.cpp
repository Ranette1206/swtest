#include "Features/Actions/MoveAction.h"

#include "Core/Random.h"
#include "Features/Game/Game.h"
#include "Core/Unit.h"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/System/EventLog.hpp"

#include <array>
#include <memory>

using namespace sw::actions;
using namespace sw::core;

static const std::array<sw::core::Direction2D, 8> Directions
	= {sw::core::Direction2D{0, 1},
	   sw::core::Direction2D{0, -1},
	   sw::core::Direction2D{1, 0},
	   sw::core::Direction2D{-1, 0},
	   sw::core::Direction2D{1, 1},
	   sw::core::Direction2D{1, -1},
	   sw::core::Direction2D{-1, 1},
	   sw::core::Direction2D{-1, -1}};

MoveAction::MoveAction(const std::shared_ptr<Unit>& actor) :
		Action(actor, ActionType::Move)
{}

bool MoveAction::tryExecute(const std::shared_ptr<sw::map::Map>& map)
{
	auto actor = m_Actor.lock();
	if (actor == nullptr)
	{
		return false;
	}

	auto& logger = sw::game::Game::getInstance().getEventLogger();

	Point2DInt currentPosition = actor->getProperty<sw::properties::PropertyType::Position>();

	if (m_Target.has_value())
	{
		const auto& target = m_Target.value();

		sw::core::Direction2D direction = (target - currentPosition);
		if (direction.x != 0)
		{
			direction.x /= std::abs(direction.x);
		}

		if (direction.y)
		{
			direction.y /= std::abs(direction.y);
		}

		auto newPosition = currentPosition + direction;
		if (!map->isPositionFree(newPosition))
		{
			return false;
		}

		currentPosition = newPosition;
		actor->setProperty<sw::properties::PropertyType::Position>(currentPosition);

		if (currentPosition == target)
		{
			m_Target.reset();
			logger.addEvent(sw::io::MarchEnded{actor->getId(), static_cast<uint32_t>(currentPosition.x), static_cast<uint32_t>(currentPosition.y)});
		}
		else
		{
			logger.addEvent(
				sw::io::UnitMoved{actor->getId(), static_cast<uint32_t>(currentPosition.x), static_cast<uint32_t>(currentPosition.y)}
			);
		}
		return true;
	}
	else
	{
		for (int attempt = 0; attempt < Directions.size(); ++attempt)
		{
			auto newPosition = currentPosition + Directions.at(sw::core::Random::getRandomIndex(8));
			if (map->isPositionFree(newPosition))
			{
				actor->setProperty<sw::properties::PropertyType::Position>(newPosition);
				logger.addEvent(
					sw::io::UnitMoved{actor->getId(), static_cast<uint32_t>(newPosition.x), static_cast<uint32_t>(newPosition.y)}
				);
				return true;
			}
		}
	}
	return false;
}

void MoveAction::setTarget(const Point2DInt& target)
{
	if (m_Actor.expired())
	{
		return;
	}

	m_Target = target;

	const auto& actor = m_Actor.lock();
	const auto& currentPosition = actor->getProperty<sw::properties::PropertyType::Position>();

	sw::game::Game::getInstance().getEventLogger().addEvent(
		sw::io::MarchStarted(m_Actor.lock()->getId(), currentPosition.x, currentPosition.y, m_Target->x, m_Target->y));
}
