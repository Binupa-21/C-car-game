#include "Player.h"
#include "AssetManager.h"


Player::Player() {
    // Get the texture from the manager
    m_sprite.setTexture(AssetManager::GetTexture("C:/Users/User/OneDrive/Desktop/uni/OOP/ConsoleApplication1/x64/Release/car.png"));

    // Scale the sprite to fit the game window (e.g., 60x60 pixels)
    float targetWidth = 90.f;
    float targetHeight = 160.f;
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setScale(targetWidth / bounds.width, targetHeight / bounds.height);

    // Set the origin to the center of the sprite for easier positioning
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Set initial position: middle lane, near bottom of window
    m_sprite.setPosition(375.f, 800.f); // Adjust Y as needed for your window size
}

// This move logic is simple: it just jumps between lanes.
void Player::move(int direction) {
    lane += direction;
    // Constrain the lane index between 0 and 2
    if (lane < 0) lane = 0;
    if (lane > 2) lane = 2;

    // X-positions for lanes: Left=225, Middle=375, Right=525
    float newX = 225.f + (lane * 150.f);
    m_sprite.setPosition(newX, m_sprite.getPosition().y);
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

sf::FloatRect Player::getBounds() const {
    return m_sprite.getGlobalBounds();
}