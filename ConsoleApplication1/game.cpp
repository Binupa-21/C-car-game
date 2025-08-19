#include "game.h"
#include "Rock.h"
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <iostream> // For std::cout
#include <memory>   // For std::unique_ptr
#include <vector>   // For std::vector

Game::Game() : window(sf::VideoMode(800, 900), "SFML Car Game") {
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(0)));
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
}

void Game::update() {
    if (isGameOver) return; // Don't update if game is over

    spawnObstacle();

    // POLYMORPHISM: We call update() on each obstacle, regardless of its specific type (Rock, etc.)
    for (auto& obstacle : obstacles) {
        obstacle->update();
        // Check for collision
        if (obstacle->getBounds().intersects(player.getBounds())) {
            isGameOver = true;
            std::cout << "GAME OVER!" << std::endl;
        }
    }
}

void Game::render() {
    window.clear(sf::Color(50, 50, 50));
    player.draw(window); // Draw the player
    // Draw all obstacles
    for (auto& obstacle : obstacles) {
        obstacle->draw(window);
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