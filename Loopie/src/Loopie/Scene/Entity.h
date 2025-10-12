#pragma once

#include "Loopie/Core/UUID.h"
#include "Loopie/Components/Component.h"

#include <string>
#include <vector>
#include <memory>

namespace Loopie
{
	class Entity
	{
	public:
		Entity(const std::string& name);
		~Entity();

		void AddComponent(Component* component); 
		template<typename T>
		std::shared_ptr<T> GetComponent() const
		{
			for (Component* comp : m_components)
			{
				// Try to cast to the requested type
				T* casted = dynamic_cast<T*>(comp);
				if (casted)
				{
					// Wrap in shared_ptr without taking ownership
					return std::shared_ptr<T>(std::shared_ptr<Component>(), casted);
				}
			}
			return nullptr;
		}
		template<typename T>
		bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}
		template<typename T>
		void RemoveComponent()
		{
			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				T* casted = dynamic_cast<T*>(*it);
				if (casted)
				{
					delete* it;
					m_components.erase(it);
					return;  // Remove first match only
				}
			}
		}

		// If a child is set up, then it means this is its parent and will update it accordingly
		void AddChild(Entity* child);
		void RemoveChild(Entity* child);
		void RemoveChild(UUID childUuid);

		UUID GetUuid() const;
		std::string GetName() const;
		bool GetIsActive() const;
		Entity* GetChild(UUID uuid) const;
		const std::vector<Entity*>& GetChildren() const;
		Entity* GetParent() const { return m_parentEntity; }

		void SetName(const std::string& name);
		void SetIsActive(bool active);
		// If a parent is set up, then it means this is its child and will update it accordingly
		void SetParent(Entity* parent);

	private:
		Entity* m_parentEntity = nullptr;
		std::vector<Entity*> m_childrenEntities;
		std::vector<Component*> m_components;

		UUID m_uuid;
		std::string m_name;
		bool m_isActive = true;
	};
}