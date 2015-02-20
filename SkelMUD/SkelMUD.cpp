//#include "Network.h"
#include "Game.h"
#include <iostream>
#include <stdio.h>

int main()
{
	Game game = Game();
	std::cout << "RUNNING..." << std::endl;
	game.Start();
	return 0;
}
