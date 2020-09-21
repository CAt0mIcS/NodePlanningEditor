#pragma once

#include "Util/Util.h"
#include "GUIBase.h"

#include <string>
#include <vector>
#include <optional>


/**
* QUESTION: 
*	Should I have a function in the Control class to draw or should I use Renderer2D::DrawControl(Control& control);?
*/


namespace GUI
{
	class Event;

	class GUI_API Control
	{
		friend class Renderer2D;
	public:

		/**
		* Enum containing all types of controls
		*/
		enum class Type
		{
			INVALID = 0,
			Node, Button, TextBox, PlainTextEdit,
			MenuItem, MenuBar
		};

	public:
		/**
		* Setter for position
		*
		* @param pos is the control position
		*/
		void SetPos(_In_ const Util::NPoint& pos) { m_Pos = pos; }

		/**
		* Setter for size
		*
		* @param size is the control size
		*/
		void SetSize(_In_ const Util::NSize& size) { m_Size = size; }

		/**
		* Setter for color
		*
		* @param color is the control color
		*/
		void SetColor(_In_ const Util::NColor& color) { m_Color = color; }

		/**
		* Setter for type
		*
		* @param type is the control type
		*/
		void SetType(_In_ Type type) { m_Type = type; }

		/**
		* Getter for the control type
		* 
		* @returns the type of the control
		*/
		Type GetType() const { return m_Type; }

		/**
		* Adds position to current position
		* 
		* @param pos is the position to add
		*/
		void MoveBy(_In_ const Util::NPoint& pos);

		/**
		* Moves the window to the new position
		* 
		* @param pos is the new position
		*/
		void MoveTo(_In_ const Util::NPoint& pos);

		/**
		* Adds size to the current size
		* 
		* @param size is the size to add
		*/
		void ResizeBy(_In_ const Util::NSize& size);

		/**
		* Resizes the window to the new size
		* 
		* @param size is the new size
		*/
		void ResizeTo(_In_ const Util::NSize& size);

		/**
		* Getter for current position
		* 
		* @returns the control's position
		*/
		const Util::NPoint& GetPos() const { return m_Pos; }

		/**
		* Getter for current size
		*
		* @returns the control's size
		*/
		const Util::NSize& GetSize() const { return m_Size; }
		
		/**
		* Getter for current color
		*
		* @returns the control's color
		*/
		const Util::NColor& GetColor() const { return m_Color; }

		/**
		* Getter for child windows
		* 
		* @returns all children of the current widget
		*/
		const std::vector<Control*>& GetChildren() const { return m_Children; }

		/**
		* Getter for current control id
		* 
		* @returns the control's id
		*/
		unsigned int GetId() const { return m_Id; }

		/**
		* Renders the widget
		*/
		virtual bool Render() = 0;

		/**
		* All events of the specific control will be dispatched to this function
		* 
		* @param e is the received event
		* @returns true if the event was handled, else false and the event will be dispatched to the client
		*/
		virtual bool OnEvent(_In_ Event& e) = 0;

		/**
		* Checks if the current control overlaps with other
		* 
		* @param other is the other control
		* @param minDst is the minimum distance allowed between nodes
		* @returns true if this controls overlaps with the other one, false otherwise
		*/
		bool OverlapsWith(_In_ const Control* other, _In_opt_ const Util::NSize& minDst = { 0.0f, 0.0f });

		/**
		* Calculates the layout of a new control
		*
		* @param parentPos is the position of the parent control
		* @param parentSize is the size of the parent control
		* @returns the new position and size of the control
		*/
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize) { return { }; }

		/**
		* Checks if control is in window bounds
		* 
		* @returns true if the control is in the window, false otherwise
		*/
		bool IsInWindow() const;

		/**
		* Virtual destructor of control
		*/
		virtual ~Control() = default;

		/**
		* Determines which control should receive the event
		*
		* @param e is the event received by the window procedure
		* @returns the controls which should receive the event
		*/
		Control* GetEventReceiver(_In_ Event& e);

		/**
		* Sets new focus to this
		*/
		void SetFocus() { s_Focus = this; }
		
		/**
		* Getter for focused control
		* 
		* @returns the control which has focus
		*/
		Control* GetFocus() const { return s_Focus; }
		
		/**
		* Checks if obj has focus
		* 
		* @returns true if obj has focus, false otherwise
		*/
		bool HasFocus() { return this == s_Focus; }

	protected:
		/**
		* Protected Control constructor
		* 
		* @param parent is the parent window
		*/
		Control(_In_opt_ Control* parent = nullptr);

	protected:
		Util::NPoint m_Pos;
		Util::NSize m_Size;
		Util::NColor m_Color;

		Control* m_Parent;
		std::vector<Control*> m_Children;

		static Control* s_Focus;

	private:
		unsigned int m_Id;
		Type m_Type;

		static unsigned int s_NextId;
	};
}


