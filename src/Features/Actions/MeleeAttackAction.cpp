#include "Features/Actions/MeleeAttackAction.h"
#include "Core/Unit.h"
#include "Core/Property.h"
#include "Core/Random.h"
#include "Features/Game/Game.h"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include <memory>
#include <cmath>

using namespace sw::actions;
using namespace sw::properties;
using namespace sw::core;

MeleeAttackAction::MeleeAttackAction(const std::shared_ptr<sw::core::Unit>& actor)
	: sw::core::Action(actor, sw::actions::ActionType::MeleeAttack)
{
}

bool MeleeAttackAction::tryExecute(const std::shared_ptr<sw::map::Map>& map)
{
	auto actor = m_Actor.lock();
	if (actor == nullptr || map == nullptr)
		return false;

	// Find target every time
	auto target = findTarget(map);
	if (target == nullptr)
		return false; // No target found

	attackTarget(target);
	return true;
}

std::shared_ptr<sw::core::Unit> MeleeAttackAction::findTarget(const std::shared_ptr<sw::map::Map>& map)
{
	auto actor = m_Actor.lock();
	if (actor == nullptr || map == nullptr)
		return nullptr;

	const auto& actorPos = actor->getProperty<PropertyType::Position>();
	const auto actorRange =
		actor->hasProperties(static_cast<uint16_t>(PropertyType::Range)) ?
		actor->getProperty<PropertyType::Range>() :
		1;

	auto units = map->getUnitsInRange(actorPos, 0, actorRange, static_cast<uint16_t>(PropertyType::Health));
	units.erase(std::remove(units.begin(), units.end(), actor), units.end());

	return (units.empty() ? nullptr : Random::getRandomElement(units));
}

void MeleeAttackAction::attackTarget(const std::shared_ptr<sw::core::Unit>& target)
{
	auto actor = m_Actor.lock();
	if (actor == nullptr || target == nullptr)
		return;

	const auto actorDamage = actor->getProperty<PropertyType::Strength>();

	int32_t targetHp = target->getProperty<PropertyType::Health>() - actorDamage;

	// Update target's health (create new property and replace)
	target->setProperty<PropertyType::Health>(targetHp);

	// Log attack event
	auto& logger = sw::game::Game::getInstance().getEventLogger();
	logger.addEvent(
		io::UnitAttacked{actor->getId(), target->getId(), static_cast<uint32_t>(actorDamage), static_cast<uint32_t>(targetHp)}
	);

	// Check if target died
	if (targetHp <= 0)
		logger.addEvent(io::UnitDied{target->getId()});
}
