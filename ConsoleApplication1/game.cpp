#include "game.h"
#include "Rock.h"
#include "AssetManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

Game::Game() : window(sf::VideoMode(800, 900), "SFML Car Game")
{
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(0)));
    m_roadSprite1.setTexture(AssetManager::GetTexture("C:/Users/User/OneDrive/Desktop/uni/OOP/ConsoleApplication1/Road.png"));
    m_roadSprite2.setTexture(AssetManager::GetTexture("C:/Users/User/OneDrive/Desktop/uni/OOP/ConsoleApplication1/Road.png"));
    m_roadSprite1.setPosition(0, 0);
    m_roadSprite2.setPosition(0, -900.f);
    m_crashSound.setBuffer(AssetManager::GetSoundBuffer("C:/Users/User/OneDrive/Desktop/uni/OOP/ConsoleApplication1/Audio/crash.wav"));
    if (m_backgroundMusic.openFromFile("C:/Users/User/OneDrive/Desktop/uni/OOP/ConsoleApplication1/Audio/bgmusic.ogg")) {
        m_backgroundMusic.setVolume(40.f);
        m_backgroundMusic.setLoop(true);
        m_backgroundMusic.play();
    }
    // Font and text initialization
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load arial.ttf" << std::endl;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(70);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(200, 400);
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
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Left) {
				player.move(-1);
			}
			else if (event.key.code == sf::Keyboard::Right) {
				player.move(1);
			}
		}
	}
	player.handleInput();

	if (isGameOver && event.type == sf::Event::KeyPressed && event.key.code
		== sf::Keyboard::R) {
		reset();
	}
}

void Game::update() {
	if (isGameOver) return;
	float m_roadSpeed = 250.f;
	float deltaTime = 1.f / 60.f;
	m_roadSprite1.move(0, m_roadSpeed * deltaTime);
	m_roadSprite2.move(0, m_roadSpeed * deltaTime);
	if (m_roadSprite1.getPosition().y > 900.f) {
		m_roadSprite1.setPosition(0, -900.f);
	}
	if (m_roadSprite2.getPosition().y > 900.f) {
		m_roadSprite2.setPosition(0, -900.f);
	}
	spawnObstacle();
	player.update();
	for (auto& obstacle : obstacles) {
		obstacle->update();
		// Only check collision if lanes match
		if (obstacle->getLane() == player.lane) {
			sf::FloatRect playerBounds = player.getBounds();
			sf::FloatRect obstacleBounds = obstacle->getBounds();
			float margin = 20.f;
			playerBounds.left += margin;
			playerBounds.top += margin;
			playerBounds.width -= 2 * margin;
			playerBounds.height -= 2 * margin;
			obstacleBounds.left += margin;
			obstacleBounds.top += margin;
			obstacleBounds.width -= 2 * margin;
			obstacleBounds.height -= 2 * margin;
			if (playerBounds.intersects(obstacleBounds)) {
				if (!isGameOver) {
					m_backgroundMusic.stop();
					m_crashSound.play();
				}
				isGameOver = true;
				isShaking = true;
				shakeClock.restart();
			}
		}
	}
	if (isShaking) {
		if (shakeClock.getElapsedTime().asSeconds() < 0.2f) {
			sf::View view = window.getView();
			float offsetX = (rand() % 10) - 5;
			float offsetY = (rand() % 10) - 5;
			view.move(offsetX, offsetY);
			window.setView(view);
		}
		else {
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
	for (auto& obstacle : obstacles) {
		obstacle->draw(window);
	}
	player.draw(window);
	window.draw(scoreText);
	if (isGameOver) {
		window.draw(gameOverText);
	}
	window.display();
}

void Game::spawnObstacle() {
	if (spawnClock.getElapsedTime().asSeconds() > 1.5f) {
		int randomLane = rand() % 3;
		obstacles.push_back(std::make_unique<Rock>(randomLane));
		spawnClock.restart();
	}
}

void Game::reset() {
	isGameOver = false;
	score = 0;
	obstacles.clear();
	player = Player();
	m_backgroundMusic.play();
	// Optionally reset player position, score, etc.
}