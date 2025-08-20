#pragma once
#include <SFML/Graphics.hpp>

// Abstract Base Class
class Obstacle {
protected:
    sf::RectangleShape shape;
    sf::Sprite sprite; // Add sprite for obstacle image
    float speed = 3.0f;

public:
    // Virtual destructor is important for base classes!
    virtual ~Obstacle() = default;

    // Pure virtual functions make this class abstract
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    sf::FloatRect getBounds() const {
        // Use sprite bounds for collision if texture is set, else fallback to shape
        if (sprite.getTexture())
            return sprite.getGlobalBounds();
        else
            return shape.getGlobalBounds();
    }
};
