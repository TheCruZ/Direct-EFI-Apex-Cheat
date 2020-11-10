#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <tlhelp32.h>
#include "MemProtector.h"
#include "Config.h"
#include "Driver.h"
#include "Entity.h"
#include "Math.h"


void LoadProtectedFunctions();
DWORD WINAPI mainThread(PVOID);
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID);
bool CheckDriverStatus();
DWORD GetProcessIdByName(wchar_t* name);
uintptr_t milliseconds_now();
void ProcessPlayer(Entity* LPlayer, Entity* target, UINT64 entitylist);
void UpdatePlayersInfo(Entity* LocalPlayer);
void PredictPosition(Entity* LocalPlayer, Entity* target, Vector* BonePosition);
void AutoBoneSwitch();
void SmoothType_Asist(float fov, float TargetDistance, Vector* Delta, float smooth_multiplier);
void SmoothType_TargetLock(float fov, float TargetDistance, Vector* Delta, float smooth_multiplier);
int AimAngles(Entity* LocalPlayer, Entity* target, Vector* out);
void CheatLoop();
void Configure();