#include "Entity.h"

namespace Loopie {
	Entity::Entity(const std::string& name)
	{
		m_name = name;
	}

	Entity::~Entity()
	{
		for (Component* component : m_components)
		{
			delete component;
		}
		m_components.clear();

		m_childrenEntities.clear();
		m_parentEntity = nullptr;
	}

	void Entity::AddComponent(Component* component)
	{
		if (component)
		{
			m_components.push_back(component);
		}
	}

	void Entity::AddChild(Entity* child)
	{
		if (child && child != this)
		{
			if (child->m_parentEntity)
			{
				child->m_parentEntity->RemoveChild(child);
			}

			m_childrenEntities.push_back(child);
			child->m_parentEntity = this;
		}
	}

	void Entity::RemoveChild(Entity* child)
	{
		for (auto it = m_childrenEntities.begin(); it != m_childrenEntities.end(); ++it)
		{
			if (*it == child)
			{
				(*it)->m_parentEntity = nullptr;
				m_childrenEntities.erase(it);
				return;
			}
		}
	}

	void Entity::RemoveChild(UUID childUuid)
	{
		for (auto it = m_childrenEntities.begin(); it != m_childrenEntities.end(); ++it)
		{
			if ((*it)->GetUuid() == childUuid)
			{
				(*it)->m_parentEntity = nullptr;
				m_childrenEntities.erase(it);
				return;
			}
		}
	}

	UUID Entity::GetUuid() const
	{
		return m_uuid;
	}

	std::string Entity::GetName() const
	{
		return m_name;
	}

	bool Entity::GetIsActive() const
	{
		return m_isActive;
	}

	Entity* Entity::GetChild(UUID uuid) const
	{
		for (const auto& child : m_childrenEntities)
		{
			if (child->GetUuid() == uuid)
			{
				return child;
			}
		}
		return nullptr;
	}

	const std::vector<Entity*>& Entity::GetChildren() const
	{
		return m_childrenEntities;
	}

	void Entity::SetName(const std::string& name)
	{
		m_name = name;
	}

	void Entity::SetIsActive(bool active)
	{
		m_isActive = active;
	}

	void Entity::SetParent(Entity* parent)
	{
		if (m_parentEntity)
		{
			m_parentEntity->RemoveChild(this);
		}

		if (parent && parent != this)
		{
			parent->AddChild(this);
		}
	}
}