// COMP710 GP 2D Framework 2017

// This include:
#include "logmanager.h"

// Library includes:
#include <Windows.h>

// Static Members:
LogManager* LogManager::sm_pInstance = 0;

LogManager& 
LogManager::GetInstance()
{
	// Singleton pattern: The first time GetInstance() get invoked, the sm_pInstance is equal to zero.
	// But the next time it get invoked, the sm_pInstance is not equal to zero, which
	// means no new LogManager instance will be created.
	if (sm_pInstance == 0)
	{
		// It's OK to invoke the default constructor of LogManager regardless of the fact that the
		// default constructor of LogManager is private.
		sm_pInstance = new LogManager();
	}

	return (*sm_pInstance);
}

LogManager::LogManager()
{

}

LogManager::~LogManager()
{

}

void 
LogManager::Log(const char* pcMessage)
{
	OutputDebugStringA(pcMessage);
	OutputDebugStringA("\n"); 
}
