#pragma once
#include "Obstacle.h"

class Rock : public Obstacle { // Inherits from Obstacle
public:
    Rock(int lane); // Constructor to place it in a lane
    void update() override; // 'override' is good practice
    void draw(sf::RenderWindow& window) override;
    void setSpeed(float s) override { speed = s; }
};
