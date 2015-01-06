#include "windows.h"
#include "stdio.h"
#include "stdlib.h"

void DoExploit()
{
    char* mallocspace = (char*)malloc(0x1000);
    free(mallocspace);
    MessageBox(NULL,"test","test",NULL);
}

DWORD CALLBACK ExploitThread(LPVOID hModule)
{
	CoInitialize(NULL);
	DoExploit();
	CoUninitialize();
	FreeLibraryAndExitThread((HMODULE)hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
        MessageBox(NULL,"1","1",MB_OK);
		HANDLE hThread = CreateThread(NULL, 0, ExploitThread, hModule, 0, NULL);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}