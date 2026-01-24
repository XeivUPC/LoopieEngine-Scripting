#pragma once

#include "Editor/Interfaces/Interface.h"
#include "Loopie/Events/IObserver.h"
#include "Editor/Events/EditorEventTypes.h"

#define MAX_UNDO_BUFFER_SIZE 1000

// TODO PSS 24/01/25: Implement command pattern.
// Right now a snapshot is being done with each AddUndo().
// This uses a lot of memory. Command pattern instead focuses
// on storing which actions were done at which moment. 
// This should heavily reduce memory stored.

namespace Loopie {
	class TextEditorInterface : public Interface, public IObserver<OnEntityOrFileNotification>
	{
	public:
		struct LineIndex
		{
			int line = 0;
			int column = 0;

			bool operator==(const LineIndex& otherLine) const 
			{
				return (line == otherLine.line && column == otherLine.column);
			}

			bool operator!=(const LineIndex& otherLine) const
			{
				return (line != otherLine.line || column != otherLine.column);
			}
		};

		struct UndoRecord 
		{
			std::vector<std::string> lines;
			LineIndex cursorPosition;
		};

		TextEditorInterface();
		~TextEditorInterface();
		void Init() override;
		void Render() override;

		// File Operations
		bool LoadFile(const std::string& filepath);
		bool SaveFile();
		bool SaveFileAs(const std::string& filepath);
		bool HasFileLoaded() const;
		std::string GetCurrentFilePath() const;
		bool IsModified() const;

		// Text content manager
		void SetText(const std::string& text);
		std::string GetText() const;
		void Clear();
		std::vector<std::string> GetLines() const;
		void SetLines(const std::vector<std::string>& lines);

		// Undo/Redo
		void AddUndo();
		void Undo();
		void Redo();
		bool CanUndo() const;
		bool CanRedo() const;

		// Cursor & Selection
		LineIndex GetCursorPosition() const;
		void SetCursorPosition(const LineIndex& position);
		bool HasSelection() const;
		std::string GetSelectedText() const;
		void DeleteSelection();

		// Text modification
		void InsertChar(char c);
		void Backspace();
		void Delete();
		void EnterChar(char c);

		// State
		void SetReadOnly(bool value);
		bool IsReadOnly() const;
		void SetTabSize(int spaces); // Should be 4 by default

	private:
		// Input Handling
		void HandleKeyboardInputs();
		void HandleMouseInputs();

		// Helper functions
		LineIndex SanitizeLineIndex(const LineIndex& lineIndex) const;
		void GetOrderedSelection(LineIndex& start, LineIndex& end) const;

		// Inherited via IObserver
		void OnNotify(const OnEntityOrFileNotification& id) override;

	private:
		std::vector<std::string> m_lines;
		std::string m_currentFilePath;
		bool m_isModified = false;
		bool m_readOnly = false;
		int m_tabSize = 4;

		LineIndex m_cursorPosition;
		LineIndex m_selectionStart;
		LineIndex m_selectionEnd;
		

		std::vector<UndoRecord> m_undoBuffer;
		int m_undoIndex = 0;

		float m_scrollX = 0.0f;
		float m_scrollY = 0.0f;
		bool m_hasCursorPositionChanged = false;

	};
}