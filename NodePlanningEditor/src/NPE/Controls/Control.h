#pragma once

#include "NPE/Util/Props.h"


namespace NPE
{
	class Control
	{
	public:
		enum class Type
		{
			INVALID = 0,
			Node,
		};

	public:
		unsigned short GetId() const { return m_Id; }
		Type GetType() const { return m_Type; }

		void MoveBy(const NPoint& pos);
		void MoveTo(const NPoint& pos);

		NPoint GetPos() const { return m_Pos; }
		NSize GetSize() const { return m_Size; }

		HWND GetNativeWindow() const { return m_hWnd; }

	protected:
		Control(const Type type, const NPoint& pos, const NSize& size);

	protected:
		HWND m_hWnd;
		NPoint m_Pos;
		NSize m_Size;

	private:
		static unsigned short m_NextId;
		unsigned short m_Id;
		Type m_Type;
	};

	inline unsigned short Control::m_NextId = 0;
}



