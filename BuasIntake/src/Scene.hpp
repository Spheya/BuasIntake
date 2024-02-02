#pragma once

#include <vector>
#include <memory>
#include "tmpl8/Surface.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"
#include "BoundingBox.hpp"
#include "RaycastResult.hpp"

class Scene {
public:
	Scene() = default;
	Scene(Scene&) = delete;
	Scene& operator=(Scene&) = delete;
	Scene(Scene&& other) noexcept;
	Scene& operator=(Scene&& other) noexcept;
	~Scene();

	void addEntity(std::shared_ptr<Entity> entity);
	void removeEntity(Entity* entity);
	bool containsEntity(const Entity* entity);

	RaycastResult castRay(tmpl8::Surface* surface, tmpl8::vec2 origin, tmpl8::vec2 direction, float maxDist = -1.0f);
	RaycastResult castBox(tmpl8::Surface* surface, BoundingBox origin, tmpl8::vec2 direction, float maxDist = -1.0f);

	void setTerrain(std::shared_ptr<Terrain> terrain);
	const std::shared_ptr<Terrain>& getTerrain() const;

	void updateEntities(float deltatime);
	void draw(tmpl8::Surface* surface) const;

private:
	std::shared_ptr<Terrain> m_terrain = nullptr;
	std::vector<std::shared_ptr<Entity>> m_entities;
	bool m_valid = true;
};