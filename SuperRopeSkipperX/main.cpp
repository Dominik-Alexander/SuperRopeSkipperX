#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Game game;

	if (FAILED(game.Initialize(hInstance, nCmdShow)))
	{
		MessageBox(NULL, L"Could not initialize game!", L"Error!", MB_ICONERROR);
		return -1;
	}
	
	game.Run();

	game.Shutdown();

	return 0;
}