#include "pch.h"
#include "Control.h"


namespace NPE
{
	Control::Control(const Type type, const NPoint& pos, const NSize& size)
		: m_Type(type), m_Id(m_NextId), m_Pos(pos), m_Size(size), m_hWnd(0)
	{
		++m_NextId;
	}

	void Control::MoveBy(const NPoint& pos)
	{
		m_Pos += pos;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}

	void Control::MoveTo(const NPoint& pos)
	{
		m_Pos = pos;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}

	void Control::ResizeBy(const NSize& size)
	{
		m_Size += size;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}

	void Control::ResizeTo(const NSize& size)
	{
		m_Size = size;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}

}

