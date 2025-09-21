#pragma once
#include <SFML/Graphics.hpp>

// Abstract Base Class
class Obstacle {
protected:
    sf::RectangleShape shape;
    sf::Sprite sprite; // Add sprite for obstacle image
    float speed = 3.0f;
    int lane = 1; // Lane index for obstacle

public:
    virtual ~Obstacle() = default;

    // Pure virtual functions make this class abstract
    virtual void update() = 0; // Only no-arg update
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setSpeed(float s) { speed = s; } // Allow dynamic speed

    sf::FloatRect getBounds() const {
        // Use sprite bounds for collision if texture is set, else fallback to shape
        if (sprite.getTexture())
            return sprite.getGlobalBounds();
        else
            return shape.getGlobalBounds();
    }
    int getLane() const { return lane; }
    float getTop() const { return sprite.getPosition().y; }
};
