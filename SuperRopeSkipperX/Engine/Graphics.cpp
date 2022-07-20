#include "Graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

HRESULT Graphics::Initialize(HWND hwnd)
{
	this->pD2DFunctionality = new D2DFunctionality();

	HRESULT result = this->pD2DFunctionality->Initialize(hwnd);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	this->pRenderTarget = this->pD2DFunctionality->pGetRenderTarget();

	this->pDWriteFunctionality = new DWriteFunctionality();

	result = this->pDWriteFunctionality->Initialize(this->pRenderTarget, hwnd);

	if (result != S_OK)
	{
		return S_FALSE;
	}

	return S_OK;
}

void Graphics::BeginDraw()
{
	this->pD2DFunctionality->BeginDraw();
}

void Graphics::EndDraw()
{
	this->pD2DFunctionality->EndDraw();
}

void Graphics::Release()
{
	if (this->pD2DFunctionality)
	{
		this->pD2DFunctionality->Release();
		this->pD2DFunctionality = NULL;
	}
}

ID2D1HwndRenderTarget* Graphics::pGetRenderTarget()
{
	return this->pRenderTarget;
}

void Graphics::Clear(float r, float g, float b)
{
	this->pD2DFunctionality->Clear(r, g, b);
}

IDWriteFactory* Graphics::GetWriteFactory()
{
	return this->pDWriteFunctionality->GetWriteFactory();
}