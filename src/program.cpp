#include "core/sfmlgameclass.hpp"
#include <SFML/Config.hpp>

using namespace core;

auto main() -> int
{
	// const int fps = 60;
	// const int width = 800;
	// const int height = 600;
	// const sf::Uint8 UnitDivisor = 50;
	// const sf::Uint8 BoardWidth = 8;
	// const float friction = 0.3;
	// const float ball_s = 2.0;
	// const float board_s = 6.0;
	// const float time_div = 10000;
	const sf::Color clear = {35, 5, 15, 255};
	SfmlGameClass game(SfmlGameSettings{
		120,
		false,
		800,
		600,
		"My SFML Game",
		clear,
		sf::Style::Titlebar,
		sf::ContextSettings{},
		50,
		4000000,
		13.5,
		150,
		350,
	});
	return SfmlGameClass::RunBareGame(game);
}
