#pragma once
#include "../Base/CommonStructure.hpp"

enum class BezierProjectileFlag : int {
	TUMBLE_RANDOMLY,
	DETONATE_CALLS_KILL,
	ORIENT_TO_FLIGHT_PATH,
	CRUSH_STYLE,
	NO_DETONATE, // 4
	DIE_ON_IMPACT,
	IGNORE_TERRAIN_HEIGHT,
	PING_PONG_SIDEWAYS_DRIFT,
	ADJUST_STRAIGHT_ONLY,
	DONT_DIE_ON_DETONATE,
	DONT_SET_NO_ATTACK_STATUS,
	DONT_TRACK_TARGET,
	DETONATE_ON_APEX,
	USE_ATTACHPOS_ON_VICTIM,
	IGNORE_CONTACT_POINTS,
	TARGET_OFFSET_ALONG_TARGET_VECTOR,
	FIRE_STRAIGHT_THEN_CURVE,
	DONT_DETONATE_WITHOUT_COLLISION,
	all_count,
};

typedef struct M_BezierProjectile_t : baseCommonModule_t {
	char pad24[0xC];
	float posInBase[3]; // position of fire object
	char pad3C[0x1C];
	float StartPos[3], EndPos[3];
	float deltaPos[3];
	float ammospeed; // is WeaponSpeed / logicframe in 0077B8E4
	int int80, alivetime;
	char pad88[0x40];
	float CurrentSpeed; // really?
}*pM_BezierProjectile;
#if 1
	static_assert(offsetof(M_BezierProjectile_t, posInBase) == 0x30);
	static_assert(offsetof(M_BezierProjectile_t, ammospeed) == 0x7C);
	static_assert(offsetof(M_BezierProjectile_t, CurrentSpeed) == 0xC8);
#endif
// +D1 a byte now to check for ground collision

typedef struct Data_BezierProjectile_t : baseBinDataHeader_t {
	std::bitset<(size_t)BezierProjectileFlag::all_count>  Flags;
	int Type;
	char pad10[8];
	float FireStraightDistance; // maybe
	float FirstHeightMin, FirstHeightMax;
	float SecondHeightMin, SecondHeightMax;
	float FirstPercentIndent, SecondPercentIndent;
	char pad34[0xB4];
	float MaxDistanceToTravel, TargetVectorOffset, Acceleration;
}*pData_BezierProjectile;
#if 1
	static_assert(offsetof(Data_BezierProjectile_t, Type) == 0xC);
	static_assert(offsetof(Data_BezierProjectile_t, TargetVectorOffset) == 0xEC);
	static_assert(offsetof(Data_BezierProjectile_t, Acceleration) == 0xF0);
#endif

namespace RA3::Module {
	void __fastcall M_BezierProjectile_Hook();
	void __fastcall M_BezierProjectile_Initialize(uintptr_t hmodEXE, int isNewSteam);

	void __fastcall M_BezierProjectile_InitializeASM();
	int __fastcall M_BezierProjectile_Module00CPP(char* pIn);

	void __fastcall M_BezierProjectile_CheckProjectilePositionASM();
	void __fastcall M_BezierProjectile_CheckProjectilePositionCPP(pM_BezierProjectile pIn, void* pGO, float DefaultHeight);

	void __fastcall M_BezierProjectile_AddToTheShieldSphereManagerASM();
}
