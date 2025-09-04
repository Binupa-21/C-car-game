#include "game.h"
#include "Rock.h"
#include "AssetManager.h"
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <iostream> // For std::cout
#include <memory>   // For std::unique_ptr
#include <vector>   // For std::vector

Game::Game() : window(sf::VideoMode(800, 900), "SFML Car Game") {
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(0)));
    // Load road textures with correct path and syntax
    m_roadSprite1.setTexture(AssetManager::GetTexture("C:\\Users\\User\\Desktop\\oop\\C-car-game-main\\Road.png"));
    m_roadSprite2.setTexture(AssetManager::GetTexture("C:\\Users\\User\\Desktop\\oop\\C-car-game-main\\Road.png"));
    m_roadSprite1.setPosition(0, 0);
    m_roadSprite2.setPosition(0, -900.f); // Position second sprite above the first

    if (!font.loadFromFile("arial.ttf")) {
        // Handle error 
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(250, 400);
}

void Game::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        render();
    }
}

void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        // Add keyboard input for the player
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                player.move(-1);
            }
            else if (event.key.code == sf::Keyboard::Right) {
                player.move(1);
            }
        }
    }
    // Always update player rotation based on key state
    player.handleInput();

    if (isGameOver && event.type == sf::Event::KeyPressed && event.key.code
        == sf::Keyboard::R) {
        reset();
    }
}

void Game::update() {
    if (isGameOver) return; // Don't update if game is over

    // --- Scroll the road ---
    float m_roadSpeed = 300.f; // Example speed, adjust as needed
    float deltaTime = 1.f / 60.f; // Fixed timestep for now
    m_roadSprite1.move(0, m_roadSpeed * deltaTime);
    m_roadSprite2.move(0, m_roadSpeed * deltaTime);

    if (m_roadSprite1.getPosition().y > 900.f) { // If it's off-screen
        m_roadSprite1.setPosition(0, -900.f);
    }
    if (m_roadSprite2.getPosition().y > 900.f) {
        m_roadSprite2.setPosition(0, -900.f);
    }

    spawnObstacle();

    player.update(); // Smoothly update car position

    // POLYMORPHISM: We call update() on each obstacle, regardless of its specific type (Rock, etc.)
    for (auto& obstacle : obstacles) {
        obstacle->update();
        // Shrink collision bounds for more forgiving collision
        sf::FloatRect playerBounds = player.getBounds();
        sf::FloatRect obstacleBounds = obstacle->getBounds();
        float margin = 20.f; // Reduce collision area by 20 pixels on each side
        playerBounds.left += margin;
        playerBounds.top += margin;
        playerBounds.width -= 2 * margin;
        playerBounds.height -= 2 * margin;
        obstacleBounds.left += margin;
        obstacleBounds.top += margin;
        obstacleBounds.width -= 2 * margin;
        obstacleBounds.height -= 2 * margin;
        // Check for collision
        if (playerBounds.intersects(obstacleBounds)) {
            isGameOver = true;
            std::cout << "GAME OVER!" << std::endl;
            // Start shake effect
            isShaking = true;
            shakeClock.restart();
        }
    }
    // Camera shake effect
    if (isShaking) {
        // Shake for 0.2 seconds
        if (shakeClock.getElapsedTime().asSeconds() < 0.2f) {
            sf::View view = window.getView();
            float offsetX = (rand() % 10) - 5;
            float offsetY = (rand() % 10) - 5;
            view.move(offsetX, offsetY);
            window.setView(view);
        } else {
            isShaking = false;
            window.setView(window.getDefaultView());
        }
    }

    score++;
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::render() {
    window.clear(sf::Color(50, 50, 50));
    window.draw(m_roadSprite1);
    window.draw(m_roadSprite2);
    // Draw all obstacles first
    for (auto& obstacle : obstacles) {
        obstacle->draw(window);
    }
    // Draw the player on top of the obstacles
    player.draw(window);
    window.draw(scoreText);
    if (isGameOver) {
        window.draw(gameOverText);
    }
    window.display();

}

void Game::spawnObstacle() {
    // Spawn a new rock every 1.5 seconds
    if (spawnClock.getElapsedTime().asSeconds() > 1.5f) {
        int randomLane = rand() % 3; // Random lane 0, 1, or 2
        obstacles.push_back(std::make_unique<Rock>(randomLane));
        spawnClock.restart();
    }
}

void Game::reset() {
    isGameOver = false;
    score = 0;
    obstacles.clear(); // Remove all old obstacles 
    player = Player(); // Reset player to original state 
}