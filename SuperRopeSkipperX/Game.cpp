#include "Game.h"

Game::Game()
{
	this->hwnd = NULL;
}

Game::~Game()
{

}

HRESULT Game::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = CreateMainWindow(hInstance);

	if (!hwnd)
	{
		MessageBox(NULL, L"Could not create main window!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	ShowWindow(hwnd, nCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(false);

	this->input = new Input();
	this->graphics = new Graphics();
	this->sound = new Sound();

	if (FAILED(input->Initialize(hInstance, hwnd)))
	{
		MessageBox(NULL, L"Could not initialize input!", L"Error!", MB_ICONERROR);
		Shutdown();
		return S_FALSE;
	}

	if (FAILED(graphics->Initialize(hwnd)))
	{
		MessageBox(NULL, L"Could not initialize graphics!", L"Error!", MB_ICONERROR);
		Shutdown();
		return S_FALSE;
	}

	if (FAILED(sound->Initialize(hwnd)))
	{
		MessageBox(NULL, L"Could not initialize sound!", L"Error!", MB_ICONERROR);
		Shutdown();
		return S_FALSE;
	}

	this->persistence = new Persistence();

	this->world = new World();

	if (FAILED(this->world->Initialize(this->input, this->graphics, this->sound, this->persistence)))
	{
		MessageBox(NULL, L"Could not initialize game world!", L"Error!", MB_ICONERROR);
		Shutdown();
		return S_FALSE;
	}

	return S_OK;
}

HWND Game::CreateMainWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"MainWindow";

	if (!RegisterClassEx(&wndClass))
		MessageBox(NULL, L"Could not register window class!", L"Error!", MB_ICONERROR);

	RECT windowDimension = { 0, 0, 480, 320 };
	AdjustWindowRect(&windowDimension, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);

	int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowDimension.right - windowDimension.left) / 2;
	int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - windowDimension.top - windowDimension.bottom) / 2;

	return CreateWindowA("MainWindow", "Super Rope Skipper X",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		windowPosX,
		windowPosY,
		windowDimension.right - windowDimension.left,
		windowDimension.bottom - windowDimension.top,
		NULL,
		NULL,
		hInstance,
		NULL);
}

void Game::Run()
{
	MSG msg = { 0 };

	int timeToWait = static_cast<int>((1000 / 60) - (GetTickCount64() - ticksLastFrame));

	if (timeToWait > 0 && timeToWait <= (1000 / 60))
	{
		Sleep(timeToWait);
	}

	float deltaTime = static_cast<float>((GetTickCount64() - ticksLastFrame) / 1000);

	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

	ticksLastFrame = static_cast<int>(GetTickCount64());

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Update(deltaTime);

		Render();
	}
}

void Game::Update(float deltaTime)
{
	this->input->CheckKeyboard();

	if (this->input->IsEscapePressed())
	{
		PostQuitMessage(0);
	}

	this->world->Update(deltaTime);
}

void Game::Render()
{
	this->graphics->BeginDraw();

	this->world->Render();

	this->graphics->EndDraw();
}

void Game::Shutdown()
{
	UnloadWorld();
	ReleaseSound();
	ReleaseGraphics();
	ReleaseInput();
	ReleaseWindow();
}

void Game::UnloadWorld()
{
	if (world)
	{
		world->Unload();
		delete world;
		world = NULL;
	}
}

void Game::UnloadPersistence()
{
	delete persistence;
	persistence = NULL;
}

void Game::ReleaseSound()
{
	if (sound)
	{
		sound->Shutdown();
		delete sound;
		sound = NULL;
	}
}

void Game::ReleaseGraphics()
{
	if (graphics)
	{
		graphics->Release();
		delete graphics;
		graphics = NULL;
	}
}

void Game::ReleaseInput()
{
	if (input)
	{
		input->Release();
		delete input;
		input = NULL;
	}
}

void Game::ReleaseWindow()
{
	if (hwnd)
	{
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hdc;

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &paintStruct);
		EndPaint(hWnd, &paintStruct);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}