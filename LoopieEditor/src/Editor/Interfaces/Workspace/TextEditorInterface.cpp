#include "TextEditorInterface.h"

#include "Editor/Interfaces/Workspace/HierarchyInterface.h"
#include "Editor/Interfaces/Workspace/AssetsExplorerInterface.h"

#include <fstream>
#include <sstream>
#include <algorithm> // for std::max and std::min

#include <imgui.h>

namespace Loopie {
	TextEditorInterface::TextEditorInterface()
	{
		m_lines.push_back("");
		m_cursorPosition = { 0, 0 };
		m_selectionStart = { 0, 0 };
		m_selectionEnd = { 0, 0 };
	}

	TextEditorInterface::~TextEditorInterface()
	{
		HierarchyInterface::s_OnEntitySelected.RemoveObserver(this);
		AssetsExplorerInterface::s_OnFileSelected.RemoveObserver(this);
	}

	void TextEditorInterface::Init() 
	{
		HierarchyInterface::s_OnEntitySelected.AddObserver(this);
		AssetsExplorerInterface::s_OnFileSelected.AddObserver(this);
	}

	void TextEditorInterface::Render()
	{
		// Render done mostly from https://github.com/BalazsJako/ImGuiColorTextEdit/blob/master/TextEditor.cpp
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse; 

		if (ImGui::Begin("Text Editor", nullptr, windowFlags))
		{
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
			{
				HandleKeyboardInputs();
			}

			m_cursorPosition = SanitizeLineIndex(m_cursorPosition);
			float footerHeight = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y;
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

			// Drawing logic
			ImVec2 charAdvance = ImGui::CalcTextSize(" ");
			float lineHeight = ImGui::GetTextLineHeightWithSpacing();
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
			ImVec2 contentSize = ImGui::GetContentRegionAvail();
			float scrollX = ImGui::GetScrollX();
			float scrollY = ImGui::GetScrollY();

			// Calculate visible lines
			int lineStart = (int)floor(scrollY / lineHeight);
			int lineEnd = std::min((int)m_lines.size(), lineStart + (int)ceil(contentSize.y / lineHeight) + 1);
			lineStart = std::max(0, lineStart);

			// Line number margin
			char lineNumBuf[16];
			snprintf(lineNumBuf, 16, " %d ", (int)m_lines.size());
			float lineNumberWidth = ImGui::CalcTextSize(lineNumBuf).x;
			float textStart = lineNumberWidth + 10.0f;

			// Colors
			ImU32 lineNumberColor = IM_COL32(100, 100, 100, 255);
			ImU32 currentLineColor = IM_COL32(40, 40, 40, 255);
			ImU32 currentLineEdgeColor = IM_COL32(80, 80, 80, 255);
			ImU32 selectionColor = IM_COL32(50, 100, 200, 255);
			ImU32 cursorColor = IM_COL32(255, 255, 255, 255);
			ImU32 textColor = IM_COL32(220, 220, 220, 255);

			for (int lineNum = lineStart; lineNum < lineEnd; ++lineNum)
			{
				ImVec2 lineStartScreenPos = ImVec2(cursorScreenPos.x, cursorScreenPos.y + lineNum * lineHeight);
				ImVec2 textScreenPos = ImVec2(lineStartScreenPos.x + textStart, lineStartScreenPos.y);

				const std::string& line = m_lines[lineNum];

				// Draw Current Line Highlight
				if (lineNum == m_cursorPosition.line && !HasSelection())
				{
					ImVec2 highlightStart = ImVec2(lineStartScreenPos.x + scrollX, lineStartScreenPos.y);
					ImVec2 highlightEnd = ImVec2(lineStartScreenPos.x + contentSize.x + scrollX, 
												 lineStartScreenPos.y + lineHeight);
					drawList->AddRectFilled(highlightStart, highlightEnd, currentLineColor);
					drawList->AddRect(highlightStart, highlightEnd, currentLineEdgeColor, 0.0f, 0, 1.0f);
				}

				// Draw Selection
				if (HasSelection())
				{
					LineIndex start, end;
					GetOrderedSelection(start, end);
					if (lineNum >= start.line && lineNum <= end.line)
					{
						int selStart = (lineNum == start.line) ? start.column : 0;
						int selEnd = (lineNum == end.line) ? end.column : (int)line.length();
						ImVec2 sStart(textScreenPos.x + selStart * charAdvance.x, lineStartScreenPos.y);
						ImVec2 sEnd(textScreenPos.x + selEnd * charAdvance.x, lineStartScreenPos.y + lineHeight);
						drawList->AddRectFilled(sStart, sEnd, selectionColor);
					}
				}

				// Draw Line Number
				snprintf(lineNumBuf, 16, "%d", lineNum + 1);
				float lineNoWidth = ImGui::CalcTextSize(lineNumBuf).x;
				drawList->AddText(ImVec2(lineStartScreenPos.x + textStart - lineNoWidth - 5.0f, lineStartScreenPos.y), 
								  lineNumberColor, lineNumBuf);

				// Draw Cursor
				if (lineNum == m_cursorPosition.line && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
				{
					static float blinkTime = 0.0f;
					blinkTime += ImGui::GetIO().DeltaTime;
					if (fmod(blinkTime, 1.0f) < 0.5f)
					{
						float cursorX = textScreenPos.x + m_cursorPosition.column * charAdvance.x;
						drawList->AddLine(ImVec2(cursorX, lineStartScreenPos.y), 
										  ImVec2(cursorX, lineStartScreenPos.y + lineHeight), cursorColor, 2.0f);
					}
				}

				// Draw Text
				if (!line.empty())
				{
					drawList->AddText(textScreenPos, textColor, line.c_str());
				}
			}

			// Dummy for scrolling
			float longestLineWidth = textStart;
			for (const auto& line : m_lines)
			{
				longestLineWidth = std::max(longestLineWidth, textStart + line.length() * charAdvance.x);
			}
			ImGui::Dummy(ImVec2(longestLineWidth + 20.0f, m_lines.size() * lineHeight));

			// Auto-scroll to cursor
			if (m_hasCursorPositionChanged)
			{
				float cursorY = m_cursorPosition.line * lineHeight;
				float cursorX = textStart + m_cursorPosition.column * charAdvance.x;
				if (cursorY < scrollY)
				{
					ImGui::SetScrollY(cursorY);
				}
				else if (cursorY + lineHeight > scrollY + contentSize.y)
				{
					ImGui::SetScrollY(cursorY + lineHeight - contentSize.y);
				}

				if (cursorX < scrollX + textStart)
				{
					ImGui::SetScrollX(cursorX - textStart);
				}
				else if (cursorX > scrollX + contentSize.x - 20.0f)
				{
					ImGui::SetScrollX(cursorX - contentSize.x + 20.0f);
				}

				m_hasCursorPositionChanged = false;
			}

			ImGui::EndChild();

			// This renders below the scrollable area
			ImGui::Separator();

			std::string statusText;
			if (m_currentFilePath.empty())
			{
				statusText = "Untitled";
			}
			else
			{
				statusText = std::filesystem::path(m_currentFilePath).filename().string(); 
			}

			if (m_isModified)
			{
				statusText += "*";
			}

			ImGui::Text("File: %s", statusText.c_str());

			// Debug tooltip to see if input is working
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Lines: %d | Cursor: %d:%d", static_cast<int>(m_lines.size()), 
								  m_cursorPosition.line, m_cursorPosition.column);
			}
		}
		ImGui::End(); 
	}

	void TextEditorInterface::HandleKeyboardInputs()
	{
		ImGuiIO& io = ImGui::GetIO();
		bool shift = io.KeyShift;
		bool ctrl = io.KeyCtrl;

		// Shortcuts
		if (ctrl && !shift)
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Z))
			{
				Undo();
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_Y))
			{
				Redo();
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_S))
			{
				SaveFile();
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_A))
			{
				// Select all
				m_selectionStart = { 0, 0 };
				m_selectionEnd = { static_cast<int>(m_lines.size()) - 1, static_cast<int>(m_lines.back().length()) };
			}
		}

		if (ctrl && shift)
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Z))
			{
				Redo();
			}
		}

		// Navigation
		if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
		{
			if (m_cursorPosition.column > 0)
			{
				m_cursorPosition.column--;
			}
			else if (m_cursorPosition.line > 0)
			{
				m_cursorPosition.line--;
				m_cursorPosition.column = static_cast<int>(m_lines[m_cursorPosition.line].length());
			}

			if (!shift)
			{
				m_selectionStart = m_selectionEnd = m_cursorPosition;
			}
			else
			{
				m_selectionEnd = m_cursorPosition;
			}
			m_hasCursorPositionChanged = true;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
		{
			if (m_cursorPosition.line < static_cast<int>(m_lines.size()) &&
				m_cursorPosition.column < static_cast<int>(m_lines[m_cursorPosition.line].length()))
			{
				m_cursorPosition.column++;
			}
			else if (m_cursorPosition.line < static_cast<int>(m_lines.size()) - 1)
			{
				m_cursorPosition.line++;
				m_cursorPosition.column = 0;
			}

			if (!shift)
			{
				m_selectionStart = m_selectionEnd = m_cursorPosition;

			}
			else
			{
				m_selectionEnd = m_cursorPosition;
			}

			m_hasCursorPositionChanged = true;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
		{
			if (m_cursorPosition.line > 0)
			{
				m_cursorPosition.line--;
				if (m_cursorPosition.line < static_cast<int>(m_lines.size()))
				{
					m_cursorPosition.column = std::min(m_cursorPosition.column,	
													   static_cast<int>(m_lines[m_cursorPosition.line].length()));
				}
			}

			if (!shift)
			{
				m_selectionStart = m_selectionEnd = m_cursorPosition;
			}
			else
			{
				m_selectionEnd = m_cursorPosition;
			}

			m_hasCursorPositionChanged = true;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
		{
			if (m_cursorPosition.line < static_cast<int>(m_lines.size()) - 1)
			{
				m_cursorPosition.line++;
				if (m_cursorPosition.line < static_cast<int>(m_lines.size()))
				{
					m_cursorPosition.column = std::min(m_cursorPosition.column,
													   static_cast<int>(m_lines[m_cursorPosition.line].length()));
				}
			}

			if (!shift)
			{
				m_selectionStart = m_selectionEnd = m_cursorPosition;
			}
			else
			{
				m_selectionEnd = m_cursorPosition;
			}

			m_hasCursorPositionChanged = true;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Home))
		{
			m_cursorPosition.column = 0;
			if (!shift)
			{
				m_selectionStart = m_selectionEnd = m_cursorPosition;
			}
			else
			{
				m_selectionEnd = m_cursorPosition;
			}

			m_hasCursorPositionChanged = true;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_End))
		{
			if (m_cursorPosition.line < static_cast<int>(m_lines.size()))
			{
				m_cursorPosition.column = static_cast<int>(m_lines[m_cursorPosition.line].length());
			}

			if (!shift)
			{
				m_selectionStart = m_selectionEnd = m_cursorPosition;
			}
			else
			{
				m_selectionEnd = m_cursorPosition;
			}

			m_hasCursorPositionChanged = true;
		}

		// Editing
		if (ImGui::IsKeyPressed(ImGuiKey_Backspace))
		{
			Backspace();
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Delete))
		{
			Delete();
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			EnterChar('\n');
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Tab))
		{
			EnterChar('\t');
		}

		// Text input
		if (io.InputQueueCharacters.Size > 0)
		{
			for (int i = 0; i < io.InputQueueCharacters.Size; i++)
			{
				unsigned int c = (unsigned int)io.InputQueueCharacters[i];
				if (c > 0 && c < 0x10000)
				{
					EnterChar((char)c);
				}
			}
		}
	}

	void TextEditorInterface::HandleMouseInputs()
	{
		// TODO: Implement mouse selection
	}


	// File Operations
	bool TextEditorInterface::LoadFile(const std::string& filepath)
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

		return true;
	}

	bool TextEditorInterface::SaveFile()
	{
		if (m_currentFilePath.empty())
		{
			return false;
		}
		return SaveFileAs(m_currentFilePath);
	}

	bool TextEditorInterface::SaveFileAs(const std::string& filepath)
	{
		std::ofstream file(filepath);
		if (!file.is_open())
		{
			return false;
		}

		for (size_t i = 0; i < m_lines.size(); ++i)
		{
			file << m_lines[i];
			if (i < m_lines.size() - 1) // if not last line of the text
			{
				file << "\n"; // then input new line between lines
			}
		}

		m_currentFilePath = filepath;
		m_isModified = false;
		return true;
	}

	bool TextEditorInterface::HasFileLoaded() const
	{
		return !m_currentFilePath.empty();
	}

	std::string TextEditorInterface::GetCurrentFilePath() const
	{
		return m_currentFilePath;
	}

	bool TextEditorInterface::IsModified() const
	{
		return m_isModified;
	}


	// Text content manager
	void TextEditorInterface::SetText(const std::string& text)
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

	std::string TextEditorInterface::GetText() const
	{
		std::string text;
		for (size_t i = 0; i < m_lines.size(); i++)
		{
			text += m_lines[i];
			if (i < m_lines.size() - 1)
			{
				text += "\n";
			}
		}
		return text;
	}

	void TextEditorInterface::Clear()
	{
		m_lines.clear();
		m_lines.push_back("");
		m_isModified = true;
		m_cursorPosition = { 0,0 };
		m_selectionStart = { 0, 0 };
		m_selectionEnd = { 0, 0 };
	}

	std::vector<std::string> TextEditorInterface::GetLines() const
	{
		return m_lines;
	}

	void TextEditorInterface::SetLines(const std::vector<std::string>& lines)
	{
		m_lines = lines;
		if (m_lines.empty())
		{
			m_lines.push_back("");
		}
		m_isModified = true;
	}


	// Undo/Redo
	void TextEditorInterface::AddUndo()
	{
		UndoRecord undoRecord;
		undoRecord.lines = m_lines;
		undoRecord.cursorPosition = m_cursorPosition;

		// Remove any redo history when making a new edit
		if (m_undoIndex < static_cast<int>(m_undoBuffer.size()))
		{
			m_undoBuffer.erase(m_undoBuffer.begin() + m_undoIndex, m_undoBuffer.end());
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

	void TextEditorInterface::Undo()
	{
		if (!CanUndo())
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

	void TextEditorInterface::Redo()
	{
		if (!CanRedo())
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

	bool TextEditorInterface::CanUndo() const
	{
		return m_undoIndex > 0;
	}

	bool TextEditorInterface::CanRedo() const
	{
		return m_undoIndex < static_cast<int>(m_undoBuffer.size()) - 1;
	}


	// Cursor & Selection
	TextEditorInterface::LineIndex TextEditorInterface::GetCursorPosition() const
	{
		return m_cursorPosition;
	}

	void TextEditorInterface::SetCursorPosition(const LineIndex& position)
	{
		m_cursorPosition = SanitizeLineIndex(position);
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
		m_hasCursorPositionChanged = true;
	}

	bool TextEditorInterface::HasSelection() const
	{
		return m_selectionStart != m_selectionEnd;
	}

	std::string TextEditorInterface::GetSelectedText() const
	{
		if (!HasSelection())
		{
			return "";
		}

		LineIndex start;
		LineIndex end;

		GetOrderedSelection(start, end);

		if (start.line == end.line)
		{
			return m_lines[start.line].substr(start.column, end.column - start.column);
		}

		std::string selectedText;
		selectedText = m_lines[start.line].substr(start.column) + "\n";

		for (int i = start.line + 1; i < end.line; ++i)
		{
			selectedText += m_lines[i] + "\n";
		}

		selectedText += m_lines[end.line].substr(0, end.column);
		return selectedText;
	}

	void TextEditorInterface::DeleteSelection()
	{
		if (!HasSelection())
		{
			return;
		}

		AddUndo();

		LineIndex start;
		LineIndex end;

		GetOrderedSelection(start, end);

		if (start.line == end.line)
		{
			std::string& line = m_lines[start.line];
			line.erase(start.column, end.column - start.column);
		}
		else
		{
			std::string& firstLine = m_lines[start.line];
			std::string& lastLine = m_lines[end.line];

			firstLine = firstLine.substr(0, start.column) + lastLine.substr(end.column);
			m_lines.erase(m_lines.begin() + start.line + 1, m_lines.begin() + end.line + 1);
		}

		m_cursorPosition = start;
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
		m_isModified = true;
	}

	// Text modification
	void TextEditorInterface::InsertChar(char c)
	{
		if (m_readOnly)
		{
			return;
		}

		if (HasSelection())
		{
			DeleteSelection();
		}

		AddUndo();

		std::string& line = m_lines[m_cursorPosition.line];
		line.insert(m_cursorPosition.column, 1, c);
		m_cursorPosition.column++;
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
		m_isModified = true;
	}

	void TextEditorInterface::Backspace()
	{
		if (m_readOnly || (m_cursorPosition.column == 0 && m_cursorPosition.line == 0))
		{
			return;
		}

		if (HasSelection())
		{
			DeleteSelection();
			return;
		}

		AddUndo();

		if (m_cursorPosition.column > 0)
		{
			m_lines[m_cursorPosition.line].erase(m_cursorPosition.column - 1, 1);
			m_cursorPosition.column--;
		}
		else // Backspace at start of line
		{
			int currLineIndex = m_cursorPosition.line;
			int prevLineIndex = currLineIndex - 1;

			m_cursorPosition.column = (int)m_lines[prevLineIndex].length();
			m_lines[prevLineIndex] += m_lines[currLineIndex];
			m_lines.erase(m_lines.begin() + currLineIndex);

			m_cursorPosition.line--;
		}

		m_cursorPosition = SanitizeLineIndex(m_cursorPosition);
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
		m_hasCursorPositionChanged = true;
		m_isModified = true;
	}

	void TextEditorInterface::Delete()
	{
		if (m_readOnly)
		{
			return;
		}

		if (HasSelection())
		{
			DeleteSelection();
			return;
		}

		AddUndo();

		// Use index instead of reference to avoid iterator invalidation issues
		int currentLineIdx = m_cursorPosition.line;
		int currentLineLen = (int)m_lines[currentLineIdx].length();

		if (m_cursorPosition.column < currentLineLen)
		{
			m_lines[currentLineIdx].erase(m_cursorPosition.column, 1);
		}
		else if (currentLineIdx < static_cast<int>(m_lines.size() - 1))
		{
			// Merge with next line
			m_lines[currentLineIdx] += m_lines[currentLineIdx + 1];
			m_lines.erase(m_lines.begin() + currentLineIdx + 1);
		}

		m_cursorPosition = SanitizeLineIndex(m_cursorPosition);
		m_selectionStart = m_cursorPosition;
		m_selectionEnd = m_cursorPosition;
		m_isModified = true;
	}

	void TextEditorInterface::EnterChar(char c)
	{
		if (c == '\n' || c == '\r')
		{
			if (m_readOnly)
			{
				return;
			}

			if (HasSelection())
			{
				DeleteSelection();
			}

			AddUndo();

			std::string& line = m_lines[m_cursorPosition.line];
			std::string newLine = line.substr(m_cursorPosition.column);
			line = line.substr(0, m_cursorPosition.column);

			m_lines.insert(m_lines.begin() + m_cursorPosition.line + 1, newLine);
			m_cursorPosition.line++;
			m_cursorPosition.column = 0;
			m_selectionStart = m_cursorPosition;
			m_selectionEnd = m_cursorPosition;
			m_isModified = true;
		}
		else if (c == '\t')
		{
			for (int i = 0; i < m_tabSize; ++i)
			{
				InsertChar(' ');
			}
		}
		else if (c >= 32)
		{
			InsertChar(c);
		}
	}

	void TextEditorInterface::SetReadOnly(bool value)
	{
		m_readOnly = value;
	}

	bool TextEditorInterface::IsReadOnly() const
	{
		return m_readOnly;
	}

	void TextEditorInterface::SetTabSize(int spaces)
	{
		m_tabSize = spaces;
	}

	// ...

	TextEditorInterface::LineIndex TextEditorInterface::SanitizeLineIndex(const LineIndex& lineIndex) const
	{
		LineIndex retLineIndex = lineIndex;
		retLineIndex.line = std::max(0, std::min(retLineIndex.line, static_cast<int>(m_lines.size() - 1)));
		retLineIndex.column = std::max(0, std::min(retLineIndex.column, static_cast<int>(m_lines[retLineIndex.line].length())));
		return retLineIndex;
	}

	void TextEditorInterface::GetOrderedSelection(LineIndex& start, LineIndex& end) const
	{
		start = m_selectionStart;
		end = m_selectionEnd;

		if (start.line > end.line || (start.line == end.line && start.column > end.column))
		{
			std::swap(start, end);
		}
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
				LoadFile(filePath);
			}
		}
	}
}