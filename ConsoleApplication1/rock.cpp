#include "Rock.h"
#include "AssetManager.h"

Rock::Rock(int lane) {
    shape.setSize(sf::Vector2f(80.f, 80.f)); // Increased size
    shape.setFillColor(sf::Color::White);
    shape.setPosition(220.f + (lane * 150.f), -100.f);
    speed = 4.0f;
    // Set up sprite
    sprite.setTexture(AssetManager::GetTexture("C:\\Users\\User\\Desktop\\oop\\C-car-game-main\\Rock.png"));
    sprite.setPosition(shape.getPosition());
    sprite.setScale(120.f / sprite.getLocalBounds().width, 120.f / sprite.getLocalBounds().height); // Increased scale
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