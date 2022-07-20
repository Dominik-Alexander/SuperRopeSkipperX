#pragma once

#include <Windows.h>
#include <sysinfoapi.h>
#include <synchapi.h>
#include "Engine/Input.h"
#include "Engine/Graphics.h"
#include "Engine/Sound.h"
#include "Engine/Persistence.h"

#include "World.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Game
{
public:
	Game();
	~Game();

	HRESULT Initialize(HINSTANCE hInstance, int nCmdShow);
	
	HWND CreateMainWindow(HINSTANCE hInstance);
	
	void Run();
	
	void Shutdown();

private:
	HWND hwnd = { 0 };
	Input* input = 0;
	Graphics* graphics = 0;
	Sound* sound = 0;
	Persistence* persistence = 0;

	int ticksLastFrame = 0;

	World* world = 0;

	void Update(float deltaTime);
	void Render();

	void UnloadWorld();
	void UnloadPersistence();
	void ReleaseSound();
	void ReleaseGraphics();
	void ReleaseInput();
	void ReleaseWindow();
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);