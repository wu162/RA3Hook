#pragma once

void __fastcall SetNoBloomASM();

void __fastcall KillTibCrystalWhenEmptyASM1();
void __fastcall KillTibCrystalWhenEmptyASM2();
void __fastcall BehaviorUpdate_TiberiumCrystal();

void __fastcall SecondaryObjectListenerModule_Initialize();
void __fastcall SecondaryObjectListenerModule_SetupUpgrade1();
void __fastcall SecondaryObjectListenerModule_SetupUpgrade2(void* pModule, void* edx, void* pUpgrade, int32_t Gcount);

void __fastcall newForceFeedback0x5216D0ASM(void* pthis, void* edx, void* projectile, void* attacker);
