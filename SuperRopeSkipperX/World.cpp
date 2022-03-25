#include "World.h"

World::World()
{
	this->state = TITLE;
	this->titleTextBox = { 0, 0, 0, 0 };
	this->gameOverTextBox = { 0, 0, 0, 0 };
	this->scoreTextBox = { 0, 0, 0, 0 };
	this->livesTextBox = { 0, 0, 0, 0 };
	this->highScoreTextBox = { 0, 0, 0, 0 };

	this->initialLives = 3;
	this->initialScore = 0;

	this->lives = initialLives;
	this->score = initialScore;
}

World::~World()
{

}

HRESULT World::Initialize(Input* pInput, Graphics* pGraphics, Sound* pSound, Persistence* pPersistence)
{
	InitializeInput(pInput);

	InitializeGraphics(pGraphics);

	InitializeSound(pSound);

	this->pPersistence = pPersistence;

	this->pBackground = new SpriteSheet(L"Assets/Background.png", this->pGraphics);

	LoadTitleFont();

	LoadGameOverFont();

	LoadPlayer();

	LoadRope();

	LoadPost();

	LoadPlayerData();

	this->currentHighScore = this->pPersistence->LoadData();

	LoadScoreFont();

	LoadHighScoreFont();

	LoadLivesFont();

	return S_OK;
}

