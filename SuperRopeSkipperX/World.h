#pragma once

#include <chrono>
#include <thread>
#include <string>

#include "Systems/Input.h"
#include "Systems/Graphics.h"
#include "Systems/Persistence.h"
#include "PlayerData.h"
#include "SpriteSheet.h"
#include "FontStyle.h"
#include "ElementsInWorld/Player.h"
#include "ElementsInWorld/Rope.h"
#include "ElementsInWorld/Post.h"

#include "Systems/Sound.h"

class World
{
private: 
	Input* pInput = 0;
	Graphics* pGraphics = 0;
	Sound* pSound = 0;
	Persistence* pPersistence = 0;

	SpriteSheet* pBackground = 0;

	FontStyle* pTitle = 0;
	FontStyle* pGameOver = 0;
	FontStyle* pLivesFont = 0;
	FontStyle* pScoreFont = 0;
	FontStyle* pHighScoreFont = 0;

	Player* pPlayer = 0;
	Rope* pRope = 0;
	Post* pPost = 0;

	PlayerData* pPlayerData = 0;

	int initialLives = 0;
	int initialScore = 0;

	int score = 0;
	int lives = 0;

	int currentHighScore = 0;

	int ropeState = 0;

	RECT titleTextBox;

	RECT scoreTextBox;
	RECT highScoreTextBox;
	RECT livesTextBox;
	RECT gameOverTextBox;

	enum worldState{ TITLE, RUNNING, GAMEOVER };

	int state;

	void SaveHighScore(Persistence* persistence);
	void LoadHighScore(Persistence* persistence);

public:
	World();
	~World();

	HRESULT Initialize(Input* pInput, Graphics* pGraphics, Sound* pSound, Persistence* pPersistence);

	HRESULT InitializeSound(Sound* pSound);

	HRESULT InitializeGraphics(Graphics* pGraphics);

	HRESULT InitializeInput(Input* pInput);

	HRESULT LoadPlayer();

	HRESULT LoadRope();

	HRESULT LoadPost();

	HRESULT LoadPlayerData();

	HRESULT LoadTitleFont();

	HRESULT LoadGameOverFont();

	HRESULT LoadScoreFont();

	HRESULT LoadHighScoreFont();

	HRESULT LoadLivesFont();

	void Update(float deltaTime);

	void UpdateTitle();

	void UpdateRunning(float deltaTime);

	void UpdateGameOver();

	void Render();

	void RenderBackground();

	void RenderTitleScreen();

	void RenderRunningGame();

	void RenderGameOverScreen();

	void ResetWorld();

	void Unload();
	void UnloadPlayerData();
	void UnloadPlayer();
	void UnloadRope();
	void UnloadPost();
	void UnloadBackground();
	void UnloadTitle();
	void UnloadGameOver();
	void UnloadLivesFont();
	void UnloadScoreFont();
	void UnloadHighScoreFont();
};

