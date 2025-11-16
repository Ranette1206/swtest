#include "Game.h"

using namespace sw::game;

Game& Game::getInstance()
{
	static Game instance;
	return instance;
}

bool Game::tryUpdate()
{
	bool mapUpdated = false;

	++m_Tick;

	for (auto& map : m_Maps)
	{
		if (map != nullptr && map->tryUpdate())
		{
			mapUpdated = true;
		}
	}

	m_EventLogger.logAllEvents(m_Tick);

	return mapUpdated;
}
