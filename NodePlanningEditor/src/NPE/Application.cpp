#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"

#include "NPE/Handlers/MouseEvent.h"

#include "NPE/Controls/Node.h"
#include "NPE/Controls/Button.h"
#include "NPE/Controls/TextBox.h"

#include "NPE/Util/Debug/Timer.h"


namespace NPE
{
	Application::Application()
		: m_Database("saves\\save.dbs", 3), m_Window({ 800, 600 }, L"NodePlanningEditor", [this](const Event& e) { OnEvent(e); }), m_MousePos{ 0, 0 }, m_Zoom(0)
	{

		//for (int i = 0; i < 1000; ++i)
		//{
		//	m_Window.AddControl(new Node(m_Window.Renderer2D, { (float)(rand() / 2), (float)(rand() / 2) }, { 450, 280}, { 15.0f, 17.0f, 19.0f }));
		//}
		LoadFile();

		Button::SetOnButtonClickedCallback([this](Button& btn) { OnButtonClicked(btn); });
		Node::SetOnNodeClickedCallback([this](Node& node) { OnNodeClicked(node); });
		TextBox::SetOnTextBoxClickedCallback([this](TextBox& txtBox) { OnTextBoxClicked(txtBox); });
	}

	Application::~Application()
	{
		SaveFile();
	}

	void Application::SaveFile()
	{
		//clear save file
		m_Database.DeleteTable("NodeInfo");
		m_Database.DeleteTable("SceneInfo");
		m_Database.DeleteTable("Lines");
		m_Database.WriteDb();

		//Table creation and setup
		QRD::Table& tbNodeInfo = m_Database.CreateTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Database.CreateTable("SceneInfo");
		QRD::Table& tbLines = m_Database.CreateTable("Lines");

		tbNodeInfo.AddField<QRD::NUMBER>("x");
		tbNodeInfo.AddField<QRD::NUMBER>("y");
		tbNodeInfo.AddField<QRD::NUMBER>("width");
		tbNodeInfo.AddField<QRD::NUMBER>("height");

		tbLines.AddField<QRD::NUMBER>("ID2");
		tbLines.AddField<QRD::NUMBER>("ID1");

		tbSceneInfo.AddField<QRD::NUMBER>("zoom");
		
		for (auto* control : m_Window.GetControls())
		{
			const auto& pos = control->GetPos();
			const auto& size = control->GetSize();
			tbNodeInfo.AddRecord(pos.x, pos.y, size.width, size.height);
		}
		
		for (std::pair<Button*, Button*>& line : m_Window.GetLines())
		{
			tbLines.AddRecord(line.first->GetId(), line.second->GetId());
		}
		
		tbSceneInfo.AddRecord(m_Zoom);
		
		m_Database.ExitDb();
	}

	void Application::LoadFile()
	{
		QRD::Table& tbNodeInfo = m_Database.GetTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Database.GetTable("SceneInfo");
		QRD::Table& tbLines = m_Database.GetTable("Lines");

		m_Zoom = std::stoi(tbSceneInfo.GetRecords()[0].GetRecordData()[0]);
		for (auto& record : tbNodeInfo.GetRecords())
		{
			auto& data = record.GetRecordData();
			m_Window.AddControl(new Node(m_Window.Renderer2D, { std::stof(data[0]), std::stof(data[1]) }, { std::stof(data[2]), std::stof(data[3]) }, { 15.0f, 17.0f, 19.0f }));
		}

		for (auto& record : tbLines.GetRecords())
		{
			auto& data = record.GetRecordData();

			std::pair<Button*, Button*> line;

			for (auto* control : m_Window.GetControls())
			{
				for (auto* child : control->GetChildren())
				{
					if (child->GetId() == std::stoi(data[0]))
					{
						line.first = (Button*)child;
						break;
					}
					else if (child->GetId() == std::stoi(data[1]))
					{
						line.second = (Button*)child;
						break;
					}
				}
				if (line.first && line.second)
					break;
			}

			if (line.first && line.second)
			{
				m_Window.AddLine(line);
			}
		}
	}

	int Application::Run()
	{
		return m_Window.ProcessMessage();
	}

