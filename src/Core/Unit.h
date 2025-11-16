#pragma once

#include "Core/Action.h"
#include "Core/Property.h"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sw::map
{
	class Map;
}

namespace sw::core
{
	class Action;

	class Unit
	{
	public:
		virtual ~Unit() = default;
		uint32_t getId() const;

		virtual bool hasProperties(uint16_t type) const;

		virtual const std::shared_ptr<Property>& getProperty(properties::PropertyType type) const;
		virtual std::shared_ptr<Property>& getProperty(properties::PropertyType type);
		virtual void setProperty(const std::shared_ptr<Property>& property);

		template <properties::PropertyType Type>
		const typename properties::PropertyTypeTraits<Type>::ValueType& getProperty() const
		{
			return getProperty(Type)->getValue<Type>();
		}

		template <properties::PropertyType Type>
		typename properties::PropertyTypeTraits<Type>::ValueType& getProperty()
		{
			return getProperty(Type)->getValue<Type>();
		}

		template <properties::PropertyType Type, typename T>
		void setProperty(const T& value)
		{
			setProperty(Property::create<Type>(value));
		}

		virtual void addAvailableAction(const std::shared_ptr<Action>& action);
		virtual std::shared_ptr<Action> getActionByType(actions::ActionType actionType) const;

		virtual bool tryUpdateOnMap(std::shared_ptr<map::Map>& map);
		virtual std::string_view getTypeName() const;

	protected:
		explicit Unit(uint32_t id, const std::string& type);

		const uint32_t m_Id;
		const std::string m_Type;

		std::map<properties::PropertyType, std::shared_ptr<Property>> m_Properties;
		uint16_t m_PropertiesMask;

		std::vector<std::shared_ptr<Action>> m_AvailableActions;
	};
}
