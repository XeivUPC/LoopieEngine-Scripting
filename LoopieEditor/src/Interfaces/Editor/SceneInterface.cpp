#include "SceneInterface.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Components/Transform.h"

#include "Loopie/Resources/AssetRegistry.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/MeshImporter.h"
#include "Loopie/Importers/TextureImporter.h"
#include "Loopie/Components/MeshRenderer.h"

#include "src/Interfaces/Editor/HierarchyInterface.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Loopie {
	SceneInterface::SceneInterface() {
		m_buffer = std::make_shared<FrameBuffer>(1, 1);
		m_camera = std::make_shared<OrbitalCamera>();
		m_camera->GetCamera()->GetTransform()->SetPosition({ 0,10,50.f });
	}

	void SceneInterface::Update(float dt, const InputEventManager& inputEvent)
	{
		
		if (!m_focused && !m_interacted)
			return;
		m_camera->ProcessEvent(inputEvent);
		m_camera->Update(dt);

	}

	void SceneInterface::Render() {

		const bool gizmoActive = ImGuizmo::IsOver() || ImGuizmo::IsUsing();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		if (gizmoActive)
		{
			flags |= ImGuiWindowFlags_NoMove;
		}

		if (ImGui::Begin("Scene",nullptr, flags)) {
			m_windowSize =  ImGui::GetContentRegionAvail();
			m_focused = ImGui::IsWindowHovered();
			ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();

			if (!m_focused && m_interacted || m_focused)
				m_interacted = ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right) || ImGui::IsMouseDown(ImGuiMouseButton_Middle);
			else 
				m_interacted = false;

			ImGui::Image((ImTextureID)m_buffer->GetTextureId(), m_windowSize, ImVec2(0,1), ImVec2(1,0));

			if (HierarchyInterface::s_SelectedEntity) {
				std::shared_ptr<Entity> selectedEntity = HierarchyInterface::s_SelectedEntity;

				Renderer::DisableDepth();
				ImGuizmo::SetRect(cursorScreenPos.x, cursorScreenPos.y, m_windowSize.x, m_windowSize.y);
				ImGuizmo::SetDrawlist();
				ImGuizmo::Manipulate(m_camera->GetCamera()->GetViewMatrixPtr(), m_camera->GetCamera()->GetProjectionMatrixPtr(), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, selectedEntity->GetTransform()->GetWorldToLocalMatrixPtr());
				Renderer::EnableDepth();
			}

		}

		Drop();
		ImGui::End();
	}

	void SceneInterface::StartScene()
	{
		m_buffer->Bind();

		ImVec2 textureSize = ImVec2((float)m_buffer->GetWidth(), (float)m_buffer->GetHeight());
		if (m_windowSize.x != textureSize.x || m_windowSize.y != textureSize.y) {
			Renderer::SetViewport(0, 0, m_windowSize.x, m_windowSize.y);
			m_camera->GetCamera()->SetViewport(0, 0, m_windowSize.x, m_windowSize.y);
			m_buffer->Resize(m_windowSize.x, m_windowSize.y);
		}

		m_buffer->Clear();	
	}

	void SceneInterface::EndScene()
	{
		m_buffer->Unbind();
	}

	void SceneInterface::Drop()
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_EXPLORER_FILE"))
			{
				std::string path = (const char*)payload->Data;
				if (TextureImporter::CheckIfIsImage(path.c_str())) {
					ChargeTexture(path);
				}
				else if (MeshImporter::CheckIfIsModel(path.c_str())) {
					ChargeModel(path);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	void SceneInterface::ChargeModel(const std::string& modelPath)
	{
		Metadata& meta = AssetRegistry::GetOrCreateMetadata(modelPath);
		MeshImporter::ImportModel(modelPath, meta);
		std::shared_ptr<Entity> parent;

		for (size_t i = 0; i < meta.CachesPath.size(); i++)
		{
			
			std::shared_ptr<Mesh> mesh = ResourceManager::GetMesh(meta, i);;

			if (mesh) {
				std::shared_ptr<Entity> newEntity;
				if (!parent) {
					newEntity = Application::GetInstance().GetScene().CreateEntity("ModelEntity",HierarchyInterface::s_SelectedEntity);
					parent = newEntity;
				}else
					newEntity = Application::GetInstance().GetScene().CreateEntity("ModelEntity", parent);
				MeshRenderer* renderer = newEntity->AddComponent<MeshRenderer>();
				renderer->SetMesh(mesh);
			}
		}
	}
	void SceneInterface::ChargeTexture(const std::string& texturePath)
	{
		Metadata& meta = AssetRegistry::GetOrCreateMetadata(texturePath);

		TextureImporter::ImportImage(texturePath, meta);
		std::shared_ptr<Texture> texture = ResourceManager::GetTexture(meta);
		if (texture) {
			if (HierarchyInterface::s_SelectedEntity != nullptr) {
				MeshRenderer* renderer = HierarchyInterface::s_SelectedEntity->GetComponent<MeshRenderer>();
				if (renderer) {
					renderer->GetMaterial()->SetTexture(texture);
				}
			}
			else {
				for (const auto& [uuid, entity] : Application::GetInstance().GetScene().GetAllEntities())
				{
					MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
					if (renderer) {
						renderer->GetMaterial()->SetTexture(texture);
					}
				}
			}
		}
	}
}