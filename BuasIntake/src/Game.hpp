#pragma once

#include "tmpl8/Surface.hpp"
#include "Scene.hpp"
#include "Player.hpp"

class Game
{
public:
	void SetTarget(tmpl8::Surface* surface) { m_screen = surface; }
	void Init();
	void Shutdown();
	void Tick(float deltaTime);
	void MouseUp(int button);
	void MouseDown(int button);
	void MouseMove(int x, int y);
	void KeyUp(int key);
	void KeyDown(int key);

	void playerDebugBoxCast(tmpl8::vec2 target);

private:
	Scene m_gameScene;
	tmpl8::Surface* m_screen;
	std::shared_ptr<Player> m_player;
};