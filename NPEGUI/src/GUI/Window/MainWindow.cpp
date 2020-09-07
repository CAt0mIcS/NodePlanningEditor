#include "pch.h"
#include "MainWindow.h"

#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/ApplicationEvent.h"

#include "Util/Exceptions.h"


namespace GUI
{
	MainWindow::MainWindow()
	{
		if (!CreateNativeWindow(L"GUI", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT))
		{
			NPE_THROW_WND_EXCEPT(GetLastError());
		}
		
		ShowWindow(m_hWnd, SW_MAXIMIZE);
		Renderer::Get().Init(m_hWnd);
	}

	int MainWindow::ProcessMessage()
	{
		MSG msg{};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		return (int)msg.wParam;
	}

	LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_KILLFOCUS:
		{
			Mouse::ClearStates();
			Keyboard::ClearStates();
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);

			PaintEvent e(hDC, &ps.rcPaint);
			m_EventCallback(e);

			EndPaint(m_hWnd, &ps);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseMoveEvent e({ (float)pt.x, (float)pt.y });
			m_EventCallback(e);

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Left);
			m_EventCallback(e);

			return 0;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Left);
			m_EventCallback(e);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Right);
			m_EventCallback(e);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Right);
			m_EventCallback(e);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Middle);
			m_EventCallback(e);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Middle);
			m_EventCallback(e);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta < 0)
			{
				MouseWheelDownEvent e(delta);
				m_EventCallback(e);
			}
			else
			{ 
				MouseWheelUpEvent e(delta);
				m_EventCallback(e);
			}
			return 0;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (wParam < 256)
			{
				KeyPressedEvent e((unsigned char)wParam);
				m_EventCallback(e);
			}
			return 0;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			if (wParam < 256)
			{
				KeyReleasedEvent e((unsigned char)wParam);
				m_EventCallback(e);
			}
			return 0;
		}
		case WM_CHAR:
		{
			if (wParam > 0 && wParam < 0x10000)
			{
				CharEvent e((unsigned char)wParam);
				m_EventCallback(e);
			}
			return 0;
		}
		case WM_SIZE:
		{
			AppResizeEvent e(GetSize());
			m_EventCallback(e);
			return 0;
		}
		case WM_MOVE:
		{
			AppMoveEvent e(GetPos());
			m_EventCallback(e);
			return 0;
		}
		case WM_CLOSE:
		{
			AppCloseEvent e;
			m_EventCallback(e);
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	
	Util::NPoint MainWindow::GetPos() const
	{
		RECT rc = GetRect();
		return { (float)rc.left, (float)rc.top };
	}

	Util::NSize MainWindow::GetSize() const
	{
		RECT rc = GetRect();
		return { (float)(rc.left + rc.right), (float)(rc.top + rc.bottom) };
	}
	
	RECT MainWindow::GetRect() const
	{
		RECT rc;
		GetWindowRect(m_hWnd, &rc);
		return rc;
	}
}