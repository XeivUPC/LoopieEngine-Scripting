#pragma once
#include <memory>

#include "Loopie/Core/Identificable.h"
#include "Loopie/Core/UUID.h"

namespace Loopie {
	class Entity;
	class Transform;

	class Component : public Identificable
	{
		friend class Entity;
	public:
		Component() = default;
		virtual ~Component();

		// Getters
		Transform* GetTransform() const;
		std::shared_ptr<Entity> GetOwner() const { return m_owner.lock(); }
		const UUID& GetUUID() const;
		bool GetIsActive() const;

		// Setters
		void SetIsActive(bool active);

		virtual void Init() = 0;

	private:
		std::weak_ptr<Entity> m_owner;
		UUID m_uuid;
		bool m_isActive = true;
	};
}