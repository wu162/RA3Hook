#pragma once

typedef struct C_Hotkey_t {
	void* vf_table;
	char pad4[4];
	void* vf_table8; // 00BECB14
	char padC[4];
	void* pHotKeyType; // its +0x10 is hotkey name pointer, +0x14 is this pC_Hotkey!
	int keyIndex; // esc is 1, q is 0x10
	int fnIndex; // ctrl, alt, shift
	char pad1C[5];
	bool isPress; 
}*pC_Hotkey;
#if 1
static_assert(offsetof(C_Hotkey_t, keyIndex) == 0x14);
static_assert(offsetof(C_Hotkey_t, isPress) == 0x21);
#endif

namespace RA3::Core {
	void __fastcall C_Hotkey_Hook();
	void __fastcall C_Hotkey_Initialize(uintptr_t hmodEXE, int isNewSteam);

	bool __fastcall C_Hotkey_PressWaypointMode();

	void __fastcall C_Hotkey_GetKeyShiftToFunctionASM();
	void __fastcall C_Hotkey_GetKeyShiftToBuild0ASM();
}
