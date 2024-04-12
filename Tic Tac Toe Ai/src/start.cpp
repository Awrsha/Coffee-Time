#include "include/Main.h"

int main() {
    Base *game = new Base; // Create a new instance of the Base class

    game->start_game(); // Start the game

    delete game; // Free the memory allocated for the game instance

    return 0;
}
