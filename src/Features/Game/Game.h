#pragma once

#include "Features/Map/Map.hpp"
#include "Features/Units/Hunter.h"
#include "Features/Units/SwordsMan.h"
#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Commands/SpawnSwordsman.hpp"
#include "IO/System/EventLog.hpp"
#include "IO/System/PrintDebug.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace sw::game
{
	class Game
	{
	public:
		static Game& getInstance();

		// Public interface
		uint64_t getTick() const
		{
			return m_Tick;
		}

		void setTick(uint64_t tick)
		{
			m_Tick = tick;
		}

		void incrementTick()
		{
			++m_Tick;
		}

		const std::vector<std::shared_ptr<map::Map>>& getMaps() const
		{
			return m_Maps;
		}

		std::vector<std::shared_ptr<map::Map>>& getMaps()
		{
			return m_Maps;
		}

		EventLog& getEventLogger()
		{
			return m_EventLogger;
		}

		const EventLog& getEventLogger() const
		{
			return m_EventLogger;
		}

		bool tryUpdate();

		template <typename Command>
		void execute(const Command& command);

		template <>
		void execute<sw::io::CreateMap>(const sw::io::CreateMap& command)
		{
			m_Maps.emplace_back(map::Map::create(command));
		}

		template <>
		void execute<sw::io::SpawnHunter>(const sw::io::SpawnHunter& command)
		{
			m_Maps.back()->execute(command);
		}

		template <>
		void execute<sw::io::SpawnSwordsman>(const sw::io::SpawnSwordsman& command)
		{
			m_Maps.back()->execute(command);
		}

		template <>
		void execute<sw::io::March>(const sw::io::March& command)
		{
			m_Maps.back()->execute(command);
		}

	private:
		Game() = default;
		~Game() = default;

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;

		uint64_t m_Tick = 0;
		std::vector<std::shared_ptr<map::Map>> m_Maps;
		EventLog m_EventLogger;
	};
}
