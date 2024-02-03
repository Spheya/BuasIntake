#include "TileMap.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include "Camera.hpp"

TileMap::TileMap(const char* filename, std::shared_ptr<tmpl8::Sprite> sprite, float tileSize) :
	m_width(0),
	m_height(0),
	m_sprite(std::move(sprite)),
	m_tileSize(tileSize)
{
	std::ifstream file;
	file.open(filename);
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	std::string row;
	while (std::getline(stream, row)) {
		++m_height;

		std::string cell;
		std::stringstream rowStream;
		rowStream << row;

		while (std::getline(rowStream, cell, ','))
			m_tiles.push_back(std::stoi(cell));
	}

	m_width = m_tiles.size() / m_height;
}

int TileMap::getTile(size_t x, size_t y) const {
	if (x > m_width || y > m_height) return -1;
	return m_tiles[x + y * m_width];
}

size_t TileMap::getWidth() const {
	return m_width;
}

size_t TileMap::getHeight() const {
	return m_height;
}

void TileMap::update(float deltatime) {}

void TileMap::draw(tmpl8::Surface* surface) {
	tmpl8::vec2 tileSizeSS = Camera::Main.scaleToScreenSpace(tmpl8::vec2(m_tileSize));

	int width = int(std::ceil(ScreenWidth / tileSizeSS.x));
	int height = int(std::ceil(ScreenHeight / tileSizeSS.y));

	tmpl8::vec2 cameraTilePos = (Camera::Main.position - position) * (1.0f / m_tileSize);
	
	size_t minX = std::max(std::min(int(cameraTilePos.x - width / 2) - 1, int(m_width - 1)), 0);
	size_t maxX = std::max(std::min(int(cameraTilePos.x + width / 2) + 2, int(m_width - 1)), 0);
	size_t minY = std::max(std::min(int(cameraTilePos.y - height / 2) - 1, int(m_height - 1)), 0);
	size_t maxY = std::max(std::min(int(cameraTilePos.y + height / 2) + 2, int(m_height - 1)), 0);

	for (size_t x = minX; x < maxX; x++) {
		for (size_t y = minY; y < maxY; y++) {
			int tile = getTile(x, y);
			if (tile != -1) {
				m_sprite->SetFrame(tile);
				m_sprite->Draw(surface, Camera::Main.toScreenSpace(position + tmpl8::vec2(x, y) * m_tileSize), tileSizeSS);
			}
		}
	}
}
