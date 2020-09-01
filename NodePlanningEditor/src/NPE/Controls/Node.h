#pragma once

#include "pch.h"
#include "NPE/Util/Props.h"


namespace NPE
{
	class MainWindow;


	class Node
	{
	public:
		Node(MainWindow* parent, const NPoint pos, const NSize size);

		void AdjustPos(NPoint posToAdd);

		NPoint GetPos() const { return m_Pos; }
		NSize GetSize() const { return m_Size; }
		float GetScale() const { return m_Scale; }

		HWND GetNativeWindow() const { return m_hWnd; }

	private:
		HWND m_hWnd;
		NPoint m_Pos;
		NSize m_Size;
		float m_Scale;
	};
}



