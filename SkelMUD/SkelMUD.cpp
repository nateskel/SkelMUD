#include <stdlib.h>
#include "Game.h"

int main(int argc, char *argv[]) {
    int port = 0;
    for(int i = 0; i<argc; ++i) {
        // TODO: process each command and error check
        port = std::atoi(argv[i]);
    }
    Logger::SetOutputLevel(Logger::L_INFO);
    Logger::Info("Initializing...");
    Game game = Game();
    Logger::Info("Starting...");
    if(port != 0)
        game.Start(port);
    else
        game.Start();
    return 0;
}