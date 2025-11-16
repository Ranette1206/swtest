#pragma once

#include "Core/Point2D.h"

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <variant>

namespace sw::properties
{

enum class PropertyType : uint16_t
{
	Health       = 1,
	Agility      = 1 << 1,
	Strength     = 1 << 2,
	Range        = 1 << 3,
	Position     = 1 << 4,
	CanBlockCell = 1 << 5,
};

using PropertyValue = std::variant<int32_t, uint32_t, bool, sw::core::Point2DInt>;

template <PropertyType Type>
struct PropertyTypeTraits;

#define PROPERTY_TYPE_DECLARE(TYPE, VALUE_TYPE)                                                                        \
template <>                                                                                                        \
struct PropertyTypeTraits<TYPE>                                                                                    \
{                                                                                                                  \
	using ValueType = VALUE_TYPE;                                                                                  \
};

PROPERTY_TYPE_DECLARE(PropertyType::Health,   int32_t)
PROPERTY_TYPE_DECLARE(PropertyType::Agility,  uint32_t)
PROPERTY_TYPE_DECLARE(PropertyType::Strength, uint32_t)
PROPERTY_TYPE_DECLARE(PropertyType::Range,    uint32_t)
PROPERTY_TYPE_DECLARE(PropertyType::Position, core::Point2DInt)
PROPERTY_TYPE_DECLARE(PropertyType::CanBlockCell, bool)
} // namespace sw::properties
