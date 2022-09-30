#pragma once

#include <string>
#include "Graphics.h"

struct FontStyle
{
private:
	IDWriteTextFormat* pTextFormat = 0;
	Graphics* pGraphics = 0;

	ID2D1SolidColorBrush* pTextBrush = 0;

	D2D1_RECT_F drawSpace;

public:
	FontStyle(Graphics* pGraphics);
	~FontStyle();

	HRESULT Initialize(float fontSize);
	void Release();

	void RenderText(const wchar_t* text, RECT rect);
};

