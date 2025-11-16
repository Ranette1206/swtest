#include "Features/Actions/RangedAttackAction.h"

#include "Core/Random.h"
#include "Core/Unit.h"
#include "Core/Property.h"
#include "Features/Game/Game.h"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include <memory>
#include <cmath>

using namespace sw::actions;
using namespace sw::properties;
using namespace sw::core;

RangedAttackAction::RangedAttackAction(const std::shared_ptr<Unit>& actor):
	sw::core::Action(actor, sw::actions::ActionType::RangedAttack)
{
}

bool RangedAttackAction::tryExecute(const std::shared_ptr<sw::map::Map>& map)
{
	auto actor = m_Actor.lock();
	if (actor == nullptr || map == nullptr)
		return false;

	auto target = findTarget(map);
	if (target == nullptr)
		return false;

	attackTarget(target);
	return true;
}

std::shared_ptr<Unit> RangedAttackAction::findTarget(const std::shared_ptr<sw::map::Map>& map)
{
	auto actor = m_Actor.lock();
	if (actor == nullptr || map == nullptr)
		return nullptr;

	const auto& actorPos = actor->getProperty<PropertyType::Position>();
	const auto actorRange = actor->getProperty<PropertyType::Range>();

	auto units = map->getUnitsInRange(actorPos, 1, actorRange, static_cast<uint16_t>(PropertyType::Health));
	units.erase(
		std::remove_if(
			units.begin(),
			units.end(),
			[](const std::shared_ptr<Unit>& unit)
			{ return unit->getProperty<PropertyType::Health>() <= 0; }),
		units.end()
	);
	if (units.empty())
		return nullptr;

	return Random::getRandomElement(units);
}

void RangedAttackAction::attackTarget(const std::shared_ptr<Unit>& target)
{
	auto actor = m_Actor.lock();
	if (actor == nullptr || target == nullptr)
		return;

	const auto damage = actor->getProperty<PropertyType::Agility>();

	int32_t targetFinalHP = target->getProperty<PropertyType::Health>() - damage;

	target->setProperty<PropertyType::Health>(targetFinalHP);

	// Log attack event
	auto& logger = sw::game::Game::getInstance().getEventLogger();
	logger.addEvent(io::UnitAttacked{actor->getId(), target->getId(), static_cast<uint32_t>(damage), static_cast<uint32_t>(targetFinalHP)});

	// Check if target died
	if (targetFinalHP <= 0)
	{
		logger.addEvent(io::UnitDied{target->getId()});
	}
}

