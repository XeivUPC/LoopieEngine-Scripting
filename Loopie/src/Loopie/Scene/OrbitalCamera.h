#pragma once
#include "Loopie/Components/Camera.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Core/Application.h"
namespace Loopie
{
	class OrbitalCamera
	{
	public:
		OrbitalCamera();
		~OrbitalCamera();
		void ProcessEvent(InputEventManager& inputEvent);
		void Update(float dt);

		Camera* GetCamera() { return m_camera; }
	private:
		float m_cameraMovementSpeed = 5.f;
		float m_cameraRotationSpeed = 0.1f;
		float m_cameraZoomSpeed = 50.f;

		float m_speedMultiplier = 2.f;

		float m_yaw = 0;
		float m_pitch = 0;

		vec3 m_inputDirection = vec3(0, 0, 0);
		vec3 m_inputRotation = vec3(0, 0, 0);

		bool m_complexMovement = false;

		std::shared_ptr<Entity>m_entity;
		Camera* m_camera;
	};
}