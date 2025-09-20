#undef Game
#include "game.h"
#include "Rock.h"
#include "cone.h" 
#include "AssetManager.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Game::Game() : m_window(sf::VideoMode(800, 900), "SFML Car Game") {
	m_window.setFramerateLimit(60);
	srand(static_cast<unsigned int>(time(0)));
	loadHighScore();

	// Load font (must be done before configuring text)
	if (!m_font.loadFromFile("Assets/arial.ttf")) { std::cerr << "Failed to load font!\n"; }

	// --- Configure Shared and Game-Specific Text ---
	m_scoreText.setFont(m_font); m_scoreText.setCharacterSize(24); m_scoreText.setFillColor(sf::Color::White); m_scoreText.setPosition(10.f, 10.f);
	m_highScoreText.setFont(m_font); m_highScoreText.setCharacterSize(30); m_highScoreText.setFillColor(sf::Color::White);

	// --- Configure Main Menu ---
	m_menuBackgroundSprite.setTexture(AssetManager::GetTexture("Assets/Mainmenu.png"));
	m_menuTitle.setFont(m_font); m_menuTitle.setString("NITRO RUSH"); m_menuTitle.setCharacterSize(80); m_menuTitle.setFillColor(sf::Color::White); m_menuTitle.setStyle(sf::Text::Bold);
	centerText(m_menuTitle, 400, 150);
	m_menuPlay.setFont(m_font); m_menuPlay.setString("Play"); m_menuPlay.setCharacterSize(40); centerText(m_menuPlay, 400, 350);
	m_menuPlay.setOutlineColor(sf::Color::Black); m_menuPlay.setOutlineThickness(2.f);
	m_menuInstructions.setFont(m_font); m_menuInstructions.setString("Instructions"); m_menuInstructions.setCharacterSize(40); centerText(m_menuInstructions, 400, 450);
	m_menuInstructions.setOutlineColor(sf::Color::Black); m_menuInstructions.setOutlineThickness(2.f);
	m_menuExit.setFont(m_font); m_menuExit.setString("Exit"); m_menuExit.setCharacterSize(40); centerText(m_menuExit, 400, 550);
	m_menuExit.setOutlineColor(sf::Color::Black); m_menuExit.setOutlineThickness(2.f);
	// Add difficulty option
	m_menuDifficulty.setFont(m_font); m_menuDifficulty.setString("Difficulty: Medium"); m_menuDifficulty.setCharacterSize(32); centerText(m_menuDifficulty, 400, 650);
	m_menuDifficulty.setOutlineColor(sf::Color::Black); m_menuDifficulty.setOutlineThickness(2.f);

	// --- Configure Instructions Screen ---
	m_instructionsText.setFont(m_font);
	std::stringstream ss;
	ss << "INSTRUCTIONS\n\n\n"
		<< "Use the LEFT and RIGHT arrow keys to change lanes.\n\n"
		<< "Dodge the falling rocks for as long as you can!\n\n\n\n"
		<< "Press any key to return to the menu.";

	// Responsive size, wrapping and spacing for instructions
	unsigned int instrSize = static_cast<unsigned int>(m_window.getSize().y * 0.03f); // ~3% of window height
	float maxWidth = m_window.getSize().x * 0.8f; // wrap to 80% of window width
	std::string wrapped = wrapText(ss.str(), m_font, instrSize, maxWidth);
	m_instructionsText.setString(wrapped);
	m_instructionsText.setCharacterSize(instrSize);
	m_instructionsText.setLineSpacing(1.2f);
	m_instructionsText.setFillColor(sf::Color::White);
	centerText(m_instructionsText, static_cast<float>(m_window.getSize().x) / 2.0f, static_cast<float>(m_window.getSize().y) * 0.35f);

	// --- Configure Game Over Menu ---
	m_gameOverTitle.setFont(m_font); m_gameOverTitle.setString("GAME OVER"); m_gameOverTitle.setCharacterSize(80); m_gameOverTitle.setFillColor(sf::Color::Red); centerText(m_gameOverTitle, 400, 200);
	m_gameOverRestart.setFont(m_font); m_gameOverRestart.setString("Restart"); m_gameOverRestart.setCharacterSize(40); centerText(m_gameOverRestart, 400, 400);
	m_gameOverMainMenu.setFont(m_font); m_gameOverMainMenu.setString("Main Menu"); m_gameOverMainMenu.setCharacterSize(40); centerText(m_gameOverMainMenu, 400, 500);
	m_gameOverExit.setFont(m_font); m_gameOverExit.setString("Exit"); m_gameOverExit.setCharacterSize(40); centerText(m_gameOverExit, 400, 600);

	// --- Configure Game Assets ---
	m_roadSprite1.setTexture(AssetManager::GetTexture("Assets/road.png"));
	m_roadSprite2.setTexture(AssetManager::GetTexture("Assets/road.png"));
	m_roadSprite1.setPosition(0, 0); m_roadSprite2.setPosition(0, -900.f);

	m_crashSound.setBuffer(AssetManager::GetSoundBuffer("Assets/crash.wav"));
	if (m_backgroundMusic.openFromFile("Assets/bgmusic.ogg")) { // FIX: use correct music file
		m_backgroundMusic.setVolume(40.f); m_backgroundMusic.setLoop(true);
	}
}

