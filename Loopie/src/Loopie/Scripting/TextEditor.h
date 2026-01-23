#pragma once

#include <string>
#include <vector>

#define MAX_UNDO_BUFFER_SIZE = 1000
/* PSS: How the text editor should be implemented
Engine: funciones y structs para manipulacion de texto.

Editor: que se encarga de los inputs del mouse y teclado para poder usar las funciones de engine.

UI: renderizado de la interfaz del editor.

Core: temas de transformación de carácteres y asserts
*/

namespace Loopie {
	class TextEditor
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

			bool operator!=(const LineIndex& o) const 
			{
				return (line != otherLine.line || column != otherLine.column);
			}
		};

		struct UndoRecord 
		{
			std::vector<std::string> lines;
			LineIndex cursorPosition;
		};

		TextEditor();
		~TextEditor();

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

		// Rendering -> IDK if this will have to be called from here
		// void Render(const char* title, const ImVec2& size = ImVec2(), bool border = false);

		// Cursor & Selection
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


		LineIndex SanitizeLineIndex(const LineIndex& lineIndex) const;


	private:
		std::vector<std::string> m_lines;
		std::string m_currentFilePath;
		bool m_isModified;
		bool m_readOnly;
		int m_tabSize;

		LineIndex m_cursorPosition;
		LineIndex m_selectionStart;
		LineIndex m_selectionEnd;
		bool m_hasCursorPositionChanged = false;

		// Undo/Redo
		std::vector<UndoRecord> m_undoBuffer;
		int m_undoIndex;

		// Scrolling
		float m_scrollX, m_scrollY;

	};
}