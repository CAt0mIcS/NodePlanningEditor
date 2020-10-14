#pragma once

#include "GUI/Controls/Control.h"
#include "GUI/Graphics/TextRenderer.h"

/**
* QUESTION:
*	Where should I put this?
*/


namespace GUI
{
	class GUI_API SceneTab : public Control
	{
	public:
		/// <summary>
		/// SceneTab constructor
		/// </summary>
		/// <param name="parent">Is the parent of the tab</param>
		SceneTab(_In_opt_ Control* parent = nullptr);

		/// <summary>
		/// Setter for the Save File path
		/// </summary>
		/// <param name="filepath">Is the full path to the new Save File</param>
		void SetFile(_In_ const std::wstring& filepath) { m_SaveFilePath = filepath; }

		/// <summary>
		/// Getter for the path to the Save File
		/// </summary>
		/// <returns>The full path to the Save File</returns>
		const std::wstring& GetFilePath() const { return m_SaveFilePath; }

		/// <summary>
		/// Renders Control and all it's children
		/// </summary>
		/// <returns>True if the Control was rendered(was in window), false otherwise</returns>
		virtual bool Render() override;

		/// <summary>
		/// All events of the specific Control will be dispatched to this function
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, else false and the event will be dispatched to the client</returns>
		virtual bool OnEvent(_In_ Event& e) override;

		/// <summary>
		/// Renders the set text
		/// </summary>
		void RenderText();

		/// <summary>
		/// Getter for the active state
		/// </summary>
		/// <returns>True if the this SceneTab is the active one, false otherwise</returns>
		bool IsActive() const { return m_IsActive; }

		/// <summary>
		/// Setter for the active state
		/// </summary>
		/// <param name="active">Specifies whether this is the current active window</param>
		void SetActive(_In_ bool active) { m_IsActive = active; }

		/// <summary>
		/// Getter for the current text
		/// </summary>
		/// <returns>The text of the SceneTab</returns>
		const NText& GetText() const { return m_Text; }

		/// <summary>
		/// Setter for the text
		/// </summary>
		/// <param name="text">Is the new text of the SceneTab</param>
		void SetText(_In_ const std::wstring& text);

		/// <summary>
		/// Setter for the font size of the text
		/// </summary>
		/// <param name="size">Is the new font size</param>
		void SetFontSize(_In_ const float size) { m_Text.fontSize = size; }

	private:
		/// <summary>
		/// Specifies the full path to the Save File
		/// </summary>
		std::wstring m_SaveFilePath;

		/// <summary>
		/// Specifies whether this is the current active window 
		/// </summary>
		bool m_IsActive;

		/// <summary>
		/// Specifies text properties for rendering
		/// </summary>
		NText m_Text;
	};
}



