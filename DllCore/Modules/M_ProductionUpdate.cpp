#include "../pch.h"
#include <format>

#include "../commonData.hpp"
#include "../utiliy.h"
#include "../utiliy_game.h"
#include "../Base/GlobalStructure.h"
#include "../Core/C_System.h"
#include "../Core/C_GameObject.h"
#include "../Core/C_Hotkey.h"
#include "M_ProductionUpdate.h"

namespace RA3::Module {

	uintptr_t _F_Data00CD7E84 = 0xCD7E84;

	uintptr_t _F_InitializeProductionUpdate1 = 0x7E11C0 + 1;
	uintptr_t _F_InitializeProductionUpdate2 = 0x789892;
	uintptr_t _F_ProductionUpdateVFT24_M = 0xC3F8C8;

	uintptr_t _F_ProductionUpdate_AddToBuildList = 0x4F467A;
	uintptr_t _F_ProductionUpdate_RemoveFromBuildList = 0x4F4753;
	uintptr_t _Ret_AptRenderItem__Manager_GetCreatedOnTick = 0x4F5C25;

	uintptr_t _F_ProductionUpdate_AddBuildListCount = 0x724074;
	uintptr_t _Ret_ProductionUpdate_AddBuildListCount = 0x724074 + 26;

	uintptr_t _call_ProductionUpdate24_GetFromQueueType = 0x717A60;

	void __fastcall M_ProductionUpdate_Hook()
	{
		int newProductionUpdateSize = sizeof(M_ProductionUpdate_t);
		WriteHookToProcess((void*)_F_InitializeProductionUpdate1, &newProductionUpdateSize, 4U);

		hookGameBlock((void*)_F_InitializeProductionUpdate2, (uintptr_t)M_ProductionUpdate_InitializeASM);
		WriteHookToProcess((void*)(_F_InitializeProductionUpdate2 + 5), (void*)&nop2, 2U);

		hookGameBlock((void*)_F_ProductionUpdate_AddToBuildList, (uintptr_t)M_ProductionUpdate_AddToBuildListASM);
		hookGameBlock((void*)_F_ProductionUpdate_RemoveFromBuildList, (uintptr_t)M_ProductionUpdate_RemoveFromBuildListASM);

		auto newCheckCanBuild = (uintptr_t)M_ProductionUpdate24_CheckCanBuild;
		WriteHookToProcess((void*)(_F_ProductionUpdateVFT24_M + 0x84), &newCheckCanBuild, 4U);

		// up shift to 9
		// and add loop to list
		hookGameBlock((void*)_F_ProductionUpdate_AddBuildListCount, (uintptr_t)M_ProductionUpdate_GetNewBuildCountASM);
		WriteHookToProcess((void*)(_F_ProductionUpdate_AddBuildListCount + 5), (void*)&nop4, 4U);
	}

	void __fastcall M_ProductionUpdate_Initialize(uintptr_t hmodEXE, int isNewSteam)
	{
		if (isNewSteam){
			_F_Data00CD7E84 = 0xCD7E04;

			_F_InitializeProductionUpdate1 = 0x81F4E0 + 1;
			_F_InitializeProductionUpdate2 = 0x7C7C32;
			_F_ProductionUpdateVFT24_M = 0xC46A30;

			_F_ProductionUpdate_AddToBuildList = 0x535F8A;
			_F_ProductionUpdate_RemoveFromBuildList = 0x536063;
			_Ret_AptRenderItem__Manager_GetCreatedOnTick = 0x537535;

			_F_ProductionUpdate_AddBuildListCount = hmodEXE + 0x362474;
			_Ret_ProductionUpdate_AddBuildListCount = hmodEXE + 0x362474 + 26;

			_call_ProductionUpdate24_GetFromQueueType = hmodEXE + 0x355EB0;
		}
	}

	__declspec(naked) void __fastcall M_ProductionUpdate_InitializeASM()
	{
		__asm {
			xorps xmm0, xmm0
			movq qword ptr[esi+0x14C], xmm0
		}

	original:
		__asm {
			mov eax, esi
			pop esi
			pop ebx
			ret 8
		}
	}

	__declspec(naked) void __fastcall M_ProductionUpdate_AddToBuildListASM()
	{
		__asm {
			movsx edx, byte ptr[esp + 0x10]
			cmp edx, -2
			jne original
			mov edx, [esi]
			mov eax, [edx + 0xAC]
			mov ecx, esi
			call eax
			cmp eax, 1
			jle original
			push eax
			mov edx, ebx
			mov ecx, edi
			call M_ProductionUpdate_AddToBuildListCPP
		}
	original:
		__asm {
			jmp _Ret_AptRenderItem__Manager_GetCreatedOnTick
		}
	}