void Game::run() {
	sf::Clock clock;
	while (m_window.isOpen()) {
		sf::Time deltaTime = clock.restart();
		processEvents();
		update(deltaTime);
		render();
	}
}

// A helper function to make text centering easier
void Game::centerText(sf::Text& text, float x, float y) {
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	text.setPosition(x, y);
}

// --- STATE MACHINE ROUTERS ---
void Game::processEvents() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) { m_window.close(); }
		switch (m_currentState) {
		case GameState::MAIN_MENU:    processMenuEvents(event); break;
		case GameState::INSTRUCTIONS: processInstructionsEvents(event); break;
		case GameState::GAME_OVER:    processGameOverEvents(event); break;
		case GameState::PLAYING:      break; // Real-time input is handled in updateGame
		}
	}
}

void Game::update(sf::Time deltaTime) {
	switch (m_currentState) {
	case GameState::MAIN_MENU:    updateMainMenu(); break;
	case GameState::PLAYING:      updateGame(deltaTime); break;
	case GameState::GAME_OVER:    updateGameOverMenu(); break;
	case GameState::INSTRUCTIONS: break; // No update logic needed
	}
}

void Game::render() {
	m_window.clear();
	switch (m_currentState) {
	case GameState::MAIN_MENU:    renderMainMenu(); break;
	case GameState::INSTRUCTIONS: renderInstructions(); break;
	case GameState::PLAYING:      renderGame(); break;
	case GameState::GAME_OVER:    renderGameOver(); break;
	}
	m_window.display();
}

// --- EVENT PROCESSING FOR MENUS ---
void Game::processMenuEvents(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Up) { m_selectedMenuItem = (m_selectedMenuItem + 3) % 4; }
		if (event.key.code == sf::Keyboard::Down) { m_selectedMenuItem = (m_selectedMenuItem + 1) % 4; }
		if (event.key.code == sf::Keyboard::Left && m_selectedMenuItem == 3) { m_difficulty = (m_difficulty + 2) % 3; }
		if (event.key.code == sf::Keyboard::Right && m_selectedMenuItem == 3) { m_difficulty = (m_difficulty + 1) % 3; }
		if (event.key.code == sf::Keyboard::Return) {
			if (m_selectedMenuItem == 0) { reset(); m_currentState = GameState::PLAYING; }
			if (m_selectedMenuItem == 1) { m_currentState = GameState::INSTRUCTIONS; }
			if (m_selectedMenuItem == 2) { m_window.close(); }
		}
	}
	// Mouse click support for menu
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		auto mousePos = m_window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
		if (m_menuPlay.getGlobalBounds().contains(mousePos)) { reset(); m_currentState = GameState::PLAYING; }
		if (m_menuInstructions.getGlobalBounds().contains(mousePos)) { m_currentState = GameState::INSTRUCTIONS; }
		if (m_menuExit.getGlobalBounds().contains(mousePos)) { m_window.close(); }
		if (m_menuDifficulty.getGlobalBounds().contains(mousePos)) { m_difficulty = (m_difficulty + 1) % 3; }
	}
}
void Game::processInstructionsEvents(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) { m_currentState = GameState::MAIN_MENU; }
}
void Game::processGameOverEvents(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Up) { m_selectedGameOverItem = (m_selectedGameOverItem + 2) % 3; }
		if (event.key.code == sf::Keyboard::Down) { m_selectedGameOverItem = (m_selectedGameOverItem + 1) % 3; }
		if (event.key.code == sf::Keyboard::Return) {
			if (m_selectedGameOverItem == 0) { reset(); m_currentState = GameState::PLAYING; }
			if (m_selectedGameOverItem == 1) { m_currentState = GameState::MAIN_MENU; }
			if (m_selectedGameOverItem == 2) { m_window.close(); }
		}
	}
	// --- Mouse click support for game over menu ---
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		auto mousePos = m_window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
		if (m_gameOverRestart.getGlobalBounds().contains(mousePos)) { reset(); m_currentState = GameState::PLAYING; }
		if (m_gameOverMainMenu.getGlobalBounds().contains(mousePos)) { m_currentState = GameState::MAIN_MENU; }
		if (m_gameOverExit.getGlobalBounds().contains(mousePos)) { m_window.close(); }
	}
}

