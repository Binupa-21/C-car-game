#include "cone.h"
#include "AssetManager.h"

Cone::Cone(int laneIndex) {
    lane = laneIndex;
    shape.setSize(sf::Vector2f(80.f, 80.f));
    shape.setFillColor(sf::Color(255, 140, 0)); // Orange for cone
    shape.setPosition(225.f + (lane * 150.f), -100.f);
    speed = 4.0f;
    sprite.setTexture(AssetManager::GetTexture("Assets/TrafficCone.png"));
    sprite.setPosition(shape.getPosition());
    sprite.setScale(120.f / sprite.getLocalBounds().width, 120.f / sprite.getLocalBounds().height);
}

void Cone::update() {
    shape.move(0, speed);
    sprite.move(0, speed);
}

void Cone::draw(sf::RenderWindow& window) {
    if (sprite.getTexture())
        window.draw(sprite);
    else
        window.draw(shape);
}