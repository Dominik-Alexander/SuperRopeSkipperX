#include "Persistence.h"

Persistence::Persistence()
{

}

Persistence::~Persistence()
{

}

HRESULT Persistence::Initialize()
{
	return S_OK;
}

void Persistence::Shutdown()
{
	
}

void Persistence::SaveData(int data)
{
	if (CreateDirectoryA(this->defaultLocation.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		std::ofstream SaveFile(defaultLocation + "/data.sav");
		SaveFile << data;
	}
	else 
	{
		MessageBox(NULL, L"Could not save data!", L"Error!", MB_ICONERROR);
	}
}

int Persistence::LoadData()
{
	std::ifstream LoadFile(this->defaultLocation + "/data.sav");

	LoadFile >> this->dataToLoad;

	return this->dataToLoad;
}
