#include "Features/Game/Game.h"
#include "IO/System/TypeRegistry.hpp"

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	auto& game = game::Game::getInstance();

	// Code for example...

	std::cout << "Commands:\n";
	io::CommandParser parser;
	parser
		.add<io::CreateMap>(
			[&game](auto command)
			{
				printDebug(std::cout, command);
				game.execute(command);
			})
		.add<io::SpawnSwordsman>(
			[&game](auto command)
			{
				printDebug(std::cout, command);
				game.execute(command);
			})
		.add<io::SpawnHunter>(
			[&game](auto command)
			{
				printDebug(std::cout, command);
				game.execute(command);
			})
		.add<io::March>(
			[&game](auto command)
			{
				printDebug(std::cout, command);
				game.execute(command);
			});

	parser.parse(file);

	std::cout << "\n\nEvents:\n";

	while (game.tryUpdate())
	{}
	/*
	auto& eventLogger = game.getEventLogger();

	game.setTick(1);
	eventLogger.addEvent(io::MapCreated{10, 10});
	eventLogger.addEvent(io::UnitSpawned{1, "Swordsman", 0, 0});
	eventLogger.addEvent(io::UnitSpawned{2, "Hunter", 9, 0});
	eventLogger.addEvent(io::MarchStarted{1, 0, 0, 9, 0});
	eventLogger.addEvent(io::MarchStarted{2, 9, 0, 0, 0});
	eventLogger.addEvent(io::UnitSpawned{3, "Swordsman", 0, 9});
	eventLogger.addEvent(io::MarchStarted{3, 0, 9, 0, 0});
	eventLogger.logAllEvents(game.getTick());

	game.setTick(2);
	game.getEventLogger().log(2, io::UnitMoved{1, 1, 0});
	game.getEventLogger().log(2, io::UnitMoved{2, 8, 0});
	game.getEventLogger().log(2, io::UnitMoved{3, 0, 8});

	game.setTick(3);
	game.getEventLogger().log(3, io::UnitMoved{1, 2, 0});
	game.getEventLogger().log(3, io::UnitMoved{2, 7, 0});
	game.getEventLogger().log(3, io::UnitMoved{3, 0, 7});

	game.setTick(4);
	game.getEventLogger().log(4, io::UnitMoved{1, 3, 0});
	game.getEventLogger().log(4, io::UnitAttacked{2, 1, 5, 0});
	game.getEventLogger().log(4, io::UnitDied{1});
	game.getEventLogger().log(4, io::UnitMoved{3, 0, 6});

	game.setTick(5);
	game.getEventLogger().log(5, io::UnitMoved{2, 6, 0});
	game.getEventLogger().log(5, io::UnitMoved{3, 0, 5});

	game.setTick(6);
	game.getEventLogger().log(6, io::UnitMoved{2, 5, 0});
	game.getEventLogger().log(6, io::UnitMoved{3, 0, 4});

	game.setTick(7);
	game.getEventLogger().log(7, io::UnitAttacked{2, 3, 5, 5});
	game.getEventLogger().log(7, io::UnitMoved{3, 0, 3});

	game.setTick(8);
	game.getEventLogger().log(8, io::UnitAttacked{2, 3, 5, 0});
	game.getEventLogger().log(8, io::UnitDied{3});
*/

	return 0;
}
