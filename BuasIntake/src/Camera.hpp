#pragma once

#include "tmpl8/Template.hpp"

class Camera {
public:
	tmpl8::vec2 position = tmpl8::vec2(0.0f);
	tmpl8::vec2 scale = tmpl8::vec2(1.0f);

	void moveTowards(float deltatime, tmpl8::vec2 target, tmpl8::vec2 speed);
	tmpl8::vec2 toScreenSpace(tmpl8::vec2 position);
	tmpl8::vec2 scaleToScreenSpace(tmpl8::vec2 scale);

	static Camera Main;
};