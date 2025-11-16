#include "Features/Units/SwordsMan.h"

#include "Features/Actions/MeleeAttackAction.h"
#include "Features/Actions/MoveAction.h"
#include "Features/Actions/RangedAttackAction.h"
#include "IO/Commands/SpawnSwordsman.hpp"

using namespace sw::units;
using namespace sw::core;
using namespace sw::properties;

std::shared_ptr<SwordsMan> SwordsMan::create(const sw::io::SpawnSwordsman & command)
{
	auto swordsman = std::shared_ptr<SwordsMan>(new SwordsMan(command.unitId));
	swordsman->setProperty(Property::create<PropertyType::Position>(sw::core::Point2DInt(command.x, command.y)));
	swordsman->setProperty(Property::create<PropertyType::Health>(static_cast<int32_t>(command.hp)));
	swordsman->setProperty(Property::create<PropertyType::Strength>(command.strength));

	swordsman->addAvailableAction(std::make_shared<actions::MeleeAttackAction>(swordsman));
	swordsman->addAvailableAction(std::make_shared<actions::MoveAction>(swordsman));

	return swordsman;
}

SwordsMan::SwordsMan(uint32_t id) :
		Unit(id, "Swordsman")
{}
