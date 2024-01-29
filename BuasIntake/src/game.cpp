#include "Game.hpp"
#include "tmpl8/SpriteAnimation.hpp"
#include <iostream>

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
}

// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void Game::Shutdown()
{
}

static Tmpl8::Sprite rotatingGun(new Tmpl8::Surface("assets/Player.png"), 16, 4);
static Tmpl8::SpriteAnimation animation{ 4, 5, 6, 7, 8, 9 };

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
	// clear the graphics window
	screen->Clear(0);

	// draw a sprite
	animation.update(deltaTime);
	rotatingGun.SetFrame(animation.getCurrentFrame());
	rotatingGun.Draw(screen, 64, 64);
}