// Terrain.cpp : Defines the entry point for the DLL application for Windows systems.
//

#include "Demeter/StdAfx.h"
#include "Demeter/Terrain.h"

using namespace std;

#ifdef _WIN32
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#else
void noop()
{
	cout << "no op" << endl;
}
#endif
