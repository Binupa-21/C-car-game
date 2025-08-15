#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::RectangleShape shape;
    float speed = 5.0f;
    int lane = 1; // 0=left, 1=middle, 2=right

public:
    Player();
    void move(int direction); // -1 for left, 1 for right
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
};
