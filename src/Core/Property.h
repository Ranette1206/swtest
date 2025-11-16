#pragma once

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include "Features/Properties/Properties.h"

namespace sw::core
{

class Property
{
	explicit Property(sw::properties::PropertyType type, sw::properties::PropertyValue value) :
			m_Type(type), m_Value(value)
	{}

public:

	static inline const std::shared_ptr<Property> Invalid = nullptr;

	virtual ~Property() = default;

	// Factory method
	template <sw::properties::PropertyType Type>
	static std::shared_ptr<Property> create(sw::properties::PropertyValue value)
	{
		using ValueType = typename sw::properties::PropertyTypeTraits<Type>::ValueType;
		if (!std::holds_alternative<ValueType>(value))
			throw std::runtime_error("Type mismatch");

		return std::shared_ptr<Property>(new Property(Type, value));
	}

	template <typename T>
	const T& getValue() const
	{
		return std::get<T>(m_Value);
	}

	template <typename T>
	T& getValue()
	{
		return std::get<T>(m_Value);
	}

	template <sw::properties::PropertyType Type>
	const typename sw::properties::PropertyTypeTraits<Type>::ValueType& getValue() const
	{
		return std::get<typename sw::properties::PropertyTypeTraits<Type>::ValueType>(m_Value);
	}

	template <sw::properties::PropertyType Type>
	typename sw::properties::PropertyTypeTraits<Type>::ValueType& getValue()
	{
		return std::get<typename sw::properties::PropertyTypeTraits<Type>::ValueType>(m_Value);
	}

	sw::properties::PropertyType getType() const { return m_Type; }

protected:

	Property() = delete;

	sw::properties::PropertyType m_Type;
	sw::properties::PropertyValue m_Value;
};

} // namespace sw::core
