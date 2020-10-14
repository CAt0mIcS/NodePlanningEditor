#include "pch.h"
#include "SceneTab.h"

#include "GUI/Graphics/TextRenderer.h"
#include "GUI/Handlers/Mouse.h"

#include "GUI/Events/MouseEvent.h"

namespace GUI
{
	SceneTab::SceneTab(_In_opt_ Control* parent)
		: GUI::Control(Control::Type::Tab, parent), m_IsActive(false), m_Text{}
	{
		m_Text.options = D2D1_DRAW_TEXT_OPTIONS_CLIP;
	}

	bool SceneTab::Render()
	{
		if (Mouse::IsOnControl(this) || IsActive())
			Renderer::Get().RenderRect(m_Pos, m_Size, Constants::g_DefaultActiveSceneTabColor);
		else
			Renderer::Get().RenderRect(m_Pos, m_Size, Constants::g_DefaultInactiveSceneTabColor);

		Renderer::Get().RenderRectOutline(m_Pos, m_Size, Constants::g_DefaultTabBorderColor);
		RenderText();

		return true;
	}

	bool SceneTab::OnEvent(_In_ Event& e)
	{
		switch (e.GetType())
		{
		case EventType::MouseMoveEvent:
		{
			// Mouse is on this SceneTab --> Issue rendering call to draw the tab in the active color
			PostRedraw();
			return true;
		}
		case EventType::SetCursorEvent:
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return true;
		}
		}

		return false;
	}

	void SceneTab::RenderText()
	{
		float xOffset = m_Size.width / 30.0f;
		float yOffset = m_Size.height / 2.0f - m_Text.fontSize / 2.0f;

		m_Text.pos = Util::NPoint{ m_Pos.x + xOffset, m_Pos.y + yOffset };
		m_Text.size = { m_Size.width - xOffset, m_Size.height - yOffset };

		TextRenderer::Get().RenderText(m_Text);
	}
	
	void SceneTab::SetText(_In_ const std::wstring& text)
	{
		m_Text.text = text;

		if (m_Text.text.back() == L'*')
		{
			return;
		}
		else
		{
			m_Text.size.width = FLT_MAX;

			auto metrics = TextRenderer::Get().GetTextMetrics(m_Text);
			m_Size.width = metrics.width + 30;
		}
		
	}
}


