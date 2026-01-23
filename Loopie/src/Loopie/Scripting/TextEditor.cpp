#include "TextEditor.h"
#include <fstream>
#include <sstream>

namespace Loopie {
	TextEditor::TextEditor()
	{
		m_lines.push_back("");
		m_cursorPosition = { 0, 0 };
		m_selectionStart = { 0, 0 };
		m_selectionEnd = { 0, 0 };
	}

	TextEditor::~TextEditor()
	{

	}


	// File Operations
	bool TextEditor::LoadFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
		{
			return false;
		}

		m_lines.clear();
		std::string textLine;
		while (std::getline(file, textLine))
		{
			m_lines.push_back(textLine);
		}

		if (m_lines.empty())
		{
			m_lines.push_back("");
		}

		m_currentFilePath = filepath;
		m_isModified = false;
		m_cursorPosition = { 0,0 };
		m_selectionStart = { 0, 0 };
		m_selectionEnd = { 0, 0 };
		m_undoBuffer.clear();
		m_undoIndex = 0;
	}

	bool TextEditor::SaveFile()
	{
		if (m_currentFilePath.empty())
		{
			return false;
		}
		return SaveFileAs(m_currentFilePath);
	}

	bool TextEditor::SaveFileAs(const std::string& filepath)
	{
		std::ofstream file(filepath);
		if (!file.is_open())
		{
			return false;
		}

		for (size_t i = 0; i < m_lines.size(); ++i)
		{
			file << m_lines[i];
			if (i < m_lines.size() - 1) // if last line of the text
			{
				file << "\n"; // then input new line at the end of the file
			}
		}

		m_currentFilePath = filepath;
		m_isModified = false;
		return true;
	}

	bool TextEditor::HasFileLoaded() const
	{
		return !m_currentFilePath.empty();
	}

	std::string TextEditor::GetCurrentFilePath() const
	{
		return m_currentFilePath;
	}

	bool TextEditor::IsModified() const
	{
		return m_isModified;
	}


	// Text content manager
	void TextEditor::SetText(const std::string& text)
	{
		m_lines.clear();
		std::istringstream stream(text);
		std::string textLine;

		while (std::getline(stream, textLine))
		{
			m_lines.push_back(textLine);
		}

		if (m_lines.empty())
		{
			m_lines.push_back("");
		}

		m_isModified = true;
		m_cursorPosition = { 0,0 };
		m_selectionStart = { 0, 0 };
		m_selectionEnd = { 0, 0 };
		m_undoBuffer.clear(); // PSS (23/01) - Revise
		m_undoIndex = 0;

	}

	std::string TextEditor::GetText() const
	{
		std::string text;
		for (size_t i = 0; i < m_lines.size(); i++)
		{
			text += m_lines[i];
			if (i < m_lines.size() - 1)
			{
				retult += "\n";
			}
		}
		return text;
	}

	void TextEditor::Clear()
	{
		m_lines.clear();
		m_lines.push_back("");
		m_isModified = true;
		m_cursorPosition = { 0,0 };
		m_selectionStart = { 0, 0 };
		m_selectionEnd = { 0, 0 };
	}

	std::vector<std::string> TextEditor::GetLines() const
	{
		return m_lines;
	}

	void TextEditor::SetLines(const std::vector<std::string>& lines)
	{
		m_lines = lines;
		if (m_lines.empty())
		{
			m_lines.push_back("");
		}
		m_isModified = true;
	}


	// Undo/Redo
	void TextEditor::AddUndo()
	{
		UndoRecord undoRecord;
		undoRecord = m_lines;
		undoRecord.cursorPosition = m_cursorPosition;

		while (m_undoIndex < static_cast<int>(m_undoBuffer.size())) // PSS (23/01) - Revise
		{
			m_undoBuffer.pop_back();
		}

		m_undoBuffer.push_back(undoRecord);
		m_undoIndex = static_cast<int>(m_undoBuffer.size());

		// Limit undo buffer size
		if (m_undoBuffer.size() > MAX_UNDO_BUFFER_SIZE)
		{
			m_undoBuffer.erase(m_undoBuffer.begin());
			m_undoIndex--;
		}

	}

	void TextEditor::Undo()
	{
		if (!CanUndo)
		{
			return;
		}

		if (m_undoIndex == static_cast<int>(m_undoBuffer.size()))
		{
			// Save current state before first undo
			AddUndo();
			m_undoIndex--; // PSS (23/01) - Revise if it interacts with undo max buffer size
		}

		m_undoIndex--;
		UndoRecord& undoRecord = m_undoBuffer[m_undoIndex];
		m_lines = undoRecord.lines;
		m_cursorPosition = undoRecord.cursorPosition;
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
	}

	void TextEditor::Redo()
	{
		if (!CanRedo)
		{
			return;
		}

		m_undoIndex++;
		UndoRecord undoRecord = m_undoBuffer[m_undoIndex];
		m_lines = undoRecord.lines;
		m_cursorPosition = undoRecord.cursorPosition;
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
	}

	bool TextEditor::CanUndo() const
	{
		return m_undoIndex > 0;
	}

	bool TextEditor::CanRedo() const
	{
		return m_undoIndex < static_cast<int>(m_undoBuffer.size()) - 1;
	}


	// Cursor & Selection
	LineIndex TextEditor::GetCursorPosition() const
	{
		return m_cursorPosition;
	}

	void TextEditor::SetCursorPosition(const LineIndex& position)
	{
		m_cursorPosition = SanitizeLineIndex(position);
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
		m_hasCursorPositionChanged = true;
	}

	bool TextEditor::HasSelection() const
	{
		return m_selectionStart != m_selectionEnd;
	}

	std::string TextEditor::GetSelectedText() const
	{

	}



	// ...

	LineIndex TextEditor::SanitizeLineIndex(const LineIndex& lineIndex) const
	{
		// PSS (23/01) - Double check that this is actually correct
		LineIndex retLineIndex = lineIndex;
		retLineIndex.line = std::max(0, std::min(retLineIndex.line, static_cast<int>m_lines.size() - 1));
		retLineIndex.column = std::max(0, std::min(retLineIndex.column, static_cast<int>(m_lines[retLineIndex.line].length())));
		return retLineIndex;
	}
}