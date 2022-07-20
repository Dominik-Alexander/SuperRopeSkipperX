#include "Input.h"

#define KEYDOWN(name, key)(name[key] & 0x80)

Input::Input()
{
}

Input::~Input()
{

}

HRESULT Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, NULL)))
	{
		MessageBox(hwnd, L"Could not create DirectInput8!", L"Error!", MB_ICONERROR);
		Release();
		return S_FALSE;
	}

	if (FAILED(pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL)))
	{
		MessageBox(hwnd, L"Could not create keyboard device!", L"Error!", MB_ICONERROR);
		Release();
		return S_FALSE;
	}

	if (FAILED(pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hwnd, L"Could not set keyboard data format!", L"Error!", MB_ICONERROR);
		Release();
		return S_FALSE;
	}

	if (FAILED(pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
	{
		MessageBox(hwnd, L"Could not set keyboard cooperative level!", L"Error!", MB_ICONERROR);
		Release();
		return S_FALSE;
	}

	if (FAILED(pKeyboard->Acquire()))
	{
		MessageBox(hwnd, L"Could not acquire keyboard!", L"Error!", MB_ICONERROR);
		Release();
		return S_FALSE;
	}

	return S_OK;
}

void Input::Release()
{
	if (pKeyboard)
	{
		pKeyboard->Unacquire();
		pKeyboard->Release();
		pKeyboard = NULL;
	}

	if (pDirectInput)
	{
		pDirectInput->Release();
		pDirectInput = NULL;
	}
}

bool Input::CheckKeyboard()
{
	HRESULT result = pKeyboard->GetDeviceState(sizeof(keyBuffer), (LPVOID)&keyBuffer);

	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			pKeyboard->Acquire();
		}
		else {
			return false;
		}
	}

	return true;
}

bool Input::IsSpacePressed()
{
	if (KEYDOWN(keyBuffer, DIK_SPACE))
	{
		return true;
	}

	return false;
}

bool Input::IsEnterPressed()
{
	if (KEYDOWN(keyBuffer, DIK_RETURN))
	{
		return true;
	}

	return false;
}

bool Input::IsEscapePressed()
{
	if (KEYDOWN(keyBuffer, DIK_ESCAPE))
	{
		PostQuitMessage(0);
		return true;
	}

	return false;
}