#pragma once

#include "Features/Units/Hunter.h"
#include "Features/Units/SwordsMan.h"
#include "IO/Commands/March.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace sw::io
{
	struct CreateMap;
}

namespace sw::core
{
	template <typename T>
	class Point2D;
	using Point2DInt = Point2D<int32_t>;
}

namespace sw::core
{
	class Unit;
}

namespace sw::map
{
	class Map : public std::enable_shared_from_this<Map>
	{
	public:
		static std::shared_ptr<Map> create(const sw::io ::CreateMap& command);
		virtual ~Map() = default;

		std::shared_ptr<core::Unit> getUnit(const uint32_t Id);

		const std::vector<std::shared_ptr<core::Unit>>& getUnits() const
		{
			return m_Units;
		}

		std::vector<std::shared_ptr<core::Unit>> getUnitsWithProperties(uint16_t propertiesMask) const;
		std::vector<std::shared_ptr<core::Unit>> getUnitsInRange(
			const core::Point2DInt& position,
			const int32_t& rangeMin,
			const int32_t& rangeMax,
			uint16_t propertiesMask) const;

		bool tryUpdate();

		bool isPositionFree(const core::Point2DInt& position);

		template <typename Command>
		void execute(const Command& command);

		template <>
		void execute<sw::io::SpawnHunter>(const sw::io::SpawnHunter& command)
		{
			addUnit(sw::units::Hunter::create(command));
		}

		template <>
		void execute<sw::io::SpawnSwordsman>(const sw::io::SpawnSwordsman& command)
		{
			addUnit(sw::units::SwordsMan::create(command));
		}

		template <>
		void execute<sw::io::March>(const sw::io::March& command)
		{
			moveToTarget(command.unitId, command.targetX, command.targetY);
		}

	protected:
		explicit Map(uint32_t width, uint32_t height);

		void addUnit(const std::shared_ptr<core::Unit>& unit);

		void moveToTarget(uint32_t Id, uint32_t targetX, uint32_t targetY);

		const uint32_t m_Width;
		const uint32_t m_Height;

		std::vector<std::shared_ptr<core::Unit>> m_Units;
	};
}
