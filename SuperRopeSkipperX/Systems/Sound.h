#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#include "../WaveHeaderType.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

class Sound
{
private:
	IDirectSound8* pDirectSound = 0;
	IDirectSoundBuffer* pPrimaryBuffer = 0;
	IDirectSoundBuffer8* pSecondaryBuffer1 = 0;

	IDirectSoundBuffer8* pSecondaryBuffer2 = 0;
	IDirectSoundBuffer8* pSecondaryBuffer3 = 0;
	IDirectSoundBuffer8* pSecondaryBuffer4 = 0;
	IDirectSoundBuffer8* pSecondaryBuffer5 = 0;

	HRESULT InitializeDirectSound(HWND hwnd);
	void ShutdownDirectSound();

	HRESULT LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer);
	void ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer);

public:
	Sound();
	~Sound();

	HRESULT Initialize(HWND hwnd);
	void Shutdown();

	HRESULT PlayWaveFile();
	HRESULT PlayHurtSound();
	HRESULT PlayScoreSound();
	HRESULT PlayConfirmSound();
	HRESULT PlayGameOverSound();

	IDirectSound8* GetDirectSound();
	IDirectSoundBuffer* GetPrimaryBuffer();
};