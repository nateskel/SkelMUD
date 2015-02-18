//#include "Network.h"
#include "Game.h"
#include <iostream>
#include <stdio.h>

int main()
{
	Game game = Game();
	//printf("RUNNING..." + endl);
	std::cout << "RUNNING..." << std::endl;
	game.Start();
	//std::cout << WSAGetLastError();
	//std::cin.get();
	return 0;
}
