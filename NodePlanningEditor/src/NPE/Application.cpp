#include "pch.h"
#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/ApplicationEvent.h"
#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Node.h"
#include "GUI/Controls/Button.h"

#include "GUI/Handlers/Mouse.h"

#include "Util/Exceptions.h"

#include "GUI/Window/FileWindow.h"


#include "Util/Debug/Timer.h"

/**
* QUESTION:
*	Use strings or wstrings
*	Is it ok to use "using namespace GUI" in source files
*	How to design a library with include directories (Example QRD) --> Good project file structure (CMake structure?)
*/

//#define NPE_DEBUG_DISABLE_AUTOSAVE
//#define NPE_DEBUG_RANDOM_NODES



namespace NPE
{
	Application::Application()
		: m_Actions(*this), m_MousePos{}, m_HandleControls{}, m_Lines{}, m_DrawLines(false), m_NeedsToSave(false)
	{
		NPE_THROW_WND_EXCEPT(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

		m_FileHandler.CreateOrLoadSave();

		NPE_LOG("Start of loading scene...\n");
		m_FileHandler.LoadScene(m_Window, m_Lines);

		#ifdef NPE_DEBUG_RANDOM_NODES
			auto& controls = m_Window.GetControls();
			controls.clear();
			m_Lines.clear();

			for (int i = 0; i < 5000; ++i)
			{
				auto* node = m_Window.AddControl<GUI::Node>(new GUI::Node(&m_Window));
				node->SetPos({ (float)rand(), (float)rand() });
				node->SetSize({ s_NodeWidth, s_NodeHeight });
				node->SetColor(Constants::g_DefaultNodeColor);
				m_Actions.CreateNodeTemplate(node);
				((GUI::TextBox*)node->GetChildren()[0])->SetText(L"Hello World!");
				((GUI::TextBox*)node->GetChildren()[1])->SetText(L"Hello World2!");
			}
		#endif

		NPE_LOG("Finnished loading scene\n");
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });
		m_Window.CreateTimer(20000, true);
		SetWindowSavedText();
	}

	bool Application::OnEvent(GUI::Control* watched, GUI::Event& e)
	{
		switch (e.GetType())
		{
		case GUI::EventType::MouseMoveEvent:
		{
			return OnMouseMove(watched, (GUI::MouseMoveEvent&)e);
		}
		case GUI::EventType::MouseButtonPressedEvent:
		{
			return OnMouseButtonPressed(watched, (GUI::MouseButtonPressedEvent&)e);
		}
		case GUI::EventType::MouseButtonReleasedEvent:
		{
			return OnMouseButtonReleased(watched, (GUI::MouseButtonReleasedEvent&)e);
		}
		case GUI::EventType::KeyPressedEvent:
		{
			return OnKeyPressed(watched, (GUI::KeyPressedEvent&)e);
		}
		case GUI::EventType::CharEvent:
		{
			return OnChar(watched, (GUI::CharEvent&)e);
		}
		case GUI::EventType::MouseWheelUpEvent:
		{
			return OnMouseWheelUp(watched, (GUI::MouseWheelUpEvent&)e);
		}
		case GUI::EventType::MouseWheelDownEvent:
		{
			return OnMouseWheelDown(watched, (GUI::MouseWheelDownEvent&)e);
		}
		case GUI::EventType::TimerEvent:
		{
			return OnTimer(watched, (GUI::TimerEvent&)e);
		}
		case GUI::EventType::AppPaintEvent:
		{
			return OnPaintEvent(watched, (GUI::PaintEvent&)e);
		}
		case GUI::EventType::AppCloseEvent:
		{
			return OnClose(watched, (GUI::AppCloseEvent&)e);
		}
		}
		return false;
	}

	bool Application::OnMouseMove(GUI::Control* watched, GUI::MouseMoveEvent& e)
	{
		if (GUI::Mouse::IsMiddlePressed())
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			
			const Util::NPoint diff = GUI::Mouse::GetPos() - m_MousePos;
			m_Actions.MoveCamera(diff, m_Window.GetControls());
			m_MousePos = GUI::Mouse::GetPos();
			m_Window.PostRedraw();

			SetNeedsToSaveAndWindowTitle(true);
			return true;
		}
		else if (GUI::Mouse::IsLeftPressed())
		{
			//Redraw caret when selecting
			if (watched->HasFocus() && watched->GetType() == GUI::Control::Type::TextBox)
			{
				watched->GetParent()->PostRedraw();
				return true;
			}
			else if (m_DrawLines)
			{
				m_Window.PostRedraw();
				return true;
			}
			else if (m_HandleControls.draggingNode)
			{
				m_Actions.MoveNodes(m_HandleControls.draggingNode, m_MousePos);
				m_MousePos = GUI::Mouse::GetPos();
				m_Window.PostRedraw();

				SetNeedsToSaveAndWindowTitle(true);
				return true;
			}
		}
		return false;
	}

	bool Application::OnMouseButtonPressed(GUI::Control* watched, GUI::MouseButtonPressedEvent& e)
	{
		m_MousePos = GUI::Mouse::GetPos();
		if (e.GetButton() == GUI::MouseButton::Left)
		{
			if (watched->GetType() == GUI::Control::Type::TextBox)
			{
				watched->GetParent()->PostRedraw();
			}
			else if (watched->GetType() == GUI::Control::Type::Button)
			{
				m_Lines.emplace_back((GUI::Button*)watched, nullptr);
				m_DrawLines = true;
				return true;
			}
			else if (watched->GetType() == GUI::Control::Type::Node)
			{
				m_HandleControls.draggingNode = (GUI::Node*)watched;
				return true;
			}
		}
		return false;
	}

	bool Application::OnMouseButtonReleased(GUI::Control* watched, GUI::MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == GUI::MouseButton::Left)
		{
			if (!m_HandleControls.draggingNode && m_DrawLines)
			{
				m_Actions.FinnishLineDrawing(m_Lines, m_Window.GetControls());
				m_Window.PostRedraw();
				SetNeedsToSaveAndWindowTitle(true);
				m_DrawLines = false;
			}

			m_HandleControls.draggingNode = nullptr;
			return true;
		}
		else if (e.GetButton() == GUI::MouseButton::Right)
		{
			m_Actions.EraseLine(m_Lines, m_MousePos);
			m_Window.PostRedraw();
			SetNeedsToSaveAndWindowTitle(true);

			if (GUI::Mouse::IsOnControl(watched) && watched->GetType() == GUI::Control::Type::Node)
			{
				m_Actions.DeleteNode((GUI::Node*)watched, m_Window.GetControls(), m_Lines);
				SetNeedsToSaveAndWindowTitle(true);
				m_Window.PostRedraw();
			}

			return true;
		}
		return false;
	}

	bool Application::OnKeyPressed(GUI::Control* watched, GUI::KeyPressedEvent& e)
	{
		//Create new Node shortcut
		if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('A'))
		{
			m_Actions.SpawnNode(m_Window, s_NodeWidth, s_NodeHeight);
			m_Window.PostRedraw();
			SetNeedsToSaveAndWindowTitle(true);
			return true;
		}
		//Save to shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed(VK_SHIFT) && GUI::Keyboard::IsKeyPressed('S'))
		{
			SaveScene(true);
			return true;
		}
		//Save shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('S'))
		{
			SaveScene();
			return true;
		}
		//Load scene from new file shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('O'))
		{
			if (m_NeedsToSave)
			{
				int result = PromptSaveChangesMsgBox();
				if (result == IDCANCEL)
					return false;
			}

			bool result = m_FileHandler.OpenScene(m_Window, m_Lines);
			m_Window.PostRedraw();
			return result;
		}

		//Redraw caret when moving with arrow keys.
		if (watched->HasFocus() && watched->GetType() == GUI::Control::Type::TextBox)
		{
			watched->GetParent()->PostRedraw();
			return true;
		}

		return false;
	}

	bool Application::OnChar(GUI::Control* watched, GUI::CharEvent& e)
	{
		if (m_Window.HasFocus())
		{
			float incr = 30.0f;
			switch (e.GetKeyCode())
			{
			case 'w':
				m_Actions.MoveCamera({ 0.0f, incr }, m_Window.GetControls());
				break;
			case 'a':
				m_Actions.MoveCamera({ incr, 0.0f }, m_Window.GetControls());
				break;
			case 's':
				m_Actions.MoveCamera({ 0.0f, -incr }, m_Window.GetControls());
				break;
			case 'd':
				m_Actions.MoveCamera({ -incr, 0.0f }, m_Window.GetControls());
				break;
			default:
				return false;
			}
			
			m_Window.PostRedraw();
			SetNeedsToSaveAndWindowTitle(true);
			return true;
		}
		//Render caret on key press
		else if (watched->GetType() == GUI::Control::Type::TextBox && watched->HasFocus())
		{
			SetNeedsToSaveAndWindowTitle(true);
			watched->GetParent()->PostRedraw();
		}

		return false;
	}

	bool Application::OnMouseWheelUp(GUI::Control* watched, GUI::MouseWheelUpEvent& e)
	{
		m_Actions.ZoomIn(m_Window.GetControls());
		SetNeedsToSaveAndWindowTitle(true);
		m_Window.PostRedraw();
		return true;
	}

	bool Application::OnMouseWheelDown(GUI::Control* watched, GUI::MouseWheelDownEvent& e)
	{
		m_Actions.ZoomOut(m_Window.GetControls());
		SetNeedsToSaveAndWindowTitle(true);
		m_Window.PostRedraw();
		return true;
	}

	bool Application::OnTimer(GUI::Control* watched, GUI::TimerEvent& e)
	{
		switch (e.GetTimer()->GetId())
		{
		#ifndef NPE_DEBUG_DISABLE_AUTOSAVE
			case s_TimerAutosaveId:
			{
				//TODO: Implement alert to user that an autosave is comming
				if (m_NeedsToSave)
					m_FileHandler.SaveScene(m_Window.GetControls(), m_Lines);
				return true;
			}
		#endif
		}

		return false;
	}

	Application::~Application()
	{
		CoUninitialize();
	}

	bool Application::OnPaintEvent(GUI::Control* watched, GUI::PaintEvent& e)
	{
		GUI::Renderer& renderer = GUI::Renderer::Get();
		renderer.BeginDraw();

		// Store current transform to be able to set it again at the end of this function
		DWrite::Matrix prevTransform = renderer.GetTransform();

		if (watched->GetType() == GUI::Control::Type::Window)
		{
			// Render the entire scene if the window requested a redraw
			renderer.RenderScene();

			D2D1::Matrix3x2F pageTransform = *(D2D1::Matrix3x2F*)&GUI::Renderer::Get().GetViewMatrix();
			GUI::Renderer::Get().SetTransform(*(DWrite::Matrix*)&pageTransform);
		}
		
		watched->Render();
		m_Actions.RenderLines(m_Lines);

		// Restore previous transform
		renderer.SetTransform(prevTransform);
		
		renderer.EndDraw();
		return true;
	}

	bool Application::OnClose(GUI::Control* watched, GUI::AppCloseEvent& e)
	{
		// Display a message box if the scene has unsaved changes
		if (m_NeedsToSave)
		{
			int result = PromptSaveChangesMsgBox();
			if (result == IDCANCEL)
			{
				return true;
			}
			else
			{
				NPE_LOG("Unsaved changes discarded due to user input");
			}
		}
		NPE_LOG(" ****** Log finnished for NodePlanningEditor ****** ");
		return false;
	}

	int Application::PromptSaveChangesMsgBox()
	{
		int result = MessageBox(m_Window.GetNativeWindow(), L"Save changes to the scene?", L"Unsaved changes", MB_YESNOCANCEL);
		if (result == IDYES)
		{
			SaveScene();
		}
		return result;
	}

	void Application::SaveScene(bool saveToNewLocation)
	{
		SetWindowSavedText();
		m_FileHandler.SaveScene(m_Window.GetControls(), m_Lines, saveToNewLocation);
		SetNeedsToSaveAndWindowTitle(false);
	}

	void Application::SetNeedsToSaveAndWindowTitle(bool needsToSave)
	{
		m_NeedsToSave = needsToSave;
		if (m_NeedsToSave)
			SetWindowSaveText();
		else
			SetWindowSavedText();
	}

	void Application::SetWindowSaveText()
	{
		std::ostringstream oss;
		oss << "NodePlanningEditor        " << m_FileHandler.GetFileName() << '*';
		m_Window.SetTitle(Util::MultiByteToWideChar(oss.str()));
	}

	void Application::SetWindowSavedText()
	{
		std::ostringstream oss;
		oss << "NodePlanningEditor        " << m_FileHandler.GetFileName();
		m_Window.SetTitle(Util::MultiByteToWideChar(oss.str()));
	}

}


