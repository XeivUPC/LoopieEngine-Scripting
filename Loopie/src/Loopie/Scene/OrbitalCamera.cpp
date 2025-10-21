#include "OrbitalCamera.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/Log.h"

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
        m_inputDirection = vec3(0);
        m_inputRotation = vec3(0);

        vec2 mouseScroll = inputEvent.GetScrollDelta();
        vec2 mouseDelta = inputEvent.GetMouseDelta();

        if (inputEvent.GetMouseButtonStatus(1) == KeyState::REPEAT)
        {
            m_inputDirection = vec3(-mouseDelta.x, mouseDelta.y, 0);
        }
        if (inputEvent.GetMouseButtonStatus(2) == KeyState::REPEAT)
        {
            m_inputRotation = vec3(-mouseDelta.x, -mouseDelta.y, 0);
        }

        if (inputEvent.GetKeyStatus(SDL_SCANCODE_LSHIFT) == KeyState::REPEAT)
            m_speedMultiplier = 2.0f;
        else
            m_speedMultiplier = 1.0f;

        if (inputEvent.GetKeyStatus(SDL_SCANCODE_W) == KeyState::REPEAT) m_inputDirection.z -= m_cameraMovementSpeed;
        if (inputEvent.GetKeyStatus(SDL_SCANCODE_S) == KeyState::REPEAT) m_inputDirection.z += m_cameraMovementSpeed;
        if (inputEvent.GetKeyStatus(SDL_SCANCODE_A) == KeyState::REPEAT) m_inputDirection.x -= m_cameraMovementSpeed;
        if (inputEvent.GetKeyStatus(SDL_SCANCODE_D) == KeyState::REPEAT) m_inputDirection.x += m_cameraMovementSpeed;

        m_inputDirection *= m_speedMultiplier;

        if(mouseScroll.y!=0)
            m_inputDirection.z = -mouseScroll.y * m_cameraZoomSpeed;

        m_inputRotation *= m_cameraRotationSpeed;
    }

    void OrbitalCamera::Update(float dt)
    {
        Transform* transform = m_entity->GetTransform();

        m_yaw += -m_inputRotation.x * dt;
        m_pitch += -m_inputRotation.y * dt;

        glm::quat yawRotation = glm::normalize(glm::angleAxis(m_yaw, vec3(0, 1, 0)));
        glm::quat pitchRotation = glm::normalize(glm::angleAxis(m_pitch, vec3(1, 0, 0)));

        transform->QuaternionSetRotation(glm::normalize(yawRotation * pitchRotation));

        if (glm::length(m_inputDirection) > 0.001f)
            transform->Translate(m_inputDirection * dt);
    }


}