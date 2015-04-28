#include "Game.h"

int main() {
    Game game = Game();
    Logger Log = Logger(Logger::Log_Level::L_DEBUG);
    Log.Info("Starting...");
    game.Start();
    return 0;
}