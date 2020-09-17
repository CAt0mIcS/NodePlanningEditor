#pragma once

#include "GUIBase.h"


namespace GUI
{
	class GUI_API Timer
	{
	public:
		Timer(HWND hWnd, bool repeat);

		void Run(unsigned int time);

		HWND GetWindow() const { return m_hWnd; }
		bool IsRepeating() const { return m_Repeat; }
		unsigned int GetId() const { return m_Id; }

		void Destroy();

	private:
		HWND m_hWnd;
		bool m_Repeat;
		unsigned int m_Id;
		static unsigned int s_NextId;
	};
}



