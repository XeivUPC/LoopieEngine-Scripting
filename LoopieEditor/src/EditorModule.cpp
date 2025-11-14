#include "EditorModule.h"

#include "Loopie/Core/Application.h"

//// Test
#include "Loopie/Core/Log.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Render/Gizmo.h"

#include "Loopie/Core/Math.h"

#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/TextureImporter.h"


#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Components/Transform.h"
///


#include <imgui.h>

namespace Loopie
{
	void EditorModule::OnLoad()
	{
		AssetRegistry::Initialize();
		Application::GetInstance().GetWindow().SetResizable(true);

		std::string defaultTeturePath = "assets/textures/simpleWhiteTexture.png";
		Metadata& meta = AssetRegistry::GetOrCreateMetadata(defaultTeturePath);
		TextureImporter::ImportImage(defaultTeturePath, meta);
		Renderer::SetDefaultTexture(ResourceManager::GetTexture(meta));

		/////SCENE
		Application::GetInstance().CreateScene(""); /// Maybe default One
		scene = &Application::GetInstance().GetScene();
		CreateBakerHouse();
		////

		m_assetsExplorer.Init();
		m_console.Init();
		m_hierarchy.Init();
		m_inspector.Init();
		m_scene.Init();
		m_mainMenu.Init();

		m_hierarchy.SetScene(scene);

	}

	void EditorModule::OnUnload()
	{
		AssetRegistry::Shutdown();
	}

	void EditorModule::OnUpdate(float dt)
	{
		Application& app = Application::GetInstance();
		InputEventManager& inputEvent = app.GetInputEvent();

		if (inputEvent.GetKeyWithModifier(SDL_SCANCODE_I, KeyModifier::CTRL)) {
			app.SetInterfaceState(!app.IsInterfaceVisible());
		}
		if (inputEvent.HasEvent(SDL_EVENT_WINDOW_FOCUS_GAINED)) {
			AssetRegistry::RefreshAssetRegistry();
		}

		m_hierarchy.Update(dt, inputEvent);
		m_assetsExplorer.Update(dt, inputEvent);
		m_scene.Update(dt, inputEvent);

		m_scene.StartScene();
		for (const auto& cam : Renderer::GetRendererCameras())
		{
			if (!cam->GetOwner()->GetIsActive() || !cam->GetIsActive())
				continue;

			cam->GetFramebuffer()->Bind();
			cam->GetFramebuffer()->Clear();
			Renderer::BeginScene(cam->GetViewMatrix(), cam->GetProjectionMatrix());

			/// FILTER OBJECTS TO RENDER

			for (auto& [uuid, entity] : scene->GetAllEntities()) {
				if (!entity->GetIsActive())
					continue;
				MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
				if (!renderer->GetIsActive())
					continue;
				Renderer::AddRenderItem(renderer->GetMesh()->GetVAO(), renderer->GetMaterial(), entity->GetTransform());
			}
			Renderer::EndScene();
			cam->GetFramebuffer()->Unbind();
		}
		m_scene.EndScene();
	}

	void EditorModule::OnInterfaceRender()
	{
		ImGui::DockSpaceOverViewport();

		m_mainMenu.Render();
		m_inspector.Render();
		m_console.Render();
		m_hierarchy.Render();
		m_assetsExplorer.Render();
		m_scene.Render();
	}

	void EditorModule::CreateBakerHouse()
	{
		m_scene.ChargeModel("assets/models/BakerHouse.fbx");
		m_scene.ChargeTexture("assets/textures/Baker_house.png");
	}	
}