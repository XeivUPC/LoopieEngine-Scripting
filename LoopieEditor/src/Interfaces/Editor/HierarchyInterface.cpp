#include "HierarchyInterface.h"
#include "Loopie/Core/Log.h"

#include <imgui.h>

namespace Loopie {
	std::shared_ptr<Entity> HierarchyInterface::s_SelectedEntity = nullptr;

	HierarchyInterface::HierarchyInterface() {
		
	}

	void HierarchyInterface::Render() {

		if (ImGui::Begin("Hierarchy")) {
			if (!m_scene) {
				ImGui::End();
				return;
			}

			for(const auto& entityData : m_scene->GetAllEntities())
			{
				const std::shared_ptr<Entity>& entity = entityData.second;
				//// Maybe scene must store only RootEntities??????
				if (entity->GetParent().lock() != nullptr)
					continue;
				DrawEntitySlot(entity);
			}

		}
		ImGui::End();
	}

	void HierarchyInterface::SetScene(Scene* scene)
	{
		m_scene = scene;
	}

	void HierarchyInterface::DrawEntitySlot(const std::shared_ptr<Entity>& entity)
	{

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		const auto& children = entity->GetChildren();
		bool hasChildren = !children.empty();

		if (!hasChildren)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (s_SelectedEntity == entity)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.get(), flags, entity->GetName().c_str());

		if (ImGui::IsItemClicked())
		{
			////Expand To Select Multiple
			s_SelectedEntity = entity;
		}

		if (opened && hasChildren)
		{
			for (const auto& child : children)
			{
				DrawEntitySlot(child);
			}
			ImGui::TreePop();
		}
	}


}