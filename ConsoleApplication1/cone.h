#pragma once
#include "Obstacle.h"

class Cone : public Obstacle {
public:
    Cone(int lane);
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void setSpeed(float s) override { speed = s; }
};
