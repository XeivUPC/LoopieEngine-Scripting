#include "TextEditorInterface.h"

#include "Loopie/Resources/AssetRegistry.h"
#include "Loopie/Core/Log.h"
#include "Editor/Interfaces/Workspace/HierarchyInterface.h"
#include "Editor/Interfaces/Workspace/AssetsExplorerInterface.h"
#include "Editor/External/TextEditor.h"


#include <fstream>
#include <sstream>
#include <algorithm> // for std::max and std::min

#include <imgui.h>

namespace Loopie {
	TextEditorInterface::TextEditorInterface()
	{
		m_editor = std::make_shared<TextEditor>();
	}

	TextEditorInterface::~TextEditorInterface()
	{
		HierarchyInterface::s_OnEntitySelected.RemoveObserver<OnEntityOrFileNotification>(this);
		AssetsExplorerInterface::s_OnFileSelected.RemoveObserver<OnEntityOrFileNotification>(this);

        Application::GetInstance().m_notifier.RemoveObserver<EngineNotification>(this);
	}

	void TextEditorInterface::Init() 
	{
		HierarchyInterface::s_OnEntitySelected.AddObserver<OnEntityOrFileNotification>(this);
		AssetsExplorerInterface::s_OnFileSelected.AddObserver<OnEntityOrFileNotification>(this);

        Application::GetInstance().m_notifier.AddObserver<EngineNotification>(this);

		m_editor->SetShowWhitespaces(false);
		m_editor->SetTabSize(4);

		auto palette = ::TextEditor::GetDarkPalette();
		m_editor->SetPalette(palette);
	}

    void TextEditorInterface::Render()
    {
        if (!m_isVisible)
            return;

        std::string title = "Text Editor";

		std::string displayFilename = m_currentFilename.empty() ? "Untitled" : m_currentFilename;
        if (m_hasUnsavedChanges)
            displayFilename += "*";

        if (m_shouldFocus)
        {
            ImGui::SetNextWindowFocus();
            m_shouldFocus = false;
        }
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

        if (ImGui::Begin(title.c_str(), nullptr, windowFlags))

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, !m_currentFile.empty()))
                    SaveFile();
                ImGui::Separator();
                if (ImGui::MenuItem("Close"))
                    CloseFile();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                bool hasUndo = m_editor->CanUndo();
                bool hasRedo = m_editor->CanRedo();
                bool hasSelection = m_editor->HasSelection();

                if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, hasUndo))
                    m_editor->Undo();
                if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, hasRedo))
                    m_editor->Redo();
                ImGui::Separator();
                if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, hasSelection))
                    m_editor->Copy();
                if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, hasSelection))
                    m_editor->Cut();
                if (ImGui::MenuItem("Paste", "Ctrl+V"))
                    m_editor->Paste();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::Text("File: %s", displayFilename.c_str());
        m_editor->Render("##TextEditorContent");

        if (m_editor->IsTextChanged())
            m_hasUnsavedChanges = true;

        ImGui::End();
    }

    void TextEditorInterface::Update(const InputEventManager& inputEvent)
    {
        if (!m_isVisible) {
            if (inputEvent.IsReadingInputText())
                inputEvent.StopReadingInputText();
            return;
        }

        HandleShortcuts(inputEvent);

        if (!inputEvent.IsReadingInputText())
            inputEvent.StartReadingInputText();
    }

    void TextEditorInterface::OpenFile(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            Log::Error("Failed to open file: {0}", filepath);
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        m_editor->SetText(buffer.str());
        m_currentFile = filepath;

        size_t lastSlash = filepath.find_last_of("/\\");
        m_currentFilename = (lastSlash != std::string::npos)
            ? filepath.substr(lastSlash + 1)
            : filepath;

        size_t lastDot = m_currentFilename.find_last_of('.');
        if (lastDot != std::string::npos)
        {
            std::string extension = m_currentFilename.substr(lastDot);
            SetLanguage(extension);
        }

        m_hasUnsavedChanges = false;
        m_isVisible = true;
        m_shouldFocus = true;
    }

    void TextEditorInterface::SaveFile()
    {
        if (m_currentFile.empty())
        {
            return;
        }

        std::string content = m_editor->GetText();
        {
            std::ofstream file(m_currentFile);
            if (!file.is_open())
            {
                Log::Error("Failed to save file: {0}", m_currentFile);
                return;
            }
            file << content;
            file.close();
        }

        AssetRegistry::RefreshAssetRegistry();

        m_hasUnsavedChanges = false;
    }

    void TextEditorInterface::CloseFile()
    {
        m_hasUnsavedChanges = false;
        m_isVisible = false;
    }

	void TextEditorInterface::SetLanguage(const std::string& extension)
	{
		if (extension == ".cpp" || extension == ".h")
			m_editor->SetLanguageDefinition(::TextEditor::LanguageDefinition::CPlusPlus());
		else if (extension == ".cs")
			m_editor->SetLanguageDefinition(::TextEditor::LanguageDefinition::CSharp());
	}

	void TextEditorInterface::UpdateWindowTitle()
	{
	}

	void TextEditorInterface::HandleShortcuts(const InputEventManager& inputEvent)
	{
		if (inputEvent.GetKeyWithModifier(SDL_SCANCODE_S, KeyModifier::CTRL))
			SaveFile();
	}

	// Inherited via IObserver
	void TextEditorInterface::OnNotify(const OnEntityOrFileNotification& id)
	{
		if (id == OnEntityOrFileNotification::OnFileSelect) 
		{
			std::string extension = AssetsExplorerInterface::s_SelectedFile.extension().string();
			if (extension == ".cs" || extension == ".txt")
			{
				std::string filePath = AssetsExplorerInterface::s_SelectedFile.string();
			    SetLanguage(extension);     
				OpenFile(filePath);
			}
		}
	}
    void TextEditorInterface::OnNotify(const EngineNotification& id)
    {
        if (id == EngineNotification::OnAssetRegistryReload) {
            if (!std::filesystem::exists(m_currentFile)) {
                CloseFile();
            }
        }
    }
}