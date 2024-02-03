#pragma once

#include <memory>
#include "tmpl8/Sprite.hpp"
#include "Entity.hpp"
#include <vector>

class TileMap : public Entity{
public:
	TileMap(const char* filename, std::shared_ptr<tmpl8::Sprite> sprite, float tileSize = 48.0f);

	int getTile(size_t x, size_t y) const;

	size_t getWidth() const;
	size_t getHeight() const;

private:
	size_t m_width;
	size_t m_height;
	std::vector<int> m_tiles;
	std::shared_ptr<tmpl8::Sprite> m_sprite;
	float m_tileSize;

	void update(float deltatime) override;
	void draw(tmpl8::Surface* surface) override;

};