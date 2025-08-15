#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
class Game {
private:
    sf::RenderWindow window;
    void handleInput();
    void update();
    void render();
	Player player; // Player instance

public:
    Game();
    void run();
};
