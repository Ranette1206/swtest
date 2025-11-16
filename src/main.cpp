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

	// Initial tick to log command events
	game.incrementTick();
	game.getEventLogger().logAllEvents(game.getTick());

	while (game.tryUpdate())
	{}

	std::cout << "/////////////////////////////////////////////////////////\n";
	std::cout << "Banchmark\n";
	std::cout << "/////////////////////////////////////////////////////////\n";
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
	eventLogger.addEvent(io::UnitMoved{1, 1, 0});
	eventLogger.addEvent(io::UnitMoved{2, 8, 0});
	eventLogger.addEvent(io::UnitMoved{3, 0, 8});
	eventLogger.logAllEvents(game.getTick());

	game.setTick(3);
	eventLogger.addEvent(io::UnitMoved{1, 2, 0});
	eventLogger.addEvent(io::UnitMoved{2, 7, 0});
	eventLogger.addEvent(io::UnitMoved{3, 0, 7});
	eventLogger.logAllEvents(game.getTick());

	game.setTick(4);
	eventLogger.addEvent(io::UnitMoved{1, 3, 0});
	eventLogger.addEvent(io::UnitAttacked{2, 1, 5, 0});
	eventLogger.addEvent(io::UnitDied{1});
	eventLogger.addEvent(io::UnitMoved{3, 0, 6});
	eventLogger.logAllEvents(game.getTick());

	game.setTick(5);
	eventLogger.addEvent(io::UnitMoved{2, 6, 0});
	eventLogger.addEvent(io::UnitMoved{3, 0, 5});
	eventLogger.logAllEvents(game.getTick());

	game.setTick(6);
	eventLogger.addEvent(io::UnitMoved{2, 5, 0});
	eventLogger.addEvent(io::UnitMoved{3, 0, 4});
	eventLogger.logAllEvents(game.getTick());

	game.setTick(7);
	eventLogger.addEvent(io::UnitAttacked{2, 3, 5, 5});
	eventLogger.addEvent(io::UnitMoved{3, 0, 3});
	eventLogger.logAllEvents(game.getTick());

	game.setTick(8);
	eventLogger.addEvent(io::UnitAttacked{2, 3, 5, 0});
	eventLogger.addEvent(io::UnitDied{3});
	eventLogger.logAllEvents(game.getTick());

	return 0;
}
