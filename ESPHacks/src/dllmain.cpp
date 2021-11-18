#include "includes.h"


void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

Hack* hack;


void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
	if (!pDevice)
		pDevice = o_pDevice;

	// draw stuffs here

	for (int i = 1; i < 32; i++) {

		Ent* curEnt = hack->entList->ents[i].ent;
		if (!hack->checkValidEnt(curEnt))
			continue;

		D3DCOLOR color;
		if (curEnt->iTeamNum == hack->localEnt->iTeamNum)
			color = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			color = D3DCOLOR_ARGB(255, 255, 0, 0);


		Vec2 entPos2D;
		if (hack->WorldToScreen(curEnt->vecOrigin, entPos2D))
			DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, color);

	}




	//DrawFilledRect(25, 25, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));
	int w = 5;
	int h = 5;


	DrawFilledRect(windowWidth / 2 - (w / 2), windowHeight / 2 - (h / 2), w, h, D3DCOLOR_ARGB(100, 245, 125, 215));

	oEndScene(pDevice);
}



DWORD WINAPI HackThread(HMODULE hModule)
{
	MessageBox(0, "Injected:)", "Hacks", NULL);

	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);


	}
	hack = new Hack();
	hack->init();

	while (!(GetAsyncKeyState(VK_F1) & 1))
	{
		hack->Update();
	}

	MessageBox(0, "Exit:)", "Hacks", NULL);
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID lpr)
{

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
		if (handle)
			CloseHandle(handle);
		break;
	}

	return TRUE;


}