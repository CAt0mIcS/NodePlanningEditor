#pragma once

#include "FileHandler.h"
#include "Actions.h"

#include <GUI/GUIApplication.h>

namespace GUI
{
	class PaintEvent;
	class MouseButtonPressedEvent;
	class MouseMoveEvent;
	class KeyPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseWheelUpEvent;
	class MouseWheelDownEvent;
	class TimerEvent;
	class AppCloseEvent;
	class CharEvent;

	class Button;
}


/**
* QUESTION:
*	friend class declaration or getters and setters for everything, or function parameters (for file handler to save controls, zoom...)
*/


namespace NPE
{
	/// <summary>
	/// Main Application class
	/// </summary>
	class Application : public GUI::GUIApplication
	{
	public:
		/// <summary>
		/// Application Constructor
		/// </summary>
		Application();

		/// <summary>
		/// Receives all events
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnEvent(GUI::Control* watched, GUI::Event& e);

		/// <summary>
		/// Receives all mouse move events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnMouseMove(GUI::Control* watched, GUI::MouseMoveEvent& e);

		/// <summary>
		/// Receives all mouse move events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnPaintEvent(GUI::Control* watched, GUI::PaintEvent& e);

		/// <summary>
		/// Called when the application gets closed
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>False if the window should close, true if the window should stay open</returns>
		bool OnClose(GUI::Control* watched, GUI::AppCloseEvent& e);
		
		/// <summary>
		/// Receives all mouse Button press events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnMouseButtonPressed(GUI::Control* watched, GUI::MouseButtonPressedEvent& e);

		/// <summary>
		/// Receives all mouse Button release events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnMouseButtonReleased(GUI::Control* watched, GUI::MouseButtonReleasedEvent& e);

		/// <summary>
		/// Receives all key press events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnKeyPressed(GUI::Control* watched, GUI::KeyPressedEvent& e);

		/// <summary>
		/// Receives all char events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnChar(GUI::Control* watched, GUI::CharEvent& e);

		/// <summary>
		/// Receives all mouse wheel up events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnMouseWheelUp(GUI::Control* watched, GUI::MouseWheelUpEvent& e);

		/// <summary>
		/// Receives all mouse wheel down events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnMouseWheelDown(GUI::Control* watched, GUI::MouseWheelDownEvent& e);

		/// <summary>
		/// Receives all timer events from OnEvent
		/// </summary>
		/// <param name="watched">Is the Control that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnTimer(GUI::Control* watched, GUI::TimerEvent& e);

		/// <summary>
		/// Application Deconstructor
		/// </summary>
		~Application();

	private:
		/// <summary>
		/// Shows a messagebox asking if the scene should be saved and saves it if the user choses yes
		/// </summary>
		/// <returns>The result of the message box</returns>
		int PromptSaveChangesMsgBox();

		/// <summary>
		/// Saves the scene to the save file
		/// </summary>
		/// <param name="saveToNewLocation">Specifies whether the scene should be saved to a new save file and the SaveFileDialog should open</param>
		void SaveScene(bool saveToNewLocation = false);

		/// <summary>
		/// Sets a member variable to needsToSave and sets the window title to the correct one
		/// </summary>
		/// <param name="needsToSave">Specifies whether the file has changed and needs to be saved</param>
		void SetNeedsToSaveAndWindowTitle(bool needsToSave);

		/// <summary>
		/// Sets text to the one which should be displayed when the scene does need to save
		/// </summary>
		void SetWindowSaveText();

		/// <summary>
		/// Sets text to the one which should be displayed when the scene doesn't need to save
		/// </summary>
		void SetWindowSavedText();

	private:
		/// <summary>
		/// Contains pointers to the controls which are used by an event to work across multiple events
		/// </summary>
		struct
		{
			/// <summary>
			/// The Node which is dragged around, set in OnMouseButtonPressed and used in OnMouseMove
			/// </summary>
			GUI::Node* draggingNode;
		} m_HandleControls;

		/// <summary>
		/// Handles things like file saving/loading/opening
		/// </summary>
		FileHandler m_FileHandler;

		/// <summary>
		/// Contains functions which respond to some kind of user input, 
		/// e.g. Move Camera
		/// </summary>
		Actions m_Actions;
		
		/// <summary>
		/// Cache old mouse position to calculate difference when moving the camera
		/// </summary>
		Util::NPoint m_MousePos;

		/// <summary>
		/// Contains all lines in the scene
		/// </summary>
		std::vector<Line> m_Lines;

		/// <summary>
		/// True if the mouse has moved and we are currently drawing a line
		/// </summary>
		bool m_DrawLines;

		/// <summary>
		/// Set to true after any kind of interaction
		/// </summary>
		bool m_NeedsToSave;

		/// <summary>
		/// Default Node width
		/// </summary>
		static constexpr float s_NodeWidth = 450.0f;

		/// <summary>
		/// Default Node height
		/// </summary>
		static constexpr float s_NodeHeight = 280.0f;

		/// <summary>
		/// The id of the timer which runs every 20s to automatically save the scene
		/// </summary>
		static constexpr int s_TimerAutosaveId = 0;

	};
}



