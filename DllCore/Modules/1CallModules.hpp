#pragma once

#include "M_BezierProjectile.h"
#include "M_LaserState.h"
#include "M_ShieldSphereUpdate.h"
//#include "M_AttributeModifierAuraUpdate.h
#include "M_PassiveAreaEffectBehavior.h"

#include "M_ProductionUpdate.h"
#include "M_ParkingPlaceBehavior.h"

namespace RA3::Module {

void __fastcall HookFunctionSeries_Module() {
	M_BezierProjectile_Hook();
	M_LaserState_Hook();
	M_ShieldSphereUpdate_Hook();
	M_PassiveAreaEffectBehavior_Hook();

	M_ProductionUpdate_Hook();
	M_ParkingPlaceBehavior_Hook();
}

void __fastcall InitializeHookFunctionSeries_Module(uintptr_t hmodEXE, int isNewSteam) {
	M_LaserState_Initialize(hmodEXE, isNewSteam);
	M_ShieldSphereUpdate_Initialize(hmodEXE, isNewSteam);
	M_BezierProjectile_Initialize(hmodEXE, isNewSteam);
	M_PassiveAreaEffectBehavior_Initialize(hmodEXE, isNewSteam);

	M_ProductionUpdate_Initialize(hmodEXE, isNewSteam);
	M_ParkingPlaceBehavior_Initialize(hmodEXE, isNewSteam);
}

// end namespace RA3::Module
}
