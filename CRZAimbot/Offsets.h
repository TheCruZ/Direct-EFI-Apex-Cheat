#pragma once

//#define OFFSET_RAN 0//0x34LLU
#define TOFFSET(d1) d1 //((d1)+offset_ran_var)

extern unsigned long long offset_ran_var;

#define ENTITY_SIZE_DEF				0x42E0			//biggest entity offset

#define OFFSET_ENTITYLIST			0x18ad3a8		//cl_entitylist
#define OFFSET_LOCAL_ENT			0x1c5bcc8		//LocalPlayer


#define OFFSET_TEAM					0x0430			//m_iTeamNum
#define OFFSET_HEALTH				0x0420			//m_iHealth
#define OFFSET_NAME					0x0561			//m_iName
#define OFFSET_SIG_NAME				0x0558			//m_iSignifierName
#define OFFSET_SHIELD				0x170			//m_shieldHealth
#define OFFSET_MAX_SHIELD			0x174			//m_shieldHealth +0x4

#define OFFSET_LIFE_STATE			0x0770			//m_lifeState
#define OFFSET_BLEED_OUT_STATE		0x2610			//m_bleedoutState

#define OFFSET_ITEM_ID				0x1608			//m_customScriptInt

#define OFFSET_BULLET_SPEED			0x1e0c			//m_flProjectileSpeed
#define OFFSET_BULLET_GRAVITY		0x1e14			//m_flProjectileScale

#define OFFSET_CURRENT_WEAPON		0x1a0c			//m_latestPrimaryWeapons
#define OFFSET_ORIGIN				0x14C			//m_vecAbsOrigin
#define OFFSET_BONES				0xF18			//m_nForceBone + 0x50-0x8 -> 48 8B 97 ?? ?? ?? ?? 48 8D 04 5B 48 C1
#define OFFSET_AIMPUNCH				0x23c8			//m_vecPunchWeapon_Angle
#define OFFSET_CAMERAPOS			0x1E6C			//40 57 48 83 EC 20 F3 0F 10 0A 48 8B F9 0F 2E 89 first ucomiss result
#define OFFSET_VIEWANGLES			0x24A0			//m_ammoPoolCapacity - 0x14  -> 0F 57 C0 8B 86 ?? ?? ?? ?? F3 segundo uso de un 0x24AX
#define OFFSET_BREATH_ANGLES		(OFFSET_VIEWANGLES - 0x10)
#define OFFSET_OBSERVER_MODE		0x32bc			//m_iObserverMode
#define OFFSET_OBSERVING_TARGET		0x32c0			//m_hObserverTarget

#define OFFSET_HELMET_TYPE			0x42cc			//m_helmetType

#define GLOW_CONTEXT 0x350 //Script_Highlight_SetCurrentContext
#define GLOW_LIFE_TIME 0x32C //Script_Highlight_SetLifeTime + 4
#define GLOW_DISTANCE 0x33C //Script_Highlight_SetFarFadeDist
#define GLOW_TYPE 0x2AC //Script_Highlight_GetState + 4
#define GLOW_COLOR 0x1D0 //Script_CopyHighlightState mov tcx nº7 
#define GLOW_VISIBLE_TYPE 0x360 //Script_Highlight_SetVisibilityType
#define GLOW_FADE 0x310 //Script_Highlight_GetCurrentInsideOpacity 3º result of 3 offsets consecutive or first + 8~



#define OFFSET_MATRIX				0x1b3bd0		//ViewMatrix
#define OFFSET_RENDER				0x40d5d98 	 	//ViewRender 