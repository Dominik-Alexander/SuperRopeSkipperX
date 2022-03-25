#include "Post.h"

Post::Post()
{
	postWidth = 16.0f;
	postHeight = 100.0f;

	leftPostPosition = DirectX::XMFLOAT2(208.0f - postWidth, 240.0f -48.0f);
	rightPostPosition = DirectX::XMFLOAT2(208.0f + 64.0f, 240.0f - 48.0f);
}

Post::~Post()
{

}

HRESULT Post::Initialize(Graphics* pGraphics)
{
	this->pGraphics = pGraphics;

	this->pPostSprite = new SpriteSheet(L"Assets/Post.png", pGraphics, postWidth, postHeight);

	return S_OK;
}

void Post::Unload()
{
	delete pPostSprite;
	pPostSprite = NULL;
}

void Post::Render()
{
	pPostSprite->Draw(0, leftPostPosition.x, leftPostPosition.y);
	pPostSprite->Draw(0, rightPostPosition.x, rightPostPosition.y);
}