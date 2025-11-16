#include "Features/Units/Hunter.h"
#include "Core/Property.h"
#include "Features/Actions/RangedAttackAction.h"
#include "Features/Actions/MoveAction.h"
#include "Features/Actions/MeleeAttackAction.h"
#include "Features/Game/Game.h"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Events/UnitSpawned.hpp"

using namespace sw::core;
using namespace sw::properties;
using namespace sw::units;

std::shared_ptr<Hunter> Hunter::create(const sw::io::SpawnHunter & command)
{
	std::shared_ptr<Hunter> hunter(new Hunter(command.unitId));
	hunter->setProperty(Property::create<PropertyType::Position>(sw::core::Point2DInt(command.x, command.y)));
	hunter->setProperty(Property::create<PropertyType::Health>(static_cast<int32_t>(command.hp)));
	hunter->setProperty(Property::create<PropertyType::Agility>(command.agility));
	hunter->setProperty(Property::create<PropertyType::Strength>(command.strength));
	hunter->setProperty(Property::create<PropertyType::Range>(command.range));

	hunter->addAvailableAction(std::make_shared<actions::RangedAttackAction>(hunter));
	hunter->addAvailableAction(std::make_shared<actions::MeleeAttackAction>(hunter));
	hunter->addAvailableAction(std::make_shared<actions::MoveAction>(hunter));

	return hunter;
}

Hunter::Hunter(uint32_t id)
	: Unit(id, "Hunter")
{
}
