#pragma once
#include <SFML/Graphics.hpp>

class Game {
private:
    sf::RenderWindow window;
    void handleInput();
    void update();
    void render();

public:
    Game();
    void run();
};
