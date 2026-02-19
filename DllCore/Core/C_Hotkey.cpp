#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "C_Hotkey.h"

namespace RA3::Core {
	// base is 00CAD80C, it is CAD814
	auto C_Hotkey_WaypointMode = (pC_Hotkey*)(0xCAD80C + 8);

	uintptr_t _F_GetKeyShiftToFunction = 0xAB72B5;

	uintptr_t _F_GetKeyShiftToBuild0 = 0xABFCE2;
	uintptr_t _call_CheckShiftToBuildCommand = 0xABBB10;

	void __fastcall C_Hotkey_Hook()
	{
		hookGameBlock((void*)_F_GetKeyShiftToFunction, (uintptr_t)C_Hotkey_GetKeyShiftToFunctionASM);
		hookGameCall((void*)_F_GetKeyShiftToBuild0, (uintptr_t)C_Hotkey_GetKeyShiftToBuild0ASM);
	}

	void __fastcall C_Hotkey_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			C_Hotkey_WaypointMode = (pC_Hotkey*)(0xCB1ED4 + 8);

			_F_GetKeyShiftToFunction = 0xA4BF35;

			_F_GetKeyShiftToBuild0 = 0xA54942;
			_call_CheckShiftToBuildCommand = 0xA50740;
		}
	}

	bool __fastcall C_Hotkey_PressWaypointMode()
	{
		auto pKey = *C_Hotkey_WaypointMode;
		if (pKey) {
			return pKey->isPress;
		}

		return false;
	}

	__declspec(naked) void __fastcall C_Hotkey_GetKeyShiftToFunctionASM()
	{
		__asm {
			test eax, 0x40 // left alt
			jnz hasAlt
			test eax, 0x80 // right alt
			jnz rightAlt
			test eax, 0x230 // shift
			mov eax, [esi]
			mov ecx, esi
			jz noShift
			mov edx, [eax + 0x24]
			call edx
			pop esi
			ret 4
		}
	noShift:
		__asm {
			mov edx, [eax + 0x20]
			call edx
			pop esi
			ret 4
		}
	hasAlt:
		__asm {
			mov edx, [esi]
			mov ecx, esi
			mov eax, [edx + 0x20]
			or edx, -1
			call eax
			pop esi
			ret 4
		}
	rightAlt:
		__asm {
			mov edx, [esi]
			mov ecx, esi
			mov eax, [edx + 0x20]
			or edx, -2
			call eax
			pop esi
			ret 4
		}
		// end
	}

	__declspec(naked) void __fastcall C_Hotkey_GetKeyShiftToBuild0ASM()
	{
		__asm {
			cmp edx, -1
			je setEDX
			cmp edx, -2
			je setEDX
			jmp _call_CheckShiftToBuildCommand
		}
	setEDX:
		__asm {
			mov [esp+4], edx
			jmp _call_CheckShiftToBuildCommand
		}
	}

// end namespace RA3::Core
}
