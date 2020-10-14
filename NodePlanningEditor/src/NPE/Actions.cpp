#include "pch.h"
#include "Actions.h"

#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/Node.h"

#include "GUI/Handlers/Mouse.h"

#include "Util/Logger.h"

#include "Util/Util.h"


namespace NPE
{
	Actions::Actions(Application& app)
		: m_App(app)
	{

	}

	void Actions::MoveCamera(const Util::NPoint& diff, GUI::Control::ContainerType& controls)
	{
		for (auto* control : controls)
		{
			if(control->GetType() != GUI::Control::Type::Tab)
				control->MoveBy(diff);
		}
	}

	void Actions::SpawnNode(GUI::MainWindow& win, float width, float height)
	{
		GUI::Node* control = win.AddControl<GUI::Node>(new GUI::Node(&win));
		control->SetColor(Constants::g_DefaultNodeColor);
		control->SetSize({ width, height });
		control->SetPos(GUI::Mouse::GetTransformedPos());
		CreateNodeTemplate(control);

		NPE_LOG("Created Node: \nPos:\tx={0} y={1}\nSize:\twidth={2} height={3}", control->GetPos().x, control->GetPos().y, control->GetSize().width, control->GetSize().height);
	}

	void Actions::MoveNodes(GUI::Node* node, const Util::NPoint& oldMousePos)
	{
		Util::NPoint diff = GUI::Mouse::GetTransformedPos() - oldMousePos;
		node->MoveBy(diff);
	}

	void Actions::ZoomIn(GUI::Control::ContainerType& controls)
	{
		Util::NPoint center = GUI::Mouse::GetTransformedPos();

		GUI::Renderer::Get().SetScale(GUI::Renderer::Get().GetScale() * Util::NSize{ s_ScaleFactor, s_ScaleFactor });
	}

	void Actions::ZoomOut(GUI::Control::ContainerType& controls)
	{
		Util::NPoint center = GUI::Mouse::GetTransformedPos();
		
		float scaleFactor = 1.0f / s_ScaleFactor;
		GUI::Renderer::Get().SetScale(GUI::Renderer::Get().GetScale() * Util::NSize{ scaleFactor, scaleFactor });
	}
	
	void Actions::RenderLines(const std::vector<Line>& lines)
	{
		for (const Line& line : lines)
		{
			float x2 = GUI::Mouse::GetTransformedPos().x;
			float y2 = GUI::Mouse::GetTransformedPos().y;
			if (line.second)
			{
				x2 = line.second->GetPos().x + line.second->GetSize().width / 2;
				y2 = line.second->GetPos().y + line.second->GetSize().height / 2;
			}
			
			float x1 = line.first->GetPos().x + line.first->GetSize().width / 2;
			float y1 = line.first->GetPos().y + line.first->GetSize().height / 2;

			float fontsize = line.first->GetSize().width / 3;
			if (fontsize == 0) fontsize = 1;

			GUI::Renderer::Get().RenderLine({ x1, y1 }, { x2, y2 }, Constants::g_DefaultLineColor, fontsize);
		}
	}
	
	void Actions::FinnishLineDrawing(std::vector<Line>& lines, const GUI::Control::ContainerType& controls)
	{
		if (lines.size() == 0)
			return;
			
		for (auto* control : controls)
		{
			if (control->IsInWindow() && control->GetType() == GUI::Control::Type::Node)
			{
				GUI::Button* btn = (GUI::Button*)control->GetChildren()[2];
				if (GUI::Mouse::IsOnTransformedControl(btn))
				{
					lines[lines.size() - 1].second = btn;
					break;
				}
			}
		}
		if (lines[lines.size() - 1].second == nullptr)
		{
			lines.erase(lines.end() - 1);
		}
		else
		{
			auto line = lines[lines.size() - 1];
			
			if (line.first->GetId() == line.second->GetId())
			{
				NPE_LOG("Cannot connect line to the same point");
				return;
			}

			//Check if connection already exists between these two buttons
			for (auto it = lines.begin(); it != lines.end() - 1; ++it)
			{
				if ((line.first->GetId() == it->first->GetId() && line.second->GetId() == it->second->GetId()) 
					|| (line.first->GetId() == it->second->GetId() && line.second->GetId() == it->first->GetId()))
				{
					NPE_LOG("Didn't connect Line because a connection between these two points already exists");
					lines.erase(lines.end() - 1);
					return;
				}
			}
			
			NPE_LOG("Connected Line");
		}
	}
	
