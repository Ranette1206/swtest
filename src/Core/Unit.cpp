#include "Core/Unit.h"

#include "Core/Action.h"
#include "Core/Property.h"

#include <stdexcept>

uint32_t sw::core::Unit::getId() const
{
	return m_Id;
}

sw::core::Unit::Unit(uint32_t id, const std::string& type) :
		m_Id(id),
		m_Type(type),
		m_PropertiesMask(0)
{}

bool sw::core::Unit::hasProperties(uint16_t type) const
{
	return (m_PropertiesMask & type) != 0;
}

const std::shared_ptr<sw::core::Property>& sw::core::Unit::getProperty(properties::PropertyType type) const
{
	auto it = m_Properties.find(type);

	if (it == m_Properties.end())
	{
		throw std::runtime_error("Property not found");
	}

	return it->second;
}

std::shared_ptr<sw::core::Property>& sw::core::Unit::getProperty(properties::PropertyType type)
{
	auto it = m_Properties.find(type);
	if (it == m_Properties.end())
	{
		throw std::runtime_error("Property not found");
	}

	return it->second;
}

void sw::core::Unit::setProperty(const std::shared_ptr<Property>& property)
{
	if (property == nullptr)
	{
		return;
	}

	properties::PropertyType type = property->getType();

	m_Properties[type] = property;
	m_PropertiesMask |= static_cast<uint16_t>(type);
}

void sw::core::Unit::addAvailableAction(const std::shared_ptr<Action>& action)
{
	if (action != nullptr)
	{
		m_AvailableActions.emplace_back(action);
	}
}

std::shared_ptr<sw::core::Action> sw::core::Unit::getActionByType(actions::ActionType actionType) const
{
	for (const auto& action : m_AvailableActions)
	{
		if (action->getType() == actionType)
		{
			return action;
		}
	}
	return nullptr;
}

bool sw::core::Unit::tryUpdateOnMap(std::shared_ptr<sw::map::Map>& map)
{
	for (auto& action : m_AvailableActions)
	{
		if (action->tryExecute(map))
		{
			return true;
		}
	}

	return false;
}

std::string_view sw::core::Unit::getTypeName() const
{
	return m_Type;
}
