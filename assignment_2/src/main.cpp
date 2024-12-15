#include "Game.h"
#include "Vec2.h"
#include <string>
#include <SFML/Graphics.hpp>


int main() {
	Game g("config.txt");
	g.run();
	return 0;
}