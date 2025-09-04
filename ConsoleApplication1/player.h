#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite m_sprite;
    float speed = 5.0f;
    float targetX = 375.f; // New: target X position for smooth lane change

public:
    int lane = 1; // Make lane public for collision logic

    Player();
    void move(int direction); // -1 for left, 1 for right
    void update(); // New: update position smoothly
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    void handleInput();
};
