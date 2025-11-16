#include "Features/Map/Map.hpp"

#include "Features/Actions/MoveAction.h"
#include "Features/Game/Game.h"
#include "IO/Events/UnitSpawned.hpp"

#include <Core/Action.h>
#include <Core/Point2D.h>
#include <Core/Property.h>
#include <Core/Unit.h>
#include <IO/Events/UnitDied.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <vector>

using namespace sw::map;
using namespace sw::properties;
using namespace sw::core;

Map::Map(uint32_t width, uint32_t height) :
		m_Width(width),
		m_Height(height)
{}

std::shared_ptr<Map> Map::create(const sw::io::CreateMap& command)
{
	const auto map = std::shared_ptr<Map>(new Map(command.width, command.height));
	if (map != nullptr)
	{
		sw::game::Game::getInstance().getEventLogger().addEvent(io::CreateMap{command.width, command.height});
	}
	return map;
}

void Map::addUnit(const std::shared_ptr<Unit>& unit)
{
	if (unit == nullptr || std::find(m_Units.begin(), m_Units.end(), unit) != m_Units.end())
	{
		return;
	}

	m_Units.emplace_back(unit);

	const auto & position = unit->getProperty<PropertyType::Position>();

	game::Game::getInstance().getEventLogger().addEvent(sw::io::UnitSpawned{
		unit->getId(),
		unit->getTypeName().data(),
		static_cast<uint32_t>(position.x),
		static_cast<uint32_t>(position.y)}
	);
}

void Map::moveToTarget(uint32_t Id, uint32_t targetX, uint32_t targetY)
{
	auto unit = getUnit(Id);
	if (unit == nullptr)
	{
		return;
	}

	auto moveAction = std::dynamic_pointer_cast<actions::MoveAction>(unit->getActionByType(actions::ActionType::Move));
	if (moveAction == nullptr)
	{
		return;
	}

	moveAction->setTarget(Point2DInt{static_cast<int32_t>(targetX), static_cast<int32_t>(targetY)});
}

std::shared_ptr<Unit> Map::getUnit(const uint32_t Id)
{
	auto it = std::find_if(
		m_Units.begin(), m_Units.end(), [&Id](const std::shared_ptr<Unit>& unit) { return unit->getId() == Id; });
	return (it == m_Units.end() ? nullptr : *it);
}

bool Map::tryUpdate()
{
	std::shared_ptr<Map> thisPtr = shared_from_this();
	bool unitMadeAction = false;
	for (auto& unit : m_Units)
	{
		if (unit != nullptr && unit->tryUpdateOnMap(thisPtr))
		{
			unitMadeAction = true;
		}
	}

	auto& Logger = sw::game::Game::getInstance().getEventLogger();

	m_Units.erase(
		std::remove_if(
			m_Units.begin(),
			m_Units.end(),
			[&Logger](const std::shared_ptr<Unit>& unit)
			{
				if (unit == nullptr)
				{
					return true;
				}

				if (!unit->hasProperties(static_cast<uint16_t>(PropertyType::Health)))
				{
					return false;
				}

				const auto& health = unit->getProperty<PropertyType::Health>();
				return (health <= 0);
			}),
		m_Units.end());

	return unitMadeAction && m_Units.size() > 1;
}

bool Map::isPositionFree(const core::Point2DInt& position)
{
	if (position.x < 0 || position.x >= m_Width || position.y < 0 || position.y >= m_Height)
	{
		return false;
	}

	const auto obstacles = getUnitsWithProperties(
		static_cast<uint16_t>(properties::PropertyType::Position)
		| static_cast<uint16_t>(properties::PropertyType::CanBlockCell));

	for (const auto& obstacle : obstacles)
	{
		if (obstacle->getProperty<PropertyType::Position>() == position)
		{
			return false;
		}
	}

	return true;
}

std::vector<std::shared_ptr<Unit>> Map::getUnitsWithProperties(uint16_t propertiesMask) const
{
	std::vector<std::shared_ptr<Unit>> result;

	for (const auto& unit : m_Units)
	{
		if (unit == nullptr)
		{
			continue;
		}

		if (unit->hasProperties(propertiesMask))
		{
			result.emplace_back(unit);
		}
	}

	return result;
}

std::vector<std::shared_ptr<Unit>> Map::getUnitsInRange(
	const sw::core::Point2DInt& position,
	const int32_t& rangeMin,
	const int32_t& rangeMax,
	uint16_t propertiesMask) const
{
	std::vector<std::shared_ptr<Unit>> result;

	propertiesMask |= static_cast<uint16_t>(PropertyType::Position);

	for (const auto& unit : m_Units)
	{
		if (unit == nullptr)
		{
			continue;
		}

		if (!unit->hasProperties(propertiesMask))
		{
			continue;
		}

		const auto& unitPos = unit->getProperty<PropertyType::Position>();

		if (position.isInRange(unitPos, rangeMax))
		{
			if (rangeMin == 0 || !position.isInRange(unitPos, rangeMin))
			{
				result.emplace_back(unit);
			}
		}
	}

	return result;
}
