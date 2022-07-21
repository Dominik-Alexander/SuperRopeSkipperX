#pragma once

#include <DirectXMath.h>
#include "../Engine/Graphics.h"
#include "../Engine/SpriteSheet.h"

class Post
{
private:
	Graphics* pGraphics = 0;
	SpriteSheet* pPostSprite = 0;

	float postWidth;
	float postHeight;

	DirectX::XMFLOAT2 leftPostPosition;
	DirectX::XMFLOAT2 rightPostPosition;

public:
	Post();
	~Post();

	HRESULT Initialize(Graphics* pGraphics);
	void Unload();

	void Render();
};