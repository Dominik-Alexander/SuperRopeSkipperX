#pragma once

#include "D2DFunctionality.h"
#include "DWriteFunctionality.h"

struct Graphics
{
private:
	D2DFunctionality* pD2DFunctionality = 0;
	DWriteFunctionality* pDWriteFunctionality = 0;

	ID2D1HwndRenderTarget* pRenderTarget = 0;

public:
	Graphics();
	~Graphics();

	HRESULT Initialize(HWND windowHandle);
	void Release();

	void BeginDraw();
	void EndDraw();

	ID2D1HwndRenderTarget* pGetRenderTarget();
	IDWriteFactory* GetWriteFactory();

	void Clear(float r, float g, float b);
};

