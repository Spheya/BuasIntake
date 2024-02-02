#pragma once

#include "tmpl8/Template.hpp"

struct RaycastResult {

	RaycastResult(bool hit, float distance, tmpl8::vec2 normal) :
		hit(hit), distance(distance), normal(normal)
	{}

	bool hit = false;
	float distance = 0.0f;
	tmpl8::vec2 normal = tmpl8::vec2(0.0f);

};