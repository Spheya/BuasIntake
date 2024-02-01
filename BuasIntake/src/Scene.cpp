#include "Scene.hpp"

#include <numeric>
#include <cassert>

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

void Scene::updateEntities(float deltatime) {
	for (auto& entity : m_entities)
		entity->update(deltatime);

	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
		[](const std::shared_ptr<Entity>& entity) { return entity->isFlaggedforDeletion(); }
	), m_entities.end());
}

void Scene::draw(tmpl8::Surface* surface) const {
	for (auto& entity : m_entities)
		entity->draw(surface);
}
