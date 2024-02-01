#pragma once

#include <memory>

#include "../../tmpl8/Sprite.hpp"
#include "../../graphics/SpriteAnimation.hpp"
#include "../Entity.hpp"

class Player : public Entity {
public:
	Player(std::shared_ptr<tmpl8::Surface> texture);
	
	void update(float deltatime) override;
	void draw(tmpl8::Surface* surface) override;

	void updateInputKeys(int key, bool down);

private:
	tmpl8::Sprite m_sprite;

	SpriteAnimation m_idleAnimation{ 0 };
	SpriteAnimation m_duckAnimation{ 1 };
	SpriteAnimation m_runAnimation{ 4, 5, 6, 7, 8, 9 };
	SpriteAnimation m_jumpAnimation{ 12 };
	SpriteAnimation m_fallAnimation{ 13 };
	SpriteAnimation* m_currentAnimation;

	tmpl8::vec2 m_velocity = tmpl8::vec2(0.0f);
	bool m_onGround = true;
	float m_squishT = 0.0f;
	tmpl8::vec2 m_squish = tmpl8::vec2(1.0f);

	bool m_moveLeftInput = false;
	bool m_moveRightInput = false;
	bool m_duckInput = false;
	bool m_jumpInput = false;

	float m_speed = 300.0f;
	float m_friction = 19.0f;
	float m_slideFriction = 2.0f;
	float m_jumpFriction = 5.0f;
	float m_jumpDuration = 0.7f;
	float m_jumpHeight = 128.0f;
	float m_duckJumpIncrease = 1.3f;
	float m_fallGravityIncrease = 1.4f;
	float m_keyReleaseGravityIncrease = 2.25f;

	void updateMovement(float deltatime);
	void updateAnimations(float deltatime);
};