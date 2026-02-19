#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "../Core/C_System.h"
#include "../Core/C_GameObject.h"
#include "M_ParkingPlaceBehavior.h"

namespace RA3::Module {

	uintptr_t _F_ParkingPlaceBehavior28_CheckEnterCursor = 0x6E3CB3;
	uintptr_t _Ret_ParkingPlaceBehavior28_CheckEnterCursor = 0x6E3CB3 + 22;
	uintptr_t _F_ParkingPlaceBehavior28_LeaveAirfield = 0x7F38D4;
	uintptr_t _Ret_ParkingPlaceBehavior28_LeaveAirfield = 0x7F38D4 + 51;
	uintptr_t _F_ParkingPlaceBehavior28_EnterAirfield = 0x7637AD;
	uintptr_t _Ret_ParkingPlaceBehavior28_EnterAirfield = 0x7637AD + 35;

	uintptr_t _call_ParkingPlaceBehavior28_GetFromID = 0x739720;

	void __fastcall M_ParkingPlaceBehavior_Hook()
	{
		hookGameBlock((void*)_F_ParkingPlaceBehavior28_CheckEnterCursor, (uintptr_t)M_ParkingPlaceBehavior28_CheckEnterCursorASM);
		WriteHookToProcess((void*)(_F_ParkingPlaceBehavior28_CheckEnterCursor + 5), (void*)&nop3, 3U);

		hookGameBlock((void*)_F_ParkingPlaceBehavior28_LeaveAirfield, (uintptr_t)M_ParkingPlaceBehavior28_LeaveAirfieldASM);
		WriteHookToProcess((void*)(_F_ParkingPlaceBehavior28_LeaveAirfield + 5), (void*)&nop1, 1U);

		hookGameBlock((void*)_F_ParkingPlaceBehavior28_EnterAirfield, (uintptr_t)M_ParkingPlaceBehavior28_EnterAirfieldASM);
	}

	void __fastcall M_ParkingPlaceBehavior_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_ParkingPlaceBehavior28_CheckEnterCursor = 0x722123;
			_Ret_ParkingPlaceBehavior28_CheckEnterCursor = 0x722123 + 22;
			_F_ParkingPlaceBehavior28_LeaveAirfield = 0x831A24;
			_Ret_ParkingPlaceBehavior28_LeaveAirfield = 0x831A24 + 51;
			_F_ParkingPlaceBehavior28_EnterAirfield = 0x7A1C6D;
			_Ret_ParkingPlaceBehavior28_EnterAirfield = 0x7A1C6D + 35;

