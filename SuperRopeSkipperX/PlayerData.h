#pragma once

#include <Windows.h>
#include <stdbool.h>
#include <fstream>

class PlayerData
{
private:
	unsigned int highScore;

public:
	PlayerData();
	~PlayerData();

	unsigned int GetHighScore();
	void SetHighScore(unsigned int newHighScore);
};

