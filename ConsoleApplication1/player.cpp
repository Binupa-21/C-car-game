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

    targetX = m_sprite.getPosition().x; // Initialize targetX
}

// This move logic is simple: it just jumps between lanes.
void Player::move(int direction) {
    lane += direction;
    // Constrain the lane index between 0 and 2
    if (lane < 0) lane = 0;
    if (lane > 2) lane = 2;

    // Set targetX for smooth movement
    targetX = 225.f + (lane * 150.f);
}

void Player::update() {
    // Smoothly move towards targetX
    float currentX = m_sprite.getPosition().x;
    float speed = 12.0f; // Lower value = slower lane change
    if (std::abs(currentX - targetX) > 1.0f) {
        float direction = (targetX > currentX) ? 1.f : -1.f;
        float newX = currentX + direction * speed;
        // Clamp if overshooting
        if ((direction > 0 && newX > targetX) || (direction < 0 && newX < targetX)) {
            newX = targetX;
        }
        m_sprite.setPosition(newX, m_sprite.getPosition().y);
    } else {
        m_sprite.setPosition(targetX, m_sprite.getPosition().y);
    }
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

sf::FloatRect Player::getBounds() const {
    return m_sprite.getGlobalBounds();
}

void Player::handleInput() {
    // Check left/right keys and set rotation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_sprite.setRotation(-10.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_sprite.setRotation(10.f);
    } else {
        m_sprite.setRotation(0.f);
    }
}                   