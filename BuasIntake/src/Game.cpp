#include "Game.hpp"
#include "tmpl8/Template.hpp"
#include "tmpl8/Sprite.hpp"
#include "SpriteAnimation.hpp"
#include <string>
#include "Player.hpp"
#include "Terrain.hpp"

namespace {
	
	std::shared_ptr<tmpl8::Surface> PlayerTexture = std::make_shared<tmpl8::Surface>("assets/Player.png");

	int mouseX = 0;
	int mouseY = 0;

}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	m_player = std::make_shared<Player>(PlayerTexture);
	m_player->position = tmpl8::vec2(300.0f, 400.0f);
	m_player->depth = -1.0f;
	m_gameScene.addEntity(m_player);

	auto terrain = std::make_shared<Terrain>(16, 10, 48.0f);
	for (int x = 0; x < 16; x++) {
		terrain->setTileSolid(x, 0, true);
		terrain->setTileSolid(x, 9, true);
	}

	for (int y = 0; y < 10; y++) {
		terrain->setTileSolid(0, y, true);
		terrain->setTileSolid(15, y, true);
	}

	terrain->setTileSolid(2, 2, true);
	terrain->setTileSolid(5, 5, true);
	terrain->setTileSolid(6, 5, true);
	terrain->setTileSolid(7, 5, true);
	m_gameScene.addEntity(terrain);
	m_gameScene.setTerrain(terrain);
}

// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void Game::Shutdown()
{
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick(float deltaTime) {
	// clear the graphics window
	m_screen->Clear(0);

	// Update Game
	m_gameScene.updateEntities(deltaTime);

	// Draw Game
	m_gameScene.draw(m_screen);

	// show fps
	std::string fpsCounter = (std::to_string(1.0f / deltaTime) + "fps");
	m_screen->Print(fpsCounter.c_str(), 32, 32, 0xffffffff);
}

void Game::MouseUp(int button) {
}

void Game::MouseDown(int button) {
}

void Game::MouseMove(int x, int y) {
	mouseX += x;
	mouseY += y;
}

void Game::KeyUp(int key) {
	m_player->updateInputKeys(key, false);
}

void Game::KeyDown(int key) {
	m_player->updateInputKeys(key, true);
}
