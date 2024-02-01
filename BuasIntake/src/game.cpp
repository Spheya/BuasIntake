#include "Game.hpp"
#include "tmpl8/Template.hpp"
#include "tmpl8/Sprite.hpp"
#include "SpriteAnimation.hpp"
#include <iostream>
#include <string>
#include "Player.hpp"
#include "SDL_keycode.h"

namespace {
	
	std::shared_ptr<tmpl8::Surface> PlayerTexture = std::make_shared<tmpl8::Surface>("assets/Player.png");

}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	m_player = std::make_shared<Player>(PlayerTexture);
	m_player->position = tmpl8::vec2(300.0f, 400.0f);
	m_gameScene.addEntity(m_player);
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
}

void Game::MouseUp(int button) {
}

void Game::MouseDown(int button) {
}

void Game::MouseMove(int x, int y) {
}

void Game::KeyUp(int key) {
	m_player->updateInputKeys(key, false);
}

void Game::KeyDown(int key) {
	m_player->updateInputKeys(key, true);
}
