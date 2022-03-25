#include "D2DFunctionality.h"

D2DFunctionality::D2DFunctionality()
{

}

D2DFunctionality::~D2DFunctionality()
{

}

HRESULT D2DFunctionality::Initialize(HWND windowHandle)
{
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	RECT rect;
	GetClientRect(windowHandle, &rect);

	result = this->pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&pRenderTarget);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	result = this->pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &pClearingBrush);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	return S_OK;
}

void D2DFunctionality::Release()
{
	if (pClearingBrush)
	{
		this->pClearingBrush->Release();
		this->pClearingBrush = NULL;
	}

	if (pD2DFactory)
	{
		this->pD2DFactory->Release();
		this->pD2DFactory = NULL;
	}

	if (pRenderTarget)
	{
		this->pRenderTarget->Release();
		this->pRenderTarget = NULL;
	}
}

void D2DFunctionality::Clear(float r, float g, float b)
{
	this->pRenderTarget->Clear(D2D1::ColorF(r, g, b));
}

void D2DFunctionality::BeginDraw()
{
	this->pRenderTarget->BeginDraw();
}

void D2DFunctionality::EndDraw()
{
	this->pRenderTarget->EndDraw();
}

ID2D1HwndRenderTarget* D2DFunctionality::pGetRenderTarget()
{
	return this->pRenderTarget;
}