// --- UPDATE LOGIC ---
void Game::updateMainMenu() {
	m_menuPlay.setFillColor(sf::Color::White); m_menuInstructions.setFillColor(sf::Color::White); m_menuExit.setFillColor(sf::Color::White); m_menuDifficulty.setFillColor(sf::Color::White);
	if (m_selectedMenuItem == 0) m_menuPlay.setFillColor(sf::Color::Red);
	if (m_selectedMenuItem == 1) m_menuInstructions.setFillColor(sf::Color::Red);
	if (m_selectedMenuItem == 2) m_menuExit.setFillColor(sf::Color::Red);
	if (m_selectedMenuItem == 3) m_menuDifficulty.setFillColor(sf::Color::Red);
	// Update difficulty text
	const char* diffStr[] = {"Easy", "Medium", "Hard"};
	m_menuDifficulty.setString(std::string("Difficulty: ") + diffStr[m_difficulty]);
	centerText(m_menuDifficulty, 400, 650);
}
void Game::updateGame(sf::Time deltaTime) {
	// Car movement logic (edge-triggered)
	static bool leftPressedLast = false;
	static bool rightPressedLast = false;
	bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	if (leftPressed && !leftPressedLast) m_player.move(-1);
	if (rightPressed && !rightPressedLast) m_player.move(1);
	leftPressedLast = leftPressed;
	rightPressedLast = rightPressed;

	m_player.handleInput(); // for rotation only
	m_player.update();

	// --- Dynamic obstacle spawn frequency and road/rock speed ---
	float minSpawnInterval, maxSpawnInterval, baseSpeed, maxSpeed;
	if (m_difficulty == 0) { // Easy
		minSpawnInterval = 1.0f; maxSpawnInterval = 2.0f; baseSpeed = 150.0f; maxSpeed = 350.0f;
	} else if (m_difficulty == 1) { // Medium
		minSpawnInterval = 0.7f; maxSpawnInterval = 1.5f; baseSpeed = 200.0f; maxSpeed = 600.0f;
	} else { // Hard
		minSpawnInterval = 0.4f; maxSpawnInterval = 1.0f; baseSpeed = 300.0f; maxSpeed = 900.0f;
	}
	float spawnInterval = std::max(minSpawnInterval, maxSpawnInterval - m_score * 0.0015f);
	float travelSpeed = std::min(baseSpeed + m_score * 0.7f, maxSpeed);
					
	// Use the new spawn interval
	if (m_spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
		if (rand() % 2 == 0) {
			m_obstacles.push_back(std::make_unique<Rock>(rand() % 3));
			std::cout << "Spawned Rock\n";
		} else {
			m_obstacles.push_back(std::make_unique<Cone>(rand() % 3));
			std::cout << "Spawned Cone\n";
		}

		m_spawnClock.restart();
	}

	// Move road at dynamic speed
	m_roadSprite1.move(0, travelSpeed * deltaTime.asSeconds());
	m_roadSprite2.move(0, travelSpeed * deltaTime.asSeconds());
	if (m_roadSprite1.getPosition().y > 900.f) { m_roadSprite1.setPosition(0, -900.f); }
	if (m_roadSprite2.getPosition().y > 900.f) { m_roadSprite2.setPosition(0, -900.f); }

	m_score++;
	m_scoreText.setString("Score: " + std::to_string(m_score));
	for (auto& obstacle : m_obstacles) {
		obstacle->setSpeed(travelSpeed * deltaTime.asSeconds()); // Keep all rocks at same speed as road
		obstacle->update();
		// Only collide if lanes match, but use a more reasonable margin
		if (obstacle->getLane() == m_player.lane) {
			sf::FloatRect playerBounds = m_player.getBounds();
			sf::FloatRect obstacleBounds = obstacle->getBounds();
			float margin = 10.f; // Small margin for more accurate collision
			playerBounds.left += margin;
			playerBounds.top += margin;
			playerBounds.width -= 2 * margin;
			playerBounds.height -= 2 * margin;
			obstacleBounds.left += margin;
			obstacleBounds.top += margin;
			obstacleBounds.width -= 2 * margin;
			obstacleBounds.height -= 2 * margin;
			if (playerBounds.intersects(obstacleBounds)) {
				m_backgroundMusic.stop(); m_crashSound.play(); saveHighScore();
				m_currentState = GameState::GAME_OVER;
			}
		}
	}
	m_obstacles.erase(std::remove_if(m_obstacles.begin(), m_obstacles.end(), [](const auto& o) { return o->getTop() > 900.f; }), m_obstacles.end());
}
void Game::updateGameOverMenu() {
	m_gameOverRestart.setFillColor(sf::Color::White); m_gameOverMainMenu.setFillColor(sf::Color::White); m_gameOverExit.setFillColor(sf::Color::White);
	if (m_selectedGameOverItem == 0) m_gameOverRestart.setFillColor(sf::Color::Red);
	if (m_selectedGameOverItem == 1) m_gameOverMainMenu.setFillColor(sf::Color::Red);
	if (m_selectedGameOverItem == 2) m_gameOverExit.setFillColor(sf::Color::Red);
}

