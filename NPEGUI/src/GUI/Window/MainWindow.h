#pragma once

#include <functional>
#include <vector>

#include "BaseWindow.h"

#include "GUI/Events/Event.h"

#include "GUI/Controls/Control.h"
#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"


namespace GUI
{
	/**
	* Event filter function type
	* 
	* @param watched is the object that received the event
	* @param e is the received event
	* @returns true if the event was handled, false otherwise
	*/
	using EventCallbackFn = std::function<bool(GUI::Control* watched, GUI::Event& e)>;

	class MainWindow : public BaseWindow<MainWindow>, public Control
	{
	public:
		/**
		* MainWindow constructor
		*/
		MainWindow();

		/**
		* Renders the window background
		* 
		* @returns always true
		*/
		virtual bool Render() override;

		/**
		* All events of the specific control will be dispatched to this function
		*
		* @param e is the received event
		* @returns true if the event was handled, else false and the event will be dispatched to the client
		*/
		virtual bool OnEvent(Event& e) override;

		/**
		* Enters the message loop
		* 
		* @returns the exit code on exit
		*/
		static int ProcessMessage();

		/**
		* Handles all window messages
		*
		* @param uMsg is the window message
		* @param wParam is the wParam
		* @param lParam is the lParam
		*/
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		/**
		* Getter for all controls in the window
		* 
		* @returns vector of all controls
		*/
		std::vector<Control*>& GetControls() { return m_Children; }

		/**
		* Adds a Control to the window
		* 
		* @returns the added Control
		*/
		Control* AddControl(Control* control) { return m_Children.emplace_back(control); }

		/**
		* Getter for window position
		* 
		* @returns the window position
		*/
		Util::NPoint GetPos() const;

		/**
		* Getter for window size
		*
		* @returns the window size
		*/
		Util::NSize GetSize() const;

		/**
		* Getter for window rect
		*
		* @returns the window rect
		*/
		RECT GetRect() const;

		/**
		* Sets the event callback
		* 
		* @tparam F is any callable and to GUI::EventCallbackFn castable type
		* @param func is the function to set the event callback to
		*/
		template<typename F>
		void SetEventCallback(F&& func) { m_EventCallbackFn = func; }

	private:
		/**
		* Receives all events, finds the event receiver and dispatches the event to first the receiver and then the user
		* 
		* @param e is the received event from window procedure
		* @returns true if the event was handled, false otherwise
		*/
		bool DispatchEvent(Event& e);

	private:
		EventCallbackFn m_EventCallbackFn;

	};

}



