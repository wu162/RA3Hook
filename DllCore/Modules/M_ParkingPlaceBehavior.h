#pragma once

typedef struct M_ParkingPlaceBehavior28_t {
	struct vft28_t {
		char pad0[0x18];
		void(__fastcall* func18)(M_ParkingPlaceBehavior28_t* pIn, void* fn, int AircraftID);
		char pad1C[0x38];
		void(__fastcall* func54)(M_ParkingPlaceBehavior28_t* pIn, void* fn, void* pGO_Aircraft);
	};
	#if 1
		static_assert(offsetof(vft28_t, func54) == 0x54);
	#endif

	vft28_t* vft28;
}*pM_ParkingPlaceBehavior28;

typedef pM_ParkingPlaceBehavior28(__cdecl* CallFunc_GetParkingPlaceBehavior28FromID)(int ObjectID, int a2);

typedef struct M_ParkingPlaceBehavior24_t {
	void* vft24;
	M_ParkingPlaceBehavior28_t d28;
}*pM_ParkingPlaceBehavior24;

typedef struct M_ParkingPlaceBehavior_t : baseCommonModule_t {
	M_ParkingPlaceBehavior24_t data;
}*pM_ParkingPlaceBehavior;

namespace RA3::Module {
	void __fastcall M_ParkingPlaceBehavior_Hook();
	void __fastcall M_ParkingPlaceBehavior_Initialize(uintptr_t hmodEXE, int isNewSteam);

	// vft28+0x3c
	void __fastcall M_ParkingPlaceBehavior28_CheckEnterCursorASM();
	//
	void __fastcall M_ParkingPlaceBehavior28_LeaveAirfieldASM();
	void __fastcall M_ParkingPlaceBehavior28_EnterAirfieldASM();
	void __fastcall M_ParkingPlaceBehavior28_EnterAirfieldCPP(pM_ParkingPlaceBehavior pIn, void* pAircraft);

	void __fastcall M_ParkingPlaceBehavior28_TakeOffFromAirfieldCPP(pM_ParkingPlaceBehavior28 pParkingPlaceBehavior28, void* pAircraft);

	pM_ParkingPlaceBehavior28 __cdecl M_ParkingPlaceBehavior28_GetFromID(int ObjectID, int a2);
}
