#pragma once

#include "Systems/Graphics.h"
#include <wincodec.h>

#include <string>

#pragma comment(lib, "windowscodecs.lib")

class SpriteSheet
{
private:
	Graphics* pGraphics = 0;
	ID2D1Bitmap* pBitmap = 0;

	float spriteWidth, spriteHeight;
	int spritesAccross;

public:
	SpriteSheet(const wchar_t* pFilename, Graphics* pGraphics);
	SpriteSheet(const wchar_t* pFilename, Graphics* pGraphics, float spriteWidth, float spriteHeight);

	~SpriteSheet();

	void Draw();
	void Draw(int index, float x, float y);
};

