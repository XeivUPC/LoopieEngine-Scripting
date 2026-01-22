#include <string>
#include <vector>

namespace Loopie
{
	class TextEditor
	{
	public:
		// File Operations
		void LoadFile(const std::string& filepath);
		void SaveFile();
		void SaveFileAs(const std::string& filepath);
		bool HasFileLoaded() const;
		std::string GetCurrentFilePath() const;
		bool IsModified() const;

		// Text Content Management
		void Undo();
		void Redo();
		bool CanUndo() const;
		bool CanRedo() const;

		// Rendering
		// void Render(const char* title, const ImVec2& size = ImVec2(), bool border = false);

		// Cursor & Selection
		struct LineIndex
		{
			int line;
			int column;
		};

		LineIndex GetCursorPosition() const;
		void SetCursorPosition(const LineIndex& position);
		bool HasSelection() const;
		std::string GetSelectedText() const;

		// State
		void SetReadOnly(bool value);
		bool IsReadOnly() const;
		void SetTabSize(int spaces); // Should be 4 by default

	private:
		// Input Handling (there might already be something within the event processing)
		void HandleKeyboardInputs();
		void HandleMouseInputs();
		void ProcessShortcuts();

	private:
		std::vector<std::string> m_lines;
		std::string m_currentFilePath;
		bool m_modified;
		bool m_readOnly;
		int m_tabSize;

		Coordinates m_cursorPosition;
		Coordinates m_selectionStart;
		Coordinates m_selectionEnd;

		// Undo/Redo
		std::vector<UndoRecord> m_undoBuffer;
		int m_undoIndex;

		// Scrolling
		float m_scrollX, m_scrollY;

	};
}