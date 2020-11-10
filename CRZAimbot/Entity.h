#pragma once

#include <Windows.h>
#include "Vector.h"
#include "Offsets.h"
#include "Driver.h"

#define NUM_ENT_ENTRIES			(1 << 12)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)
int paddingsub();
class Entity {
public:
	uintptr_t ptr;
	BYTE buffer[ENTITY_SIZE_DEF];
	Vector getPosition();
	bool isPlayer();
	int getTeamId();
	int getHealth();
	int getShield();
	int getMaxShield();
	unsigned char getArmorType();
	unsigned char getHelmetType();
	bool isOkLifeState();
	bool isBleedOut();
	Vector GetViewAngles();
	Vector GetBreathAngles();
	Vector GetCamPos();
	Vector GetRecoil();
	Vector GetViewAnglesV();
	Vector GetVelocity();

	uintptr_t CurrentWeapon(uintptr_t pid, uintptr_t entityList);
	void SetViewAngles(uintptr_t pid, Vector angles);
	Vector getBonePosition(uintptr_t g_PID, int id);
	uintptr_t Observing(uintptr_t g_PID, uintptr_t entitylist);

private:
	struct Bone {
		BYTE shit[0xCC];
		float x;
		BYTE shit2[0xC];
		float y;
		BYTE shit3[0xC];
		float z;
	};
};
Entity* getEntity(uintptr_t g_PID, uintptr_t ptr);