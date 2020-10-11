#pragma once

#include "GUIBase.h"
#include "Window/MainWindow.h"


namespace GUI
{
	class GUI_API GUIApplication
	{
	public:
		/// <summary>
		/// Adds a function to dispatch all events to
		/// </summary>
		/// <typeparam name="F">Is any callable and to GUI::EventCallbackFn castable type</typeparam>
		/// <param name="func">Is the function which will be called when any event is received</param>
		template<typename F, typename = std::enable_if_t<std::is_invocable<F, Control*, Event&>::value>>
		void InstallEventFilter(_In_ F&& func);

		/// <summary>
		/// Starts the application loop
		/// </summary>
		/// <returns>The exit code when quitting the application</returns>
		int Run();

		/// <summary>
		/// Getter for the MainWindow
		/// </summary>
		/// <returns>The Main Window</returns>
		MainWindow& GetWindow() { return m_Window; }

	protected:
		/// <summary>
		/// GUIApplication Constructor
		/// </summary>
		GUIApplication();

	protected:
		/// <summary>
		/// Object representing the Main Window in the Application
		/// </summary>
		MainWindow m_Window;
	};


	template<typename F, typename>
	inline void GUIApplication::InstallEventFilter(_In_ F&& func)
	{
		m_Window.SetEventCallback(func);
	}
}



