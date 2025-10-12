#pragma once

#include "Loopie/Core/UUID.h"
#include "Loopie/Scene/Entity.h"

#include <string>
#include <unordered_map>

namespace Loopie {
	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		Entity* CreateEntity(const std::string& name = "Entity");
		void RemoveEntity(UUID uuid);

		Entity* GetEntity(UUID uuid) const;
		Entity* GetEntity(const std::string& name) const;
		// Returns the unordered_map of the UUID and Entity ptrs
		// Usage: for (const auto& [uuid, entity] : scene.GetAllEntities()) {entity->Update();}
		const std::unordered_map<UUID, Entity*>& GetAllEntities() const;

	private:
		std::unordered_map<UUID, Entity*> m_entities;
	};
}