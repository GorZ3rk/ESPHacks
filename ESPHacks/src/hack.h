#pragma once

#include "csgo.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))


#define TORAD(x) ((x) * 0.01745329252)

#define W2S(x, y) hack->WorldToScreen(x, y)


struct Vec2 {
	float x, y;

};

struct Vec3 {
	float x, y, z;

	Vec3 operator+(Vec3 d)
	{
		return { x + d.x, y + d.y, z + d.z };
	}
	Vec3 operator*(float d)
	{
		return { x * d, y * d, z * d };
	}

};

struct Vec4 {
	float x, y, z, w;
};

// entitylist
class Ent {
public:
	// using padding techniques
	union {
		// isDormant
		DEFINE_MEMBER_N(bool, isDormant, hazedumper::signatures::m_bDormant);
		// ihealth
		DEFINE_MEMBER_N(int, iHealth, hazedumper::netvars::m_iHealth);
		// vecOrigin
		DEFINE_MEMBER_N(Vec3, vecOrigin, hazedumper::netvars::m_vecOrigin);
		// iTeamNum
		DEFINE_MEMBER_N(int, iTeamNum, hazedumper::netvars::m_iTeamNum);
		// boneMatrix
		DEFINE_MEMBER_N(int, boneMatrix, hazedumper::netvars::m_dwBoneMatrix);
		// armorValue
		DEFINE_MEMBER_N(int, ArmorValue, hazedumper::netvars::m_ArmorValue);
		// aimPunchAngle
		DEFINE_MEMBER_N(Vec3, aimPunchAngle, hazedumper::netvars::m_aimPunchAngle);
		// angEyeAnglesX
		DEFINE_MEMBER_N(float, angEyeAnglesX, hazedumper::netvars::m_angEyeAnglesX);
		// angEyeAnglesY
		DEFINE_MEMBER_N(float, angEyeAnglesY, hazedumper::netvars::m_angEyeAnglesY);
		// vecVelocity
		DEFINE_MEMBER_N(Vec3, vecVelocity, hazedumper::netvars::m_vecVelocity);
		// bHasHelmet
		DEFINE_MEMBER_N(bool, bHasHelmet, hazedumper::netvars::m_bHasHelmet);



	};
};


class EntListObj {
public:
	struct Ent* ent;
	char padding[12];

};


class EntList {
public:
	EntListObj ents[32];
};



class Hack {
public:
	uintptr_t dwEntityList = hazedumper::signatures::dwEntityList;
	uintptr_t dwViewMatrix = hazedumper::signatures::dwViewMatrix;

	uintptr_t engine;
	uintptr_t client;

	Ent* localEnt;
	EntList* entList;
	float viewMatrix[16];

	ID3DXLine* LineL;
	ID3DXFont* FontF;

	Vec2 crosshair2D;
	int crosshairSize = 4;

	~Hack();

	void init();
	void Update();



	bool checkValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 GetBonePos(Ent* ent, int bone);

	Vec3 TransformVec(Vec3 src, Vec3 ang, float d);

	struct Settings {
		bool showTeammates		= true;
		bool snaplines			= true;
		bool box2D				= false;
		bool status2D			= false;
		bool statusText			= true;
		bool box3D				= true;
		bool velEsp				= true;
		bool headlineEsp		= true;
		bool rcsCrosshair		= true;
	} settings;
};