	void Actions::EraseLine(std::vector<Line>& lines, const Util::NPoint& oldMousePos)
	{
		auto linesIntersect = [](const Util::NPoint& p1, const Util::NPoint& p2, const Util::NPoint& q1, const Util::NPoint& q2)
		{
			return (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x))
				* ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) < 0)
				&&
				(((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x))
					* ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) < 0);
		};

		for (unsigned int i = 0; i < lines.size(); ++i)
		{
			if (linesIntersect(lines[i].first->GetPos(), lines[i].second->GetPos(), oldMousePos, GUI::Mouse::GetTransformedPos()))
			{
				lines.erase(lines.begin() + i);
			}
		}
	}
	
	void Actions::DeleteNode(GUI::Node* watched, GUI::Control::ContainerType& controls, std::vector<Line>& lines)
	{
		for (unsigned int i = 0; i < controls.size(); ++i)
		{
			if (controls[i]->GetType() == GUI::Control::Type::Node && controls[i]->GetId() == watched->GetId())
			{
				GUI::Button* lineBtn = (GUI::Button*)controls[i]->GetChildren()[2];

				for (unsigned int j = 0; j < lines.size(); ++j)
				{
					if (lines[j].first == lineBtn || lines[j].second == lineBtn)
					{
						lines.erase(lines.begin() + j);
					}
				}

				delete controls[i];
				controls.erase(controls.begin() + i);
				break;
			}
		}
	}
	
	void Actions::CreateNodeTemplate(GUI::Node* node)
	{
		GUI::TextBox* txtbox = node->AddChild<GUI::TextBox>(new GUI::TextBox(node));
		float xOffsetTxt = node->GetSize().width / 22.5f;
		float txtX = node->GetPos().x + xOffsetTxt;

		float yOffsetTxt = node->GetSize().height / 7.0f;
		float txtY = node->GetPos().y + yOffsetTxt;

		float txtWidth = node->GetSize().width - (xOffsetTxt * 2);

		//20% of Node's height is this TextBox
		float txtPercentOfNode = 0.2f;
		float txtHeight = node->GetSize().height * txtPercentOfNode;

		txtbox->SetColor(Constants::g_DefaultTextBoxColor);
		txtbox->SetSize({ txtWidth, txtHeight });
		txtbox->SetPos({ txtX, txtY });
		txtbox->SetFontSize((node->GetSize().width + node->GetSize().height) / 24.3f);


		txtbox = node->AddChild<GUI::TextBox>(new GUI::TextBox(node));
		yOffsetTxt = node->GetSize().height / 7.0f;
		txtY = node->GetPos().y + yOffsetTxt + (node->GetSize().height / 3.5f);

		//40% of Node's height is this TextBox
		txtPercentOfNode = 0.4f;
		txtHeight = node->GetSize().height * txtPercentOfNode;

		txtbox->SetColor(Constants::g_DefaultTextBoxColor);
		txtbox->SetSize({ txtWidth, txtHeight });
		txtbox->SetPos({ txtX, txtY });
		txtbox->SetFontSize((node->GetSize().width + node->GetSize().height) / 48.6f);
		txtbox->SetMultiline(true);


		GUI::Button* btn = node->AddChild<GUI::Button>(new GUI::Button(node));
		btn->SetColor(Constants::g_DefaultLineButtonColor);

		float middleOfNodeX = node->GetPos().x + (node->GetSize().width / 2);

		float btnWidthAndHeight = std::min(node->GetSize().width, node->GetSize().height) / 12;
		float btnX = middleOfNodeX - (btnWidthAndHeight / 2);

		float yOffsetBtn = node->GetSize().height / 56;
		float btnY = node->GetPos().y + yOffsetBtn;

		btn->SetPos({ btnX, btnY });
		btn->SetSize({ btnWidthAndHeight, btnWidthAndHeight });
	}
}