	void __fastcall M_ProductionUpdate_AddToBuildListCPP(pC_GameObject pIn, void* pBuildUnit, int queueType)
	{
		auto selfThing = pIn->pThingTemplate;
		auto selfPlayer = pIn->pRelevantPlayer;

		auto pSys = GS_GetGameSystemPointer();
		auto pNode = pSys->pGameObjectNode;
		int NodeCount = pSys->GameObjectNodeCount;
		for(int i = 0; i < NodeCount; i++) {
			if(!pNode[i]) {
				continue;
			}

			auto pObj = (pC_GameObject)(pNode[i]->pGameObject);
			if (!pObj || pObj == pIn) {
				continue;
			}

			if (pObj->pThingTemplate != selfThing) {
				continue;
			}

			if (pObj->pRelevantPlayer != selfPlayer) {
				continue;
			}

			auto pBuild = M_ProductionUpdate24_GetFromQueueType((void*)pObj, 0, queueType);
			if (pBuild) {
				auto fnCheck = pBuild->BuildList_vft->func90;
				if (fnCheck(pBuild, fnCheck, pBuildUnit)) {
					continue;
				}

				auto fnBuildList = pBuild->BuildList_vft->func24;
				int buildcount = fnBuildList(pBuild, fnBuildList, pBuildUnit, -2, _F_Data00CD7E84, 0, 0, 0, 0, 0);
			}
		}
		// end
	}

	__declspec(naked) void __fastcall M_ProductionUpdate_RemoveFromBuildListASM()
	{
		__asm {
			cmp [edi+0x128], ebx
			jne original
			mov dword ptr [edi + 0x128], 0
		}
	original:
		__asm {
			jmp _Ret_AptRenderItem__Manager_GetCreatedOnTick
		}
	}

	__declspec(naked) void __fastcall M_ProductionUpdate_GetNewBuildCountASM()
	{
		__asm {
			mov byte ptr[esp + 0x13], 1
			// check if is JapanWallPiece
			cmp dword ptr[esi + 8], 4173660217u
			je JapanWallCount
			// get shift
			movsx edx, byte ptr[esp + 0xC0]
			test edx, edx
			js AddToLoopList // if < 0
			lea edi, [edx * 8 + 1]
			jmp _Ret_ProductionUpdate_AddBuildListCount
		}
	AddToLoopList:
		__asm {
			cmp edx, -2
			je AddToSameList
			mov[ebx + 0x128], esi
			mov edi, 3
			jmp _Ret_ProductionUpdate_AddBuildListCount
		}
	AddToSameList:
		__asm {
			mov edi, 5
			jmp _Ret_ProductionUpdate_AddBuildListCount
		}
	JapanWallCount:
		__asm {
			// ecx is pM_ProductionUpdate24
			xor eax, eax
			// increase production only if the list is empty.
			cmp[ebx + 0x10], eax
			sete al
			mov edi, eax
			jmp _Ret_ProductionUpdate_AddBuildListCount
		}
	}

	bool __fastcall M_ProductionUpdate24_CheckCanBuild(pM_ProductionUpdate24 pIn)
	{
		auto disableBuild = pIn->bDisableBuild;
		if (disableBuild) {
			return true;
		}

		auto cd = pIn->LoopCoolDown;
		cd++;
		pIn->LoopCoolDown = cd;

		if (pIn->FirstBuildUnit || cd < 30) {
			return false;
		}

		auto buildUnit = pIn->LoopBuildUnit;
		if (!buildUnit) {
			return false;
		}

		//auto fnCheck = pIn->BuildList_vft->func00;
		auto fnCheck = pIn->BuildList_vft->func90; // now use func90
		if (fnCheck(pIn, fnCheck, buildUnit)) {
			pIn->LoopCoolDown = 0;
			return false;
		}

		auto fnBuildList = pIn->BuildList_vft->func24;
		int buildcount = fnBuildList(pIn, fnBuildList, buildUnit, 3, _F_Data00CD7E84, 0, 0, 0, 0, 0);
		pIn->LoopCoolDown = 0;

		return false;
	}

	pM_ProductionUpdate24 __fastcall M_ProductionUpdate24_GetFromQueueType(void* pGO, int useless, int type)
	{
		typedef pM_ProductionUpdate24(__fastcall* callFunc)(void* pGO, void* callFunc, int type);

		auto fn = (callFunc)_call_ProductionUpdate24_GetFromQueueType;
		return fn(pGO, fn, type);
	}

// end RA3::Module
}