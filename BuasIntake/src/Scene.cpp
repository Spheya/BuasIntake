#include "Scene.hpp"

#include <numeric>
#include <cassert>
#include <algorithm>

Scene::Scene(Scene&& other) noexcept {
	other.m_valid = false;

	m_terrain = std::move(other.m_terrain);
	m_entities = std::move(other.m_entities);

	for (auto& entity : m_entities)
		entity->scene = this;
}

Scene& Scene::operator=(Scene&& other) noexcept {
	other.m_valid = false;

	m_terrain = std::move(other.m_terrain);
	m_entities = std::move(other.m_entities);

	for (auto& entity : m_entities)
		entity->scene = this;

	return *this;
}

Scene::~Scene() {
	if (!m_valid) return;

	for (auto& entity : m_entities)
		entity->scene = nullptr;
}

void Scene::addEntity(std::shared_ptr<Entity> entity) {
	assert(!containsEntity(entity.get()));
	assert(entity->scene == nullptr);

	entity->scene = this;
	m_entities.push_back(std::move(entity));
}

void Scene::removeEntity(Entity* entity) {
	assert(!containsEntity(entity));

	entity->scene = nullptr;

	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), 
		[entity](const std::shared_ptr<Entity>& other) { return entity == other.get(); }
	), m_entities.end());
}

bool Scene::containsEntity(const Entity* entity) {
	return std::find_if(m_entities.begin(), m_entities.end(), 
		[entity](const std::shared_ptr<Entity>& other) { return entity == other.get(); }
	) != m_entities.end();
}

RaycastResult Scene::castRay(tmpl8::Surface* surface, tmpl8::vec2 origin, tmpl8::vec2 direction, float maxDist) {
	return m_terrain->castRay(surface, origin, direction, maxDist);
}

RaycastResult Scene::castBox(tmpl8::Surface* surface, BoundingBox origin, tmpl8::vec2 direction, float maxDist) {
	RaycastResult r1 = castRay(surface, origin.topLeft(), direction, maxDist);
	RaycastResult r2 = castRay(surface, origin.topRight(), direction, maxDist);
	RaycastResult r3 = castRay(surface, origin.bottomLeft(), direction, maxDist);
	RaycastResult r4 = castRay(surface, origin.bottomRight(), direction, maxDist);

	// There's probably a better way to do this
	if (r1.distance < r2.distance) {
		if (r3.distance < r4.distance) {
			if (r1.distance < r3.distance) {
				return r1;
			} else {
				return r3;
			}
		} else {
			if (r1.distance < r4.distance) {
				return r1;
			} else {
				return r4;
			}
		}
	} else {
		if (r3.distance < r4.distance) {
			if (r2.distance < r3.distance) {
				return r2;
			} else {
				return r3;
			}
		} else {
			if (r2.distance < r4.distance) {
				return r2;
			} else {
				return r4;
			}
		}
	}
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
