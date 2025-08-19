#include "Rock.h"

Rock::Rock(int lane) {
    shape.setSize(sf::Vector2f(60.f, 60.f));
    shape.setFillColor(sf::Color::White);
    // Place it at the top of the given lane
    shape.setPosition(220.f + (lane * 150.f), -100.f);
    speed = 4.0f;
}

void Rock::update() {
    shape.move(0, speed); // Move downwards
}

void Rock::draw(sf::RenderWindow& window) {
    window.draw(shape);
}