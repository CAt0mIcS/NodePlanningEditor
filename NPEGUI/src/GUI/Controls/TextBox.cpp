#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/ApplicationEvent.h"

#include "Util/Exceptions.h"
#include "Util/Direct2D.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*/



namespace GUI
{
	TextBox::TextBox(_In_opt_ Control* parent)
		: Control(Type::TextBox, parent), m_Text{}, m_IsMultiline(false), m_Caret(this)
	{
		m_Text.options = D2D1_DRAW_TEXT_OPTIONS_CLIP;
	}

	bool TextBox::Render()
	{
		if (this->IsInWindow())
		{
			const float max = std::min(GetSize().width, GetSize().height);
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), max / 5.0f, max / 5.0f);

			if (IsMultiline())
				SetFontSize((m_Size.width + m_Size.height) / 34.8f);
			else
				SetFontSize((m_Size.width + m_Size.height) / 17.4f);

			constexpr float minScale = 0.25f;
			if (Renderer::Get().GetScale().width > minScale && Renderer::Get().GetScale().height > minScale)
			{
				m_Caret.Render();
				RenderText();
			}

			for (auto* child : m_Children)
			{
				child->Render();
			}

			return true;
		}
		return false;
	}

	bool TextBox::OnEvent(_In_ Event& e)
	{
		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:
		{
			if (((MouseButtonPressedEvent&)e).GetButton() == MouseButton::Left)
			{
				this->SetFocus();
				m_Caret.OnMouseButtonPressed((MouseButtonPressedEvent&)e);
			}

			return true;
		}
		case EventType::MouseButtonReleasedEvent:
		{
			m_Caret.OnMouseButtonReleased((MouseButtonReleasedEvent&)e);
			return true;
		}
		case EventType::KeyPressedEvent:
		{
			if(this->HasFocus())
				m_Caret.OnKeyPressed((KeyPressedEvent&)e);
			return true;
		}
		case EventType::MouseMoveEvent:
		{
			if(this->HasFocus())
				m_Caret.OnMouseMove((MouseMoveEvent&)e);
			return true;
		}
		case EventType::CharEvent:
		{
			if(this->HasFocus())
				m_Caret.OnCharEvent((CharEvent&)e);
			return true;
		}
		case EventType::MouseWheelUpEvent:
		{
			if (this->IsMultiline())
				OnMouseWheelUp((MouseWheelUpEvent&)e);
			return true;
		}
		case EventType::MouseWheelDownEvent:
		{
			if (this->IsMultiline())
				OnMouseWheelDown((MouseWheelDownEvent&)e);
			return true;
		}
		case EventType::SetCursorEvent:
		{
			/**
			* Mouse is already guaranteed to be on the Control, we don't need to check here
			* Default cursor is automatically restored when exiting the Control
			*/
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			return true;
		}
		}
		
		return false;
	}

	void TextBox::RenderText()
	{
		if (m_Text.text == L"")
			return;

		float xOffset = m_Size.width / 30.0f;
		float yOffset;
		if (this->IsMultiline())
		{
			yOffset = m_Size.height / 10.0f;
		}
		else
		{
			yOffset = m_Size.height / 2.0f - m_Text.fontSize / 2.0f;
		}

		m_Text.pos = Util::NPoint{ m_Pos.x + xOffset, m_Pos.y + yOffset };
		m_Text.size = { m_Size.width - xOffset, m_Size.height - yOffset };

		if (this->IsMultiline())
		{
			DWrite::OverhangMetrics overhangMetrics = TextRenderer::Get().GetOverhangMetrics(m_Text);
			if (overhangMetrics.bottom > 0)
			{
				GetParent()->ResizeBy({ 0, ceil(overhangMetrics.bottom) });
				GetParent()->Render();
			}
		}
		

		TextRenderer::Get().RenderText(m_Text);
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize)
	{
		if (IsMultiline())
		{
			float xOffsetTxt = parentSize.width / 22.5f;
			float txtX = parentPos.x + xOffsetTxt;
			float txtWidth = parentSize.width - (xOffsetTxt * 2);
			float yOffsetTxt = parentSize.height / 7.0f;
			float txtY = parentPos.y + yOffsetTxt + (parentSize.height / 3.5f);

			//40% of Node's height is this TextBox
			float txtPercentOfNode = 0.4f;
			float txtHeight = parentSize.height * txtPercentOfNode;

			return { {{txtX, txtY}, {txtWidth, txtHeight}} };
		}
		else
		{
			float xOffsetTxt = parentSize.width / 22.5f;
			float txtX = parentPos.x + xOffsetTxt;

			float yOffsetTxt = parentSize.height / 7.0f;
			float txtY = parentPos.y + yOffsetTxt;

			float txtWidth = parentSize.width - (xOffsetTxt * 2);

			//20% of Node's height is this TextBox
			float txtPercentOfNode = 0.2f;
			float txtHeight = parentSize.height * txtPercentOfNode;

			return { { {txtX, txtY}, {txtWidth, txtHeight} } };
		}
		
	}

	void TextBox::OnMouseWheelUp(_In_ MouseWheelUpEvent& e)
	{
		
	}

	void TextBox::OnMouseWheelDown(_In_ MouseWheelDownEvent& e)
	{
		
	}
}


