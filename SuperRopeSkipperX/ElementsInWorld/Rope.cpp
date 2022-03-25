#include "Rope.h"

Rope::Rope()
{
	ropeWidth = 64.0f;
	ropeHeight = 16.0f;
	initialRopePosition = DirectX::XMFLOAT2(208.0f, 268.0f - 64.0f);
	currentRopePosition = initialRopePosition;
	initialRopeSpeed = 20.0f;
	actualRopeSpeed = initialRopeSpeed;
	maxRopeSpeed = 100.0f;
	maxRopeHeight = 240.0f - 48.0f;
	minRopeHeight = 240.0f + 32.0f;
	state = DOWNWARDS;
}

Rope::~Rope()
{

}

HRESULT Rope::Initialize(Graphics* pGraphics)
{
	this->pGraphics = pGraphics;

	this->pRopeSprite = new SpriteSheet(L"Assets/Rope.png", pGraphics, ropeWidth, ropeHeight);

	return S_OK;
}

void Rope::Unload()
{
	delete pRopeSprite;
	pRopeSprite = NULL;
}

void Rope::Update(float deltaTime)
{
	switch (this->state)
	{
	case UPWARDS:
		MoveUpwards(deltaTime);
		break;
	case DOWNWARDS:
		MoveDownwards(deltaTime);
		break;
	}
}

void Rope::MoveDownwards(float deltaTime)
{
	currentRopePosition.y += 1.0f * actualRopeSpeed * deltaTime;

	if (currentRopePosition.y >= minRopeHeight)
	{
		currentRopePosition.y = minRopeHeight;
		state = UPWARDS;
	}
}

void Rope::MoveUpwards(float deltaTime)
{
	currentRopePosition.y -= 1.0f * actualRopeSpeed * deltaTime;

	if (currentRopePosition.y <= maxRopeHeight)
	{
		currentRopePosition.y = maxRopeHeight;
		state = DOWNWARDS;
	}
}

void Rope::Render()
{
	pRopeSprite->Draw(0, currentRopePosition.x, currentRopePosition.y);
}

int Rope::GetState()
{
	return state;
}

bool Rope::AtHighest()
{
	return(currentRopePosition.y <= maxRopeHeight);
}

bool Rope::AtLowest()
{
	return(currentRopePosition.y >= minRopeHeight);
}

void Rope::IncreaseSpeed()
{
	if (actualRopeSpeed >= maxRopeSpeed)
	{
		actualRopeSpeed = maxRopeSpeed;
	}
	else {
		actualRopeSpeed += 5.0f;
	}
}

void Rope::ResetSpeed()
{
	actualRopeSpeed = initialRopeSpeed;
}

void Rope::ResetPosition()
{
	currentRopePosition = initialRopePosition;
}