			_call_ParkingPlaceBehavior28_GetFromID = 0x777CD0;
		}
	}

	__declspec(naked) void __fastcall M_ParkingPlaceBehavior28_CheckEnterCursorASM()
	{
		__asm {
			xor esi, esi
			align 16
		}
	checkUnit:
		__asm {
			cmp [eax + 0x34], edi // check unit is not self
			je ofs2E3CC9
			cmp dword ptr[eax + 0x34], 0 // check has unit
			jne ofs2E3CB8
			cmp dword ptr[eax + 0x30], 4 // chech is not Helipad
			je ofs2E3CB8
			inc esi
		}
	ofs2E3CB8:
		__asm {
			add eax, 0x3C
			cmp eax, edx
			jne checkUnit
			//
			test esi, esi
			sete al
			pop edi
			pop esi
			add esp, 0x24
			ret 4
		}
	ofs2E3CC9:
		__asm {
			mov esi, [esp + 0x2C + 4]
			jmp _Ret_ParkingPlaceBehavior28_CheckEnterCursor
		}
	}

	__declspec(naked) void __fastcall M_ParkingPlaceBehavior28_LeaveAirfieldASM()
	{
		__asm {
			mov ecx, [edi + 0x418] // get this player
			test ecx, ecx
			je LeaveAirfield
			mov edx, [ecx + 0x1318] // get CurrentAircraftCount
			cmp edx, [ecx + 0x1314] // compare with MaxAircraftCount
			jg LeaveAirfield
			jmp _Ret_ParkingPlaceBehavior28_LeaveAirfield
		}
	LeaveAirfield:
		__asm {
			mov eax, [eax + 0xDC] // get airfield ID
			push 0
			push eax
			call _call_ParkingPlaceBehavior28_GetFromID
			add esp, 8
			test eax, eax
			jz EndBlock
			mov edx, edi
			mov ecx, eax
			call M_ParkingPlaceBehavior28_TakeOffFromAirfieldCPP
		}
	EndBlock:
		__asm {
			jmp _Ret_ParkingPlaceBehavior28_LeaveAirfield
		}
	}

	__declspec(naked) void __fastcall M_ParkingPlaceBehavior28_EnterAirfieldASM()
	{
		__asm {
			cmp byte ptr[ecx + 0x70], 0
			je NoEnter
			mov eax, [ecx + 0x38]
			mov ecx, [ecx + 0x3C]
			cmp eax, ecx
			je NoEnter
			align 16
		}
	checkSeat:
		__asm {
			cmp dword ptr[eax + 0x34], 0
			jne nextSeat
			cmp byte ptr[eax + 0x38], 0
			jne nextSeat
			cmp dword ptr[eax + 0x30], 4
			jne EnterAirfield
		}
	nextSeat:
		__asm {
			add eax, 0x3C
			cmp eax, ecx
			jne checkSeat
		}
	NoEnter:
		__asm {
			pop edi
			pop esi
			pop ebp
			mov eax, 1
			pop ebx
			add esp, 0x50
			ret 8
		}

	EnterAirfield:
		__asm {
			mov edx, esi
			lea ecx, [edi - 0x28]
			call M_ParkingPlaceBehavior28_EnterAirfieldCPP
			mov ecx, [esi + 0x374]
			jmp _Ret_ParkingPlaceBehavior28_EnterAirfield
		}
	}

	void __fastcall M_ParkingPlaceBehavior28_EnterAirfieldCPP(pM_ParkingPlaceBehavior pIn, void* pAircraft)
	{
		auto aircraft = (pC_GameObject)pAircraft;

		auto airfieldInAircraft = aircraft->ProducerID;
		if (!airfieldInAircraft) {
			return;
		}

		auto airfield = (pC_GameObject)pIn->pGameObject;
		auto airfieldInThis = airfield->CurrentID;
		if (airfieldInAircraft == airfieldInThis) {
			return;
		}

		aircraft->ProducerID = airfieldInThis;
		auto oldAirfield = M_ParkingPlaceBehavior28_GetFromID(airfieldInAircraft, 0);
		if (oldAirfield) {
			M_ParkingPlaceBehavior28_TakeOffFromAirfieldCPP(oldAirfield, pAircraft);
		}
	}

	void __fastcall M_ParkingPlaceBehavior28_TakeOffFromAirfieldCPP(pM_ParkingPlaceBehavior28 pParkingPlaceBehavior28, void* pAircraft)
	{
		auto fn54 = pParkingPlaceBehavior28->vft28->func54;
		fn54(pParkingPlaceBehavior28, fn54, pAircraft);

		auto fn18 = pParkingPlaceBehavior28->vft28->func18;
		auto pGO_Aircraft = (pC_GameObject)pAircraft;
		fn18(pParkingPlaceBehavior28, fn18, pGO_Aircraft->CurrentID);
	}

	pM_ParkingPlaceBehavior28 __cdecl M_ParkingPlaceBehavior28_GetFromID(int ObjectID, int a2)
	{
		auto fnGet = (CallFunc_GetParkingPlaceBehavior28FromID)_call_ParkingPlaceBehavior28_GetFromID;
		return fnGet(ObjectID, a2);
	}


// end namespace RA3::Module
}
