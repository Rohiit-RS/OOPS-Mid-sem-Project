// main.cpp
// Compile:  g++ -std=c++11 -o uno main.cpp Card.cpp Player.cpp Deck.cpp Game.cpp
// Run:      ./uno
#include <ctime>
#include "Game.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    cout << "\n              U N O\n\nFirst player to empty their hand wins.\n\n";
    Game game;
    game.run();
    return 0;
}
