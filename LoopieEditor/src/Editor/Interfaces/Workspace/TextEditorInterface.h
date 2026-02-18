#pragma once

#include "Editor/Interfaces/Interface.h"
#include "Loopie/Events/IObserver.h"
#include "Loopie/Events/EventTypes.h"
#include "Editor/Events/EditorEventTypes.h"

#include <filesystem>
#include <string>

#define MAX_UNDO_BUFFER_SIZE 1000

class TextEditor;

namespace Loopie {
	class TextEditorInterface : public Interface, public IObserver<OnEntityOrFileNotification>, public IObserver<EngineNotification>
	{
	public:
		TextEditorInterface();
		~TextEditorInterface();
		void Init();
		void Render();
		void Update(const InputEventManager& inputEvent) override;

		void OpenFile(const std::string& filepath);
		void SaveFile();
		void SaveFileAs(const std::string& filepath);
		void NewFile();
		void CloseFile();

		bool IsVisible() const { return m_isVisible; }
		void SetVisible(bool visible) { m_isVisible = visible; }
		bool HasUnsavedChanges() const { return m_hasUnsavedChanges; }

		void SetLanguage(const std::string& extension);

	private:

		// Inherited via IObserver
		void OnNotify(const OnEntityOrFileNotification& id) override;
		void OnNotify(const EngineNotification& id) override;

		void UpdateWindowTitle();
		void HandleShortcuts(const InputEventManager& inputEvent);

	private:
		std::shared_ptr<TextEditor> m_editor;
		std::string m_currentFile;
		std::string m_currentFilename;
		bool m_isVisible = false;
		bool m_hasUnsavedChanges = false;
		bool m_shouldFocus = false;

	};
}