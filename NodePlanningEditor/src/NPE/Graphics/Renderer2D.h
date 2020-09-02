#pragma once

#include "pch.h"

#include "NPE/Controls/NodeRect.h"


namespace NPE
{
	class Renderer2D
	{
	public:
		Renderer2D(HWND hWnd);
		Renderer2D() = default;

		void Init(HWND hWnd);

		void Draw();

		void DrawNode(const NodeRect& node);

	private:
		void InitGraphicsResources();
		void InitDPIScale();

	private:
		Microsoft::WRL::ComPtr<ID2D1Factory>			m_pFactory;
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>	m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_pBrush;

		HWND m_hWnd;
		float m_DPIScale;
	};
}




