#pragma once

#include "tmpl8/Template.hpp"
#include "tmpl8/Surface.hpp"
#include "Entity.hpp"
#include "RaycastResult.hpp"
#include "TileMap.hpp"

class Terrain : public Entity {
public:
	Terrain(std::shared_ptr<const TileMap> collisionMap, float tileSize = 32.0f);
	Terrain(size_t width, size_t height, float tileSize = 32.0f);
	~Terrain();

	void setTileSolid(size_t x, size_t y, bool solid);
	bool isTileSolid(size_t x, size_t y);

	void drawDebugBoxes(tmpl8::Surface* surface);
	RaycastResult castRay(tmpl8::Surface* surface, tmpl8::vec2 origin, tmpl8::vec2 direction, float maxDist = -1.0f);
	
	void update(float deltatime) override {};
	void draw(tmpl8::Surface* surface) override;

private:
	size_t m_width;
	size_t m_height;
	float m_tileSize;

	bool* m_solidTiles;
};