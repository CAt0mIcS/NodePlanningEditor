#pragma once

#include "GUIBase.h"
#include "Renderer.h"


namespace GUI
{
	/**
	* QUESTION:
	*	Wherer should I put this?
	*/

	/// <summary>
	/// Contains all necessary information to render a text
	/// </summary>
	struct GUI_API NText
	{
		std::wstring text = L"";
		Util::NPoint pos = { 0.0f, 0.0f, };
		Util::NSize size = { 0.0f, 0.0f };
		Util::NColor color = { 255.0f, 255.0f, 255.0f };
		float fontSize = 20;
		std::wstring fontFamily = L"Consolas";
		std::wstring localeName = L"";
		DWrite::FontWeight fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
		DWrite::FontStyle fontStyle = DWRITE_FONT_STYLE_NORMAL;
		DWrite::FontStretch fontStretch = DWRITE_FONT_STRETCH_NORMAL;
		Direct2D::DrawTextOptions options = D2D1_DRAW_TEXT_OPTIONS_NONE;
		DWrite::MeasuringMode measuringMode = DWRITE_MEASURING_MODE_NATURAL;
	};

	class GUI_API TextRenderer
	{
	public:
		/// <summary>
		/// TextRenderer Constructor
		/// </summary>
		TextRenderer();

		/// <summary>
		/// Getter for static TextRenderer (singelton design)
		/// </summary>
		/// <returns>The TextRenderer</returns>
		static TextRenderer& Get() { return *s_TextRenderer; }
		
		/// <summary>
		/// Initializes the TextRenderer
		/// </summary>
		void Init();

		/// <summary>
		/// Returns the text metrics for the current text
		/// </summary>
		/// <param name="text">Is the text for which to get the metrics for</param>
		/// <returns>The text metrics to text</returns>
		DWrite::TextMetrics GetTextMetrics(
			_In_ const NText& text
		);

		/// <summary>
		/// Getter for the line metrics of the current text
		/// </summary>
		/// <param name="text">Is the text for which to get the metrics for</param>
		/// <returns>The line metrics to the current text</returns>
		std::vector<DWrite::LineMetrics> GetLineMetrics(
			_In_ const NText& text
		);

		/// <summary>
		/// Getter for cluster metrics for the current text
		/// </summary>
		/// <param name="text">Is the text to get the metrics for</param>
		/// <param name="clusterCount">Will be filled with the amount of clusters</param>
		/// <returns>The cluster metrics for the current text</returns>
		std::vector<DWrite::ClusterMetrics> GetClusterMetrics(
			_In_ const NText& text, 
			_Out_ unsigned int* clusterCount
		);

		/// <summary>
		/// Getter for overhang metrics for the current text
		/// </summary>
		/// <param name="text">Is the text to get the metrics for</param>
		/// <returns>The overhang metrics for the current text</returns>
		DWrite::OverhangMetrics GetOverhangMetrics(
			_In_ const NText& text
		);

		/// <summary>
		/// Renders text to specified bounding rect
		/// </summary>
		/// <param name="text">Is the NText structure containing all text info</param>
		void RenderText(
			_In_ const NText& text
		);

		/// <summary>
		/// Creates IDWriteTextLayout and calls its HitTestPoint function
		/// </summary>
		/// <param name="text">Is the current text</param>
		/// <param name="isTrailingHit">Will be filled by IDWriteTextLayout::HitTestPoint</param>
		/// <param name="isInside">Will be filled by IDWRiteTextLayout::HitTestPoint</param>
		/// <returns>The hit test metrics</returns>
		DWrite::HitTestMetrics HitTestPoint(
			_In_ const NText& text, 
			_Out_ BOOL* isTrailingHit, 
			_Out_ BOOL* isInside
		);

		/// <summary>
		/// Creates IDWriteTextLayout and calls its HitTestTextPosition function
		/// </summary>
		/// <param name="text">Is the current text</param>
		/// <param name="textPos">Is the text position to look for</param>
		/// <param name="isTrailingHit">Is received by HitTestPoint</param>
		/// <param name="caretX">Is the x output position</param>
		/// <param name="caretY">Is the y output position</param>
		/// <returns>The hit test text metrics</returns>
		DWrite::HitTestMetrics HitTestTextPosition(
			_In_ const NText& text, 
			_In_ unsigned int textPos, 
			_In_ BOOL isTrailingHit, 
			_Out_ float* caretX, 
			_Out_ float* caretY
		);
		
		/// <summary>
		/// Creates a IDWriteTextLayout with the text
		/// </summary>
		/// <param name="text">Is the text of the new layout</param>
		/// <param name="ppLayout">Contains newly created TextLayout, or null in case of failure</param>
		void CreateTextLayout(
			_In_ const NText& text, 
			_Out_ IDWriteTextLayout** ppLayout
		);

	private:
		/// <summary>
		/// Creates all graphics resources
		/// </summary>
		void CreateTextGraphicsResources();

	private:
		/// <summary>
		/// DWrite Factory object, used to create all other DWrite objects
		/// </summary>
		Microsoft::WRL::ComPtr<IDWriteFactory> m_pFactory;

		/// <summary>
		/// Static TextRenderer instance for singelton design
		/// </summary>
		static std::shared_ptr<TextRenderer> s_TextRenderer;
	};
}



