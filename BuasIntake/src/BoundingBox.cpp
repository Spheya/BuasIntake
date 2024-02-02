#include "BoundingBox.hpp"

tmpl8::vec2 BoundingBox::topLeft() const {
	return center + size * tmpl8::vec2(-0.5f, -0.5);
}

tmpl8::vec2 BoundingBox::bottomLeft() const {
	return center + size * tmpl8::vec2(-0.5f, +0.5);
}

tmpl8::vec2 BoundingBox::topRight() const {
	return center + size * tmpl8::vec2(+0.5f, -0.5);
}

tmpl8::vec2 BoundingBox::bottomRight() const {
	return center + size * tmpl8::vec2(+0.5f, +0.5);
}

bool BoundingBox::intersects(BoundingBox other) const {
	tmpl8::vec2 offset = tmpl8::vec2::abs(center - other.center) - size;
	return tmpl8::Min(std::abs(offset.x), std::abs(offset.y)) < 0.0f;
}

void BoundingBox::drawDebug(tmpl8::Surface* surface, tmpl8::Pixel color) const {
	surface->Box(int(topLeft().x), int(topLeft().y), int(bottomRight().x), int(bottomRight().y), color);
}