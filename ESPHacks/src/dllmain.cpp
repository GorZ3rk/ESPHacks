#include "includes.h"
#include <sstream>
#include <string.h>

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


	DrawText("EvilC0der here :-)", windowWidth / 2, windowHeight * 1 / 4, D3DCOLOR_ARGB(255, 0, 255, 0));


	for (int i = 1; i < 32; i++) {

		Ent* curEnt = hack->entList->ents[i].ent;
		if (!hack->checkValidEnt(curEnt))
			continue;

		D3DCOLOR color;
		if (curEnt->iTeamNum == hack->localEnt->iTeamNum)
			color = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			color = D3DCOLOR_ARGB(255, 255, 0, 0);


		if (hack->settings.showTeammates && curEnt->iTeamNum == hack->localEnt->iTeamNum)
			continue;






		Vec3 entHead3D = hack->GetBonePos(curEnt, 8);

		entHead3D.z += 8;


		Vec2 entPos2D, entHead2D;
		// snapline
		if (hack->WorldToScreen(curEnt->vecOrigin, entPos2D))
		{
			if (hack->settings.snaplines)
				DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, color);

			if (hack->settings.velEsp)
			{
				Vec3 velOff = curEnt->vecOrigin + (curEnt->vecVelocity * .25);
				Vec2 velOff2D;

				if (hack->WorldToScreen(velOff, velOff2D))
				{
					DrawLine(entPos2D, velOff2D, 2, color);
					DrawFilledRect(velOff2D.x - 2, velOff2D.y - 2, 4, 4, color);
				}

			}




			if (hack->WorldToScreen(entHead3D, entHead2D))
			{
				if (hack->settings.box2D)
				{
					DrawEspBox2D(entPos2D, entHead2D, 2, color);
				}
				
				if (hack->settings.box3D)
				{
					DrawEspBox3D(entHead3D, curEnt->vecOrigin, curEnt->angEyeAnglesY, 25, 2, color);
				}






				if (hack->settings.status2D)
				{
					int height = ABS(entPos2D.y - entHead2D.y);
					int dx = (entPos2D.x - entPos2D.x);

					float healthPerc = curEnt->iHealth / 100.f;
					float armorPerc = curEnt->ArmorValue / 100.f;

					Vec2 botHealth, topHealth, botArmor, topArmor;
					int healthHeight = height * healthPerc;
					int armorHeight = height * armorPerc;

					botHealth.y = botArmor.y = entPos2D.y;

					botHealth.x = entPos2D.x - (height / 4) - 2;

					botArmor.x = entPos2D.x + (height / 4) + 2;

					topHealth.y = entHead2D.y + height - healthHeight;
					topArmor.y = entHead2D.y + height - armorHeight;

					topHealth.x = entPos2D.x - (height / 4) - 2 - (dx * healthPerc);
					topArmor.x = entPos2D.x + (height / 4) + 2 - (dx * armorPerc);

					DrawLine(botHealth, topHealth, 2, D3DCOLOR_ARGB(255, 46, 139, 87));
					DrawLine(botArmor, topArmor, 2, D3DCOLOR_ARGB(255, 30, 144, 255));
				}
			

				if (hack->settings.headlineEsp)
				{
					Vec3 head3D = hack->GetBonePos(curEnt, 8);
					Vec3 entAngles;
					entAngles.x = curEnt->angEyeAnglesX;
					entAngles.y = curEnt->angEyeAnglesY;
					entAngles.z = 0;
					Vec3 endPoint = hack->TransformVec(head3D, entAngles, 60);

					Vec2 endPoint2D, head2D;
					hack->WorldToScreen(head3D, head2D);
					if (hack->WorldToScreen(endPoint, endPoint2D))
					{
						DrawLine(head2D, endPoint2D, 2, color);
					}

				}

	
				if (hack->settings.statusText)
				{
					std::stringstream s1, s2;
					s1 << curEnt->iHealth;
					s2 << curEnt->ArmorValue;
					std::string t1 = "hp: " + s1.str();
					std::string t2 = "ap: " + s2.str();

					char* healthMsg = (char*)t1.c_str();
					char* armorMsg = (char*)t2.c_str();

					DrawText(healthMsg, entPos2D.x, entPos2D.y, D3DCOLOR_ARGB(255, 255, 255, 255));
					DrawText(armorMsg, entPos2D.x, entPos2D.y + 12, D3DCOLOR_ARGB(255, 255, 255, 255));

					if (!curEnt->bHasHelmet)
						DrawText("no helmet", entPos2D.x, entPos2D.y + 24, D3DCOLOR_ARGB(255, 255, 255, 255));


				}


			}
		}

	}




	//DrawFilledRect(25, 25, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));
	//int w = 5;
	//int h = 5;


	//DrawFilledRect(windowWidth / 2 - (w / 2), windowHeight / 2 - (h / 2), w, h, D3DCOLOR_ARGB(100, 245, 125, 215));


	if (hack->settings.rcsCrosshair)
	{
		Vec2 l, r, t, b;
		l = r = t = b = hack->crosshair2D;
		l.x -= hack->crosshairSize;
		r.x += hack->crosshairSize;
		b.y -= hack->crosshairSize;
		t.y += hack->crosshairSize;

		DrawLine(l, r, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
		DrawLine(t, b, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
	}




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

		Vec3 pAng = hack->localEnt->aimPunchAngle;
		hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
		hack->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);



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