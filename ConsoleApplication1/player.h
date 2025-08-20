#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite m_sprite;
    float speed = 5.0f;
    int lane = 1; // 0=left, 1=middle, 2=right
    float targetX = 375.f; // New: target X position for smooth lane change

public:
    Player();
    void move(int direction); // -1 for left, 1 for right
    void update(); // New: update position smoothly
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    void handleInput();
    // Optionally, add a reset method if you want to use it
    // void reset();
};
