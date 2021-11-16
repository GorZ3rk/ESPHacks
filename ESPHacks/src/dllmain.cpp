#include "includes.h"


void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
	if (!pDevice)
		pDevice = o_pDevice;

	// draw stuffs here

	DrawFilledRect(25, 25, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));

	oEndScene(pDevice);
}



DWORD WINAPI HackThread(HMODULE hModule)
{
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);


	}

	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);



	while (!GetAsyncKeyState(VK_END))
	{
		Sleep(1);
	}


	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	}
	return TRUE;

}