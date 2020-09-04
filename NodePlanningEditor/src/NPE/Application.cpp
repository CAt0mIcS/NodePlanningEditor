#include "pch.h"
#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"

#include "NPE/Controls/Node.h"

#include "NPE/Util/Debug/Timer.h"


namespace NPE
{
	Application::Application()
		: m_Window({ 800, 600 }, L"NodePlanningEditor", [this](const Event& e) { OnEvent(e); }), m_Zoom(0), m_MousePos{ 0, 0 }
	{
		for (int i = 0; i <= 5000; ++i)
		{
			m_Window.AddControl(new Node(m_Window.Renderer2D, { (float)(rand() / 2), (float)(rand() / 2) }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
		}
		Button::SetOnButtonClickedCallback([this](Button& btn) { OnButtonClicked(btn); });
		Node::SetOnNodeClickedCallback([this](Node& node) { OnNodeClicked(node); });
	}

	int Application::Run()
	{
		return m_Window.ProcessMessage();
	}

	void Application::OnButtonClicked(Button& control)
	{
		//TODO: Implement functions bellow this point
		//WARNING: OnButtonClicked is only called once, thus Mouse::IsLeftPressed() is only checked once and the line only draws when clicking the button
		DrawLine(control);
	}

	void Application::OnNodeClicked(Node& node)
	{
		//TODO: Implement functions bellow this point
		//WARNING: Same as in Application::OnButtonClicked (cannot detect "ControlHoldMouseEvent")
		MoveNodesWithMouse(node);
		ResizeNodes(node);
	}

	void Application::OnEvent(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent && Mouse::IsLeftPressed())
		{
			m_MousePos = Mouse::GetPos();
		}

		MoveNodes(e);
		Zoom(e);
		OnPaint(e);
	}

	void Application::MoveNodesWithMouse(Node& node)
	{
		if (Mouse::GetPos().x < node.GetPos().x + node.GetSize().width && Mouse::GetPos().x > node.GetPos().x + node.GetSize().width - 100)
			return;

		NPoint diff{};
		diff.x = Mouse::GetPos().x - m_MousePos.x;
		diff.y = Mouse::GetPos().y - m_MousePos.y;

		node.MoveBy(diff);

		m_MousePos = Mouse::GetPos();

		m_Window.Renderer2D.BeginDraw();
		m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
		
		for (auto* control : m_Window.GetControls())
			control->Render();
		
		m_Window.Renderer2D.EndDraw();
	}

	void Application::ResizeNodes(Node& node)
	{
		////TODO: Implement function (not required)
		//
		////If mouse is in the lower right corner of the node
		//const NPoint& mousePos = Mouse::GetPos();
		//const NPoint& nodePos = node.GetPos();
		//const NSize& nodeSize = node.GetSize();
		//
		//if (mousePos.x < nodePos.x + nodeSize.width && mousePos.x > nodePos.x + nodeSize.width - 100)
		//{
		//	SetCursor(LoadCursor(NULL, IDC_CROSS));
		//	NPoint diff{};
		//	diff.x = Mouse::GetPos().x - m_MousePos.x;
		//	diff.y = Mouse::GetPos().y - m_MousePos.y;
		//
		//	
		//	node.ResizeTo({ nodeSize.width + diff.x, nodeSize.height + diff.y });
		//
		//	m_Window.Renderer2D.BeginDraw();
		//	for (auto* control : m_Window.GetControls())
		//	{
		//		control->Render();
		//	}
		//	m_Window.Renderer2D.EndDraw();
		//}
		//SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	void Application::DrawLine(const Button& btn)
	{
		if(Mouse::IsLeftPressed())
		{
			m_Window.Renderer2D.BeginDraw();
		
			m_Window.Renderer2D.RenderScene({ 35.0f, 37.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
				control->Render();

			NPoint btnPos = { btn.GetPos().x + btn.GetSize().width / 2, btn.GetPos().y + btn.GetSize().height / 2 };
			m_Window.Renderer2D.RenderLine(btnPos, Mouse::GetPos(), { 255.0f, 255.0f, 255.0f }, 2);
			
			m_Window.Renderer2D.EndDraw();
		}

	}

	void Application::OnPaint(const Event& e)
	{
		if (e.GetType() == EventType::AppPaintEvent)
		{
			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene(NColor{ 35.0f, 38.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
				control->Render();
			m_Window.Renderer2D.EndDraw();
		}
	}

	void Application::MoveNodes(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent && Mouse::IsMiddlePressed())
		{
			m_MousePos = Mouse::GetPos();
		}

		else if (e.GetType() == EventType::MouseMoveEvent && Mouse::IsMiddlePressed())
		{
			NPoint diff{};
			diff.x = Mouse::GetPos().x - m_MousePos.x;
			diff.y = Mouse::GetPos().y - m_MousePos.y;

			int mBuff = 0;
			if (diff.x > mBuff || diff.y > mBuff || diff.x < -mBuff || diff.y < -mBuff)
			{
				m_MousePos.x = Mouse::GetPos().x;
				m_MousePos.y = Mouse::GetPos().y;

				m_Window.Renderer2D.BeginDraw();
				m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
				for (auto* control : m_Window.GetControls())
				{
					control->MoveBy(diff);
					control->Render();
				}
				m_Window.Renderer2D.EndDraw();
			}
		}
	}

	void Application::Zoom(const Event& e)
	{
		NPoint screenCenter = Mouse::GetPos();

		if (e.GetType() == EventType::MouseWheelUpEvent)
		{
			++m_Zoom;

			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
			{
				const NPoint& pos = control->GetPos();
				const NSize& size = control->GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= -m_ZoomFactor;
				newPos.y *= -m_ZoomFactor;

				newSize.width = size.width * m_ResizeFactor;
				newSize.height = size.height * m_ResizeFactor;
				control->MoveBy(newPos);
				control->ResizeTo(newSize);
				control->Render();
			}
			m_Window.Renderer2D.EndDraw();
		}
		else if (e.GetType() == EventType::MouseWheelDownEvent)
		{
			--m_Zoom;
			if (m_Zoom <= -m_ZoomBoundary)
			{
				m_Zoom = -m_ZoomBoundary;
				return;
			}

			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
			for(auto* control : m_Window.GetControls())
			{
				const NPoint& pos = control->GetPos();
				const NSize& size = control->GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= m_ZoomFactor;
				newPos.y *= m_ZoomFactor;

				newSize.width = size.width / m_ResizeFactor;
				newSize.height = size.height / m_ResizeFactor;
				control->MoveBy(newPos);
				control->ResizeTo(newSize);
				control->Render();
			}
			m_Window.Renderer2D.EndDraw();
		}
	}
}


