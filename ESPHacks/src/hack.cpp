#include "includes.h"

Hack::~Hack() {
	this->FontF->Release();
	this->LineL->Release();
}


void Hack::init() {
	client = (uintptr_t)GetModuleHandle("client.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");
	entList = (EntList*)(client + dwEntityList);
	localEnt = entList->ents[0].ent;
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)(client + dwViewMatrix), sizeof(viewMatrix));
}

bool Hack::checkValidEnt(Ent* ent) {
	if (ent == nullptr)
		return false;

	if (ent == localEnt)
		return false;

	if (ent->iHealth <= 0)
		return false;

	if (ent->isDormant)
		return false;

	return true;


}

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen) {
	
	Vec4 clipboards;
	clipboards.x		= pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipboards.y		= pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipboards.z		= pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipboards.w		= pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipboards.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x				= clipboards.x / clipboards.w;
	NDC.y				= clipboards.y / clipboards.w;
	NDC.z				= clipboards.z / clipboards.w;

	screen.x			= (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y			= - (windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	
	return true;

}

Vec3 Hack::GetBonePos(Ent* ent, int bone)
{
	uintptr_t bonePtr = ent->boneMatrix;
	Vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);

	return bonePos;

}

Vec3 Hack::TransformVec(Vec3 src, Vec3 ang, float d)
{
	Vec3 newPos;
	newPos.x = src.x + (cosf(TORAD(ang.y)) * d);
	newPos.y = src.y + (sinf(TORAD(ang.y)) * d);
	newPos.z = src.z + (tanf(TORAD(ang.x)) * d);
	return newPos;


}
