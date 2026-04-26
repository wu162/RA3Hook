#include "../pch.h"

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"

#include "C_ObjectCreationList.h"

namespace RA3::Core {

	uintptr_t _F_OCL_FlingObjectCreateObject = 0x75DA68;
	uintptr_t _Ret_OCL_FlingObjectCreateObject = 0x75DA68 + 7;

	void __fastcall C_ObjectCreationList_Hook()
	{
		// allow it to send the producer's ID
		hookGameBlock((void*)_F_OCL_FlingObjectCreateObject, (uintptr_t)C_OCL_FlingObjectCreateObjectASM);
		WriteHookToProcess((void*)(_F_OCL_FlingObjectCreateObject + 5), (void*)&nop2, 2U);
	}

	void __fastcall C_ObjectCreationList_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_OCL_FlingObjectCreateObject = hmodEXE + 0x35DA68;
			_Ret_OCL_FlingObjectCreateObject = hmodEXE + 0x35DA68 + 7;
		}
	}

	extern uintptr_t _call_UpdateGameObjectProducerID;
	__declspec(naked) void __fastcall C_OCL_FlingObjectCreateObjectASM()
	{
		// check producer
		__asm {
			mov ecx, [esp + 0xAC]
			test ecx, ecx
			jz getTargetAsProducer
			lea eax, [ecx-0x38]
			jmp setProducer
		}
	getTargetAsProducer:
		__asm {
			mov eax, [esp + 0xA4]
			test eax, eax
			jz oldBlock
		}

	setProducer:
		__asm {
			push eax
			mov ecx, ebp
			call _call_UpdateGameObjectProducerID
		}
	oldBlock:
		__asm {
			movss xmm0, dword ptr[esi]
			mov eax, [edi + 4]
			jmp _Ret_OCL_FlingObjectCreateObject
		}
	}

// end namespace RA3::Core
}
