#include "FontStyle.h"

FontStyle::FontStyle(Graphics* pGraphics)
{
	this->pGraphics = pGraphics;
	this->drawSpace = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
}

FontStyle::~FontStyle()
{

}

HRESULT FontStyle::Initialize(float fontSize)
{
	HRESULT result = this->pGraphics->pGetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pTextBrush);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	result = this->pGraphics->GetWriteFactory()->CreateTextFormat(L"System Fett", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"en-us", &pTextFormat);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	result = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	result = pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	result = pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	return S_OK;
}

void FontStyle::Release()
{
	if (this->pTextFormat)
	{
		pTextFormat->Release();
		this->pTextFormat = NULL;
	}
}

void FontStyle::RenderText(const wchar_t* text, RECT rect)
{
	const wchar_t* textToWrite = text;
	UINT32 textLength = (UINT32)wcslen(textToWrite);

	this->drawSpace.left = static_cast<FLOAT>(rect.left);
	this->drawSpace.top = static_cast<FLOAT>(rect.top);
	this->drawSpace.right = static_cast<FLOAT>(rect.right);
	this->drawSpace.bottom = static_cast<FLOAT>(rect.bottom);

	this->pGraphics->pGetRenderTarget()->DrawText(textToWrite, textLength, this->pTextFormat, this->drawSpace, this->pTextBrush);
}