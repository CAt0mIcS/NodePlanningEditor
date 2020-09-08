#include "pch.h"
#include "Application.h"

#include "GUI/Events/ApplicationEvent.h"

#include "GUI/Controls/TextBox.h"


namespace NPE
{
	Application::Application()
		: m_FileHandler("saves\\save.dbs")
	{
		m_FileHandler.LoadScene(this->GetWindow());
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });
	}

	bool Application::OnEvent(GUI::Control* watched, GUI::Event& e)
	{
		switch (e.GetType())
		{
		case GUI::EventType::AppPaintEvent:
		{
			return OnPaintEvent((GUI::PaintEvent&)e);
		}
		}
		return false;
	}

	bool Application::OnPaintEvent(GUI::PaintEvent& e)
	{
		GUI::Renderer& renderer = GUI::Renderer::Get();
	
		renderer.BeginDraw();
		renderer.RenderScene();

		for (auto* control : m_Window.GetChildren())
		{
			control->Render();
		}

		renderer.EndDraw();
		
		return true;
	}

}


