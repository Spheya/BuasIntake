#include "Player.hpp"

#include <cmath>
#include <iostream>
#include <SDL_keycode.h>

Player::Player(std::shared_ptr<tmpl8::Surface> texture) :
	Entity(),
	m_sprite(texture, 16, 4),
	m_currentAnimation(&m_idleAnimation)
{
	size = tmpl8::vec2(96.0f, 96.0f);

	m_sprite.SetOrigin(tmpl8::vec2(0.5f, 1.0f));
}

void Player::update(float deltatime) {
	updateMovement(deltatime);
	updateAnimations(deltatime);

	m_duckStart = false;

	m_squishT += deltatime;
	m_slideCooldownT -= deltatime;
}

void Player::updateMovement(float deltatime) {
	m_onGround = position.y >= 400.0f;
	tmpl8::vec2 prevVelocity = m_velocity;
	float horizontalInput = float(int(m_moveRightInput) - int(m_moveLeftInput));

	// Change movement values when sliding or when in air
	float friction = m_friction;
	if (!m_onGround) {
		friction = m_jumpFriction;
	} else if (m_duckInput) {
		if (m_duckStart && m_slideCooldownT < 0.0f && std::abs(m_velocity.x) > m_slideMinSpeed) {
			m_slideCooldownT = m_slideCooldown;
			m_velocity.x *= m_slideSpeedMultiplier;
		}

		horizontalInput = 0.0f;
		friction = m_slideFriction;
	}

	// An exponential function for horizontal movement to simulate friction
	float t = std::exp(-deltatime * friction);
	m_velocity.x = m_velocity.x * t + horizontalInput * m_speed * (1.0f - t);

	// Gravity
	float gravity = deltatime * (2.0f * m_jumpHeight) / (0.25 * m_jumpDuration * m_jumpDuration);
	if (m_velocity.y > 0.0f) {
		gravity *= m_fallGravityIncrease;
	} else if(!m_jumpInput) {
		gravity *= m_keyReleaseGravityIncrease;
	}

	m_velocity.y += gravity;

	// Jumping
	if (m_jumpInput && m_onGround) {
		m_onGround = false;
		m_velocity.y = - 2.0f * m_jumpHeight / (m_jumpDuration * 0.5f);

		if (m_slideCooldownT > 0.0f) {
			m_velocity.x *= m_slideSpeedMultiplier;
		} else if (m_duckInput) {
			m_velocity.y *= m_duckJumpIncrease;
		}

		prevVelocity.y = m_velocity.y; // hacky way to not use verlet intergration over the initial jump force
	}

	// Verlet intergration to update the position
	position += (m_velocity + prevVelocity) * deltatime * 0.5f;

	// Collision Detection
	if (position.y > 400.0f) {
		position.y = 400.0f;
		m_onGround = true;
		m_velocity.y = 0.0f;
	}
}

void Player::updateAnimations(float deltatime) {
	const auto* prevAnimation = m_currentAnimation;
	bool moveInput = m_moveRightInput != m_moveLeftInput;

	// Check for new animation
	if (m_onGround) {
		m_sprite.SetOrigin(tmpl8::vec2(0.5f, 1.0f));
		if (m_duckInput) {
			m_currentAnimation = &m_duckAnimation;
		} else if (moveInput) {
			m_currentAnimation = &m_runAnimation;
		} else {
			m_currentAnimation = &m_idleAnimation;
		}
	} else {
		m_sprite.SetOrigin(tmpl8::vec2(0.5f, 1.0f - (8.0f / 48.0f)));
		if (m_velocity.y > 0.0f) {
			m_currentAnimation = &m_fallAnimation;
		} else {
			m_currentAnimation = &m_jumpAnimation;
		}
	}

	if (m_currentAnimation != prevAnimation) {
		m_currentAnimation->restart();

		// Squish on animation state change while on the ground
		if (m_onGround)
			m_squishT = 0.0f;
	}

	// Update Animation
	m_currentAnimation->update(deltatime);
	m_sprite.SetFrame(m_currentAnimation->getCurrentFrame());

	// Squishing
	float verticalSquish = 1.0f + std::abs(m_velocity.y) * 0.0004f;
	float horizontalSquish = 1.0f + 0.125 * std::exp(-4.0f * m_squishT) * std::cos(20.0f * m_squishT);
	if (m_squishT > 0.25f) horizontalSquish = 1.0f;
	tmpl8::vec2 targetSquish = tmpl8::vec2(horizontalSquish / verticalSquish, verticalSquish / horizontalSquish);
	m_squish = targetSquish;

	// Make player face the right direction
	if (moveInput)
		size.x = std::abs(size.x) * (m_moveLeftInput ? -1.0f : 1.0f);
}


void Player::draw(tmpl8::Surface* surface) {
	m_sprite.Draw(surface, position, size * m_squish);
}

void Player::updateInputKeys(int key, bool down) {
	if (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_Z)
		m_jumpInput = down;

	if (key == SDL_SCANCODE_LEFT)
		m_moveLeftInput = down;

	if (key == SDL_SCANCODE_DOWN) {
		m_duckStart = down && !m_duckInput;
		m_duckInput = down;
	}

	if (key == SDL_SCANCODE_RIGHT)
		m_moveRightInput = down;
}