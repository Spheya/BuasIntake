#include "Scene.hpp"

#include <numeric>
#include <cassert>
#include <algorithm>

void Scene::addEntity(std::shared_ptr<Entity> entity) {
	assert(!containsEntity(entity.get()));

	m_entities.push_back(std::move(entity));
}

void Scene::removeEntity(const Entity* entity) {
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), 
		[entity](const std::shared_ptr<Entity>& other) { return entity == other.get(); }
	), m_entities.end());
}

bool Scene::containsEntity(const Entity* entity) {
	return std::find_if(m_entities.begin(), m_entities.end(), 
		[entity](const std::shared_ptr<Entity>& other) { return entity == other.get(); }
	) != m_entities.end();
}

float Scene::castRay(tmpl8::vec2 origin, tmpl8::vec2 direction, float maxDist) {
	return m_terrain->castRay(nullptr, origin, direction, maxDist);
}

float Scene::castBox(BoundingBox origin, tmpl8::vec2 direction, float maxDist) {
	return castRay(origin.center, direction, maxDist);
}

void Scene::setTerrain(std::shared_ptr<Terrain> terrain) {
	m_terrain = terrain;
}

const std::shared_ptr<Terrain>& Scene::getTerrain() const {
	return m_terrain;
}

void Scene::updateEntities(float deltatime) {
	for (auto& entity : m_entities)
		entity->update(deltatime);

	// Remove entities flagged for deletion
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
		[](const std::shared_ptr<Entity>& entity) { return entity->isFlaggedforDeletion(); }
	), m_entities.end());

	// Sort the entities to prepare for drawing
	std::sort(m_entities.begin(), m_entities.end(),
		[](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) { return a->depth > b->depth; }
	);
}

void Scene::draw(tmpl8::Surface* surface) const {
	for (auto& entity : m_entities)
		entity->draw(surface);
}
