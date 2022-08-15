#pragma once

#include <initguid.h>

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")

struct Input
{
private:
	IDirectInput8* pDirectInput = 0;
	IDirectInputDevice8* pKeyboard = 0;

	unsigned char keyBuffer[256]{ 0 };

public:
	Input();
	~Input();

	HRESULT Initialize(HINSTANCE hInstance, HWND hwnd);
	void Release();

	bool CheckKeyboard();

	bool IsSpacePressed();
	bool IsEnterPressed();
	bool IsEscapePressed();
};