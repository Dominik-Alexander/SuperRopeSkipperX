#pragma once

#include <Windows.h>
#include <fstream>
#include <string>

class Persistence
{
private:
	std::string defaultLocation = "SavedData";
	unsigned int dataToLoad = 0;

public:
	Persistence();
	~Persistence();

	HRESULT Initialize();

	void Shutdown();

	void SaveData(int data);
	int LoadData();
};