#pragma once

#include "../tmpl8/Template.hpp"
#include "../tmpl8/Surface.hpp"

class Entity {
public:
	virtual ~Entity() = default;

	virtual void update(float deltatime) = 0;
	virtual void draw(tmpl8::Surface* surface) = 0;

	void flagForDeletion() { m_deletionFlag = true; }
	bool isFlaggedforDeletion() { return m_deletionFlag; }

	tmpl8::vec2 position = tmpl8::vec2(0.0f);
	tmpl8::vec2 size = tmpl8::vec2(16.0f);

private:
	bool m_deletionFlag = false;
};