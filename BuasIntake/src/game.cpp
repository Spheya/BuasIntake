#include "Game.hpp"
#include "tmpl8/Template.hpp"
#include "tmpl8/SpriteAnimation.hpp"
#include "tmpl8/Sprite.hpp"
#include <iostream>
#include <string>

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	auto* surface = new Tmpl8::Surface("assets/Player.png");
	Tmpl8::Sprite rotatingGun(surface, 16, 4);

	// rendering on surface
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

	std::string fpsCounter = (std::to_string(1.0f / deltaTime) + "fps");
	screen->Print(fpsCounter.c_str(), 32, 32, 0xffffffff);

	// draw a sprite
	animation.update(deltaTime);
	rotatingGun.SetFrame(animation.getCurrentFrame());
	rotatingGun.Draw(screen, Tmpl8::vec2(64.0f, 64.0f), Tmpl8::vec2(192.0f, 192.0f));
}