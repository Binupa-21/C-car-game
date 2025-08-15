#include "game.h"


Game::Game() : window(sf::VideoMode(800, 900), "SFML Car Game") {
    window.setFramerateLimit(60);
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
    // Game logic will go here
}

void Game::render() {
    window.clear(sf::Color(50, 50, 50));
    player.draw(window); // Draw the player
    window.display();
}