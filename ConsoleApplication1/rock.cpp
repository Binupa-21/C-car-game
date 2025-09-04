#include "Rock.h"
#include "AssetManager.h"

Rock::Rock(int laneIndex) {
    lane = laneIndex; // Store lane index in base class
    shape.setSize(sf::Vector2f(80.f, 80.f)); 
    shape.setFillColor(sf::Color::White);
    shape.setPosition(225.f + (lane * 150.f), -100.f); // Restore original lane positions
    speed = 4.0f;
    // Set up sprite
    sprite.setTexture(AssetManager::GetTexture("C:/Users/User/OneDrive/Desktop/uni/OOP/ConsoleApplication1/rock.png"));
    sprite.setPosition(shape.getPosition());
    sprite.setScale(120.f / sprite.getLocalBounds().width, 120.f / sprite.getLocalBounds().height);
}

void Rock::update() {
    shape.move(0, speed); // Move downwards
    sprite.move(0, speed); // Move sprite downwards
}

void Rock::draw(sf::RenderWindow& window) {
    if (sprite.getTexture())
        window.draw(sprite);
    else
        window.draw(shape);
}