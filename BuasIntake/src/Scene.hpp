#pragma once

#include <vector>
#include <memory>
#include "tmpl8/Surface.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"
#include "BoundingBox.hpp"

class Scene {
public:
	void addEntity(std::shared_ptr<Entity> entity);
	void removeEntity(const Entity* entity);
	bool containsEntity(const Entity* entity);

	float castRay(tmpl8::vec2 origin, tmpl8::vec2 direction, float maxDist = -1.0f);
	float castBox(BoundingBox origin, tmpl8::vec2 direction, float maxDist = -1.0f);

	void setTerrain(std::shared_ptr<Terrain> terrain);
	const std::shared_ptr<Terrain>& getTerrain() const;

	void updateEntities(float deltatime);
	void draw(tmpl8::Surface* surface) const;

private:
	std::shared_ptr<Terrain> m_terrain = nullptr;
	std::vector<std::shared_ptr<Entity>> m_entities;
};