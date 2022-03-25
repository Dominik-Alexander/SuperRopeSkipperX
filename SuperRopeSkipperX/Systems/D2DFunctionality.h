#pragma once

#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")

class D2DFunctionality
{
private:
	ID2D1Factory* pD2DFactory = 0;
	ID2D1HwndRenderTarget* pRenderTarget = 0;
	ID2D1SolidColorBrush*  pClearingBrush = 0;

public:
	D2DFunctionality();
	~D2DFunctionality();

	HRESULT Initialize(HWND windowHandle);
	void Release();

	void Clear(float r, float g, float b);

	void BeginDraw();
	void EndDraw();

	ID2D1HwndRenderTarget* pGetRenderTarget();
};

