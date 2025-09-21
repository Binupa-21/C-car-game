#undef Game
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Obstacle.h"
#include <vector>
#include <memory>

// Define the different states our application can be in.
enum class GameState { MAIN_MENU, INSTRUCTIONS, PLAYING, GAME_OVER };

class Game {
public:
	Game();
	void run();

private:
	// --- Helper Function ---
	void centerText(sf::Text& text, float x, float y);

	// --- State Machine Router Functions ---
	void processEvents();
	void update(sf::Time deltaTime);
	void render();

	// --- State-Specific Functions ---
	void processMenuEvents(sf::Event& event);
	void processInstructionsEvents(sf::Event& event);
	void processGameOverEvents(sf::Event& event);

	void updateMainMenu();
	void updateGame(sf::Time deltaTime);
	void updateGameOverMenu();

	void renderMainMenu();
	void renderInstructions();
	void renderGame();
	void renderGameOver();

	// --- Game Utility Functions ---
	void reset();
	void spawnObstacle();
	void updateRoad(sf::Time deltaTime);
	void saveHighScore();
	void loadHighScore();

private:
	sf::RenderWindow m_window;
	GameState m_currentState = GameState::MAIN_MENU; // Start in the menu!
	Player m_player;
	std::vector<std::unique_ptr<Obstacle>> m_obstacles;
	sf::Font m_font;
	sf::Clock m_spawnClock;
	int m_score = 0;
	int m_highScore = 0;

	// --- Main Menu Assets ---
	sf::Sprite m_menuBackgroundSprite;
	sf::Text m_menuTitle;
	sf::Text m_menuPlay;
	sf::Text m_menuInstructions;
	sf::Text m_menuExit;
	sf::Text m_menuDifficulty; 
	int m_selectedMenuItem = 0;
	int m_difficulty = 1; // 0: Easy, 1: Medium, 2: Hard

	// --- Instructions Assets ---
	sf::Text m_instructionsText;

	// --- Game Assets ---
	sf::Text m_scoreText;
	sf::Sprite m_roadSprite1, m_roadSprite2;
	sf::Sound m_crashSound;
	sf::Music m_backgroundMusic;

	// --- Game Over Assets ---
	sf::Text m_gameOverTitle;
	sf::Text m_highScoreText;
	sf::Text m_gameOverRestart;
	sf::Text m_gameOverMainMenu;
	sf::Text m_gameOverExit;
	int m_selectedGameOverItem = 0;
};