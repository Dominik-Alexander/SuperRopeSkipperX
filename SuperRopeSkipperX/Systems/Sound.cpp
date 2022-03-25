#include "Sound.h"

Sound::Sound()
{
}

Sound::~Sound()
{
}

HRESULT Sound::Initialize(HWND hwnd)
{
	HRESULT result = InitializeDirectSound(hwnd);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not initialize DirectSound and primary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	char input[] = "Assets/Jump.wav";
	char score[] = "Assets/Score.wav";
	char hurt[] = "Assets/Hurt.wav";
	char confirm[] = "Assets/Confirm.wav";
	char gameOver[] = "Assets/GameOver.wav";

	// Load a wave audio file onto a secondary buffer.
	result = LoadWaveFile(input, &pSecondaryBuffer1);
	
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not load wave into secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	result = LoadWaveFile(hurt, &pSecondaryBuffer2);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not load wave into secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	result = LoadWaveFile(score, &pSecondaryBuffer3);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not load wave into secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	result = LoadWaveFile(confirm, &pSecondaryBuffer4);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not load wave into secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	result = LoadWaveFile(gameOver, &pSecondaryBuffer5);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not load wave into secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

void Sound::Shutdown()
{
	ShutdownWaveFile(&pSecondaryBuffer5);

	ShutdownWaveFile(&pSecondaryBuffer4);

	ShutdownWaveFile(&pSecondaryBuffer3);

	ShutdownWaveFile(&pSecondaryBuffer2);
	
	ShutdownWaveFile(&pSecondaryBuffer1);

	// Shutdown the Direct Sound API.
	ShutdownDirectSound();
}

HRESULT Sound::InitializeDirectSound(HWND hwnd)
{
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// Initialize the direct sound interface pointer for the default sound device.
	HRESULT result = DirectSoundCreate8(NULL, &pDirectSound, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not initialize DirectSound pointer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = pDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set cooperative level to priority!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = pDirectSound->CreateSoundBuffer(&bufferDesc, &pPrimaryBuffer, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not get control of the primary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = pPrimaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set primary buffer to wave format!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

void Sound::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer.
	if (pPrimaryBuffer)
	{
		pPrimaryBuffer->Release();
		pPrimaryBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if (pDirectSound)
	{
		pDirectSound->Release();
		pDirectSound = 0;
	}

	return;
}

HRESULT Sound::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	FILE* filePtr;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;


	// Open the wave file in binary.
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		MessageBox(NULL, L"Could not open wave file in binary!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Read in the wave file header.
	size_t count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		MessageBox(NULL, L"Could not read in wave file header!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		MessageBox(NULL, L"Not RIFF!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		MessageBox(NULL, L"Not Wave!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		MessageBox(NULL, L"Not fmt!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		MessageBox(NULL, L"Not WAVE_FORMAT_PCM!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 1)
	{
		MessageBox(NULL, L"Not mono format!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
	{
		MessageBox(NULL, L"Not correct sample rate!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		MessageBox(NULL, L"Not 16 bit format!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		MessageBox(NULL, L"No data chunk header!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	HRESULT result = pDirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"No buffer with specific buffer settings!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Not tested DS8 interface!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		MessageBox(NULL, L"No temporary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		MessageBox(NULL, L"Could not read into new buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		MessageBox(NULL, L"Could not close file!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not lock secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not unlock secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	return S_OK;
}

void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	// Release the secondary sound buffer.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

HRESULT Sound::PlayWaveFile()
{
	// Set position at the beginning of the sound buffer.
	HRESULT result = pSecondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set position at beginning of sound buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Set volume of the buffer to 100%.
	result = pSecondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set volume to 100!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Play the contents of the secondary sound buffer.
	result = pSecondaryBuffer1->Play(0, 0, 0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not play contents of secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT Sound::PlayHurtSound()
{
	HRESULT result;

	result = pSecondaryBuffer2->SetCurrentPosition(0);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set position at beginning of sound buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Set volume of the buffer to 100%.
	result = pSecondaryBuffer2->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set volume to 100!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Play the contents of the secondary sound buffer.
	result = pSecondaryBuffer2->Play(0, 0, 0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not play contents of secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT Sound::PlayScoreSound()
{
	HRESULT result;

	result = pSecondaryBuffer3->SetCurrentPosition(0);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set position at beginning of sound buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Set volume of the buffer to 100%.
	result = pSecondaryBuffer3->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set volume to 100!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Play the contents of the secondary sound buffer.
	result = pSecondaryBuffer3->Play(0, 0, 0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not play contents of secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT Sound::PlayConfirmSound()
{
	HRESULT result;

	result = pSecondaryBuffer4->SetCurrentPosition(0);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set position at beginning of sound buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Set volume of the buffer to 100%.
	result = pSecondaryBuffer4->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set volume to 100!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Play the contents of the secondary sound buffer.
	result = pSecondaryBuffer4->Play(0, 0, 0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not play contents of secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT Sound::PlayGameOverSound()
{
	HRESULT result;

	result = pSecondaryBuffer5->SetCurrentPosition(0);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set position at beginning of sound buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Set volume of the buffer to 100%.
	result = pSecondaryBuffer5->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not set volume to 100!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	// Play the contents of the secondary sound buffer.
	result = pSecondaryBuffer5->Play(0, 0, 0);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not play contents of secondary buffer!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

IDirectSound8* Sound::GetDirectSound()
{
	return pDirectSound;
}

IDirectSoundBuffer* Sound::GetPrimaryBuffer()
{
	return this->pPrimaryBuffer;
}