// --- RENDER LOGIC ---
void Game::renderMainMenu() { m_window.draw(m_menuBackgroundSprite); m_window.draw(m_menuTitle); m_window.draw(m_menuPlay); m_window.draw(m_menuInstructions); m_window.draw(m_menuExit); m_window.draw(m_menuDifficulty); }
void Game::renderInstructions() { m_window.draw(m_menuBackgroundSprite); m_window.draw(m_instructionsText); }
void Game::renderGame() {
	m_window.draw(m_roadSprite1); m_window.draw(m_roadSprite2); m_player.draw(m_window);
	for (const auto& obstacle : m_obstacles) { obstacle->draw(m_window); }
	m_window.draw(m_scoreText);
}
void Game::renderGameOver() {
	renderGame(); // Draw the final game state in the background
	m_highScoreText.setString("Score: " + std::to_string(m_score) + "\nHigh Score: " + std::to_string(m_highScore));
	centerText(m_highScoreText, 400, 320);
	m_window.draw(m_gameOverTitle); m_window.draw(m_highScoreText); m_window.draw(m_gameOverRestart); m_window.draw(m_gameOverMainMenu); m_window.draw(m_gameOverExit);
}

// --- GAME UTILITY FUNCTIONS ---
void Game::reset() {
	m_score = 0; m_obstacles.clear(); m_player.reset(); m_spawnClock.restart(); m_backgroundMusic.play();
}
void Game::spawnObstacle() {
	if (m_spawnClock.getElapsedTime().asSeconds() > 1.2f) {
		m_obstacles.push_back(std::make_unique<Rock>(rand() % 3)); // FIX: Only pass lane
		m_spawnClock.restart();
	}
}
void Game::updateRoad(sf::Time deltaTime) {
	float roadSpeed = 200.0f; m_roadSprite1.move(0, roadSpeed * deltaTime.asSeconds()); m_roadSprite2.move(0, roadSpeed * deltaTime.asSeconds());
	if (m_roadSprite1.getPosition().y > 900.f) { m_roadSprite1.setPosition(0, -900.f); }
	if (m_roadSprite2.getPosition().y > 900.f) { m_roadSprite2.setPosition(0, -900.f); } // FIX typo
}
void Game::saveHighScore() {
	if (m_score > m_highScore) { m_highScore = m_score; std::ofstream file("highscore.txt"); if (file.is_open()) file << m_highScore; }
}
void Game::loadHighScore() {
	std::ifstream file("highscore.txt"); if (file.is_open()) file >> m_highScore;
}

// Simple word-wrapping helper: breaks input into lines that fit within maxWidth
// using the provided font and character size. This is a basic implementation
// and not pixel-perfect but effective for menu/instruction text.
std::string wrapText(const std::string& input, const sf::Font& font, unsigned int charSize, float maxWidth) {
	std::istringstream words(input);
	std::string word;
	std::string line;
	std::string output;
	while (words >> word) {
		std::string test = line.empty() ? word : line + " " + word;
		sf::Text tmp(test, font, charSize);
		if (tmp.getLocalBounds().width > maxWidth) {
			if (!line.empty()) {
				output += line + "\n";
				line = word;
			} else {
				// single long word — force it onto its own line
				output += test + "\n";
				line.clear();
			}
		} else {
			line = test;
		}
	}
	if (!line.empty()) output += line;
	return output;
}