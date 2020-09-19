#pragma once

#include "Event.h"
#include "Util/Util.h"
#include "GUI/Handlers/Mouse.h"


namespace GUI
{

	enum class MouseButton
	{
		INVALID = 0,
		Left, Middle, Right
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(_In_ const Util::NPoint& pos)
		{
			Mouse::SetPos(pos);
		}

		const Util::NPoint& GetPos() const { return Mouse::GetPos(); }

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
		MouseButtonPressedEvent(_In_ const MouseButton btn)
			: m_Button(btn)
		{
			switch (btn)
			{
			case MouseButton::Left:
			{
				Mouse::SetLeftPressed(true);
				break;
			}
			case MouseButton::Right:
			{
				Mouse::SetRightPressed(true);
				break;
			}
			case MouseButton::Middle:
			{
				Mouse::SetMiddlePressed(true);
				break;
			}
			}
		}

		MouseButton GetButton() const { return m_Button; }

		NPE_DECLARE_TYPE_FN(MouseButtonPressedEvent)

	private:
		MouseButton m_Button;
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(_In_ const MouseButton btn)
			: m_Button(btn)
		{
			switch (btn)
			{
			case MouseButton::Left:
			{
				Mouse::SetLeftPressed(false);
				break;
			}
			case MouseButton::Right:
			{
				Mouse::SetRightPressed(false);
				break;
			}
			case MouseButton::Middle:
			{
				Mouse::SetMiddlePressed(false);
				break;
			}
			}
		}

		MouseButton GetButton() const { return m_Button; }

		NPE_DECLARE_TYPE_FN(MouseButtonReleasedEvent)

	private:
		MouseButton m_Button;
	};

	class MouseWheelEvent : public Event
	{
	public:
		int GetDelta() const { return m_Delta; }

	protected:
		MouseWheelEvent(_In_ int delta)
			: m_Delta(delta) {}

	private:
		int m_Delta;
	};

	class MouseWheelUpEvent : public MouseWheelEvent
	{
	public:
		MouseWheelUpEvent(_In_ int delta)
			: MouseWheelEvent(delta)
		{

		}

		NPE_DECLARE_TYPE_FN(MouseWheelUpEvent)
	};

	class MouseWheelDownEvent : public MouseWheelEvent
	{
	public:
		MouseWheelDownEvent(_In_ int delta)
			: MouseWheelEvent(delta)
		{

		}

		NPE_DECLARE_TYPE_FN(MouseWheelDownEvent)
	};

	//TODO: Check if Set-/ReleaseCapture work and are in the correct order
	class MouseEnterEvent : public Event
	{
	public:
		MouseEnterEvent(_In_ HWND hWnd)
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

