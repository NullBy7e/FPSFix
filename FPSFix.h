#pragma once

IDirect3D9* WINAPI Direct3DCreate9 (UINT SDKVersion);

void InitInstance(HANDLE hModule);
void ExitInstance(void);
void LoadOriginalDll(void);

#pragma data_seg (".d3d9_shared")
HINSTANCE           hOriginalDll;
HINSTANCE           hThisInstance;
#pragma data_seg ()

HANDLE hInitThread;
HANDLE hProcess;

BOOL bRunning;
INT BoundKey;

DWORD dwBase;