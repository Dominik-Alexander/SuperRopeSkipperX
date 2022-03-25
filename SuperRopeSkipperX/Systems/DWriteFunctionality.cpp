#include "DWriteFunctionality.h"

DWriteFunctionality::DWriteFunctionality()
{
}

DWriteFunctionality::~DWriteFunctionality()
{

}

HRESULT DWriteFunctionality::Initialize(ID2D1HwndRenderTarget* pRenderTarget, HWND hwnd)
{
	HRESULT result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory));

	if (result != S_OK)
	{
		MessageBox(NULL, L"Could not initialize DirectWrite!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	this->pRenderTarget = pRenderTarget;

	if (this->pRenderTarget == NULL)
	{
		return S_FALSE;
	}

	return S_OK;
}

void DWriteFunctionality::Release()
{
	if (pWriteFactory)
	{
		pWriteFactory->Release();
		pWriteFactory = NULL;
	}
}

IDWriteFactory* DWriteFunctionality::GetWriteFactory()
{
	return this->pWriteFactory;
}