#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "player.h"
#include "obstacle.h"

class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    bool isGameOver = false;
    sf::Clock spawnClock;
    void handleInput();
    void update();
    void render();
    void spawnObstacle();
public:
    Game();
    void run();
};
