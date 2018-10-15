#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include "SDK.h"
#include "hooking.h"
#include "KeyManger.h"
#include "tools.h"
#include "Features.h"
#include "Render.h"
#include "math.h"
#include "GuiList.h"
#include "MD5_Checksum.h"

IClientEntityList * ClientEntityList;
IVEngineClient * EngineClient;
CGlobalVarsBase *GlobalVars;
IEngineTrace *EngineTrace;
CHLClient *Client;
ISurface * Surface;
CInput * Input;
IPanel * Panel;

typedef void(__thiscall *PaintTraverse_t)(void*, unsigned int, bool, bool);
typedef void(__thiscall *Createmove_t)(void *, int, float, bool);

Createmove_t oCHLClientCreatemove;
Createmove_t oInputCreatemove;
PaintTraverse_t oPaintTraverse;
GetChecksum_t GetChecksum;

cKeyManger *KeyManger;
cFeatures *Features;
cGuiLast *GuiList;



void __stdcall  hkPaintTraverse_t(int VGUIPanel, bool ForceRepaint, bool AllowForce) {
	oPaintTraverse(Panel, VGUIPanel, ForceRepaint, AllowForce);
	if (strcmp("FocusOverlayPanel", Panel->GetName(VGUIPanel)) != 0)
		return;
	KeyManger->Control();
	static bool bInsertClicked = false;
	static bool bShowMenu = false;
	if (GetAsyncKeyState(VK_INSERT) || GetAsyncKeyState(VK_F9)) {
		if (!bInsertClicked)
			bShowMenu = !bShowMenu;
		bInsertClicked = true;
	}
	else {
		bInsertClicked = false;
	}


	Render::Printf(15, 15, cColor(255, 255, 255, 255), 0, "Dino D-Day Hack -- by mactec");
	if (EngineClient->IsInGame()) {
		int Screen_Width, Screen_Height;
		EngineClient->GetScreenSize(Screen_Width, Screen_Height);

		CBaseEntity* pLocal = (CBaseEntity*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());
		if (pLocal&&Features->bGetValue(v_ESPEnabled))
			for (int i = 0; i <= ClientEntityList->GetHighestEntityIndex(); ++i) {
				CBaseEntity* Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
				if (!Entity)
					continue;
				if (Entity == pLocal)
					continue;
				if (!Features->bGetValue(v_ESPEveryone))
					if (Entity->GetTeam() == pLocal->GetTeam())
						continue;
				if (Entity->GetHealth() <= 1)
					continue;
				if (Entity->GetLifeState() != 0)
					continue;

				Vector vecOrigin2D, vecOrigin = Entity->GetOrigin();
				if (WorldToScreen(vecOrigin, vecOrigin2D)) {
					if (Features->bGetValue(v_ESPBoxInfo)) {
						PlayerInfo_t Info;
						EngineClient->GetPlayerInfo(i, &Info);
						std::string strName = Info.Name;
						strName += " [%dm]";
						Render::Printf(vecOrigin2D.x, vecOrigin2D.y + 8, cColor(255, 0, 0, 255), 1, (char*)strName.c_str(), (int)(Dist3D(pLocal->GetOrigin(), vecOrigin)*0.01904f));
						Render::Printf(vecOrigin2D.x, vecOrigin2D.y + 16, cColor(0, 255, 0, 255), 1, "HP %d", Entity->GetHealth());
					}

					cColor col = cColor(244, 66, 238, 255);
					Vector vecHead2D, vecHead = Entity->GetBoneOrigin(10);

					if (Entity->GetTeam() == pLocal->GetTeam()) {
						col = cColor(55, 0, 200, 255);
					}
					else {
						if (GetVisible(pLocal->GetEyePosition(), vecHead, Entity, pLocal))
							col = cColor(255, 0, 0, 255);
					}

					vecHead += Vector(0, 0, 10.f);
					if (WorldToScreen(vecHead, vecHead2D)) {
						int Height = abs(vecHead2D.y - vecOrigin2D.y);
						int Width = 0.4*Height + abs(vecHead2D.x - vecOrigin2D.x);

						if (Features->bGetValue(v_ESPBox))
							Render::DrawESPRect(vecHead2D.x - Width / 2, vecHead2D.y, Width, Height, col);

						if (Features->bGetValue(v_ESPLines))
							Render::DrawLine(vecHead2D.x - Width / 2, vecOrigin2D.y, Screen_Width / 2, Screen_Height, col);

					}
				}
			}

	}

	if (bShowMenu) {
		GuiList->Control();
		GuiList->Draw();
	}

}




