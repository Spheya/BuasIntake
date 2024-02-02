#pragma once

#include "tmpl8/Template.hpp"
#include "tmpl8/Surface.hpp"

struct BoundingBox {

	tmpl8::vec2 center;
	tmpl8::vec2 size;

	tmpl8::vec2 topLeft() const;
	tmpl8::vec2 bottomLeft() const;
	tmpl8::vec2 topRight() const;
	tmpl8::vec2 bottomRight() const;

	bool intersects(BoundingBox other) const;

	void drawDebug(tmpl8::Surface* surface, tmpl8::Pixel color = 0xffffffff) const;
};