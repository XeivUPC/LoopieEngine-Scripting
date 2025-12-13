#include "TopBarInterface.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include "Loopie/Resources/AssetRegistry.h"
#include "Loopie/Importers/TextureImporter.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Components/Component.h"
#include "Loopie/Core/Log.h"

namespace Loopie
{
    TopBarInterface::TopBarInterface()
    {
        std::vector<std::string> iconsToLoad = {
            "assets/icons/icon_play.png",
            "assets/icons/icon_pause.png",
            "assets/icons/icon_stop.png",
            "assets/icons/icon_nextFrame.png"
        };

        std::vector<Metadata> iconsToLoadMetadatas;
        for (size_t i = 0; i < iconsToLoad.size(); i++)
        {
            Metadata& meta = AssetRegistry::GetOrCreateMetadata(iconsToLoad[i]);
            TextureImporter::ImportImage(iconsToLoad[i], meta);
            iconsToLoadMetadatas.emplace_back(meta);
        }

        m_playIcon = ResourceManager::GetTexture(iconsToLoadMetadatas[0]);
        m_pauseIcon = ResourceManager::GetTexture(iconsToLoadMetadatas[1]);
        m_stopIcon = ResourceManager::GetTexture(iconsToLoadMetadatas[2]);
        m_nextFrameIcon = ResourceManager::GetTexture(iconsToLoadMetadatas[3]);
    }

    void TopBarInterface::Update(const InputEventManager& inputEvent)
    {
        if (m_actualMode == PAUSE)
            return;
        if (m_actualMode == STOP)
        {
            Application::GetInstance().GetScene().ReadAndLoadSceneFile("recoverScene.scene", false);
            m_actualMode = DEACTIVATED;
            return;
        }
        for (const auto& [uuid, entity] : Application::GetInstance().GetScene().GetAllEntities())
        {
            for (auto& component : entity->GetComponents())
            {
                component->Update();
            }
        }
        if (m_actualMode == NEXTFRAME)
            m_actualMode = PAUSE;
    }

    void TopBarInterface::Render()
    {
        ImGui::Begin("##", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        float avail = ImGui::GetContentRegionAvail().x;

        float space = (avail - 28 * 3) * 0.5f;

        ImGui::Dummy({ space, 0 });
        ImGui::SameLine();
        if (m_actualMode == DEACTIVATED)
        {
            if (ImGui::ImageButton("play", (ImTextureID)m_playIcon->GetRendererId(), ImVec2(20, 15)))
            {
                Application::GetInstance().GetScene().SaveScene("recoverScene.scene");
                m_actualMode = PLAY;
            }
        }
        else
        {
            if (ImGui::ImageButton("stop", (ImTextureID)m_stopIcon->GetRendererId(), ImVec2(20, 15)))
            {
                m_actualMode = STOP;
            }
        }
        ImGui::SameLine();
        if (ImGui::ImageButton("pause", (ImTextureID)m_pauseIcon->GetRendererId(), ImVec2(20, 15)))
        {
            m_actualMode = PAUSE;
        }            
        ImGui::SameLine();
        if (ImGui::ImageButton("nextFrame", (ImTextureID)m_nextFrameIcon->GetRendererId(), ImVec2(20, 15)))
        {
            m_actualMode = NEXTFRAME;
        }
        ImGui::SameLine();
        ImGui::Dummy({ space, 0 });
        ImGui::End();
    }
}