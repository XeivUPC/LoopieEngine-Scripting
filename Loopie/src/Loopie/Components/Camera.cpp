#include "Camera.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Math/MathUtils.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Render/Renderer.h"


namespace Loopie
{
	Camera* Camera::s_Main = nullptr;

	Camera::Camera(float fov, float near_plane, float far_plane, bool canBeMainCamera) : m_fov(fov), m_nearPlane(near_plane), m_farPlane(far_plane), m_canBeMainCamera(canBeMainCamera)
	{
		m_renderTarget = nullptr;
		Renderer::RegisterCamera(*this);

		if (!s_Main && CanBeMainCamera())
			SetAsMainCamera();
	}

	Camera::~Camera() {
		Renderer::UnregisterCamera(*this);

		if (GetOwner() && GetTransform())
			GetTransform()->m_transformNotifier.RemoveObserver(this);

		if (s_Main == this)
		{
			s_Main = nullptr;
			/// FindNewCamera
			const std::vector<Camera*>& cameras = Renderer::GetRendererCameras();
			for (const auto camera : cameras) {
				if (camera->CanBeMainCamera())
				{
					SetMainCamera(camera);
					return;
				}
			}
		}
	}

	void Camera::Init()
	{
		CalculateMatrices();

		GetTransform()->m_transformNotifier.AddObserver(this);
	}

	void Camera::OnNotify(const TransformNotification& id)
	{
		if (id == TransformNotification::OnDirty)
			SetDirty();
	}

	void Camera::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		m_viewport = vec4(x, y, width, height);
		SetDirty();
	}

	const matrix4& Camera::GetViewMatrix() const
	{
		CalculateMatrices();
		return m_viewMatrix;
	}

	const matrix4& Camera::GetProjectionMatrix() const
	{
		CalculateMatrices();
		return m_projectionMatrix;
	}

	const matrix4& Camera::GetViewProjectionMatrix() const
	{
		CalculateMatrices();
		return m_viewProjectionMatrix;
	}

	void Camera::SetFov(float fov)
	{
		m_fov = fov;
		SetDirty();
	}

	float Camera::GetFov() const
	{
		return m_fov;
	}

	void Camera::SetNearPlane(float nearPlane)
	{
		m_nearPlane = nearPlane;
		SetDirty();
	}

	float Camera::GetNearPlane() const
	{
		return m_nearPlane;
	}

	void Camera::SetFarPlane(float farPlane)
	{
		m_farPlane = farPlane;
		SetDirty();
	}

	float Camera::GetFarPlane() const
	{
		return m_farPlane;
	}

	const Frustum& Camera::GetFrustum() const {
		CalculateMatrices();
		return m_frustum;
	}


	void Camera::CalculateMatrices() const
	{
		if (!m_dirty)
			return;

		auto transform = GetTransform();

		const vec3 position = transform->GetPosition();
		const vec3 forward = -transform->Forward();
		const vec3 up = transform->Up();

		m_viewMatrix = glm::lookAt(position, position + forward, up);
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_viewport.z / m_viewport.w, m_nearPlane, m_farPlane);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

		m_frustum.FromMatrix(m_viewProjectionMatrix);
		m_dirty = false;
	}

	void Camera::SetDirty() const {
		m_dirty = true;
	}

	bool Camera::SetMainCamera(Camera* camera) {
		if (!camera) {
			if (s_Main)
				s_Main->m_isMainCamera = false;
			s_Main = nullptr;
			return true;
		}
		else if (camera->m_canBeMainCamera) {
			if (s_Main)
				s_Main->m_isMainCamera = false;
			s_Main = camera;
			s_Main->m_isMainCamera = true;
			camera->SetDirty();
			return true;
		}
		else
			Log::Warn("Camera cannot be MainCamera");
		return false;
	}

	bool Camera::SetAsMainCamera() {
		return SetMainCamera(this);
	}

	json Camera::Serialize() const
	{
		json cameraObj = json::object();
		cameraObj["is_main_camera"] = m_isMainCamera;
		cameraObj["fov"] = m_fov;
		cameraObj["near_plane"] = m_nearPlane;
		cameraObj["far_plane"] = m_farPlane;

		json componentWrapper = json::object();
		componentWrapper["camera"] = cameraObj;

		return componentWrapper;
	}

	void Camera::Deserialize(const json& data)
	{
		m_fov = data.value("fov", 60.0f);
		m_nearPlane = data.value("near_plane", 0.1f);
		m_farPlane = data.value("far_plane", 100.0f);
		m_isMainCamera = data.value("is_main_camera", false);
			
		if (m_isMainCamera)
		{
			SetAsMainCamera();
		}
	}
}