	Button* lineDrawOriginBtn = nullptr;
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
		//NoOverlappingNodes(node);
	}

	void Application::OnTextBoxClicked(TextBox& txtBox)
	{
		if (Mouse::IsLeftPressed())
		{

		}
	}

	void Application::OnEvent(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent && (Mouse::IsLeftPressed() || Mouse::IsRightPressed()))
		{
			m_MousePos = Mouse::GetPos();
		}
		
		CreateOrDeleteLine(e);
		MoveNodes(e);
		Zoom(e);
		OnPaint(e);
		NewNode(e);
		SaveShortcut(e);
	}

	void Application::NoOverlappingNodes(Node& node)
	{
		if (Mouse::IsLeftPressed())
		{
			for (const auto* control : m_Window.GetControls())
			{
				if (node.GetId() != control->GetId() && node.IsInWindow() && node.OverlapsWith(control, { 10.0f, 10.0f }))
				{
					MessageBox(NULL, L"OVERLAPPING", L"", NULL);
				}
			}
		}
	}

	void Application::NewNode(const Event& e)
	{
		if (e.GetType() == EventType::KeyPressedEvent)
		{
			//Shift + A
			if (Keyboard::IsKeyPressed(VK_SHIFT) && Keyboard::IsKeyPressed(65))
			{
				float width = 450.0f;
				float height = 280.0f;

				if (m_Zoom > 0)
				{
					for (int i = 0; i < m_Zoom; ++i)
					{
						width *= m_ResizeFactor;
						height *= m_ResizeFactor;
					}
				}
				else
				{
					for (int i = m_Zoom; i < 0; ++i)
					{
						width /= m_ResizeFactor;
						height /= m_ResizeFactor;
					}
				}

				Control* control = m_Window.AddControl(new Node(m_Window.Renderer2D, Mouse::GetPos(), { width, height }, { 15.0f, 16.0f, 19.0f }));
				
				m_Window.Renderer2D.BeginDraw();
				control->Render();
				m_Window.Renderer2D.EndDraw();
			}
		}
	}

	void Application::MoveNodesWithMouse(Node& node)
	{
		NPoint diff{};
		diff.x = Mouse::GetPos().x - m_MousePos.x;
		diff.y = Mouse::GetPos().y - m_MousePos.y;

		node.MoveBy(diff);

		m_MousePos = Mouse::GetPos();

		m_Window.Renderer2D.BeginDraw();
		m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
		
		for (auto* control : m_Window.GetControls())
			control->Render();
		m_Window.RenderLines();
		
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

	void Application::DrawLine(Button& btn)
	{
		if(Mouse::IsLeftPressed())
		{
			m_Window.Renderer2D.RenderScene({ 35.0f, 37.0f, 40.0f });
			m_Window.Renderer2D.BeginDraw();
		
			for (auto* control : m_Window.GetControls())
				control->Render();

			NPoint btnPos = { btn.GetPos().x + btn.GetSize().width / 2, btn.GetPos().y + btn.GetSize().height / 2 };
			m_Window.Renderer2D.RenderLine(btnPos, Mouse::GetPos(), { 160.0f, 160.0f, 160.0f }, (unsigned int)btn.GetSize().width / 3);
			lineDrawOriginBtn = &btn;

			m_Window.RenderLines();

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
			m_Window.RenderLines();
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
				m_Window.RenderLines();
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
			m_Window.RenderLines();
			m_Window.Renderer2D.EndDraw();
		}
		else if (e.GetType() == EventType::MouseWheelDownEvent)
		{
			--m_Zoom;
			if (m_Zoom <= m_ZoomBoundary)
			{
				m_Zoom = m_ZoomBoundary;
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
			m_Window.RenderLines();
			m_Window.Renderer2D.EndDraw();
		}
	}

	void Application::CreateOrDeleteLine(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonReleasedEvent)
		{
			MouseButtonReleasedEvent& eReleased = (MouseButtonReleasedEvent&)e;
			if (eReleased.GetButton() == MouseButton::Left)
			{
				for (auto* control : m_Window.GetControls())
				{
					if (control->IsInWindow() && control->GetType() == Control::Type::Node)
					{
						Button* btn = (Button*)control->GetChildren()[0];
						if (Mouse::IsOnControl(btn))
						{
							m_Window.AddLine({ lineDrawOriginBtn, btn });
							break;
						}
					}
				}
			}
			//Delete node (get line function and check if mouse pos on line)
			else if (eReleased.GetButton() == MouseButton::Right)
			{
				auto linesIntersect = [](const NPoint& p1, const NPoint& p2, const NPoint& q1, const NPoint& q2)
				{
					return (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x))
						* ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) < 0)
						&&
						(((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x))
							* ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) < 0);
				};

				for (unsigned int i = 0; i < m_Window.GetLines().size(); ++i)
				{
					if (linesIntersect(m_Window.GetLines()[i].first->GetPos(), m_Window.GetLines()[i].second->GetPos(), m_MousePos, Mouse::GetPos()))
					{
						m_Window.GetLines().erase(m_Window.GetLines().begin() + i);
					}
				}
				m_Window.Renderer2D.BeginDraw();
				m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
				for (auto* control : m_Window.GetControls())
					control->Render();
				m_Window.RenderLines();
				m_Window.Renderer2D.EndDraw();
			}
		}
	}

	void Application::SaveShortcut(const Event& e)
	{
		if (Keyboard::IsKeyPressed(VK_CONTROL) && Keyboard::IsKeyPressed(83))
		{
			SaveFile();
		}
	}
}


