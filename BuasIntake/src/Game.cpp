#include "Game.hpp"
#include "tmpl8/Template.hpp"
#include "tmpl8/Sprite.hpp"
#include "SpriteAnimation.hpp"
#include <string>
#include "Player.hpp"
#include "Terrain.hpp"
#include "TileMap.hpp"
#include "Camera.hpp"

namespace {
	
	std::shared_ptr<tmpl8::Surface> PlayerTexture = std::make_shared<tmpl8::Surface>("assets/Player.png");
	std::shared_ptr<tmpl8::Surface> TileSet = std::make_shared<tmpl8::Surface>("assets/TileSet.png");

	int mouseX = 0;
	int mouseY = 0;


	std::shared_ptr<TileMap> map;

}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	std::shared_ptr<tmpl8::Sprite> sprite = std::make_shared<tmpl8::Sprite>(TileSet, 16, 4);
	sprite->SetOrigin(tmpl8::vec2(0.0f));
	map = std::make_shared<TileMap>("assets/DebugLevel.csv", sprite);
	m_gameScene.addEntity(map);

	m_player = std::make_shared<Player>(PlayerTexture);
	m_player->position = tmpl8::vec2(300.0f, 400.0f);
	m_player->depth = -1.0f;
	m_gameScene.addEntity(m_player);
	Camera::Main.position = m_player->position;

	auto terrain = std::make_shared<Terrain>(map, 48.0f);
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
	m_screen->Clear(0xffadd4d9);

	// Update Game
	m_gameScene.updateEntities(deltaTime);
	Camera::Main.moveTowards(deltaTime, m_player->position, tmpl8::vec2(6.0f, 2.5f));

	// Draw Game
	m_gameScene.draw(m_screen);

	// show fps
	//std::string fpsCounter = (std::to_string(1.0f / deltaTime) + "fps");
	//m_screen->Print(fpsCounter.c_str(), 32, 32, 0xffffffff);
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
