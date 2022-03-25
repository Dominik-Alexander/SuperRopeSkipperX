#include "Player.h"

Player::Player()
{
	this->playerWidth = 64.0f;
	this->playerHeight = 64.0f;
	this->initialPlayerPosition = DirectX::XMFLOAT2(208.0f, 240.0f);
	this->currentPlayerPosition = initialPlayerPosition;
	this->initialPlayerSpeed = 80.0f;
	this->actualPlayerSpeed = initialPlayerSpeed;
	this->maxJumpHeight = 176.0f;

	this->state = STANDING;
}

Player::~Player()
{

}

HRESULT Player::Initialize(Input* pPlayerInput, Graphics* pGraphics, Sound* pSound)
{
	this->pPlayerInput = pPlayerInput;
	this->pGraphics = pGraphics;
	this->pSound = pSound;

	this->pPlayerSprite = new SpriteSheet(L"Assets/Skippy.png", pGraphics, playerWidth, playerHeight);

	return S_OK;
}

void Player::Unload()
{
	if (this->pPlayerSprite)
	{
		delete this->pPlayerSprite;
		this->pPlayerSprite = NULL;
	}
}

void Player::Update(float deltaTime)
{
	switch (this->state)
	{
	case STANDING:
		this->pPlayerInput->CheckKeyboard();

		if (this->pPlayerInput->IsSpacePressed() == true)
		{
			this->actualPlayerSpeed = this->initialPlayerSpeed;
			this->pSound->PlayWaveFile();
			this->state = JUMPING;
		}
		break;
	case JUMPING:
		this->currentPlayerPosition.y -= 1.0f * this->actualPlayerSpeed * deltaTime;

		this->actualPlayerSpeed -= 0.5f;

		if (this->actualPlayerSpeed <= 40.0f)
		{
			this->actualPlayerSpeed = 40.0f;
		}

		if (this->currentPlayerPosition.y <= maxJumpHeight)
		{
			this->currentPlayerPosition.y = maxJumpHeight;
			this->state = FALLING;
		}
		break;
	case FALLING:
		this->currentPlayerPosition.y += 1.0f * this->actualPlayerSpeed * deltaTime;

		this->actualPlayerSpeed += 10.0f;

		if (this->currentPlayerPosition.y >= this->initialPlayerPosition.y)
		{
			this->currentPlayerPosition = this->initialPlayerPosition;
			this->state = STANDING;
		}
		break;
	}
}

void Player::Render()
{
	pPlayerSprite->Draw(0, this->currentPlayerPosition.x, this->currentPlayerPosition.y);
}

bool Player::AtHighest()
{
	return(this->currentPlayerPosition.y == this->maxJumpHeight);
}

bool Player::AtLowest()
{
	return (this->currentPlayerPosition.y == this->initialPlayerPosition.y);
}