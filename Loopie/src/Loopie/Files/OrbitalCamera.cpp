#include "OrbitalCamera.h"
#include "Transform.h"

namespace Loopie
{
	OrbitalCamera::OrbitalCamera()
	{
		m_entity = std::make_shared<Entity>("OrbitalCamera");
		m_entity->AddComponent<Transform>();
		m_camera = m_entity->AddComponent<Camera>();
	}

	OrbitalCamera::~OrbitalCamera()
	{
	}
	void OrbitalCamera::ProcessEvent(InputEventManager& inputEvent)
	{
		m_complexMovement = true;
		if (inputEvent.GetMouseButtonStatus(1) == KeyState::REPEAT)
		{
			m_complexMovement = false;
			m_inputDirection = vec3(inputEvent.GetMouseDelta().x, inputEvent.GetMouseDelta().y, 0);
			m_inputDirection = normalize(m_inputDirection);
		}
		if (inputEvent.GetMouseButtonStatus(2) == KeyState::REPEAT)
		{
			if (inputEvent.GetKeyStatus(SDL_SCANCODE_LSHIFT) == KeyState::REPEAT)
				m_speedMultiplier = 2.0f;
			else
				m_speedMultiplier = 2.0f;
		
			if (inputEvent.GetKeyStatus(SDL_SCANCODE_W) == KeyState::REPEAT)
				m_inputDirection.z += 1;
			if(inputEvent.GetKeyStatus(SDL_SCANCODE_A) == KeyState::REPEAT)
				m_inputDirection.x -= 1;
			if(inputEvent.GetKeyStatus(SDL_SCANCODE_S) == KeyState::REPEAT)
				m_inputDirection.z -= 1;
			if(inputEvent.GetKeyStatus(SDL_SCANCODE_D) == KeyState::REPEAT)
				m_inputDirection.x += 1;
		}
	}
	void OrbitalCamera::Update(float dt)
	{
		std::shared_ptr<Transform>& transform = m_entity->GetTransform();
		if (!m_complexMovement) {
			transform->Translate

		}
	}
}