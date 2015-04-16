#include "Game.h"
#include "Logger.h"

int main()
{
	Game game = Game();
	Logger Log = Logger(Logger::Log_Level::L_INFO);
	Log.Info("Starting...");
	game.Start();
	return 0;
}
