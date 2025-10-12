#include "Scene.h"

namespace Loopie {
	Scene::~Scene()
	{
		for (auto& [uuid, entity] : m_entities)
		{
			delete entity;
		}
		m_entities.clear();
	}

	Entity* Scene::CreateEntity(const std::string& name)
	{
		Entity* entity = new Entity(name);
		m_entities[entity->GetUuid()] = entity;
		return entity;
	}

	void Scene::RemoveEntity(UUID uuid)
	{
		Entity* entity = GetEntity(uuid);
		if (entity)
		{
			delete entity;
		}
		m_entities.erase(uuid);
	}

	Entity* Scene::GetEntity(UUID uuid) const
	{
		auto it = m_entities.find(uuid);
		return (it != m_entities.end()) ? it->second : nullptr;
	}

	Entity* Scene::GetEntity(const std::string& name) const
	{
		for (const auto& [uuid, entity] : m_entities)
		{
			if (entity->GetName() == name)
			{
				return entity;
			}
		}
		return nullptr;
	}

	const std::unordered_map<UUID, Entity*>& Scene::GetAllEntities() const
	{
		return m_entities;
	}
}
