#include "Game.h"

int main() {
    Game game = Game();
    Logger::SetOutputLevel(Logger::L_DEBUG);
    Logger::Info("Starting...");
    game.Start();
    return 0;
}