void __fastcall hkInputCreateMove(void * ecx, void * edx, int sequence_number, float input_sample_frametime, bool active) {
	oInputCreatemove(ecx, sequence_number, input_sample_frametime, active);
	if (!EngineClient->IsInGame() && !EngineClient->IsConnected())
		return;

	CVerifiedUserCmd *pVerifiedCommands = *(CVerifiedUserCmd**)((uint32_t)Input + 0xE8);
	if (!pVerifiedCommands)
		return;


	CVerifiedUserCmd *pVerifiedCmd = &pVerifiedCommands[sequence_number % 150];

	CUserCmd *pUserCmd = Input->GetUserCMD(0, sequence_number);
	if (!pUserCmd || !pVerifiedCmd)
		return;


	bool bBulletTime = false;

	CBaseEntity * pLocal = (CBaseEntity*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());
	if (!pLocal)
		return;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)ClientEntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (pWeapon) {
		float m_flNextPrimaryAttack = pWeapon->NextAttack();
		float flCurTime = pLocal->GetTickBase()*GlobalVars->interval_per_tick;
		if (m_flNextPrimaryAttack > flCurTime) {
			bBulletTime = true;
		}

	}

	if (Features->bGetValue(v_NameStealer) && pUserCmd->tick_count % 2 == 0) {
		std::vector<std::string> vNames;
		vNames.reserve(EngineClient->GetMaxClients());
		for (int i = 0; i < EngineClient->GetMaxClients(); i++) {
			CBaseEntity* pEntity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
			if (!pEntity || pEntity == pLocal)
				continue;
			PlayerInfo_t Info;
			EngineClient->GetPlayerInfo(i, &Info);
			if (std::string(Info.Name).length() > 2)
				vNames.push_back(Info.Name);
		}
		if (vNames.size() > 0)
			EngineClient->ClientCMD((std::string("setinfo name \"") + vNames[rand() % vNames.size()] + std::string("\"")).c_str());
	}


	Vector vOldAngles = pUserCmd->viewangles;
	float oSide = pUserCmd->sidemove;
	float oForward = pUserCmd->fowardmove;

	if (Features->bGetValue(v_BunnyHop))
		if (pUserCmd->buttons & IN_JUMP)
			if (!(pLocal->GetFlags() & FL_ONGROUND))
				pUserCmd->buttons &= ~IN_JUMP;


	static bool bDirectionAutoStrafe = false;
	if (Features->iGetValue(v_AutoStrafe) == AUTOSTRAFE_PERFECT) {
		if (GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) {
			if (pUserCmd->mousedx < 0)
				pUserCmd->sidemove = -445.f;
			if (pUserCmd->mousedx > 0)
				pUserCmd->sidemove = 445.f;
			else {
				vOldAngles.y += (bDirectionAutoStrafe) ? -2 : 2;
				pUserCmd->sidemove += (bDirectionAutoStrafe) ? -440.f : 440.f;
			}
			bDirectionAutoStrafe = !bDirectionAutoStrafe;
			pUserCmd->fowardmove = 0;
			oForward = pUserCmd->fowardmove;
			oSide = pUserCmd->sidemove;
			Normalize(vOldAngles);
			pUserCmd->viewangles = vOldAngles;

		}
	}

	if (Features->iGetValue(v_AutoStrafe) == AUTOSTRAFE_SILENT && !(pLocal->GetFlags() & FL_ONGROUND) && GetAsyncKeyState(VK_SPACE)) {
		if (pUserCmd->mousedx < 0)
			pUserCmd->sidemove = -445.f;
		if (pUserCmd->mousedx > 0)
			pUserCmd->sidemove = 445.f;
		oSide = pUserCmd->sidemove;
	}


	int AimID = -1;
	int lastDist = 360000000;
	if (!bBulletTime&&Features->bGetValue(v_AIMEnabled) && (Features->bGetValue(v_AIMAutoshoot) || pUserCmd->buttons & IN_ATTACK)) {
		for (int i = 0; i <= ClientEntityList->GetHighestEntityIndex(); ++i) {
			CBaseEntity* Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
			if (!Entity)
				continue;
			if (Entity == pLocal)
				continue;
			if (Entity->GetTeam() == pLocal->GetTeam())
				continue;
			if (Entity->GetHealth() <= 1)
				continue;
			if (Entity->GetLifeState() != 0)
				continue;
			Vector vecHead = Entity->GetBoneOrigin(10);
			if (!GetVisible(pLocal->GetEyePosition(), vecHead, Entity, pLocal))
				continue;

			float distance = Dist3D(pLocal->GetOrigin(), Entity->GetOrigin());
			if (distance < lastDist) {
				AimID = i;
				lastDist = distance;
			}
		}
	}

	if (AimID != -1) {
		CBaseEntity* Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(AimID);
		Vector vecHead = Entity->GetBoneOrigin(10);
		Vector vecAim = vecHead - pLocal->GetEyePosition();
		vecAim.NormalizeInPlace();
		pUserCmd->viewangles = vecAim.toAngles();
		pUserCmd->buttons |= IN_ATTACK;
		if (Features->bGetValue(v_AIMNoRecoil) && pWeapon)
			pUserCmd->viewangles -= pLocal->GetPunch();


		if (Features->bGetValue(v_AIMNoSpread) && pWeapon&& pUserCmd->buttons & IN_ATTACK)
			CompestateSpread(pUserCmd, pWeapon);


	}

	if (!Features->bGetValue(v_AIMEnabled) && Features->bGetValue(v_AIMNoRecoil) && pWeapon)
		pUserCmd->viewangles -= pLocal->GetPunch();

	if (!Features->bGetValue(v_AIMEnabled) && Features->bGetValue(v_AIMNoSpread) && pWeapon&&pUserCmd->buttons & IN_ATTACK)
		CompestateSpread(pUserCmd, pWeapon);

	CorrectMovement(vOldAngles, pUserCmd, oForward, oSide);


	if (!Features->bGetValue(v_AIMSilent))
		EngineClient->SetViewAngles(pUserCmd->viewangles);

	pVerifiedCmd->m_cmd = *pUserCmd;
	pVerifiedCmd->m_crc = GetChecksum(pUserCmd);
}

