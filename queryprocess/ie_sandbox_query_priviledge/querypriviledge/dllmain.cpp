// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <TlHelp32.h>
#include <strsafe.h>
#include <comdef.h>
#include <bcrypt.h>
#include <vector>



void QueryPriviledge(int, FILE*);
void DebugPrintf(LPCSTR lpFormat, ...)
{
	CHAR buf[1024];
	va_list va;

	va_start(va, lpFormat);

	StringCbVPrintfA(buf, sizeof(buf), lpFormat, va);

	OutputDebugStringA(buf);
}

DWORD GetParentProcessId()
{
	DWORD pid = 0;
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(PROCESSENTRY32);

	pid = GetCurrentProcessId();

	if (Process32First(h, &pe)) {
		do {
			if (pe.th32ProcessID == pid) {
				return pe.th32ParentProcessID;
			}
		} while (Process32Next(h, &pe));
	}

	CloseHandle(h);

	return 0;
}

DWORD EnumAndQuery(FILE* fp)
{
	DWORD pid = 0;
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe = { 0 };
	CHAR exeName[0x200] = { 0 };
	pe.dwSize = sizeof(PROCESSENTRY32);

	

	if (Process32First(h, &pe)) {
		do {
			WideCharToMultiByte(CP_ACP, 0, pe.szExeFile, -1, exeName, 256, NULL, NULL);
			fwrite(exeName, 1, strlen(exeName), fp);
			memset(exeName, 0, 0x200);
			fwrite("\t\n",1, 2, fp);
			QueryPriviledge(pe.th32ParentProcessID, fp);
		} while (Process32Next(h, &pe));
	}
	CloseHandle(h);

	return 0;
}

void QueryPriviledge(int pid, FILE* fp)
{
	HANDLE hProcess = NULL;
	char* outbuf = (char*)malloc(0x1000);



	if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_ALL_ACCESS ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_CREATE_PROCESS, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_CREATE_PROCESS ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_CREATE_THREAD, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_CREATE_THREAD ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PPROCESS_DUP_HANDLE ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_QUERY_INFORMATION ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_QUERY_LIMITED_INFOMATION ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_SET_INFORMATION ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_SET_QUOTA ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_SUSPEND_RESUME ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_TERMINATE ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_VM_OPERATION, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_VM_OPERATION ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_VM_READ ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(PROCESS_VM_WRITE, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " PROCESS_VM_WRITE ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}
	if ((hProcess = OpenProcess(SYNCHRONIZE, FALSE, pid)) != NULL)
	{
		sprintf(outbuf, " SYNCHRONIE ");
		fwrite(outbuf, 1, strlen(outbuf), fp);
		memset(outbuf, 0, 0x1000);
		CloseHandle(hProcess);
	}

	fwrite("\n\t", 1, 2, fp);
	

}

void DoExploit()
{
	FILE *fp;
	char* mallocspace = (char*)malloc(0x1000);
	free(mallocspace);
	MessageBoxA(NULL, "test", "test", NULL);
	fp = fopen("QueryPriviledge.txt", "w+");
	if (fp == NULL)
	{
		DebugPrintf("open QueryPriviledge.txt failed.\n");
	}
	EnumAndQuery(fp);
	fclose(fp);
}

DWORD CALLBACK ExploitThread(LPVOID hModule)
{
	CoInitialize(NULL);
	DoExploit();
	CoUninitialize();
	FreeLibraryAndExitThread((HMODULE)hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		MessageBoxA(NULL, "1", "1", MB_OK);
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
