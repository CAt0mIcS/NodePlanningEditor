#pragma once

#include "Event.h"

#include "Mouse.h"


namespace NPE
{

	enum class Button
	{
		INVALID = 0,
		LeftMouseButton, MiddleMouseButton, RightMouseButton
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const NPoint& pos)
		{
			Mouse::SetPos(pos);
		}

		const NPoint& GetPos() const { return Mouse::GetPos(); }

		NPE_DECLARE_TYPE_FN(MouseMoveEvent)
	};

	class MouseButtonEvent : public Event
	{
	public:
		bool IsLeftPressed() const { return Mouse::IsLeftPressed(); }
		bool IsMiddlePressed() const { return Mouse::IsMiddlePressed(); }
		bool IsRightPressed() const { return Mouse::IsRightPressed(); }

	protected:
		MouseButtonEvent() {}
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const Button btn)
		{
			switch (btn)
			{
			case Button::LeftMouseButton:
			{
				Mouse::SetLeftPressed(true);
				break;
			}
			case Button::RightMouseButton:
			{
				Mouse::SetRightPressed(true);
				break;
			}
			case Button::MiddleMouseButton:
			{
				Mouse::SetMiddlePressed(true);
				break;
			}
			}
		}

		NPE_DECLARE_TYPE_FN(MouseButtonPressedEvent)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const Button btn)
		{
			switch (btn)
			{
			case Button::LeftMouseButton:
			{
				Mouse::SetLeftPressed(false);
				break;
			}
			case Button::RightMouseButton:
			{
				Mouse::SetRightPressed(false);
				break;
			}
			case Button::MiddleMouseButton:
			{
				Mouse::SetMiddlePressed(false);
				break;
			}
			}
		}

		NPE_DECLARE_TYPE_FN(MouseButtonReleasedEvent)
	};

	class MouseWheelEvent : public Event
	{
	public:
		int GetDelta() const { return m_Delta; }

	protected:
		MouseWheelEvent(int delta)
			: m_Delta(delta) {}

	private:
		int m_Delta;
	};

	class MouseWheelUpEvent : public MouseWheelEvent
	{
	public:
		MouseWheelUpEvent(int delta)
			: MouseWheelEvent(delta)
		{

		}

		NPE_DECLARE_TYPE_FN(MouseWheelUpEvent);
	};

	class MouseWheelDownEvent : public MouseWheelEvent
	{
	public:
		MouseWheelDownEvent(int delta)
			: MouseWheelEvent(delta)
		{

		}

		NPE_DECLARE_TYPE_FN(MouseWheelDownEvent)
	};

	//TODO: Check if Set-/ReleaseCapture work and are in the correct order
	class MouseEnterEvent : public Event
	{
	public:
		MouseEnterEvent(HWND hWnd)
		{
			SetCapture(hWnd);
		}

		NPE_DECLARE_TYPE_FN(MouseEnterEvent)
	};

	class MouseLeaveEvent : public Event
	{
	public:
		MouseLeaveEvent()
		{
			ReleaseCapture();
		}

		NPE_DECLARE_TYPE_FN(MouseLeaveEvent)
	};
}
