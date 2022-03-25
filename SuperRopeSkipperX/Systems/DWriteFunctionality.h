#pragma once

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "dwrite.lib")

class DWriteFunctionality
{
private:
	IDWriteFactory* pWriteFactory = 0;
	ID2D1HwndRenderTarget* pRenderTarget = 0;

public:
	DWriteFunctionality();
	~DWriteFunctionality();

	HRESULT Initialize(ID2D1HwndRenderTarget* pRenderTarget, HWND hwnd);
	void Release();

	IDWriteFactory* GetWriteFactory();
};

