#include "Game.hpp"
#include "tmpl8/Template.hpp"
#include "tmpl8/Sprite.hpp"
#include "SpriteAnimation.hpp"
#include <iostream>
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

	auto terrain = std::make_shared<Terrain>(24, 16, 48.0f);
	for (int x = 0; x < 24; x++)
		terrain->setTileSolid(x, 10, true);

	for (int y = 0; y < 16; y++)
		terrain->setTileSolid(10, y, true);

	terrain->setTileSolid(2, 2, true);
	m_gameScene.addEntity(terrain);
	m_gameScene.setTerrain(terrain);
}

// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void Game::Shutdown()
{
}

void Game::playerDebugBoxCast(tmpl8::vec2 target) {
	BoundingBox player = m_player->getBoundingBox();
	BoundingBox endPoint = m_player->getBoundingBox();
	tmpl8::vec2 toTarget = target - player.center;
	float dist = m_gameScene.getTerrain()->castRay(m_screen, player.center, toTarget.normalized(), toTarget.length());
	endPoint.center = player.center + toTarget.normalized() * dist;
	endPoint.drawDebug(m_screen, 0xffffff00);
	m_screen->Line(player.center.x, player.center.y, endPoint.center.x, endPoint.center.y, 0xffffff00);
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
	// clear the graphics window
	m_screen->Clear(0);

	// Update Game
	m_gameScene.updateEntities(deltaTime);

	// Draw Game
	m_gameScene.draw(m_screen);

	// show fps
	std::string fpsCounter = (std::to_string(1.0f / deltaTime) + "fps");
	m_screen->Print(fpsCounter.c_str(), 32, 32, 0xffffffff);


	// debug
	playerDebugBoxCast(tmpl8::vec2(mouseX, mouseY));
	playerDebugBoxCast(tmpl8::vec2(m_player->getBoundingBox().center.x + 300.0f, m_player->getBoundingBox().center.y));
	playerDebugBoxCast(tmpl8::vec2(m_player->getBoundingBox().center.x - 300.0f, m_player->getBoundingBox().center.y));
	playerDebugBoxCast(tmpl8::vec2(m_player->getBoundingBox().center.x, m_player->getBoundingBox().center.y + 300.0));
	playerDebugBoxCast(tmpl8::vec2(m_player->getBoundingBox().center.x, m_player->getBoundingBox().center.y - 300.0));
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
