#include "PlayerData.h"

PlayerData::PlayerData()
{
	this->highScore = 0;
}

PlayerData::~PlayerData()
{

}

unsigned int PlayerData::GetHighScore()
{
	return this->highScore;
}

void PlayerData::SetHighScore(unsigned int newHighScore)
{
	this->highScore = newHighScore;
}
