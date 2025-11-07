#pragma once 

#include "src/Interfaces/Interface.h"
#include "Loopie/Render/FrameBuffer.h"
#include "src/Others/OrbitalCamera.h"

namespace Loopie {
	class SceneInterface : public Interface {
	public:
		SceneInterface();
		~SceneInterface() = default;
		void Init() override {}
		void Update(float dt, const InputEventManager& inputEvent) override;
		void Render() override;

		void StartScene();
		void EndScene();

   		Camera* GetCamera() { return m_camera->GetCamera(); }

		void ChargeModel(const std::string& modelPath);
		void ChargeTexture(const std::string& texturePath);

	private:
		void Drop();



	private:
		std::shared_ptr<FrameBuffer> m_buffer;
		std::shared_ptr<OrbitalCamera> m_camera;

		bool m_interacted = false;
		ImVec2 m_windowSize;
	};
}