void __fastcall hkCHClientCreateMove(void *ecx, void* edx, int sequence_number, float input_sample_frametime, bool active) {
	static int speed_factor = 0;
	oCHLClientCreatemove(ecx, sequence_number, input_sample_frametime, active);
	void* base;
	__asm mov base, ebp;
	uint32_t* retaddr = (uint32_t*)(*(uint32_t*)base + 0x4);
	if (GetAsyncKeyState(VK_LSHIFT)) {
		if (speed_factor > 1) {
			speed_factor--;
			*retaddr -= 0x5;
		}
		else {
			speed_factor = Features->iGetValue(v_pSpeed);
		}
	}
}

void LoadHack() { 
	Features = new cFeatures();
	KeyManger = new cKeyManger();
	KeyManger->init();

	Surface = (ISurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface031");
	Client = (CHLClient*)GetInterface("client.dll", "VClient016");
	Panel = (IPanel*)GetInterface("vgui2.dll", "VGUI_Panel009");
	EngineTrace = (IEngineTrace*)GetInterface("engine.dll", "EngineTraceClient004");
	EngineClient = (IVEngineClient*)GetInterface("engine.dll", "VEngineClient015");
	ClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");

	GuiList = new cGuiLast();

	oPaintTraverse = (PaintTraverse_t)hook_vmt(Panel, 41, (uint32_t)hkPaintTraverse_t);

	uint32_t GetCheckSumAddr = FindPatternIDA((uint32_t)GetModuleHandleSafe("client.dll"), "55 8B EC 51 56 8D 45 FC 50 8B F1 E8 ? ? ? ? 6A 04");
	GetChecksum = (GetChecksum_t)(GetCheckSumAddr);

	uint32_t *ClientVMT = *(uint32_t**)Client;
	Input = *(CInput**)(uint32_t*)(ClientVMT[15] + 0x2);
	Input = *(CInput**)(Input);


	uint32_t *InputVMT = *(uint32_t**)Input;
	GlobalVars = *(CGlobalVarsBase**)(uint32_t*)(InputVMT[3] + 0x111);
	GlobalVars = *(CGlobalVarsBase**)(GlobalVars);


	oInputCreatemove = (Createmove_t)hook_vmt(Input, 3, (uint32_t)hkInputCreateMove); 
	oCHLClientCreatemove = (Createmove_t)hook_vmt(Client, 21, (uint32_t)hkCHClientCreateMove);
}