#pragma once

#include <DirectXMath.h>

#include "../Systems/Input.h"
#include "../Systems/Graphics.h"
#include "../Systems/Sound.h"
#include "../SpriteSheet.h"

class Player
{
private:
	Graphics* pGraphics = 0;
	Input* pPlayerInput = 0;
	Sound* pSound = 0;

	SpriteSheet* pPlayerSprite = 0;

	float playerWidth;
	float playerHeight;

	DirectX::XMFLOAT2 initialPlayerPosition;
	DirectX::XMFLOAT2 currentPlayerPosition;

	float initialPlayerSpeed;
	float actualPlayerSpeed;
	float maxJumpHeight;

	enum playerState { STANDING, JUMPING, FALLING };

	int state;

public:
	Player();
	~Player();

	HRESULT Initialize(Input* pPlayerInput, Graphics* pGraphics, Sound* pSound);
	void Unload();

	void Update(float deltaTime);
	void Render();

	bool AtHighest();
	bool AtLowest();
};

