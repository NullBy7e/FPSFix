#include "stdafx.h"
#include "FPSFix.h"

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    LPVOID lpDummy = lpReserved;
    lpDummy = NULL;
    
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH: InitInstance(hModule); break;
	    case DLL_PROCESS_DETACH: ExitInstance(); break;
        
        case DLL_THREAD_ATTACH:  break;
	    case DLL_THREAD_DETACH:  break;
	}
    return TRUE;
}

IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
	if (!hOriginalDll) LoadOriginalDll();

	typedef IDirect3D9 *(WINAPI* D3D9_Type)(UINT SDKVersion);
	D3D9_Type D3DCreate9_fn = (D3D9_Type)GetProcAddress(hOriginalDll, "Direct3DCreate9");

	if (!D3DCreate9_fn)
	{
		::ExitProcess(0);
	}

	IDirect3D9 *pIDirect3D9_orig = D3DCreate9_fn(SDKVersion);
	return pIDirect3D9_orig;
}

DWORD Module(LPSTR ModuleName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	do
		if (!strcmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	while (Module32Next(hModule, &mEntry));
	return 0;
}

void LoadOriginalDll(void)
{
	TCHAR buffer[MAX_PATH];
	::GetSystemDirectory(buffer, MAX_PATH);
	strcat_s(buffer, "\\d3d9.dll");

	if (!hOriginalDll) hOriginalDll = ::LoadLibrary(buffer);

	if (!hOriginalDll)
	{
		::ExitProcess(0);
	}
}


HRESULT InitThread()
{
	bRunning = true;

	char k[3];
	GetPrivateProfileString("FPSFix", "Key", "", k, 3, ".\\FPSFix.ini");
	int key = 0;
	std::stringstream hs;
	hs << std::hex << k;
	hs >> key;

	while (bRunning)
	{
		UINT_PTR pCurrentAnimation = Pointer<UINT_PTR>(0x12E29E8, 0x0, 0xFC)();

		if (GetAsyncKeyState(key) & 1)
		{
			*(DWORD*)pCurrentAnimation = 0;
		}
		Sleep(1);
	}
	return 0;
}

void ExitInstance()
{
	if (hOriginalDll)
	{
		::FreeLibrary(hOriginalDll);
		hOriginalDll = NULL;
	}

	bRunning = false;
	CloseHandle(hInitThread);
}

void InitInstance(HANDLE hModule) 
{
	hOriginalDll        = NULL;
	hThisInstance       = NULL;

	hThisInstance = (HINSTANCE)  hModule;

	dwBase = Module("DARKSOULS.exe");

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	if (hProcess == NULL)
		::ExitProcess(0);

	hInitThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&InitThread, 0, 0, 0);
}
