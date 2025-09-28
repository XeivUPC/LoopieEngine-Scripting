#include "ProjectSetupInterface.h"

#include "Loopie/Files/FileDialog.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Core/Window.h"

#include <imgui.h>
namespace Loopie
{
	ProjectSetupInterface::ProjectSetupInterface()
	{

	}


	void ProjectSetupInterface::Render()
	{
		ImGui::Begin("Create Project", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::InputText("Project Name", m_projectName, IM_ARRAYSIZE(m_projectName));

		ImGui::LabelText("Path", m_projectPath.string().c_str());
		ImGui::SameLine();
		if (ImGui::Button("##", {20,20}))
		{
			DialogResult result = FileDialog::SelectFolder();
			if (result.Status == DialogResultType::SUCCESS)
			{
				m_projectPath = result.Paths[0];
			}
		}
		if (ImGui::Button("Create Project", { 150,20 }))
		{
			if(Application::GetInstance().m_activeProject.Create(m_projectPath, m_projectName)){
				Log::Info("Project Created");
			}
			else {
				Log::Info("Project Couldn't be Created");
			}
		}
		ImGui::End();

		ImGui::Begin("Load Project", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::LabelText("Path", m_projectPath.string().c_str());
		ImGui::SameLine();
		if (ImGui::Button("##", { 20,20 }))
		{
			DialogResult result = FileDialog::SelectFolder();
			if (result.Status == DialogResultType::SUCCESS)
			{
				m_projectPath = result.Paths[0];
			}
		}
		if (ImGui::Button("Load Project", { 150,20 }))
		{
			if (Application::GetInstance().m_activeProject.Open(m_projectPath)) {
				Log::Info("Project Selected");
			}
			else {
				Log::Info("Project Couldn't be Selected");
			}
		}
		ImGui::End();
	}
}