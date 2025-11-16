#pragma once

#include "details/PrintFieldVisitor.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <sstream>

namespace sw
{
	class EventLog
	{
	public:
		template <class TEvent>
		void log(uint64_t tick, TEvent&& event)
		{
			std::cout << "[" << tick << "] " << TEvent::Name << " ";
			PrintFieldVisitor visitor(std::cout);
			event.visit(visitor);
			std::cout << std::endl;
		}

		template <class TEvent>
		void addEvent(TEvent&& event)
		{
			std::ostringstream oss;
			oss << TEvent::Name << " ";
			PrintFieldVisitor visitor(oss);
			event.visit(visitor);
			m_EventPool.emplace_back(oss.str());
		}

		void logAllEvents(const uint64_t& tick)
		{
			for (const auto& eventStr : m_EventPool)
			{
				std::cout << "[" << tick << "] " << eventStr << std::endl;
			}
			m_EventPool.clear();
		}

	private:
		std::vector<std::string> m_EventPool;
	};
}
