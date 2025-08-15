#include "Player.h"

Player::Player() {
    shape.setSize(sf::Vector2f(50.f, 80.f));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(375, 750); // Start in the middle lane
}

// This move logic is simple: it just jumps between lanes.
void Player::move(int direction) {
    lane += direction;
    // Constrain the lane index between 0 and 2
    if (lane < 0) lane = 0;
    if (lane > 2) lane = 2;

    // X-positions for lanes: Left=225, Middle=375, Right=525
    float newX = 225.f + (lane * 150.f);
    shape.setPosition(newX, shape.getPosition().y);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}