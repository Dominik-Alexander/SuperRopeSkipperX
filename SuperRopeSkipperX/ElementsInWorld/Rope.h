#pragma once

#include <DirectXMath.h>

#include "../Engine/Graphics.h"
#include "../SpriteSheet.h"

class Rope
{
private:
	Graphics* pGraphics = 0;

	SpriteSheet* pRopeSprite = 0;

	float ropeWidth;
	float ropeHeight;

	DirectX::XMFLOAT2 initialRopePosition;
	DirectX::XMFLOAT2 currentRopePosition;

	float initialRopeSpeed;
	float actualRopeSpeed;

	float maxRopeSpeed;
	float maxRopeHeight;
	float minRopeHeight;

	enum ropeState { UPWARDS, DOWNWARDS };

	int state;

public:
	Rope();
	~Rope();

	HRESULT Initialize(Graphics* pGraphics);
	void Unload();

	void Update(float deltaTime);
	void MoveDownwards(float deltaTime);
	void MoveUpwards(float deltaTime);
	void Render();

	int GetState();

	bool AtHighest();
	bool AtLowest();

	void IncreaseSpeed();
	void ResetSpeed();
	void ResetPosition();
};

