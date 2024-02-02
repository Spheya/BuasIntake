#include "Terrain.hpp"

#include <cmath>
#include <cassert>
#include <iostream>

Terrain::Terrain(size_t width, size_t height, float tileSize) :
	m_width(width),
	m_height(height),
	m_tileSize(tileSize),
	m_solidTiles(new bool[width * height])
{
	for (size_t x = 0; x < m_width; x++) {
		for (size_t y = 0; y < m_height; y++) {
			setTileSolid(x, y, false);
		}
	}
}

Terrain::~Terrain() {
	delete[] m_solidTiles;
}

void Terrain::setTileSolid(size_t x, size_t y, bool solid) {
	assert(x >= 0 && x < m_width);
	assert(y >= 0 && y < m_height);

	m_solidTiles[x * m_height + y] = solid;
}

bool Terrain::isTileSolid(size_t x, size_t y) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_width)
		return false;

	return m_solidTiles[x * m_height + y];
}

void Terrain::drawDebugBoxes(tmpl8::Surface* surface) {
	for (size_t x = 0; x < m_width; x++) {
		for (size_t y = 0; y < m_height; y++) {
			tmpl8::vec2 min = position + tmpl8::vec2(position.x + x * m_tileSize, position.y + y * m_tileSize);
			tmpl8::vec2 max = min + tmpl8::vec2(m_tileSize, m_tileSize);
			if (isTileSolid(x, y)) {
				surface->Box(int(min.x) + 1, int(min.y) + 1, int(max.x) - 1, int(max.y) - 1, 0xffff0000);
			} else {
				surface->Box(int(min.x) + 1, int(min.y) + 1, int(max.x) - 1, int(max.y) - 1, 0xff444444);
			}
		}
	}
}

RaycastResult Terrain::castRay(tmpl8::Surface* surface, tmpl8::vec2 origin, tmpl8::vec2 direction, float maxDist) {
	// Algorithm based on https://www.shadertoy.com/view/4dX3zl

	tmpl8::vec2 original = origin;

	origin = (origin - position) * (1.0f / m_tileSize); // convert to tilespace
	float endDist = 0.0f;

	tmpl8::ivec2 mapPos = tmpl8::ivec2(origin.x, origin.y);
	tmpl8::ivec2 rayStep = tmpl8::ivec2(direction.x < 0.0 ? -1 : +1, direction.y < 0.0f ? -1 : +1);
	tmpl8::vec2 deltaDist = tmpl8::vec2(1.0f / std::abs(direction.x), 1.0f / std::abs(direction.y));
	tmpl8::vec2 sideDist = (
		tmpl8::vec2::sign(direction) * (tmpl8::vec2(float(mapPos.x) - origin.x, float(mapPos.y) - origin.y))
		+ tmpl8::vec2(direction.x < 0.0 ? 0.0f : 1.0f, direction.y < 0.0f ? 0.0f : 1.0f)
	) * deltaDist;

	tmpl8::vec2 normal(0.0f);

	RaycastResult result(false, maxDist, normal);

	for (int i = 0; i < 32; i++) {
		if (maxDist > 0.0f && endDist * m_tileSize >= maxDist) break;
		if (isTileSolid(mapPos.x, mapPos.y)) {
			result = RaycastResult(true, endDist * m_tileSize - 0.001f, normal);
			break;
		}

		if (sideDist.x < sideDist.y) {
			endDist = sideDist.x;
			sideDist.x += deltaDist.x;
			mapPos.x += rayStep.x;
			normal = tmpl8::vec2(-float(rayStep.x), 0.0f);
		} else {
			endDist = sideDist.y;
			sideDist.y += deltaDist.y;
			mapPos.y += rayStep.y;
			normal = tmpl8::vec2(0.0f, -float(rayStep.y));
		}

		tmpl8::vec2 testPoint = origin * m_tileSize + position + direction * endDist * m_tileSize;
	}

	tmpl8::vec2 endPoint = original + direction * result.distance;

	return result;
}

void Terrain::draw(tmpl8::Surface* surface) {
	drawDebugBoxes(surface);
}
