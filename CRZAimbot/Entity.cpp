#include "Entity.h"

//unsigned long long offset_ran_var = OFFSET_RAN;

Entity* getEntity(uintptr_t g_PID, uintptr_t ptr) {
	Protect(_ReturnAddress());
	Entity* entity = new Entity();
	entity->ptr = ptr;
	Unprotect(Driver::read_memory);
	Driver::read_memory(g_PID, ptr, (uintptr_t)entity->buffer, TOFFSET(ENTITY_SIZE_DEF));
	Protect(Driver::read_memory);
	Unprotect(_ReturnAddress());
	return entity;
}

int paddingsub() {
	return NULL;
}

uintptr_t Entity::Observing(uintptr_t g_PID, uintptr_t entitylist) {

	if (*(int*)(buffer + TOFFSET(OFFSET_OBSERVER_MODE)) == 5) {
		unsigned long ot = *(unsigned long*)(buffer + TOFFSET(OFFSET_OBSERVING_TARGET));
		int index = ot & ENT_ENTRY_MASK;
		int ObserverID = index;
		if (ObserverID > 0) {
			uintptr_t centity2 = Driver::read<uintptr_t>(g_PID, entitylist + ((uintptr_t)ObserverID << 5));
			return centity2;
		}
		return 1;
	}
	return 0;
}
int Entity::getTeamId() {

	return *(int*)(buffer + TOFFSET(OFFSET_TEAM));
}
int Entity::getHealth() {

	return *(int*)(buffer + TOFFSET(OFFSET_HEALTH));
}

bool Entity::isOkLifeState() {

	return *(int*)(buffer + TOFFSET(OFFSET_LIFE_STATE)) == 0;
}

unsigned char Entity::getArmorType() {

	return *(unsigned char*)(buffer + TOFFSET(OFFSET_HELMET_TYPE) + 0x4);
}

bool Entity::isBleedOut() {
	return *(int*)(buffer + TOFFSET(OFFSET_BLEED_OUT_STATE)) != 0;
}

int Entity::getShield() {
	return *(int*)(buffer + TOFFSET(OFFSET_SHIELD));
}

int Entity::getMaxShield() {
	return *(int*)(buffer + TOFFSET(OFFSET_MAX_SHIELD));
}

unsigned char Entity::getHelmetType() {
	return *(unsigned char*)(buffer + TOFFSET(OFFSET_HELMET_TYPE));
}

Vector Entity::getPosition() {
	return *(Vector*)(buffer + TOFFSET(OFFSET_ORIGIN));
}
bool Entity::isPlayer() {
	return *(uintptr_t*)(buffer + TOFFSET(OFFSET_NAME)) == 125780153691248;
}
Vector Entity::getBonePosition(uintptr_t g_PID, int id) {

	Vector position = getPosition();
	uintptr_t boneArray = *(uintptr_t*)(buffer + TOFFSET(OFFSET_BONES));
	Vector bone = Vector();
	UINT32 boneloc = (id * 0x30);
	Bone bo = {};
	bo = Driver::read<Bone>(g_PID, boneArray + boneloc);

	bone.x = bo.x + position.x;
	bone.y = bo.y + position.y;
	bone.z = bo.z + position.z;
	return bone;
}

Vector Entity::GetViewAngles() {
	return *(Vector*)(buffer + TOFFSET(OFFSET_VIEWANGLES));
}

Vector Entity::GetBreathAngles() {
	return *(Vector*)(buffer + TOFFSET(OFFSET_BREATH_ANGLES));
}

Vector Entity::GetViewAnglesV() {
	return *(Vector*)(buffer + TOFFSET(OFFSET_VIEWANGLES));
}

void Entity::SetViewAngles(uintptr_t pid, Vector angles) {
	Driver::write<Vector>(pid, ptr + TOFFSET(OFFSET_VIEWANGLES), angles);
}


Vector Entity::GetCamPos()
{

	return *(Vector*)(buffer + TOFFSET(OFFSET_CAMERAPOS));
}

Vector Entity::GetRecoil()
{

	return *(Vector*)(buffer + TOFFSET(OFFSET_AIMPUNCH));
}

uintptr_t Entity::CurrentWeapon(uintptr_t pid, uintptr_t entityList) {

	uintptr_t ActWeaponId = *(uintptr_t*)(buffer + TOFFSET(OFFSET_CURRENT_WEAPON)) & 0xFFFF;
	if (ActWeaponId)
	{

		return Driver::read<uintptr_t>(pid, entityList + (ActWeaponId << 5));
	}
	return 0;
}
Vector Entity::GetVelocity() {

	return *(Vector*)(buffer + TOFFSET(OFFSET_ORIGIN) - 0xC);
}

