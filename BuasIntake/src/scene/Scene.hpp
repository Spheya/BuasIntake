#pragma once

#include <vector>
#include <memory>
#include "../tmpl8/Surface.hpp"
#include "Entity.hpp"

class Scene {
public:
	void addEntity(std::shared_ptr<Entity> entity);
	void removeEntity(const Entity* entity);
	bool containsEntity(const Entity* entity);

	void updateEntities(float deltatime);
	void draw(tmpl8::Surface* surface) const;

private:
	std::vector<std::shared_ptr<Entity>> m_entities;
};