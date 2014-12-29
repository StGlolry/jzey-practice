// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include <tchar.h>
#include "malloc.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

FILE *out;

void queryAccess(HKEY hkey, wchar_t* regkey);

void enumRegKeys(HKEY hKey)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys = 0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	char* outbuffer = (char*)malloc(0x1000);
	wchar_t* outbuffer_wide = (wchar_t*)malloc(0x1000);




	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		//printf("\nNumber of subkeys: %d\n", cSubKeys);
		sprintf(outbuffer, "\nNumber of subkeys: %d\n", cSubKeys);
		fwrite(outbuffer, 1, strlen(outbuffer), out);
		memset(outbuffer, 0, 0x1000);

		for (i = 0; i<cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				//_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
				swprintf(outbuffer_wide, 100,TEXT("(%d) %s\n"), cSubKeys,achKey);
				fwrite(outbuffer_wide, 2, lstrlenW(outbuffer_wide), out);
				queryAccess(hKey, achKey);

				// recursively open subkey
				HKEY recursiveKey;
				if (RegOpenKeyEx(hKey, achKey, 0, KEY_READ, &recursiveKey) == ERROR_SUCCESS)
					enumRegKeys(recursiveKey);
			}
		}
	}

	


}

void queryAccess(HKEY hkey,wchar_t* regkey)
{

	HKEY outhkey;

	//fwrite(regkey, 1, strlen(regkey), out);

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_ALL_ACCESS, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_ALL_ACCESS ", 1, strlen(" KEY_ALL_ACCESS "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_CREATE_LINK, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_CREATE_LINK ", 1, strlen(" KEY_CREATE_LINK "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_CREATE_SUB_KEY, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_CREATE_SUB_KEY ", 1, strlen(" KEY_CREATE_SUB_KEY "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_ENUMERATE_SUB_KEYS, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_ENUMERATE_SUB_KEYS  ", 1, strlen(" KEY_ENUMERATE_SUB_KEYS  "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_EXECUTE, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_EXECUTE  ", 1, strlen(" KEY_EXECUTE  "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_NOTIFY, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_NOTIFY  ", 1, strlen(" KEY_NOTIFY  "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_QUERY_VALUE, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_QUERY_VALUE ", 1, strlen(" KEY_QUERY_VALUE "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_READ, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_READ  ", 1, strlen(" KEY_READ  "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_SET_VALUE, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_SET_VALUE ", 1, strlen(" KEY_SET_VALUE "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_WOW64_32KEY, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_WOW64_32KEY  ", 1, strlen(" KEY_WOW64_32KEY  "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_WOW64_64KEY, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_WOW64_64KEY  ", 1, strlen(" KEY_WOW64_64KEY  "), out);
		RegCloseKey(outhkey);
	}

	if (RegOpenKeyEx(hkey, regkey, 0, KEY_WRITE, &outhkey) == ERROR_SUCCESS)
	{
		fwrite(" KEY_WRITE  ", 1, strlen(" KEY_WRITE  "), out);
		RegCloseKey(outhkey);
	}
	

	fwrite(" \n\n ", 1, 10, out);
}

void MainWorker()
{
	MessageBoxA(0, "inject success", "Injected", MB_OK);
	fopen_s(&out,"result.txt", "w+");
	if (out == NULL)
	{
		OutputDebugStringA("can't create result.txt");
	}

	HKEY hTestKey;

	fwrite("\n *************** Now in HKCU\\SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights  *************** \n", 1, strlen("\n *************** Now in HKCU\\SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights  *************** \n"), out);
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		TEXT("SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights"),
		0,
		KEY_READ,
		&hTestKey) == ERROR_SUCCESS
		)
	{
		enumRegKeys(hTestKey);
	}

	RegCloseKey(hTestKey);


	fwrite("\n *************** Now in HKCU\\SOFTWARE\\Microsoft\\Internet Explorer\\LowRegistry  *************** \n", 1, strlen("\n *************** Now in HKCU\\SOFTWARE\\Microsoft\\Internet Explorer\\LowRegistry  *************** \n"), out);

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		TEXT("SOFTWARE\\Microsoft\\Internet Explorer\\LowRegistry"),
		0,
		KEY_READ,
		&hTestKey) == ERROR_SUCCESS
		)
	{
		enumRegKeys(hTestKey);
	}

	RegCloseKey(hTestKey);

	fclose(out);
	MessageBoxA(0, "enumerate finished", "Injected", MB_OK);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:

		break;

	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		MainWorker();
		break;
		break;
	}
	return TRUE;
}

