//#include "Network.h"
#include "Game.h"
#include "Logger.h"
#include <iostream>
#include <stdio.h>

int main()
{
	Game game = Game();
	Logger Log = Logger(Logger::Log_Level::L_INFO);
	Log.Info("Running...");
	Log.Debug("TEST");
	Log.Error("This is an error");
	//std::cout << "RUNNING..." << std::endl;
	game.Start();
	return 0;
}
