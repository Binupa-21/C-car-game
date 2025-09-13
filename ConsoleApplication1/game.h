#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include "obstacle.h"
#include "player.h"

class Game {
private:
	sf::RenderWindow window;
	Player player;
	std::vector<std::unique_ptr<Obstacle>> obstacles;
	bool isGameOver = false;
	sf::Clock spawnClock;
	sf::Sprite m_roadSprite1;
	sf::Sprite m_roadSprite2;
	float m_roadSpeed = 200.0f;  // Pixels per second
	// Camera shake variables
	sf::Clock shakeClock;
	bool isShaking = false;

	void handleInput();
	void update();
	void render();
	void spawnObstacle();
	sf::Sound m_crashSound;  // <--  The object that plays the sound
	sf::Music m_backgroundMusic;  // <--  The object for background music

	sf::Font font;
	sf::Text scoreText;
	sf::Text gameOverText;
	int score = 0;



public:
	Game();
	void run();
	void reset();
};