HRESULT World::InitializeSound(Sound* pSound)
{
	this->pSound = pSound;

	if (!this->pSound)
	{
		MessageBox(NULL, L"Could not set sound in game world!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT World::InitializeGraphics(Graphics* pGraphics)
{
	this->pGraphics = pGraphics;

	if (!this->pGraphics)
	{
		MessageBox(NULL, L"Could not set graphics in game world!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT World::InitializeInput(Input* pInput)
{
	this->pInput = pInput;

	if (!this->pInput)
	{
		MessageBox(NULL, L"Could not set input in game world!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT World::LoadPlayer()
{
	this->pPlayer = new Player();

	if (FAILED(pPlayer->Initialize(this->pInput, this->pGraphics, this->pSound)))
	{
		MessageBox(NULL, L"Could not set graphics in initialize player!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT World::LoadRope()
{
	this->pRope = new Rope();

	if (FAILED(pRope->Initialize(this->pGraphics)))
	{
		MessageBox(NULL, L"Could not set graphics in initialize rope!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT World::LoadPost()
{
	this->pPost = new Post();

	if (FAILED(pPost->Initialize(this->pGraphics)))
	{
		MessageBox(NULL, L"Could not set graphics in initialize post!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT World::LoadPlayerData()
{
	this->pPlayerData = new PlayerData();

	if (this->pPlayerData == NULL)
	{
		MessageBox(NULL, L"Could not set graphics in initialize player data!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT World::LoadTitleFont()
{
	this->pTitle = new FontStyle(this->pGraphics);

	this->pTitle->Initialize(50.0f);

	this->titleTextBox = { 0, 0, 480, 320 };

	return S_OK;
}

HRESULT World::LoadGameOverFont()
{
	this->pGameOver = new FontStyle(this->pGraphics);

	this->pGameOver->Initialize(80.0f);

	this->gameOverTextBox = { 0, 0, 480, 320 };

	return S_OK;
}

HRESULT World::LoadScoreFont()
{
	this->pScoreFont = new FontStyle(this->pGraphics);
	this->pScoreFont->Initialize(20.0f);

	this->scoreTextBox = { 50, 0, 100, 100 };

	return S_OK;
}

HRESULT World::LoadHighScoreFont()
{
	this->pHighScoreFont = new FontStyle(this->pGraphics);
	this->pHighScoreFont->Initialize(20.0f);

	this->highScoreTextBox = { 50, 0, 100, 150 };

	return S_OK;
}

HRESULT World::LoadLivesFont()
{
	this->pLivesFont = new FontStyle(this->pGraphics);
	this->pLivesFont->Initialize(20.0f);

	this->livesTextBox = { 50, 0, 100, 50 };

	return S_OK;
}

void World::Update(float deltaTime)
{
	switch (state)
	{
	case TITLE:
		UpdateTitle();
		break;
	case RUNNING:
		UpdateRunning(deltaTime);
		break;
	case GAMEOVER:
		UpdateGameOver();
		break;
	}
}

void World::UpdateTitle()
{
	if (pInput->IsEnterPressed())
	{
		pSound->PlayConfirmSound();
		state = RUNNING;
	}
}

void World::UpdateRunning(float deltaTime)
{
	pPlayer->Update(deltaTime);
	pRope->Update(deltaTime);

	if (pPlayer->AtLowest() && pRope->AtLowest())
	{
		pSound->PlayHurtSound();
		lives -= 1;
		pRope->ResetSpeed();
	}

	if (pRope->AtLowest() && !pPlayer->AtLowest())
	{
		pSound->PlayScoreSound();
		score += 1;
		pRope->IncreaseSpeed();
	}

	if (lives <= 0)
	{
		pSound->PlayGameOverSound();
		state = GAMEOVER;
	}

	if (score > currentHighScore)
	{
		currentHighScore = score;
	}
}

void World::UpdateGameOver()
{
	pPersistence->SaveData(currentHighScore);
	ResetWorld();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	state = TITLE;
}

void World::Render()
{
	RenderBackground();

	switch (state)
	{
	case TITLE:
	{
		RenderTitleScreen();
		break;
	}
	case RUNNING:
	{
		RenderRunningGame();
		break;
	}
	case GAMEOVER:
	{
		RenderGameOverScreen();
		break;
	}
	}
}

void World::RenderBackground()
{
	pGraphics->Clear(0.0f, 0.0f, 1.0f);

	pBackground->Draw();
}

void World::RenderTitleScreen()
{
	pTitle->RenderText(L"SuperRopeSkipperX", titleTextBox);
}

void World::RenderRunningGame()
{
	ropeState = pRope->GetState();

	if (ropeState == 0)
	{
		pRope->Render();
	}

	pPlayer->Render();

	if (ropeState == 1)
	{
		pRope->Render();
	}

	pPost->Render();

	std::wstring scoreString = L"Score: " + std::to_wstring(score);
	const wchar_t* printableScore = scoreString.c_str();

	std::wstring highScoreString = L"Highscore: " + std::to_wstring(currentHighScore);
	const wchar_t* printableHighScore = highScoreString.c_str();

	std::wstring livesString = L"Lives: " + std::to_wstring(lives);
	const wchar_t* printableLives = livesString.c_str();

	pScoreFont->RenderText(printableScore, scoreTextBox);
	pHighScoreFont->RenderText(printableHighScore, highScoreTextBox);
	pLivesFont->RenderText(printableLives, livesTextBox);
}

void World::RenderGameOverScreen()
{
	pGameOver->RenderText(L"Game Over!", gameOverTextBox);
}

void World::ResetWorld()
{
	this->score = this->initialScore;
	this->lives = this->initialLives;
	this->currentHighScore = this->pPersistence->LoadData();
}

void World::SaveHighScore(Persistence* persistence)
{
	persistence->SaveData(this->score);
}

void World::LoadHighScore(Persistence* persistence)
{
	this->currentHighScore = persistence->LoadData();
}

void World::Unload()
{
	UnloadPlayerData();

	UnloadPlayer();

	UnloadRope();

	UnloadPost();

	UnloadBackground();

	UnloadTitle();

	UnloadGameOver();

	UnloadLivesFont();

	UnloadScoreFont();

	UnloadHighScoreFont();
}

void World::UnloadPlayerData()
{
	if (pPlayerData)
	{
		delete pPlayerData;
		pPlayerData = NULL;
	}
}

void World::UnloadPlayer()
{
	if (pPlayer)
	{
		pPlayer->Unload();
		delete pPlayer;
		pPlayer = NULL;
	}
}

void World::UnloadRope()
{
	if (pRope)
	{
		pRope->Unload();
		delete pRope;
		pRope = NULL;
	}
}

void World::UnloadPost()
{
	if (pPost)
	{
		pPost->Unload();
		delete pPost;
		pPost = NULL;
	}
}

void World::UnloadBackground()
{
	if (pBackground)
	{
		delete pBackground;
		pBackground = NULL;
	}
}

void World::UnloadTitle()
{
	if (pTitle)
	{
		delete pTitle;
		pTitle = NULL;
	}
}

void World::UnloadGameOver()
{
	if (pGameOver)
	{
		delete pGameOver;
		pGameOver = NULL;
	}
}

void World::UnloadLivesFont()
{
	if (pLivesFont)
	{
		delete pLivesFont;
		pLivesFont = NULL;
	}
}

void World::UnloadScoreFont()
{
	if (pScoreFont)
	{
		delete pScoreFont;
		pScoreFont = NULL;
	}
}

void World::UnloadHighScoreFont()
{
	if (pHighScoreFont)
	{
		delete pHighScoreFont;
		pHighScoreFont = NULL;
	}
}
