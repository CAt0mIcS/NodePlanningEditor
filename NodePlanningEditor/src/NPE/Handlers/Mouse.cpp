#include "pch.h"
#include "Mouse.h"
#include "NPE/Controls/Control.h"


namespace NPE
{
	bool Mouse::IsOnControl(const Control* const control)
	{
		const NPoint& controlPos = control->GetPos();
		const NSize& controlSize = control->GetSize();

		return m_Pos.x > controlPos.x && m_Pos.y > controlPos.y && m_Pos.x < controlPos.x + controlSize.width && m_Pos.y < controlPos.y + controlSize.height;

	}
}