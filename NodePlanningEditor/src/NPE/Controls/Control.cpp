#include "pch.h"
#include "Control.h"

#include "NPE/Graphics/Renderer2D.h"


namespace NPE
{
	Control::Control(Renderer2D& renderer, const Type type, const NPoint& pos, const NSize& size, const NColor& color)
		: m_Renderer(renderer), m_Type(type), m_Pos(pos), m_Size(size), m_Children{}, m_Color(color)
	{
	}

	void Control::MoveBy(const NPoint& pos)
	{
		for (auto& child : m_Children)
		{
			child->m_Pos += pos;
		}

		m_Pos += pos;
	}

	void Control::MoveTo(const NPoint& pos)
	{
		for (auto& child : m_Children)
		{
			NPoint ratio{ pos.x / m_Pos.x, pos.y / m_Pos.y };
			child->m_Pos += ratio;
		}

		m_Pos = pos;
	}

	void Control::ResizeBy(const NSize& size)
	{
		m_Size += size;
		
		for (auto& child : m_Children)
		{
			auto newLayout = child->CalculateLayout(m_Pos, m_Size);

			if (newLayout)
			{
				child->MoveTo(newLayout.value().first);
				child->ResizeTo(newLayout.value().second);
			}
		}

	}

	void Control::ResizeTo(const NSize& size)
	{
		m_Size = size;

		for (auto& child : m_Children)
		{
			auto newLayout = child->CalculateLayout(m_Pos, m_Size);
			
			if (newLayout)
			{
				child->MoveTo(newLayout.value().first);
				child->ResizeTo(newLayout.value().second);
			}
		}
	}

	//TODO: Implement function
	bool Control::OverlapsWith(const Control& other, const NSize& minDst)
	{
		bool ret = false;

		const NPoint& otherPos = other.GetPos();
		const NSize& otherSize = other.GetSize();



		return ret;
	}

	bool Control::IsInWindow() const
	{
		RECT rc;
		GetWindowRect(m_Renderer.GetNativeWindow(), &rc);
		
		//bool ret = false;
		//if (m_Pos.x + m_Size.width > 0 && m_Pos.y + m_Size.height > 0)
		//{
			//ret = true;
		//}
		//if (!(m_Pos.x < rc.right && m_Pos.y < rc.bottom))
		//{
			//ret = false;
		//}

		return m_Pos.x + m_Size.width > 0 && m_Pos.y + m_Size.height > 0 && m_Pos.x < rc.right && m_Pos.y < rc.bottom;
	}

}

