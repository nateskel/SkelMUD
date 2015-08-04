#include "Game.h"

int main() {
    Logger::SetOutputLevel(Logger::L_DEBUG);
    Game game = Game();
    Logger::Info("Starting...");
    game.Start();
    return 0;
}